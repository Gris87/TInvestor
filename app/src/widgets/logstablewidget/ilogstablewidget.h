#pragma once



#include <QWidget>



class ILogsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ILogsTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~ILogsTableWidget() = default;

    ILogsTableWidget(const ILogsTableWidget& another)            = delete;
    ILogsTableWidget& operator=(const ILogsTableWidget& another) = delete;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
