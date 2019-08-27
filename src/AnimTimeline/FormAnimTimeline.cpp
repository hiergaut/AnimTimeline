#include "FormAnimTimeline.h"
#include "ui_FormAnimTimeline.h" // moc not found in <AnimTimeline/*>, created on build
//#include <AnimTimeline/animtimeline.h>

#include <QDebug>
#include <QDesktopWidget>
#include <QEvent>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>

FormAnimTimeline::FormAnimTimeline(QWidget* parent)
    : QWidget(parent)
    ,ui(new Ui::FormAnimTimeline)

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

//    connect(ui->widget_leftSlider, &QWidgetSlider::slide, ui->frame_selector, &QFrameSelector::onSlideLeftSlider);
    ui->splitter->setCollapsible(0, false);
    ui->splitter->setCollapsible(2, false);
    connect(ui->splitter, &QSplitter::splitterMoved, ui->frame_selector, &QFrameSelector::onSplitterMove);
//    connect(ui->widget_leftSlider, &QWidgetSlider::slideRelease, ui->frame_selector, &QFrameSelector::onLeftSlideRelease);
//    connect(ui->widget_rightSlider, &QWidgetSlider::slide, ui->frame_selector, &QFrameSelector::onSlideRightSlider);
//    connect(ui->widget_rightSlider, &QWidgetSlider::slideRelease, ui->frame_selector, &QFrameSelector::onRightSlideRelease);
    connect(ui->scrollAreaWidgetContents, &QWidgetRuler::rulerChanged, ui->frame_selector, &QFrameSelector::onRulerChange);
    connect(ui->scrollAreaWidgetContents, &QWidgetRuler::rulerChanged, ui->frame_timescale, &QFrameTimescale::onRulerChange);

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
//    ui->frame_selector->setLeftSlider(ui->widget_leftSlider);
    ui->frame_selector->setLeftSpacer(ui->frame_spacer);
    ui->frame_selector->setRightSpacer(ui->frame_rightSpacer);
//    ui->frame_selector->setRightSlider(ui->widget_rightSlider);
    ui->frame_selector->setCursorSpin(ui->doubleSpinBox_cursor);
    ui->frame_selector->setStartSpin(ui->doubleSpinBox_start);
    ui->frame_selector->setEndSpin(ui->doubleSpinBox_end);
    ui->frame_selector->setRemoveKeyPoseButton(ui->toolButton_deleteKeyPose);
    ui->frame_selector->setDurationSpin(ui->doubleSpinBox_maxDuration);
    ui->frame_selector->setNbKeyPosesSpin(ui->spinBox_nbKeyPoses);
    ui->frame_selector->setShiftDown(ui->scrollArea->getShiftDown());
    ui->frame_selector->setMidMouseDown(ui->scrollArea->getMidMouseDown());
    ui->frame_selector->setCtrlDown(ui->scrollArea->getCtrlDown());
    ui->frame_selector->setSplitter(ui->splitter);

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
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::playClicked, this, &FormAnimTimeline::playClicked);
    connect(ui->toolButton_playPause, &QToolButtonPlayPause::pauseClicked, this, &FormAnimTimeline::pauseClicked);

    connect(ui->frame_selector, &QFrameSelector::cursorChanged, this, &FormAnimTimeline::cursorChanged);
    connect(ui->frame_selector, &QFrameSelector::startChanged, this, &FormAnimTimeline::startChanged);
    connect(ui->frame_selector, &QFrameSelector::endChanged, this, &FormAnimTimeline::endChanged);
    connect(ui->frame_selector, &QFrameSelector::durationChanged, this, &FormAnimTimeline::durationChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPoseAdded, this, &FormAnimTimeline::keyPoseAdded);
    connect(ui->frame_selector, &QFrameSelector::keyPoseDeleted, this, &FormAnimTimeline::keyPoseDeleted);
    connect(ui->frame_selector, &QFrameSelector::keyPoseChanged, this, &FormAnimTimeline::keyPoseChanged);
    connect(ui->frame_selector, &QFrameSelector::keyPosesMoved, this, &FormAnimTimeline::keyPosesMoved);
    connect(ui->frame_selector, &QFrameSelector::keyPoseMoved, this, &FormAnimTimeline::keyPoseMoved);

    // move timeline into parent (not nil) or screen area
//    QRect rec = QApplication::desktop()->screenGeometry();
//    int rightBorder = (parent) ? (parent->x() + parent->width()) : (rec.x() + rec.width());
//    int bottomBorder = (parent) ? (parent->y() + parent->height()) : (rec.y() + rec.height());

//    int timelineLeft = rightBorder - this->width() - MARGIN_RIGHT;
//    int timelineTop = bottomBorder - this->height() - MARGIN_DOWN;
//    move(timelineLeft, timelineTop);

    // set sizePolicy to allow zoom in scrollArea
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    // first draw of ruler with current width (default in animtimeline.ui) of dialog
    ui->scrollAreaWidgetContents->onDrawRuler(width() - 2); // left/right border width = 2 *1 pixel
}

