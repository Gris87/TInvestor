#pragma once



#include <QWidget>



class IOperationsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IOperationsTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IOperationsTableWidget() = default;

    IOperationsTableWidget(const IOperationsTableWidget& another)            = delete;
    IOperationsTableWidget& operator=(const IOperationsTableWidget& another) = delete;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
