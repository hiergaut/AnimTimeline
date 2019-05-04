//#include "session.h"
#include "configurations.h"
#include <AnimTimeline/configurations.h>
#include <AnimTimeline/session.h>

#include <QDebug>
#include <QTimer>
#include <set>
//#include <boost/circular_buffer.hpp> // need boost lib

//template <class T>
//struct s_Env {
//    double start;
//    double end;
//    double cursor;
//    double duration;

//    std::set<double> keyPoses;

////    bool play;
//    T render;
//};

Session::Session(QObject* parent)
    : QObject(parent)
{
    //    qDebug() << "sizeof Env = " << sizeof (Env);
    saveDelay = new QTimer(this);
    //    connect(saveDelay, SIGNAL(timeout()), this, SLOT(onSaveEnv()));
    connect(saveDelay, &QTimer::timeout, this, &Session::envSaved);
#ifndef QT_NO_DEBUG_OUTPUT
    connect(this, &Session::envSaved, this, &Session::envSavedTrace);
#endif
    saveDelay->setSingleShot(true);

    //    connect(timeline, &AnimTimeline::startChanged, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::endChanged, this, &Session::onChangeEnv);
    //    //    connect(this, &AnimTimeline::cursorChanged, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::durationChanged, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::keyPoseAdded, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::keyPoseDeleted, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::keyPoseChanged, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::keyPoseMoved, this, &Session::onChangeEnv);
    //    connect(timeline, &AnimTimeline::keyPosesMoved, this, &Session::onChangeEnv);
    //    //    connect(this, &AnimTimeline::playClicked, &session, &Session::onChangeEnv);
    //    connect(this, &AnimTimeline::pauseClicked, &session, &Session::onChangeEnv);

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
}

Session::~Session()
{
    onClearSession();
    delete saveDelay;
}

// timeline changed, save timeline and anim environment
void Session::onChangeEnv()
{
    qDebug() << "Session::onChangeEnv()";
    saveDelay->start(DELAY_AUTO_SAVE);
}

void Session::onClearSession()
{
    while (!undo.empty()) {
        emit renderDeleted(undo.back().anim);
        qDebug() << "\033[35mrenderDeleted(" << undo.back().anim << ")\033[0m";
        //        undo.pop();
        undo.pop_back();
    }
    while (!redoHeap.empty()) {
        emit renderDeleted(redoHeap.top().anim);
        qDebug() << "\033[35mrenderDeleted(" << redoHeap.top().anim << ")\033[0m";
        redoHeap.pop();
    }

    size = 0;

    onChangeEnv();
    //    emit sessionCleared();
}

void Session::onUndo()
{
    if (undo.empty()) {
        qDebug() << "\033[31mSession::onUndo() : empty buffer ! (suggestion for client : "
                    "you need to catch envSaved to launch onSaveRendering with your anim "
                    "structure due of undo/redo calling, wanted to restore your environment\033[0m";
        return;
    }
    //    if (redoHeap.empty()) {
    //        redoHeap.push(undo.top());
    //        undo.pop();
    //    }

    if (undo.size() > 1) {
        redoHeap.push(undo.back());
        undo.pop_back();

        setEnv(undo.back());
        //        emit undid();
    } else {
        qDebug() << "\033[31mSession::onUndo() : last element, can't delete !\033[0m";
    }
}

void Session::onRedo()
{
    if (redoHeap.empty()) {
        qDebug() << "\033[31mSession::onRedo() : empty stack !\033[0m";
        return;
    }

    undo.emplace_back(redoHeap.top());
    redoHeap.pop();

    //    if (!redoHeap.empty()) {
    setEnv(undo.back());
    //    emit redid();
    //    } else {
    //        qDebug() << "\033[31mSession::onRedo() : empty stack !\033[0m";
    //    }

    //    if (undo.empty()) {
    //        undo.push(redoHeap.top());
    //        redoHeap.pop();
    //    }

    //    if (!redoHeap.empty()) {
    //        undo.push(redoHeap.top());
    //        redoHeap.pop();

    //        setEnv(undo.top());
    //        emit redid();
    //    }
}

