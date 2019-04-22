#include "animtimeline.h"
#include "ui_animtimeline.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QEvent>
#include <QPainter>
#include <QWheelEvent>

AnimTimeline::AnimTimeline(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AnimTimeline)
{
    //    Ui::ui =ui;
    //    qDebug() << this;
    qDebug() << "AnimTimeline::AnimTimeline(" << parent << ") : setup start";
    ui->setupUi(this);
    qDebug() << "AnimTimeline::AnimTimeline(" << parent << ") : setup end";

    // --------------------------- INTERNAL CONNECTIONS --------------------------
    // WIDGET_RULER
    connect(ui->scrollArea, SIGNAL(changePrecision(int)), ui->scrollAreaWidgetContents, SLOT(onDrawRuler(int)));

    // FRAME_BUTTONS
    connect(ui->toolButton_help, &QToolButton::clicked, ui->frame_buttons, &QFrameButtons::helpClicked);

    // FRAME_SELECTOR
//    connect(ui->doubleSpinBox_start, &QDoubleSpinBoxSmart::valueChanged, ui->frame_selector, &QFrameSelector::onChangeStartSpin);
//    connect(ui->doubleSpinBox_end, &QDoubleSpinBoxSmart::valueChanged, ui->frame_selector, &QFrameSelector::onChangeEndSpin);
//    connect(ui->doubleSpinBox_cursor, &QDoubleSpinBoxSmart::valueChanged, ui->frame_selector, &QFrameSelector::onChangeCursorSpin);
//    connect(ui->doubleSpinBox_maxDuration, &QDoubleSpinBoxSmart::valueChanged, ui->frame_selector, &QFrameSelector::onChangeDuration);
    connect(ui->doubleSpinBox_start, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeStartSpin);
    connect(ui->doubleSpinBox_end, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeEndSpin);
    connect(ui->doubleSpinBox_cursor, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeCursorSpin);
    connect(ui->doubleSpinBox_maxDuration, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeDuration);
    //    connect(ui->doubleSpinBox_cursor, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeCursorSpin()));
    //    connect(ui->doubleSpinBox_end, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeEndSpin()));
    //    connect(ui->doubleSpinBox_maxDuration, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeDuration()));
    //    connect(ui->doubleSpinBox_start, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeStartSpin()));

    connect(ui->label_leftSlider, &QLabelSlider::slide, ui->frame_selector, &QFrameSelector::onSlideLeftSlider);
    connect(ui->label_leftSlider, &QLabelSlider::slideRelease, ui->frame_selector, &QFrameSelector::onSlideRelease);
    connect(ui->label_rightSlider, &QLabelSlider::slide, ui->frame_selector, &QFrameSelector::onSlideRightSlider);
    connect(ui->label_rightSlider, &QLabelSlider::slideRelease, ui->frame_selector, &QFrameSelector::onSlideRelease);

    connect(ui->scrollArea, &QScrollAreaRuler::removeKeyPose, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);
    connect(ui->scrollArea, SIGNAL(addKeyPose()), ui->frame_selector, SLOT(onInternalAddingKeyPose()));
    connect(ui->scrollArea, &QScrollAreaRuler::nextKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->scrollArea, &QScrollAreaRuler::previousKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);

    connect(ui->toolButton_deleteKeyPose, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);
    connect(ui->toolButton_end, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToEnd);
    connect(ui->toolButton_forward, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->toolButton_keyPose, SIGNAL(clicked()), ui->frame_selector, SLOT(onInternalAddingKeyPose()));
//    connect(ui->toolButton_lessEnd, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onEndIncLess);
//    connect(ui->toolButton_lessStart, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onStartIncLess);
//    connect(ui->toolButton_playPause, &QToolButtonPlayPause::playClicked, ui->frame_selector, &QFrameSelector::onPlay);
//    connect(ui->toolButton_playPause, &QToolButtonPlayPause::pauseClicked, ui->frame_selector, &QFrameSelector::onPause);
//    connect(ui->toolButton_plusEnd, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onEndIncPlus);
//    connect(ui->toolButton_plusStart, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onStartIncPlus);
    connect(ui->toolButton_rearward, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);
    connect(ui->toolButton_start, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToStart);

    connect(ui->spinBox_nbKeyPoses, &QSpinBoxSmart::nextKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->spinBox_nbKeyPoses, &QSpinBoxSmart::previousKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);


    // SCROLL_AREA_RULER
    connect(ui->frame_buttons, &QFrameButtons::keyPressed, ui->scrollArea, &QScrollAreaRuler::onKeyPress);
    connect(ui->frame_buttons, &QFrameButtons::keyReleased, ui->scrollArea, &QScrollAreaRuler::onKeyRelease);

    //    connect(ui->frame_buttons, &QFrame::keyPressEvent, ui->scrollArea, &QScrollArea::keyPressEvent);



    // --------------------------- SET INTERNAL REFERENCES --------------------
    // FRAME_SELECTOR
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
    //    ui->frame_selector->setDrawLock(ui->scrollAreaWidgetContents->getDrawLock());
    //    ui->frame_selector->setDrawLock(ui->scrollAreaWidgetContents->getDrawLock());
    //    ui->frame_timescale->setDrawLock(ui->scrollAreaWidgetContents->getDrawLock());

    // FRAME_BUTTONS
    ui->frame_buttons->setAnimTimeline(this);
    ui->frame_buttons->setRuler(ui->scrollAreaWidgetContents);
    //    ui->frame_buttons->setScrollArea(ui->scrollArea);

    // SCROLL_AREA_RULER
    ui->scrollArea->setAnimTimeline(this);
    ui->scrollArea->setRuler(ui->scrollAreaWidgetContents);
    ui->scrollArea->setPlayPause(ui->toolButton_playPause);

    // --------------------------- SET DEFAULT UI VALUES -----------------------
    // FRAME_SELECTOR
    ui->frame_selector->setStart(ui->doubleSpinBox_start->value());
    ui->frame_selector->setCursor(ui->doubleSpinBox_cursor->value());
    ui->frame_selector->setEnd(ui->doubleSpinBox_end->value());

    // WIDGET_RULER
    ui->scrollAreaWidgetContents->setMaxDuration(ui->doubleSpinBox_maxDuration->value());

    // connect internal signals to external ones (black box)
    // --------------------------- SIGNAL TO SIGNAL ---------------------------
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

    //    if (parent) {
    //        rightBorder = parent->x() + parent->width();
    //        bottomBorder = parent->y() + parent->height();

    //        int parent_x = parent->x();
    //        int parent_y = parent->y();
    //        int parent_height = parent->height();
    //        int parent_width = parent->width();

    //        int timelineLeft = parent->x() + parent->width() - this->width() - INIT_MARGIN;
    //        int timelineTop = parent->y() + parent->height() - this->height() - INIT_MARGIN;
    //        move(timelineLeft, timelineTop);
    //    }
    // if nil parent, the timeline is moved into the current screen resolution/area
    //    else {
    //        int height = rec.height();
    //        int width = rec.width();
    //        rightBorder = rec.x() + rec.width();
    //        bottomBorder = rec.y() + rec.height();
    //    }

    QRect rec = QApplication::desktop()->screenGeometry();
    int rightBorder = (parent) ? (parent->x() + parent->width()) : (rec.x() + rec.width());
    int bottomBorder = (parent) ? (parent->y() + parent->height()) : (rec.y() + rec.height());

    int timelineLeft = rightBorder - this->width() - INIT_MARGIN;
    int timelineTop = bottomBorder - this->height() - INIT_MARGIN;
    move(timelineLeft, timelineTop);
    //    qDebug() << "end construct animTimeline";
    //    qDebug() << "ruler width : " << ui->scrollAreaWidgetContents->width();
    //    qDebug() << "timeline width : " << width();
    //    Ui::animTimelineWidth =&this->width();
    //    setFocusPolicy(Qt::StrongFocus);

    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    // draw ruler
    ui->scrollAreaWidgetContents->onDrawRuler(width() - 2); // left/right border width = 2 *1 pixel
}

