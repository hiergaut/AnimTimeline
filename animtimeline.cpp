#include "ui_animtimeline.h" // moc not found in <AnimTimeline/*>, created on build
#include <AnimTimeline/animtimeline.h>

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
    ui->setupUi(this);
    //    qDebug() << "AnimTimeline::AnimTimeline(" << parent << ") : ui setup end";

    // --------------------------- INTERNAL CONNECTIONS -----------------------
    // FRAME_BUTTONS
    connect(ui->toolButton_help, &QToolButton::clicked, ui->frame_buttons, &QFrameButtons::helpClicked);

    // FRAME_SELECTOR
    connect(ui->doubleSpinBox_start, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeStartSpin);
    connect(ui->doubleSpinBox_end, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeEndSpin);
    connect(ui->doubleSpinBox_cursor, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeCursorSpin);
    connect(ui->doubleSpinBox_maxDuration, &QDoubleSpinBoxSmart::editingFinished, ui->frame_selector, &QFrameSelector::onChangeDurationSpin);

    connect(ui->label_leftSlider, &QLabelSlider::slide, ui->frame_selector, &QFrameSelector::onSlideLeftSlider);
    connect(ui->label_leftSlider, &QLabelSlider::slideRelease, ui->frame_selector, &QFrameSelector::onLeftSlideRelease);
    connect(ui->label_rightSlider, &QLabelSlider::slide, ui->frame_selector, &QFrameSelector::onSlideRightSlider);
    connect(ui->label_rightSlider, &QLabelSlider::slideRelease, ui->frame_selector, &QFrameSelector::onRightSlideRelease);

    connect(ui->scrollArea, &QScrollAreaRuler::removeKeyPose, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);
    connect(ui->scrollArea, SIGNAL(addKeyPose()), ui->frame_selector, SLOT(onAddingKeyPose()));
    connect(ui->scrollArea, &QScrollAreaRuler::nextKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->scrollArea, &QScrollAreaRuler::previousKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);
    connect(ui->scrollArea, SIGNAL(durationChanged(double)), ui->frame_selector, SLOT(onChangeDuration(double)));

    connect(ui->toolButton_deleteKeyPose, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);
    connect(ui->toolButton_end, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToEnd);
    connect(ui->toolButton_forward, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->toolButton_keyPose, SIGNAL(clicked()), ui->frame_selector, SLOT(onAddingKeyPose()));

    connect(ui->toolButton_rearward, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);
    connect(ui->toolButton_start, &QToolButton::clicked, ui->frame_selector, &QFrameSelector::onSetCursorToStart);

    connect(ui->spinBox_nbKeyPoses, &QSpinBoxSmart::nextKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToNextKeyPose);
    connect(ui->spinBox_nbKeyPoses, &QSpinBoxSmart::previousKeyPose, ui->frame_selector, &QFrameSelector::onSetCursorToPreviousKeyPose);
    connect(ui->spinBox_nbKeyPoses, &QSpinBoxSmart::deleteKeyPose, ui->frame_selector, &QFrameSelector::onDeleteKeyPose);

    // SCROLL_AREA_RULER
    connect(ui->frame_buttons, &QFrameButtons::keyPressed, ui->scrollArea, &QScrollAreaRuler::onKeyPress);
    connect(ui->frame_buttons, &QFrameButtons::keyReleased, ui->scrollArea, &QScrollAreaRuler::onKeyRelease);

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
    ui->frame_selector->setDurationSpin(ui->doubleSpinBox_maxDuration);
    ui->frame_selector->setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);
    ui->frame_selector->setShiftDown(ui->scrollArea->getShiftDown());
    ui->frame_selector->setMidMouseDown(ui->scrollArea->getMidMouseDown());
    ui->frame_selector->setCtrlDown(ui->scrollArea->getCtrlDown());

    // FRAME_BUTTONS
    ui->frame_buttons->setAnimTimeline(this);
    ui->frame_buttons->setRuler(ui->scrollAreaWidgetContents);
    ui->frame_buttons->setHelpButton(ui->toolButton_help);

    // SCROLL_AREA_RULER
    ui->scrollArea->setAnimTimeline(this);
    ui->scrollArea->setRuler(ui->scrollAreaWidgetContents);
    ui->scrollArea->setPlayPause(ui->toolButton_playPause);
    ui->scrollArea->setSpinDuration(ui->doubleSpinBox_maxDuration);
    ui->scrollArea->setSelector(ui->frame_selector);
    ui->scrollArea->setCursorSpin(ui->doubleSpinBox_cursor);

    // WIDGET_RULER
    ui->scrollAreaWidgetContents->setSpinStart(ui->doubleSpinBox_start);
    ui->scrollAreaWidgetContents->setSpinEnd(ui->doubleSpinBox_end);
    ui->scrollAreaWidgetContents->setSpinCursor(ui->doubleSpinBox_cursor);
    ui->scrollAreaWidgetContents->setSpinDuration(ui->doubleSpinBox_maxDuration);
    ui->scrollAreaWidgetContents->setShiftDown(ui->scrollArea->getShiftDown());
    ui->scrollAreaWidgetContents->setCtrlDown(ui->scrollArea->getCtrlDown());

    // --------------------------- SET DEFAULT UI VALUES ----------------------
    // FRAME_SELECTOR
    ui->frame_selector->setStart(ui->doubleSpinBox_start->value());
    ui->frame_selector->setCursor(ui->doubleSpinBox_cursor->value());
    ui->frame_selector->setEnd(ui->doubleSpinBox_end->value());
    ui->frame_selector->setDuration(ui->doubleSpinBox_maxDuration->value());

    // ------------- CONNECT INTERNAL SIGNAL TO EXTERNAL SIGNAL ---------------
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::playClicked, this, &AnimTimeline::playClicked);
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::pauseClicked, this, &AnimTimeline::pauseClicked);

    connect(ui->frame_selector, &QFrameSelector::cursorChanged, this, &AnimTimeline::cursorChanged);
    connect(ui->frame_selector, &QFrameSelector::startChanged, this, &AnimTimeline::startChanged);
    connect(ui->frame_selector, &QFrameSelector::endChanged, this, &AnimTimeline::endChanged);
    connect(ui->frame_selector, &QFrameSelector::durationChanged, this, &AnimTimeline::durationChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPoseAdded, this, &AnimTimeline::keyPoseAdded);
    connect(ui->frame_selector, &QFrameSelector::keyPoseDeleted, this, &AnimTimeline::keyPoseDeleted);
    connect(ui->frame_selector, &QFrameSelector::keyPoseChanged, this, &AnimTimeline::keyPoseChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPosesMoved, this, &AnimTimeline::keyPosesMoved);
    connect(ui->frame_selector, &QFrameSelector::keyPoseMoved, this, &AnimTimeline::keyPoseMoved);

    // move timeline into parent (not nil) or screen area
    QRect rec = QApplication::desktop()->screenGeometry();
    int rightBorder = (parent) ? (parent->x() + parent->width()) : (rec.x() + rec.width());
    int bottomBorder = (parent) ? (parent->y() + parent->height()) : (rec.y() + rec.height());

    int timelineLeft = rightBorder - this->width() - MARGIN_RIGHT;
    int timelineTop = bottomBorder - this->height() - MARGIN_DOWN;
    move(timelineLeft, timelineTop);

    // set sizePolicy to allow zoom in scrollArea
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    // first draw of ruler with current width (default in animtimeline.ui) of dialog
    ui->scrollAreaWidgetContents->onDrawRuler(width() - 2); // left/right border width = 2 *1 pixel
}

