#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ui->widget->onChangeDuration(40);
//    ui->widget->onChangeCursor(5);
//    ui->widget->hide();
//    ui->widget->setVisible(false);
//    ui->widget->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPlay()
{
    qDebug() << "\033[35mplay\033[0m";

}

void MainWindow::onPause()
{
    qDebug() << "\033[35mpause\033[0m";

}

void MainWindow::onAddKeyPose(double time)
{
    qDebug() << "\033[35madding keyPose " << time << " \033[0m";

}

void MainWindow::onKeyPoseChanged(int num)
{
    qDebug() << "\033[35mchange keyPose " << num << " \033[0m";
}

void MainWindow::onRemoveKeyPose(int num)
{
    qDebug() << "\033[35mremove keyPose " << num << " \033[0m";
}

void MainWindow::onChangeCursor(double time)
{
    qDebug() << "\033[35mcursorChanged " << time << " \033[0m";

}

void MainWindow::on_pushButton_clicked()
{
   ui->widget->onAddingKeyPose(ui->doubleSpinBox_4->value());
}
