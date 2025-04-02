#pragma once



#include <QWidget>



class IStocksTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IStocksTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IStocksTableWidget() = default;

    IStocksTableWidget(const IStocksTableWidget& another)            = delete;
    IStocksTableWidget& operator=(const IStocksTableWidget& another) = delete;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
