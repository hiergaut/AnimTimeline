#include "animtimeline.h"
#include "ui_animtimeline.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QWheelEvent>

AnimTimeline::AnimTimeline(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AnimTimeline)
{

    ui->setupUi(this);
    //  ui->frame_selector->setGeometry(5, 5, 10, 10);
    //  ui->scrollAreaWidgetContents->setMinimumWidth(1000);
    connect(ui->scrollArea, SIGNAL(changePrecision(int)), ui->scrollAreaWidgetContents, SLOT(onChangePrecision(int)));
    connect(ui->frame_selector, SIGNAL(changeCursor(double, bool)), ui->doubleSpinBox_cursor, SLOT(setValue(double)));
    //  connect(ui->scrollAreaWidgetContents, SIGNAL(changeScale()), ui->frame_selector, SLOT(onRedrawScale()));
    //  connect(ui->scrollAreaWidgetContents, SIGNAL(changeScale()), ui->frame_selector, SLOT(paintEvent()));

    //  ui->scrollAreaWidgetContents->setPlayZone(ui->frame_playZone);
    //  ui->scrollAreaWidgetContents->setLeftSlider(ui->label_leftSlider);
    //  ui->scrollAreaWidgetContents->setLeftSpacer(ui->horizontalSpacer);
    //  ui->scrollAreaWidgetContents->setFrameSelector(ui->frame_selector);
    ui->frame_selector->setPlayZone(ui->frame_playZone);
    ui->frame_selector->setLeftSlider(ui->label_leftSlider);
    ui->frame_selector->setLeftSpacer(ui->frame_spacer);
    ui->frame_selector->setRightSlider(ui->label_rightSlider);
    //  qDebug() << "AnimeTimeline created";

    //  QIcon * ico = new QIcon();
    //  ico->addPixmap(QPixmap(":/images/pause.png"), QIcon::Normal, QIcon::Off);
    //  ico->addPixmap(QPixmap(":/images/play.png"), QIcon::Normal, QIcon::On);
    //  ui->toolButton_playPause->setIcon(*ico);
    //  ui->toolButton_playPause->setCheckable(false);

    //  connect(ui->scrollAreaWidgetContents, SIGNAL(rulerChange(double, int, double)), ui->frame_selector, SLOT(onRulerChange(double, int, double)));
    //  ui->frame_selector->drawRulerScale();
    //  ui->scrollAreaWidgetContents->updateTimeline(500);
}

AnimTimeline::~AnimTimeline() { delete ui; }

void AnimTimeline::onCursorChanged(double time, bool isOnKeyPose)
{
    //    qDebug() << "AnimTimeline: render " << time;
    qDebug() << "\033[35mAnimTimeline: render " << time << "\033[0m";

    if (isOnKeyPose) {
        ui->doubleSpinBox_cursor->setStyleSheet("background-color: yellow");
    } else {
        //        ui->doubleSpinBox_cursor->setStyleSheet("background-color: lightBlue");
        ui->doubleSpinBox_cursor->setStyleSheet("background-color: #5555ff");
    }
    update();

    emit render(time);
}

//void AnimTimeline::onKeyPose(bool isOn)
//{
//    qDebug() << "AnimTimeline: onKeyPose";

//}

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