void Session::onSaveRendering(void* anim, size_t bytes)
{
    //    Env env { *start, *end, *cursor, *duration, *keyPoses, anim};
    //    undo.push(env);
    //    qDebug() << "onSaveRendering : size of anim " << sizeof (anim);
    //    qDebug() << "onSaveRendering(" << anim << ") : sizeof session = " << sizeof (undo);

    while (!redoHeap.empty()) {
        emit renderDeleted(redoHeap.top().anim);
        qDebug() << "\033[35mrenderDeleted(" << redoHeap.top().anim << ")\033[0m";
        size -= redoHeap.top().bytes;
        redoHeap.pop();
    }

    undo.emplace_back(Env { *start, *end, *cursor, *duration, *keyPoses, anim, bytes });
    size += bytes;

    while (size > BUFFER_SESSION_MAX_SIZE) {
        qDebug() << "\033[31monSaveRendering : buffer overflow\033[0m";
        emit renderDeleted(undo.front().anim);
        qDebug() << "\033[35mrenderDeleted(" << undo.front().anim << ")\033[0m";
        size -= undo.front().bytes;
        undo.pop_front();
    }

    //    qDebug() << "Session::onSaveRendering(" << anim << ", " << bytes << ") : buff size = " << size;
    qDebug() << "Session::onSaveRendering() : buff size (bytes) =" << size << "/" << BUFFER_SESSION_MAX_SIZE << "(" << size * 100.0 / BUFFER_SESSION_MAX_SIZE << "% )";

    //    emit envSaved();
}

void Session::setEnv(Env env)
{
    *start = env.start;
    //    startSpin->setValue(*start);
    selector->updateStartSpin();

    *end = env.end;
    //    endSpin->setValue(*end);
    selector->updateEndSpin();

    *keyPoses = env.keyPoses;
    nbKeyPosesSpin->setValue(static_cast<int>(keyPoses->size()));

    //    *cursor = env.cursor;
    //    cursorSpin->setValue(*cursor);
    selector->updateCursorSpin();

    *duration = env.duration;
    //    durationSpin->setValue(*duration);
    selector->updateDurationSpin();

    //    *play = env.play;
    //    if (*play) {
    //        playButton->onPlayMode();
    //    }
    //    else {
    //        playButton->onPauseMode();
    //    }

    selector->redrawPlayZone();
    ruler->onDrawRuler(ruler->width());

    emit rendered(env.anim);
    qDebug() << "\033[35mrendered(" << env.anim << ")\033[0m";
}

#ifndef QT_NO_DEBUG_OUTPUT
void Session::envSavedTrace()
{
    qDebug() << "\033[35menvSaved()\033[0m";
}
#endif

//void Session::saveFirst()
//{

//}

// ------------------------------- SETTERS ------------------------------------
//void Session::setPlay(bool* value)
//{
//    play = value;
//}

void Session::setKeyPoses(std::set<double>* value)
{
    keyPoses = value;
}

void Session::setDuration(double* value)
{
    duration = value;
}

void Session::setCursor(double* value)
{
    cursor = value;
}

void Session::setEnd(double* value)
{
    end = value;
}

void Session::setStart(double* value)
{
    start = value;
}

void Session::setDurationSpin(QDoubleSpinBoxSmart* value)
{
    durationSpin = value;
}

void Session::setCursorSpin(QDoubleSpinBoxSmart* value)
{
    cursorSpin = value;
}

void Session::setEndSpin(QDoubleSpinBoxSmart* value)
{
    endSpin = value;
}

void Session::setStartSpin(QDoubleSpinBoxSmart* value)
{
    startSpin = value;
}

void Session::setNbKeyPosesSpin(QSpinBoxSmart* value)
{
    nbKeyPosesSpin = value;
}

void Session::setSelector(QFrameSelector* value)
{
    selector = value;
}

void Session::setRuler(QWidgetRuler* value)
{
    ruler = value;
}

void Session::setPlayButton(QToolButtonPlayPause* value)
{
    playButton = value;
}
