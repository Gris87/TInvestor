#pragma once



#include <QWidget>



class INotificationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit INotificationWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~INotificationWidget() override = default;

    INotificationWidget(const INotificationWidget& another)            = delete;
    INotificationWidget& operator=(const INotificationWidget& another) = delete;
};
