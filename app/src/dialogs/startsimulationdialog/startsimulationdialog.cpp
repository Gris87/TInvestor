#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <QDebug>



StartSimulationDialog::StartSimulationDialog(QWidget* parent) :
    IStartSimulationDialog(parent),
    ui(new Ui::StartSimulationDialog)
{
    qDebug() << "Create StartSimulationDialog";

    ui->setupUi(this);
}

StartSimulationDialog::~StartSimulationDialog()
{
    qDebug() << "Destroy StartSimulationDialog";

    delete ui;
}

void StartSimulationDialog::on_startButton_clicked()
{
    accept();
}
