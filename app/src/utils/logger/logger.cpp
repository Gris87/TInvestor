#include "src/utils/logger/logger.h"

#include <QDateTime>
#include <QtLogging>



const QtMsgType logLevel = QtInfoMsg;

QMap<QtMsgType, QString> logLevelToString{
    {QtDebugMsg,    "DEBUG   "},
    {QtInfoMsg,     "INFO    "},
    {QtWarningMsg,  "WARNING "},
    {QtCriticalMsg, "CRITICAL"},
    {QtFatalMsg,    "FATAL   "}
};

// HACK for bad QtInfoMsg
QMap<QtMsgType, int> logLevelToInteger{
    {QtDebugMsg,    0},
    {QtInfoMsg,     1},
    {QtWarningMsg,  2},
    {QtCriticalMsg, 3},
    {QtFatalMsg,    4}
};



QtMessageHandler oldMessageHandler;

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_ASSERT_X(QtInfoMsg != 1, "messageHandler()", "It's time to remove hack");

    if (logLevelToInteger[type] < logLevelToInteger[logLevel])
    {
        return;
    }

    oldMessageHandler(
        type,
        context,
        QString("%1 %2 %3:%4 %5: %6")
            .arg(
                QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),
                logLevelToString[type],
                QString(context.file).remove("..\\..\\..\\app\\"),
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
