#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



DecisionMakerWidget::DecisionMakerWidget(QWidget* parent) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget)
{
    qDebug() << "Create DecisionMakerWidget";

    ui->setupUi(this);
}

DecisionMakerWidget::~DecisionMakerWidget()
{
    qDebug() << "Destroy DecisionMakerWidget";

    delete ui;
}
