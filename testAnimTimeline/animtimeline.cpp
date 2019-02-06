#include "animtimeline.h"
#include "ui_animtimeline.h"

AnimTimeline::AnimTimeline(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimTimeline)
{
    ui->setupUi(this);
}

AnimTimeline::~AnimTimeline()
{
    delete ui;
}