FormAnimTimeline::~FormAnimTimeline() { delete ui; }

// todo : ctrlDown = shiftDown = false, on focus event (initialize state)
void FormAnimTimeline::resizeEvent(QResizeEvent* event)
{
    qDebug() << "FormAnimTimeline::resizeEvent(" << event->size() << ")";

    ui->scrollAreaWidgetContents->onDrawRuler(event->size().width() - 2);
}

// ------------------------------- EXTERNAL SLOTS -----------------------------
void FormAnimTimeline::onChangeStart(double time)
{
    qDebug() << "\033[32monChangeStart(" << time << ")\033[0m";
    ui->frame_selector->onChangeStart(time, false);
}

void FormAnimTimeline::onChangeEnd(double time)
{
    qDebug() << "\033[32monChangeEnd(" << time << ")\033[0m";
    ui->frame_selector->onChangeEnd(time, false);
}

void FormAnimTimeline::onChangeCursor(double time)
{
    qDebug() << "\033[32monChangeCursor(" << time << ")\033[0m";
    ui->frame_selector->onChangeCursor(time, false);
}

void FormAnimTimeline::onChangeDuration(double time)
{
    qDebug() << "\033[32monChangeDuration(" << time << ")\033[0m";
    ui->frame_selector->onChangeDuration(time, false);
}

void FormAnimTimeline::onAddingKeyPose(double time)
{
    qDebug() << "\033[32monAddingKeyPose(" << time << ")\033[0m";
    ui->frame_selector->onAddingKeyPose(time, false);
}

void FormAnimTimeline::onClearKeyPoses()
{
    qDebug() << "\033[32monClearKeyPoses()\033[0m";
    ui->frame_selector->onClearKeyPoses();
}

void FormAnimTimeline::onSetPlayMode()
{
    qDebug() << "\033[32monSetPlayMode()\033[0m";
    ui->toolButton_playPause->onPlayMode();
}

void FormAnimTimeline::onSetPauseMode()
{
    qDebug() << "\033[32monSetPauseMode()\033[0m";
    ui->toolButton_playPause->onPauseMode();
}

FormAnimTimelineWithSession::FormAnimTimelineWithSession(QWidget* parent)
    : FormAnimTimeline(parent)
{
    connect(this, &FormAnimTimelineWithSession::startChanged, &session, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::endChanged, &session, &AnimTimelineSession::onChangeEnv);
    //    connect(this, &FormAnimTimelineWithSession::cursorChanged, this, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::durationChanged, &session, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::keyPoseAdded, &session, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::keyPoseDeleted, &session, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::keyPoseChanged, &session, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::keyPoseMoved, &session, &AnimTimelineSession::onChangeEnv);
    connect(this, &FormAnimTimelineWithSession::keyPosesMoved, &session, &AnimTimelineSession::onChangeEnv);
    //        connect(this, &FormAnimTimelineWithSession::playClicked, &session, &AnimTimelineSession::onChangeEnv);
    //        connect(this, &FormAnimTimelineWithSession::pauseClicked, &session, &AnimTimelineSession::onChangeEnv);

    connect(ui->scrollArea, &QScrollAreaRuler::undo, &session, &AnimTimelineSession::onUndo);
    connect(ui->scrollArea, &QScrollAreaRuler::redo, &session, &AnimTimelineSession::onRedo);

    // signal to signal
    connect(&session, &AnimTimelineSession::envSaved, this, &FormAnimTimelineWithSession::envSaved);
    connect(&session, &AnimTimelineSession::rendered, this, &FormAnimTimelineWithSession::rendered);
    connect(&session, &AnimTimelineSession::renderDeleted, this, &FormAnimTimelineWithSession::renderDeleted);

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

void FormAnimTimelineWithSession::onChangeStart(double time)
{
    FormAnimTimeline::onChangeStart(time);

    session.onClearSession();
}

void FormAnimTimelineWithSession::onChangeEnd(double time)
{
    FormAnimTimeline::onChangeEnd(time);

    session.onClearSession();
}

void FormAnimTimelineWithSession::onChangeDuration(double time)
{
    FormAnimTimeline::onChangeDuration(time);

    session.onClearSession();
}

void FormAnimTimelineWithSession::onAddingKeyPose(double time)
{
    FormAnimTimeline::onAddingKeyPose(time);

    session.onClearSession();
}

void FormAnimTimelineWithSession::onClearKeyPoses()
{
    FormAnimTimeline::onClearKeyPoses();

    session.onClearSession();
}

void FormAnimTimelineWithSession::onSaveRendering(void* anim, size_t bytes)
{
    qDebug() << "\033[32monSaveRendering(" << anim << ", " << bytes << ")\033[0m";
    session.onSaveRendering(anim, bytes);
}
