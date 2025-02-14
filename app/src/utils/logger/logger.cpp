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



QtMessageHandler oldMessageHandler;

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (type < logLevel)
    {
        return;
    }

    QString typeStr = logLevelToString[type];

    oldMessageHandler(
        type,
        context,
        QString("%1 %2 %3:%4 %5: %6")
            .arg(
                QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),
                typeStr,
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
