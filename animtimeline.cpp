#include "animtimeline.h"
#include "ui_animtimeline.h"

//#include <QDebug>
#include <QDesktopWidget>
#include <QEvent>
#include <QPainter>
#include <QWheelEvent>

AnimTimeline::AnimTimeline(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AnimTimeline)
{

    ui->setupUi(this);

    connect(ui->scrollArea, SIGNAL(changePrecision(int)), ui->scrollAreaWidgetContents, SLOT(onChangePrecision(int)));

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
    ui->frame_selector->setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);
    ui->frame_selector->setShiftDown(ui->scrollArea->getShiftDown());

    ui->frame_buttons->setAnimTimeline(this);
    ui->frame_buttons->setRuler(ui->scrollAreaWidgetContents);

    ui->scrollArea->setRuler(ui->scrollAreaWidgetContents);
    ui->scrollArea->setPlayPause(ui->toolButton_playPause);

    // signal to signal
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::playClicked, this, &AnimTimeline::playClicked);
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::pauseClicked, this, &AnimTimeline::pauseClicked);
    connect(ui->scrollAreaWidgetContents, &QWidgetRuler::durationChanged, this, &AnimTimeline::durationChanged);

    connect(ui->frame_selector, &QFrameSelector::cursorChanged, this, &AnimTimeline::cursorChanged);
    connect(ui->frame_selector, &QFrameSelector::startChanged, this, &AnimTimeline::startChanged);
    connect(ui->frame_selector, &QFrameSelector::endChanged, this, &AnimTimeline::endChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPoseAdded, this, &AnimTimeline::keyPoseAdded);
    connect(ui->frame_selector, &QFrameSelector::keyPoseDeleted, this, &AnimTimeline::keyPoseDeleted);
    connect(ui->frame_selector, &QFrameSelector::keyPoseChanged, this, &AnimTimeline::keyPoseChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPosesMoved, this, &AnimTimeline::keyPosesMoved);
    connect(ui->frame_selector, &QFrameSelector::keyPoseMoved, this, &AnimTimeline::keyPoseMoved);

    // if nil parent, the timeline is moved into the current screen resolution/area
    if (!parent) {
        QRect rec = QApplication::desktop()->screenGeometry();
        int height = rec.height();
        int width = rec.width();
        int timelineLeft = width - this->width() - INIT_MARGIN;
        int timelineTop = height - this->height() - INIT_MARGIN;
        move(timelineLeft, timelineTop);
    }
}

AnimTimeline::~AnimTimeline() { delete ui; }

void AnimTimeline::showEvent(QShowEvent* ev)
{
    (void)ev;

    QWidget* parent = static_cast<QWidget*>(this->parent());

    // timeline can move into specific parent area
    if (parent) {

        int parent_x = parent->x();
        int parent_y = parent->y();
        int parent_height = parent->height();
        int parent_width = parent->width();
        int timelineLeft = parent_x + parent_width - this->width() - INIT_MARGIN;
        int timelineTop = parent_y + parent_height - this->height() - INIT_MARGIN;
        move(timelineLeft, timelineTop);
    }
}

void AnimTimeline::onChangeAnimDuration(double time)
{
    //    qDebug() << "onChangeAnimDuration : " << time << endl;
    ui->scrollAreaWidgetContents->setMaxDuration(time);
    ui->frame_selector->updateDurationSpin();
}

void AnimTimeline::onChangeCursor(double time)
{
    //    qDebug() << "onChangeCursor : " << time << endl;
    ui->frame_selector->onChangeCursor(time);
}

void AnimTimeline::onChangeStart(double time)
{
    //    qDebug() << "onChangeStart : " << time << endl;
    ui->frame_selector->onChangeStart(time);
}

void AnimTimeline::onChangeEnd(double time)
{
    //    qDebug() << "onChangeEnd : " << time << endl;
    ui->frame_selector->onChangeEnd(time);
}

void AnimTimeline::onAddingKeyPose(double time)
{
    //    qDebug() << "onAddingKeyPose : " << time << endl;
    ui->frame_selector->onAddingKeyPose(time, false);
}

void AnimTimeline::onClearKeyPoses()
{
    //    qDebug() << "onClearKeyPoses : " << endl;
    ui->frame_selector->onClearKeyPoses();
}

void AnimTimeline::onSetPauseMode()
{
    //    qDebug() << "onSetPauseMode : " << endl;
    ui->toolButton_playPause->onPauseMode();
}

void AnimTimeline::onSetPlayMode()
{
    //    qDebug() << "onSetPlayMode : " << endl;
    ui->toolButton_playPause->onPlayMode();
}

double AnimTimeline::getCursor()
{
    //    qDebug() << "getCursor : " << endl;
    return ui->frame_selector->getCursor();
}

double AnimTimeline::getStart()
{
    //    qDebug() << "getStart : " << endl;
    return ui->frame_selector->getStart();
}

double AnimTimeline::getEnd()
{
    //    qDebug() << "getEnd : " << endl;
    return ui->frame_selector->getEnd();
}

int AnimTimeline::getNbKeyPoses()
{
    //    qDebug() << "getNbKeyPoses : " << endl;
    return ui->frame_selector->getNbKeyPoses();
}

double AnimTimeline::getKeyPose(int id)
{
    //    qDebug() << "getKeyPose : " << endl;
    return ui->frame_selector->getKeyPose(id);
}
