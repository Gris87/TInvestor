#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>



AuthDialog::AuthDialog(QWidget* parent) :
    IAuthDialog(parent),
    ui(new Ui::AuthDialog)
{
    qDebug() << "Create AuthDialog";

    ui->setupUi(this);
}

AuthDialog::~AuthDialog()
{
    qDebug() << "Destroy AuthDialog";

    delete ui;
}
