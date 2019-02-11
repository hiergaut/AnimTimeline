#include "animtimeline.h"
#include "ui_animtimeline.h"

#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QWheelEvent>

AnimTimeline::AnimTimeline(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnimTimeline) {

  ui->setupUi(this);
//  ui->frame_selector->setGeometry(5, 5, 10, 10);
//  ui->scrollAreaWidgetContents->setMinimumWidth(1000);
  connect(ui->scrollArea, SIGNAL(changePrecision(int)), ui->scrollAreaWidgetContents, SLOT(onChangePrecision(int)));
//  connect(ui->scrollAreaWidgetContents, SIGNAL(changeScale()), ui->frame_selector, SLOT(onRedrawScale()));
//  connect(ui->scrollAreaWidgetContents, SIGNAL(changeScale()), ui->frame_selector, SLOT(paintEvent()));

//  ui->scrollAreaWidgetContents->setPlayZone(ui->frame_playZone);
//  ui->scrollAreaWidgetContents->setLeftSlider(ui->label_leftSlider);
//  ui->scrollAreaWidgetContents->setLeftSpacer(ui->horizontalSpacer);
//  ui->scrollAreaWidgetContents->setFrameSelector(ui->frame_selector);
  ui->frame_selector->setPlayZone(ui->frame_playZone);
  ui->frame_selector->setLeftSlider(ui->label_leftSlider);
  ui->frame_selector->setLeftSpacer(ui->horizontalSpacer);
//  qDebug() << "AnimeTimeline created";
}

AnimTimeline::~AnimTimeline() { delete ui; }

//void AnimTimeline::paintEvent(QPaintEvent *event) {
////  ui->frame_selector->setGeometry(5, 5, 1000, 100);


////    QPainter painter(this);
////    painter.setRenderHint(QPainter::HighQualityAntialiasing);

////    QPen linePen;
////    linePen.setWidth(1);
////    painter.setPen(linePen);

////    painter.drawLine(0, 0, 200, 200);

//}

//void AnimTimeline::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "AnimeTimeline: wheelEvent";
////    event->accept();
//}