AnimTimeline::~AnimTimeline() { delete ui; }

//void AnimTimeline::showEvent(QShowEvent*)
//{
//    QWidget* parent = static_cast<QWidget*>(this->parent());

//    // timeline can move into specific parent area
//    if (parent) {

//        int parent_x = parent->x();
//        int parent_y = parent->y();
//        int parent_height = parent->height();
//        int parent_width = parent->width();
//        int timelineLeft = parent_x + parent_width - this->width() - INIT_MARGIN;
//        int timelineTop = parent_y + parent_height - this->height() - INIT_MARGIN;
//        move(timelineLeft, timelineTop);
//    }
//}

// ------------------------------- EXTERNAL SLOTS -----------------------------

void AnimTimeline::onSetPlayMode()
{
    qDebug() << "\033[32monSetPlayMode()\033[0m";
    ui->toolButton_playPause->onPlayMode();
}

void AnimTimeline::onSetPauseMode()
{
    qDebug() << "\033[32monSetPauseMode()\033[0m";
    ui->toolButton_playPause->onPauseMode();
}

void AnimTimeline::onChangeAnimDuration(double time)
{
    qDebug() << "\033[32monChangeAnimDuration(" << time << ")\033[0m";
    ui->scrollAreaWidgetContents->setMaxDuration(time);
    ui->frame_selector->updateDurationSpin();
}

void AnimTimeline::onChangeCursor(double time)
{
    qDebug() << "\033[32monChangeCursor(" << time << ")\033[0m";
    ui->frame_selector->onChangeCursor(time);
}

void AnimTimeline::onChangeStart(double time)
{
    qDebug() << "\033[32monChangeStart(" << time << ")\033[0m";
    ui->frame_selector->onChangeStart(time);
}

void AnimTimeline::onChangeEnd(double time)
{
    qDebug() << "\033[32monChangeEnd(" << time << ")\033[0m";
    ui->frame_selector->onChangeEnd(time);
}

void AnimTimeline::onAddingKeyPose(double time)
{
    qDebug() << "\033[32monAddingKeyPose(" << time << ")\033[0m";
    ui->frame_selector->onAddingKeyPose(time);
}

void AnimTimeline::onClearKeyPoses()
{
    qDebug() << "\033[32monClearKeyPoses()\033[0m";
    ui->frame_selector->onClearKeyPoses();
}

// ------------------------------- GETTER -------------------------------------

//double AnimTimeline::getCursor()
//{
//    //    qDebug() << "getCursor : " << endl;
//    return ui->frame_selector->getCursor();
//}

//double AnimTimeline::getStart()
//{
//    //    qDebug() << "getStart : " << endl;
//    return ui->frame_selector->getStart();
//}

//double AnimTimeline::getEnd()
//{
//    //    qDebug() << "getEnd : " << endl;
//    return ui->frame_selector->getEnd();
//}

//int AnimTimeline::getNbKeyPoses()
//{
//    //    qDebug() << "getNbKeyPoses : " << endl;
//    return ui->frame_selector->getNbKeyPoses();
//}

//double AnimTimeline::getKeyPose(int id)
//{
//    //    qDebug() << "getKeyPose : " << endl;
//    return ui->frame_selector->getKeyPose(id);
//}
