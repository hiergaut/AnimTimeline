#include "animtimeline.h"
#include "ui_animtimeline.h"

//#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QWheelEvent>
//#include <QApplication>
#include <QDesktopWidget>

AnimTimeline::AnimTimeline(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AnimTimeline)
{

    ui->setupUi(this);
    //  ui->frame_selector->setGeometry(5, 5, 10, 10);
    //  ui->scrollAreaWidgetContents->setMinimumWidth(1000);
    connect(ui->scrollArea, SIGNAL(changePrecision(int)), ui->scrollAreaWidgetContents, SLOT(onChangePrecision(int)));
//    connect(ui->frame_selector, SIGNAL(changeCursor(double, bool)), ui->doubleSpinBox_cursor, SLOT(setValue(double)));
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
    ui->frame_selector->setCursorSpin(ui->doubleSpinBox_cursor);
    ui->frame_selector->setStartSpin(ui->doubleSpinBox_start);
    ui->frame_selector->setEndSpin(ui->doubleSpinBox_end);
    ui->frame_selector->setRemoveKeyPoseButton(ui->toolButton_deleteKeyPose);
    ui->frame_selector->setStartInc(ui->doubleSpinBox_startInc);
    ui->frame_selector->setEndInc(ui->doubleSpinBox_endInc);
    ui->frame_selector->setTotalDurationSpin(ui->doubleSpinBox_maxDuration);

    ui->frame_buttons->setAnimTimeline(this);
    ui->frame_buttons->setRuler(ui->scrollAreaWidgetContents);

//    connect(ui->frame_selector, SIGNAL(addKeyPose(double)), this, SIGNAL(addKeyPose()));

    // signal to signal
    connect(ui->frame_selector, &QFrameSelector::keyPoseAdded, this, &AnimTimeline::keyPoseAdded);
    connect(ui->frame_selector, &QFrameSelector::cursorChanged, this, &AnimTimeline::cursorChanged);
//    connect(ui->frame_selector, &QFrameSelector::onAddingKeyPose, this, &AnimTimeline::onAddingKeyPose);
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::playClicked, this, &AnimTimeline::playClicked);
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::pauseClicked, this, &AnimTimeline::pauseClicked);
    connect(ui->frame_selector, &QFrameSelector::keyPoseChanged, this, &AnimTimeline::keyPoseChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPosesChanged, this, &AnimTimeline::keyPosesChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPoseDeleted, this, &AnimTimeline::keyPoseDeleted);
    connect(ui->scrollAreaWidgetContents, &QWidgetRuler::durationChanged, this, &AnimTimeline::durationChanged);
//    connect(this, &AnimTimeline::onChangeDuration, ui->scrollAreaWidgetContents, &QWidgetRuler::setMaxDuration);
    //  qDebug() << "AnimeTimeline created";

    //  QIcon * ico = new QIcon();
    //  ico->addPixmap(QPixmap(":/images/pause.png"), QIcon::Normal, QIcon::Off);
    //  ico->addPixmap(QPixmap(":/images/play.png"), QIcon::Normal, QIcon::On);
    //  ui->toolButton_playPause->setIcon(*ico);
    //  ui->toolButton_playPause->setCheckable(false);

    //  connect(ui->scrollAreaWidgetContents, SIGNAL(rulerChange(double, int, double)), ui->frame_selector, SLOT(onRulerChange(double, int, double)));
    //  ui->frame_selector->drawRulerScale();
    //  ui->scrollAreaWidgetContents->updateTimeline(500);
    auto geometry = QApplication::desktop()->screenGeometry();
    int screenHeight = geometry.height();
    int screenWidth = geometry.width();
    int dialogLeft = screenWidth - this->width() -50;
    int dialogTop = screenHeight - this->height() -50;
    move(dialogLeft, dialogTop);

}

AnimTimeline::~AnimTimeline() { delete ui; }


void AnimTimeline::onChangeAnimDuration(double time)
{
    ui->scrollAreaWidgetContents->setMaxDuration(time);
    ui->frame_selector->updateDurationSpin();
//    ui->frame_selector->updatePlayZone();
}

void AnimTimeline::onChangeCursor(double time)
{
//    ui->frame_selector->setCursor(time);
    ui->frame_selector->onChangeCursor(time);
}

void AnimTimeline::onChangeStart(double time)
{
    ui->frame_selector->onChangeStart(time);
}

void AnimTimeline::onChangeEnd(double time)
{
    ui->frame_selector->onChangeEnd(time);
}

void AnimTimeline::onAddingKeyPose(double time)
{
    ui->frame_selector->onAddingKeyPose(time);
}

void AnimTimeline::onSetPauseMode()
{
    ui->toolButton_playPause->onPauseMode();
}

void AnimTimeline::onSetPlayMode()
{
    ui->toolButton_playPause->onPlayMode();
}

double AnimTimeline::getCursor()
{
    return ui->frame_selector->getCursor();
}

double AnimTimeline::getStart()
{
    return ui->frame_selector->getStart();
}

double AnimTimeline::getEnd()
{
    return ui->frame_selector->getEnd();
}

int AnimTimeline::getNbKeyPoses()
{
    return ui->frame_selector->getNbKeyPoses();
}

double AnimTimeline::getKeyPose(int id)
{
    return ui->frame_selector->getKeyPose(id);
}



//void AnimTimeline::onCursorChanged(double time, bool isOnKeyPose)
//{
//    //    qDebug() << "AnimTimeline: render " << time;
//    qDebug() << "\033[35mAnimTimeline: render " << time << "\033[0m";

//    if (isOnKeyPose) {
//        ui->doubleSpinBox_cursor->setStyleSheet("background-color: yellow");
//    } else {
//        //        ui->doubleSpinBox_cursor->setStyleSheet("background-color: lightBlue");
//        ui->doubleSpinBox_cursor->setStyleSheet("background-color: #5555ff");
//    }
//    update();

//    emit render(time);
//}

//void AnimTimeline::onAddKeyPose(double time)
//{

//}

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

//void AnimTimeline::on_doubleSpinBox_cursor_editingFinished()
//{
//    qDebug() << "AnimTimline: cursor edited";
//}
