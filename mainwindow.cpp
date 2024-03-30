#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enterGame_clicked()
{
    ui->Mainpage->setCurrentIndex(1);
}


void MainWindow::on_page2_to_page1_clicked()
{
    ui->Mainpage->setCurrentIndex(0);
}

