#include <AnimTimeline/animtimeline.h>
#include "ui_animtimeline.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QEvent>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>

AnimTimeline::AnimTimeline(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AnimTimeline)
{
    //    qDebug() << "AnimTimeline::AnimTimeline(" << parent << ") : ui setup start";
    ui->setupUi(this);
//    qDebug() << "AnimTimeline::AnimTimeline(" << parent << ") : ui setup end";

    //
    // --------------------------- INTERNAL CONNECTIONS --------------------------
    // WIDGET_RULER
    //    connect(ui->scrollArea, SIGNAL(changePrecision(int)), ui->scrollAreaWidgetContents, SLOT(onDrawRuler(int)));

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
    connect(ui->doubleSpinBox_maxDuration, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeDurationSpin);
    //    connect(ui->doubleSpinBox_cursor, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeCursorSpin()));
    //    connect(ui->doubleSpinBox_end, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeEndSpin()));
    //    connect(ui->doubleSpinBox_maxDuration, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeDuration()));
    //    connect(ui->doubleSpinBox_start, SIGNAL(valueChanged(double)), ui->frame_selector, SLOT(onChangeStartSpin()));

    connect(ui->label_leftSlider, &QLabelSlider::slide, ui->frame_selector, &QFrameSelector::onSlideLeftSlider);
    connect(ui->label_leftSlider, &QLabelSlider::slideRelease, ui->frame_selector, &QFrameSelector::onLeftSlideRelease);
    connect(ui->label_rightSlider, &QLabelSlider::slide, ui->frame_selector, &QFrameSelector::onSlideRightSlider);
    connect(ui->label_rightSlider, &QLabelSlider::slideRelease, ui->frame_selector, &QFrameSelector::onRightSlideRelease);

    connect(ui->scrollArea, &QScrollAreaRuler::removeKeyPose, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);
    connect(ui->scrollArea, SIGNAL(addKeyPose()), ui->frame_selector, SLOT(onAddingKeyPose()));
    //    connect(ui->scrollArea, SIGNAL(keyPoseOnMouseAdded()), ui->frame_selector, SLOT(onAddingKeyPoseOnMouse()));
    connect(ui->scrollArea, &QScrollAreaRuler::nextKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->scrollArea, &QScrollAreaRuler::previousKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);
    connect(ui->scrollArea, SIGNAL(durationChanged(double)), ui->frame_selector, SLOT(onChangeDuration(double)));

    connect(ui->toolButton_deleteKeyPose, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);
    connect(ui->toolButton_end, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToEnd);
    connect(ui->toolButton_forward, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->toolButton_keyPose, SIGNAL(clicked()), ui->frame_selector, SLOT(onAddingKeyPose()));
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
    connect(ui->spinBox_nbKeyPoses, &QSpinBoxSmart::deleteKeyPose, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);

    //
    // SCROLL_AREA_RULER
    connect(ui->frame_buttons, &QFrameButtons::keyPressed, ui->scrollArea, &QScrollAreaRuler::onKeyPress);
    connect(ui->frame_buttons, &QFrameButtons::keyReleased, ui->scrollArea, &QScrollAreaRuler::onKeyRelease);
    //    connect(ui->scrollAreaWidgetContents, &QWidgetRuler::rulerZoomed, ui->scrollArea, &QScrollAreaRuler::onZoomRuler);

    //    connect(ui->frame_buttons, &QFrame::keyPressEvent, ui->scrollArea, &QScrollArea::keyPressEvent);

    //
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
    //    ui->frame_selector->setStartInc(ui->doubleSpinBox_startInc);
    //    ui->frame_selector->setEndInc(ui->doubleSpinBox_endInc);
    ui->frame_selector->setDurationSpin(ui->doubleSpinBox_maxDuration);
    ui->frame_selector->setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);
    ui->frame_selector->setShiftDown(ui->scrollArea->getShiftDown());
    //    ui->frame_selector->setDrawLock(ui->scrollAreaWidgetContents->getDrawLock());
    //    ui->frame_selector->setDrawLock(ui->scrollAreaWidgetContents->getDrawLock());
    //    ui->frame_timescale->setDrawLock(ui->scrollAreaWidgetContents->getDrawLock());
    ui->frame_selector->setMidMouseDown(ui->scrollArea->getMidMouseDown());
    ui->frame_selector->setCtrlDown(ui->scrollArea->getCtrlDown());

    // FRAME_BUTTONS
    ui->frame_buttons->setAnimTimeline(this);
    ui->frame_buttons->setRuler(ui->scrollAreaWidgetContents);
    //    ui->frame_buttons->setScrollArea(ui->scrollArea);
    ui->frame_buttons->setHelpButton(ui->toolButton_help);

    // SCROLL_AREA_RULER
    ui->scrollArea->setAnimTimeline(this);
    ui->scrollArea->setRuler(ui->scrollAreaWidgetContents);
    ui->scrollArea->setPlayPause(ui->toolButton_playPause);
    ui->scrollArea->setSpinDuration(ui->doubleSpinBox_maxDuration);
    ui->scrollArea->setSelector(ui->frame_selector);
    ui->scrollArea->setCursorSpin(ui->doubleSpinBox_cursor);
    //    ui->scrollArea->setZero(ui->scrollAreaWidgetContents->getZero());

    // WIDGET_RULER
    ui->scrollAreaWidgetContents->setSpinStart(ui->doubleSpinBox_start);
    ui->scrollAreaWidgetContents->setSpinEnd(ui->doubleSpinBox_end);
    ui->scrollAreaWidgetContents->setSpinCursor(ui->doubleSpinBox_cursor);
    ui->scrollAreaWidgetContents->setSpinDuration(ui->doubleSpinBox_maxDuration);
    //    ui->scrollAreaWidgetContents->setAreaRuler(ui->scrollArea);
    ui->scrollAreaWidgetContents->setShiftDown(ui->scrollArea->getShiftDown());
    ui->scrollAreaWidgetContents->setCtrlDown(ui->scrollArea->getCtrlDown());

    //
    // --------------------------- SET DEFAULT UI VALUES -----------------------
    // FRAME_SELECTOR
    ui->frame_selector->setStart(ui->doubleSpinBox_start->value());
    ui->frame_selector->setCursor(ui->doubleSpinBox_cursor->value());
    ui->frame_selector->setEnd(ui->doubleSpinBox_end->value());
    //    ui->frame_selector->onChangeDuration(ui->doubleSpinBox_maxDuration->value());
    ui->frame_selector->setDuration(ui->doubleSpinBox_maxDuration->value());

    // WIDGET_RULER
    //    ui->scrollAreaWidgetContents->setMaxDuration(ui->doubleSpinBox_maxDuration->value());

    //
    // connect internal signals to external ones (black box)
    // --------------------- INTERNAL SIGNAL TO EXTERNAL SIGNAL ----------------
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::playClicked, this, &AnimTimeline::playClicked);
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::pauseClicked, this, &AnimTimeline::pauseClicked);
    //    connect(ui->scrollAreaWidgetContents, &QWidgetRuler::durationChanged, this, &AnimTimeline::durationChanged);

    connect(ui->frame_selector, &QFrameSelector::cursorChanged, this, &AnimTimeline::cursorChanged);
    connect(ui->frame_selector, &QFrameSelector::startChanged, this, &AnimTimeline::startChanged);
    connect(ui->frame_selector, &QFrameSelector::endChanged, this, &AnimTimeline::endChanged);
    connect(ui->frame_selector, &QFrameSelector::durationChanged, this, &AnimTimeline::durationChanged);

    connect(ui->frame_selector, &QFrameSelector::keyPoseAdded, this, &AnimTimeline::keyPoseAdded);
    connect(ui->frame_selector, &QFrameSelector::keyPoseDeleted, this, &AnimTimeline::keyPoseDeleted);
    connect(ui->frame_selector, &QFrameSelector::keyPoseChanged, this, &AnimTimeline::keyPoseChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPosesMoved, this, &AnimTimeline::keyPosesMoved);
    connect(ui->frame_selector, &QFrameSelector::keyPoseMoved, this, &AnimTimeline::keyPoseMoved);

    //    connect(ui->scrollArea	, &QScrollAreaRuler::undo, this, &AnimTimeline::undid);
    //    connect(ui->scrollArea, &QScrollAreaRuler::redo, this, &AnimTimeline::redid);
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

    int timelineLeft = rightBorder - this->width() - MARGIN_RIGHT;
    int timelineTop = bottomBorder - this->height() - MARGIN_DOWN;
    move(timelineLeft, timelineTop);
    //    qDebug() << "end construct animTimeline";
    //    qDebug() << "ruler width : " << ui->scrollAreaWidgetContents->width();
    //    qDebug() << "timeline width : " << width();
    //    Ui::animTimelineWidth =&this->width();
    //    setFocusPolicy(Qt::StrongFocus);

    // set sizePolicy to set, to allow zoom in scrollArea
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    // first draw of ruler with current width (default in animtimeline.ui) of dialog
    ui->scrollAreaWidgetContents->onDrawRuler(width() - 2); // left/right border width = 2 *1 pixel
}

