#pragma once



#include <QWidget>



class IDecisionMakerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IDecisionMakerWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IDecisionMakerWidget() = default;

    IDecisionMakerWidget(const IDecisionMakerWidget& another)            = delete;
    IDecisionMakerWidget& operator=(const IDecisionMakerWidget& another) = delete;
};
