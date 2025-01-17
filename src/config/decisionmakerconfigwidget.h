#ifndef DECISIONMAKERCONFIGWIDGET_H
#define DECISIONMAKERCONFIGWIDGET_H

#include <QWidget>



namespace Ui {
class DecisionMakerConfigWidget;
}



class DecisionMakerConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerConfigWidget(QWidget *parent = nullptr);
    ~DecisionMakerConfigWidget();

private:
    Ui::DecisionMakerConfigWidget *ui;
};



#endif // DECISIONMAKERCONFIGWIDGET_H