AnimTimeline::~AnimTimeline() { delete ui; }

// -------------------------- LEGACY ---------------------------------------
void AnimTimeline::resizeEvent(QResizeEvent* event)
{
    qDebug() << "AnimTimeline::resizeEvent(" << event->size() << ")";

    ui->scrollAreaWidgetContents->onDrawRuler(event->size().width() - 2);
}

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
void AnimTimeline::onChangeStart(double time)
{
    qDebug() << "\033[32monChangeStart(" << time << ")\033[0m";
    ui->frame_selector->onChangeStart(time, false);
    //    session.onClearSession();
}

void AnimTimeline::onChangeEnd(double time)
{
    qDebug() << "\033[32monChangeEnd(" << time << ")\033[0m";
    ui->frame_selector->onChangeEnd(time, false);
    //    session.onClearSession();
}

void AnimTimeline::onChangeCursor(double time)
{
    qDebug() << "\033[32monChangeCursor(" << time << ")\033[0m";
    ui->frame_selector->onChangeCursor(time, false);
    //    session.onClearSession();
}

void AnimTimeline::onChangeDuration(double time)
{
    qDebug() << "\033[32monChangeDuration(" << time << ")\033[0m";
    //    ui->scrollAreaWidgetContents->setMaxDuration(time);
    //    ui->doubleSpinBox_maxDuration->setValue(time);
    //    ui->frame_selector->onUpdateDurationSpin();
    //    ui->frame_selector->onUpdateDurationSpin();
    ui->frame_selector->onChangeDuration(time, false);
    //    session.onClearSession();
}