AnimTimeline::~AnimTimeline() { delete ui; }

// todo : ctrlDown = shiftDown = false, on focus event (initialize state)
void AnimTimeline::resizeEvent(QResizeEvent* event)
{
    qDebug() << "AnimTimeline::resizeEvent(" << event->size() << ")";

    ui->scrollAreaWidgetContents->onDrawRuler(event->size().width() - 2);
}

// ------------------------------- EXTERNAL SLOTS -----------------------------
void AnimTimeline::onChangeStart(double time)
{
    qDebug() << "\033[32monChangeStart(" << time << ")\033[0m";
    ui->frame_selector->onChangeStart(time, false);
}

void AnimTimeline::onChangeEnd(double time)
{
    qDebug() << "\033[32monChangeEnd(" << time << ")\033[0m";
    ui->frame_selector->onChangeEnd(time, false);
}

void AnimTimeline::onChangeCursor(double time)
{
    qDebug() << "\033[32monChangeCursor(" << time << ")\033[0m";
    ui->frame_selector->onChangeCursor(time, false);
}

void AnimTimeline::onChangeDuration(double time)
{
    qDebug() << "\033[32monChangeDuration(" << time << ")\033[0m";
    ui->frame_selector->onChangeDuration(time, false);
}

void AnimTimeline::onAddingKeyPose(double time)
{
    qDebug() << "\033[32monAddingKeyPose(" << time << ")\033[0m";
    ui->frame_selector->onAddingKeyPose(time, false);
}

void AnimTimeline::onClearKeyPoses()
{
    qDebug() << "\033[32monClearKeyPoses()\033[0m";
    ui->frame_selector->onClearKeyPoses();
}

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

AnimTimelineWithSession::AnimTimelineWithSession(QWidget* parent)
    : AnimTimeline(parent)
{
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

    // signal to signal
    connect(&session, &Session::envSaved, this, &AnimTimelineWithSession::envSaved);
    connect(&session, &Session::rendered, this, &AnimTimelineWithSession::rendered);
    connect(&session, &Session::renderDeleted, this, &AnimTimelineWithSession::renderDeleted);

    session.setStart(ui->frame_selector->getStart());
    session.setEnd(ui->frame_selector->getEnd());
    session.setCursor(ui->frame_selector->getCursor());
    session.setDuration(ui->scrollAreaWidgetContents->getMaxDuration());
    session.setKeyPoses(ui->frame_selector->getKeyPoses());
    session.setStartSpin(ui->doubleSpinBox_start);
    session.setEndSpin(ui->doubleSpinBox_end);
    session.setCursorSpin(ui->doubleSpinBox_cursor);
    session.setDurationSpin(ui->doubleSpinBox_maxDuration);
    session.setPlayButton(ui->toolButton_playPause);
    session.setRuler(ui->scrollAreaWidgetContents);
    session.setSelector(ui->frame_selector);
    session.setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);
}

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
