#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onChangeCursor(double time);
    void onPlay();
    void onPause();
    void onAddKeyPose(double time);
    void onKeyPoseChanged(int num);
    void onRemoveKeyPose(int num);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Dialog * dialog;
};

#endif // MAINWINDOW_H
