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
    ~IDecisionMakerWidget() override = default;

    IDecisionMakerWidget(const IDecisionMakerWidget& another)            = delete;
    IDecisionMakerWidget& operator=(const IDecisionMakerWidget& another) = delete;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
