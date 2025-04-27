#include "src/utils/logger/logger.h"

#include <QDateTime>
#include <QThread>
#include <QtLogging>



#ifdef Q_OS_WINDOWS
const char* appPrefixPath = R"(..\..\..\app\)";
#else
const char* appPrefixPath = "../../../app/";
#endif

constexpr int HEX_DIGITS = 16;



const QtMsgType logLevel = QtInfoMsg;

static const QMap<QtMsgType, QString> logLevelToString{
    {QtDebugMsg,    "DEBUG   "},
    {QtInfoMsg,     "INFO    "},
    {QtWarningMsg,  "WARNING "},
    {QtCriticalMsg, "CRITICAL"},
    {QtFatalMsg,    "FATAL   "}
};

// HACK for bad QtInfoMsg (Remove on Qt 7)
static const QMap<QtMsgType, int> logLevelToInteger{
    {QtDebugMsg,    0},
    {QtInfoMsg,     1},
    {QtWarningMsg,  2},
    {QtCriticalMsg, 3},
    {QtFatalMsg,    4}
};



static QtMessageHandler oldMessageHandler; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_ASSERT_X(QtInfoMsg != 1, "messageHandler()", "It's time to remove hack");

    if (logLevelToInteger[type] < logLevelToInteger[logLevel])
    {
        return;
    }

    oldMessageHandler(
        type,
        context,
        QString("%1 %2 0x%3 %4:%5 %6: %7")
            .arg(
                QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),
                logLevelToString[type],
                QString::number(reinterpret_cast<qint64>(QThread::currentThreadId()), HEX_DIGITS)
                    .toUpper()
                    .rightJustified(4, '0'),
                QString(context.file).remove(appPrefixPath),
                QString::number(context.line),
                QString(context.function),
                msg
            )
    );
}

void Logger::init()
{
    oldMessageHandler = qInstallMessageHandler(messageHandler);
}

void Logger::deinit()
{
    qInstallMessageHandler(oldMessageHandler);
}
