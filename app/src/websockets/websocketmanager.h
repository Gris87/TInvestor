#pragma once



#include "src/websockets/iwebsocketmanager.h"



class WebSocketManager : public IWebSocketManager
{
    Q_OBJECT

public:
    explicit WebSocketManager(QObject* parent = nullptr);
    ~WebSocketManager();

    WebSocketManager(const WebSocketManager& another)            = delete;
    WebSocketManager& operator=(const WebSocketManager& another) = delete;
};