void AnimTimeline::onAddingKeyPose(double time)
{
    qDebug() << "\033[32monAddingKeyPose(" << time << ")\033[0m";
    ui->frame_selector->onAddingKeyPose(time, false);
    //    session.onClearSession();
}

void AnimTimeline::onClearKeyPoses()
{
    qDebug() << "\033[32monClearKeyPoses()\033[0m";
    ui->frame_selector->onClearKeyPoses();
    //    session.onClearSession();
}

void AnimTimeline::onSetPlayMode()
{
    qDebug() << "\033[32monSetPlayMode()\033[0m";
    ui->toolButton_playPause->onPlayMode();
    //    session.onClearSession();
}

void AnimTimeline::onSetPauseMode()
{
    qDebug() << "\033[32monSetPauseMode()\033[0m";
    ui->toolButton_playPause->onPauseMode();
    //    session.onClearSession();
}

//void AnimTimeline::onSaveRendering(void* render)
//{
//    qDebug() << "\033[32monSaveRendering(" << render << ")\033[0m";
//    session.onSaveRendering(render);
//}

// ------------------------------- GETTERS -------------------------------------
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

// ############################### ANIM_TIMELINE_WITH_SESSION
//template <class T>
//AnimTimelineWithSession<T>::AnimTimelineWithSession(QWidget* parent)
//    : AnimTimeline(parent)
//{
//    //
//    // ---------------------- SESSION CONNECTION -------------------------------
//    connect(this, &AnimTimeline::startChanged, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::endChanged, &session, &Session::onChangeEnv);
//    //    connect(this, &AnimTimeline::cursorChanged, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::durationChanged, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::keyPoseAdded, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::keyPoseDeleted, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::keyPoseChanged, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::keyPoseMoved, &session, &Session::onChangeEnv);
//    connect(this, &AnimTimeline::keyPosesMoved, &session, &Session::onChangeEnv);
//    //    connect(this, &AnimTimeline::playClicked, &session, &Session::onChangeEnv);
//    //    connect(this, &AnimTimeline::pauseClicked, &session, &Session::onChangeEnv);

//    connect(ui->scrollArea, &QScrollAreaRuler::undo, &session, &Session::onUndo);
//    connect(ui->scrollArea, &QScrollAreaRuler::redo, &session, &Session::onRedo);

//    // signal to signal
//    connect(&session, &Session::envSaved, this, &AnimTimelineWithSession::envSaved);
//    connect(&session, &Session::rendered, this, &AnimTimelineWithSession::rendered);
//    connect(&session, &Session::renderDeleted, this, &AnimTimelineWithSession::renderDeleted);
//    //    connect(&session, &Session::sessionCleared, this, &AnimTimeline::sessionCleared);
//    //    connect(&session, &Session::undid, this, &AnimTimeline::undid);
//    //    connect(&session, &Session::redid, this, &AnimTimeline::redid);

//    session.setStart(ui->frame_selector->getStart());
//    session.setEnd(ui->frame_selector->getEnd());
//    session.setCursor(ui->frame_selector->getCursor());
//    session.setDuration(ui->scrollAreaWidgetContents->getMaxDuration());
//    session.setKeyPoses(ui->frame_selector->getKeyPoses());
//    //    session.setPlay(ui->toolButton_playPause->getPlay());

