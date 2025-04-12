#pragma once



#include <QWidget>



class IDecisionMakerConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IDecisionMakerConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IDecisionMakerConfigWidget() override = default;

    IDecisionMakerConfigWidget(const IDecisionMakerConfigWidget& another)            = delete;
    IDecisionMakerConfigWidget& operator=(const IDecisionMakerConfigWidget& another) = delete;

    virtual void updateUiFromConfig() = 0;
};
