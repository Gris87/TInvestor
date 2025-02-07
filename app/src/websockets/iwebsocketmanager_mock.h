#pragma once



#include "src/websockets/iwebsocketmanager.h"

#include <gmock/gmock.h>



class WebSocketManagerMock : public IWebSocketManager
{
public:
    WebSocketManagerMock() : IWebSocketManager() {}
    ~WebSocketManagerMock() override = default;

    WebSocketManagerMock(const WebSocketManagerMock &another) = delete;
    WebSocketManagerMock& operator=(const WebSocketManagerMock &another) = delete;
};
