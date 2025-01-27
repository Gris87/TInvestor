#include "logger.h"

#include <QDateTime>
#include <QtLogging>



const QtMsgType logLevel = QtInfoMsg;



QtMessageHandler oldMessageHandler;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (type < logLevel)
    {
        return;
    }

    QString typeStr;

    switch (type)
    {
        case QtDebugMsg:    typeStr="DEBUG   "; break;
        case QtInfoMsg:     typeStr="INFO    "; break;
        case QtWarningMsg:  typeStr="WARNING "; break;
        case QtCriticalMsg: typeStr="CRITICAL"; break;
        case QtFatalMsg:    typeStr="FATAL   "; break;
        default:            typeStr="UNKNOWN "; break;
    }

    oldMessageHandler(type, context, QString("%1 %2 %3:%4 %5: %6")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(typeStr)
        .arg(QString(context.file).remove("..\\..\\..\\app\\"))
        .arg(context.line)
        .arg(context.function)
        .arg(msg)
    );
}

void Logger::init()
{
    oldMessageHandler = qInstallMessageHandler(messageHandler);
}
