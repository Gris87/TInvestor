#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>



MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroy MainWindow";

    delete ui;
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";
}
