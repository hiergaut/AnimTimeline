#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(AnimTimeline * timeline, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->widget =timeline;
}

Dialog::~Dialog()
{
    delete ui;
}
