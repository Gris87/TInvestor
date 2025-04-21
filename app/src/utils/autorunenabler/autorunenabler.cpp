#include "src/utils/autorunenabler/autorunenabler.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



const QString AUTORUN_PATH = "CurrentVersion/Run/TInvestor";



AutorunEnabler::AutorunEnabler(
    ISettingsEditor* autorunSettingsEditor, IProcessRunnerFactory* processRunnerFactory, IFileFactory* fileFactory
) :
    IAutorunEnabler(),
    mAutorunSettingsEditor(autorunSettingsEditor),
    mProcessRunnerFactory(processRunnerFactory),
    mFileFactory(fileFactory)
{
    qDebug() << "Create AutorunEnabler";
}

AutorunEnabler::~AutorunEnabler()
{
    qDebug() << "Destroy AutorunEnabler";
}

void AutorunEnabler::setEnabled(bool enabled)
{
    if (enabled)
    {
        enable();
    }
    else
    {
        disable();
    }
}

#ifdef Q_OS_WINDOWS
void AutorunEnabler::enable()
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const QString appPath = QDir::toNativeSeparators(qApp->applicationFilePath());

    mAutorunSettingsEditor->setValue(AUTORUN_PATH, QString("\"%1\" --autorun").arg(appPath));
}

void AutorunEnabler::disable()
{
    mAutorunSettingsEditor->remove(AUTORUN_PATH);
}
#else
void AutorunEnabler::enable()
{
    const QString rebootLine =
        QString("@reboot sh -c \"cd %1 && LD_LIBRARY_PATH=%1 ./TInvestor --autorun\"").arg(qApp->applicationDirPath());

    const std::shared_ptr<IProcessRunner> process1 = mProcessRunnerFactory->newInstance();

    process1->setStandardOutputFile("/tmp/TInvestor_autorun_cron");
    process1->start("crontab", QStringList() << "-l");

    if (process1->waitForFinished())
    {
        const std::shared_ptr<IFile> cronFile = mFileFactory->newInstance("/tmp/TInvestor_autorun_cron");

        if (cronFile->open(QIODevice::ReadOnly))
        {
            QString content = QString::fromUtf8(cronFile->readAll());
            cronFile->close();

            if (content == "")
            {
                content =
                    "# Edit this file to introduce tasks to be run by cron.\n# \n# Each task to run has to be defined through a "
                    "single line\n# indicating with different fields when the task will be run\n# and what command to run for "
                    "the task\n# \n# To define the time you can provide concrete values for\n# minute (m), hour (h), day of "
                    "month (dom), month (mon),\n# and day of week (dow) or use '*' in these fields (for 'any').\n# \n# Notice "
                    "that tasks will be started based on the cron's system\n# daemon's notion of time and timezones.\n# \n# "
                    "Output of the crontab jobs (including errors) is sent through\n# email to the user the crontab file belongs "
                    "to (unless redirected).\n# \n# For example, you can run a backup of all your user accounts\n# at 5 a.m "
                    "every week with:\n# 0 5 * * 1 tar -zcf /var/backups/home.tgz /home/\n# \n# For more information see the "
                    "manual pages of crontab(5) and cron(8)\n# \n# m h  dom mon dow   command\n";
            }
            else
            {
                const QStringList lines = content.split('\n');

                if (lines.contains(rebootLine))
                {
                    return;
                }
            }

            content += rebootLine;
            content += "\n";

            bool ok = cronFile->open(QIODevice::WriteOnly);
            Q_ASSERT_X(ok, "AutorunEnabler::enable()", "Failed to open file");

            cronFile->write(content.toUtf8());
            cronFile->close();

            const std::shared_ptr<IProcessRunner> process2 = mProcessRunnerFactory->newInstance();

            process2->start("crontab", QStringList() << "/tmp/TInvestor_autorun_cron");

            ok = process2->waitForFinished();
            Q_ASSERT_X(ok, "AutorunEnabler::enable()", "Failed to update cron");
        }
    }
}

void AutorunEnabler::disable()
{
    const QString rebootLine =
        QString("@reboot sh -c \"cd %1 && LD_LIBRARY_PATH=%1 ./TInvestor --autorun\"").arg(qApp->applicationDirPath());

    const std::shared_ptr<IProcessRunner> process1 = mProcessRunnerFactory->newInstance();

    process1->setStandardOutputFile("/tmp/TInvestor_autorun_cron");
    process1->start("crontab", QStringList() << "-l");

    if (process1->waitForFinished())
    {
        const std::shared_ptr<IFile> cronFile = mFileFactory->newInstance("/tmp/TInvestor_autorun_cron");

        if (cronFile->open(QIODevice::ReadOnly))
        {
            QString content = QString::fromUtf8(cronFile->readAll());
            cronFile->close();

            QStringList lines = content.split('\n');

            if (lines.removeAll(rebootLine) == 0)
            {
                return;
            }

            content = lines.join('\n');

            bool ok = cronFile->open(QIODevice::WriteOnly);
            Q_ASSERT_X(ok, "AutorunEnabler::enable()", "Failed to open file");

            cronFile->write(content.toUtf8());
            cronFile->close();

            const std::shared_ptr<IProcessRunner> process2 = mProcessRunnerFactory->newInstance();

            process2->start("crontab", QStringList() << "/tmp/TInvestor_autorun_cron");

            ok = process2->waitForFinished();
            Q_ASSERT_X(ok, "AutorunEnabler::enable()", "Failed to update cron");
        }
    }
}
#endif
