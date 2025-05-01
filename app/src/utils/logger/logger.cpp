#include "src/utils/logger/logger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QThread>
#include <QtLogging>



#ifdef Q_OS_WINDOWS
const char* const APP_PREFIX_PATH = R"(..\..\..\app\)";
#else
const char* const APP_PREFIX_PATH = "../../../app/";
#endif

constexpr int HEX_DIGITS = 16;



const QtMsgType LOG_LEVEL = QtInfoMsg;

// clang-format off
static const QMap<QtMsgType, QString> LOG_LEVEL_TO_STRING{ // clazy:exclude=non-pod-global-static
    {QtDebugMsg,    "DEBUG   "},
    {QtInfoMsg,     "INFO    "},
    {QtWarningMsg,  "WARNING "},
    {QtCriticalMsg, "CRITICAL"},
    {QtFatalMsg,    "FATAL   "}
};

// HACK for bad QtInfoMsg (Remove on Qt 7)
static const QMap<QtMsgType, int> LOG_LEVEL_TO_INTEGER{ // clazy:exclude=non-pod-global-static
    {QtDebugMsg,    0},
    {QtInfoMsg,     1},
    {QtWarningMsg,  2},
    {QtCriticalMsg, 3},
    {QtFatalMsg,    4}
};
// clang-format on



static QtMessageHandler oldMessageHandler; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
static bool             isLogToFile;

static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_ASSERT_X(QtInfoMsg != 1, "messageHandler()", "It's time to remove hack");

    if (LOG_LEVEL_TO_INTEGER[type] < LOG_LEVEL_TO_INTEGER[LOG_LEVEL])
    {
        return;
    }

    QString msgText = QString("%1 %2 0x%3 %4:%5 %6: %7")
                          .arg(
                              QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),
                              LOG_LEVEL_TO_STRING[type],
                              QString::number(reinterpret_cast<qint64>(QThread::currentThreadId()), HEX_DIGITS)
                                  .toUpper()
                                  .rightJustified(4, '0'),
                              QString(context.file).remove(APP_PREFIX_PATH),
                              QString::number(context.line),
                              QString(context.function),
                              msg
                          );

    if (isLogToFile)
    {
        msgText += "\n";

        QFile logFile(qApp->applicationDirPath() + "/logs.txt");

        const bool ok = logFile.open(QIODevice::Append);
        Q_ASSERT_X(ok, "messageHandler()", "Failed to open log file");
        logFile.write(msgText.toUtf8());
        logFile.close();
    }
    else
    {
        oldMessageHandler(type, context, msgText);
    }
}

void Logger::init()
{
    oldMessageHandler = qInstallMessageHandler(messageHandler);
}

void Logger::deinit()
{
    qInstallMessageHandler(oldMessageHandler);
}

void Logger::enableLogToFile()
{
    isLogToFile = true;
}
