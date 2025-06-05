#pragma once



#include <QWidget>



class ILogLevelTableItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ILogLevelTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ILogLevelTableItemWidget() override = default;

    ILogLevelTableItemWidget(const ILogLevelTableItemWidget& another)            = delete;
    ILogLevelTableItemWidget& operator=(const ILogLevelTableItemWidget& another) = delete;
};
