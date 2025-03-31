#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <QDebug>



StartAutoPilotDialog::StartAutoPilotDialog(QWidget* parent) :
    IStartAutoPilotDialog(parent),
    ui(new Ui::StartAutoPilotDialog)
{
    qDebug() << "Create StartAutoPilotDialog";

    ui->setupUi(this);
}

StartAutoPilotDialog::~StartAutoPilotDialog()
{
    qDebug() << "Destroy StartAutoPilotDialog";

    delete ui;
}

void StartAutoPilotDialog::on_startButton_clicked()
{
    accept();
}
