#pragma once



#include <QObject>



class IWebSocketManager : public QObject
{
public:
    explicit IWebSocketManager(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    virtual ~IWebSocketManager() = default;

    IWebSocketManager(const IWebSocketManager& another)            = delete;
    IWebSocketManager& operator=(const IWebSocketManager& another) = delete;
};
