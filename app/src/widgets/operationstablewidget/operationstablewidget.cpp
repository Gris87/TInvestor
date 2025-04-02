#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <QDebug>



OperationsTableWidget::OperationsTableWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    IOperationsTableWidget(parent),
    ui(new Ui::OperationsTableWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create OperationsTableWidget";

    ui->setupUi(this);
}

OperationsTableWidget::~OperationsTableWidget()
{
    qDebug() << "Destroy OperationsTableWidget";

    delete ui;
}

void OperationsTableWidget::saveWindowState(const QString& /*type*/)
{
}

void OperationsTableWidget::loadWindowState(const QString& /*type*/)
{
}