//    session.setStartSpin(ui->doubleSpinBox_start);
//    session.setEndSpin(ui->doubleSpinBox_end);
//    session.setCursorSpin(ui->doubleSpinBox_cursor);
//    session.setDurationSpin(ui->doubleSpinBox_maxDuration);
//    session.setPlayButton(ui->toolButton_playPause);
//    session.setRuler(ui->scrollAreaWidgetContents);
//    session.setSelector(ui->frame_selector);
//    session.setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);
//}

AnimTimelineWithSession::AnimTimelineWithSession(QWidget* parent)
    : AnimTimeline(parent)
{
    //    saveDelay = new QTimer(this);
    //    ////    connect(saveDelay, SIGNAL(timeout()), this, SLOT(onSaveEnv()));
    //    connect(saveDelay, &QTimer::timeout, this, &AnimTimelineWithSession::onSaveEnv);
    //    saveDelay->setSingleShot(true);

    connect(this, &AnimTimelineWithSession::startChanged, &session, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::endChanged, &session, &Session::onChangeEnv);
    //    connect(this, &AnimTimelineWithSession::cursorChanged, this, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::durationChanged, &session, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::keyPoseAdded, &session, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::keyPoseDeleted, &session, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::keyPoseChanged, &session, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::keyPoseMoved, &session, &Session::onChangeEnv);
    connect(this, &AnimTimelineWithSession::keyPosesMoved, &session, &Session::onChangeEnv);
    //        connect(this, &AnimTimelineWithSession::playClicked, &session, &Session::onChangeEnv);
    //        connect(this, &AnimTimelineWithSession::pauseClicked, &session, &Session::onChangeEnv);

    connect(ui->scrollArea, &QScrollAreaRuler::undo, &session, &Session::onUndo);
    connect(ui->scrollArea, &QScrollAreaRuler::redo, &session, &Session::onRedo);
    //    connect(this, &AnimTimeline::redid, this, &AnimTimelineWithSession::onRedo);

    // signal to signal
    connect(&session, &Session::envSaved, this, &AnimTimelineWithSession::envSaved);
    connect(&session, &Session::rendered, this, &AnimTimelineWithSession::rendered);
    connect(&session, &Session::renderDeleted, this, &AnimTimelineWithSession::renderDeleted);
    //    connect(&session, &Session::sessionCleared, this, &AnimTimeline::sessionCleared);
    //    connect(&session, &Session::undid, this, &AnimTimeline::undid);
    //    connect(&session, &Session::redid, this, &AnimTimeline::redid);

    session.setStart(ui->frame_selector->getStart());
    session.setEnd(ui->frame_selector->getEnd());
    session.setCursor(ui->frame_selector->getCursor());
    session.setDuration(ui->scrollAreaWidgetContents->getMaxDuration());
    session.setKeyPoses(ui->frame_selector->getKeyPoses());
    //    session.setPlay(ui->toolButton_playPause->getPlay());

    session.setStartSpin(ui->doubleSpinBox_start);
    session.setEndSpin(ui->doubleSpinBox_end);
    session.setCursorSpin(ui->doubleSpinBox_cursor);
    session.setDurationSpin(ui->doubleSpinBox_maxDuration);
    session.setPlayButton(ui->toolButton_playPause);
    session.setRuler(ui->scrollAreaWidgetContents);
    session.setSelector(ui->frame_selector);
    session.setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);

    //        session.onChangeEnv();
}

//AnimTimelineWithSession::~AnimTimelineWithSession()
//{
////    delete saveDelay;
//}

// timeline changed, save timeline and anim environment
//void AnimTimelineWithSession::onChangeEnv()
//{
//    saveDelay->start(100);
//}

void AnimTimelineWithSession::onChangeStart(double time)
{
    AnimTimeline::onChangeStart(time);

    session.onClearSession();
}

void AnimTimelineWithSession::onChangeEnd(double time)
{
    AnimTimeline::onChangeEnd(time);

    session.onClearSession();
}

void AnimTimelineWithSession::onChangeDuration(double time)
{
    AnimTimeline::onChangeDuration(time);

    session.onClearSession();
}

void AnimTimelineWithSession::onAddingKeyPose(double time)
{
    AnimTimeline::onAddingKeyPose(time);

    session.onClearSession();
}

void AnimTimelineWithSession::onClearKeyPoses()
{
    AnimTimeline::onClearKeyPoses();

    session.onClearSession();
}

void AnimTimelineWithSession::onSaveRendering(void* anim, size_t bytes)
{
    qDebug() << "\033[32monSaveRendering(" << anim << ", " << bytes << ")\033[0m";
    session.onSaveRendering(anim, bytes);
}
