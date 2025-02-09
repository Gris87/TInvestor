#include "src/websockets/websocketmanager.h"

#include <QDebug>



WebSocketManager::WebSocketManager(QObject* parent) :
    IWebSocketManager(parent)
{
    qDebug() << "Create WebSocketManager";
}

WebSocketManager::~WebSocketManager()
{
    qDebug() << "Destroy WebSocketManager";
}
