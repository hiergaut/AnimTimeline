#include "configurations.h"
//#include <AnimTimeline/configurations.h>
#include "configurations.h"
//#include <AnimTimeline/session.h>
#include "AnimTimelineSession.h"

#include <QDebug>
#include <QTimer>
#include <set>
//#include <boost/circular_buffer.hpp> // need boost lib

AnimTimelineSession::AnimTimelineSession(QObject* parent)
    : QObject(parent)
{
    saveDelay = new QTimer(this);
    connect(saveDelay, &QTimer::timeout, this, &AnimTimelineSession::envSaved);
#ifndef QT_NO_DEBUG_OUTPUT
    connect(this, &AnimTimelineSession::envSaved, this, &AnimTimelineSession::envSavedTrace);
#endif
    saveDelay->setSingleShot(true);
}

AnimTimelineSession::~AnimTimelineSession()
{
    onClearSession();
    delete saveDelay;
}

// timeline changed, save timeline and anim environment
void AnimTimelineSession::onChangeEnv()
{
    qDebug() << "AnimTimelineSession::onChangeEnv()";
    saveDelay->start(AnimTimeline::DELAY_AUTO_SAVE);
}

void AnimTimelineSession::onClearSession()
{
    while (!undo.empty()) {
        emit renderDeleted(undo.back().anim);
        qDebug() << "\033[35mrenderDeleted(" << undo.back().anim << ")\033[0m";
        undo.pop_back();
    }
    while (!redoHeap.empty()) {
        emit renderDeleted(redoHeap.top().anim);
        qDebug() << "\033[35mrenderDeleted(" << redoHeap.top().anim << ")\033[0m";
        redoHeap.pop();
    }

    size = 0;

    onChangeEnv();
}

void AnimTimelineSession::onUndo()
{
    if (undo.empty()) {
        qDebug() << "\033[31mSession::onUndo() : empty buffer ! (suggestion for client : "
                    "you need to catch envSaved to launch onSaveRendering with your anim "
                    "structure due of undo/redo calling, wanted to restore your environment\033[0m";
        return;
    }

    if (undo.size() > 1) {
        redoHeap.push(undo.back());
        undo.pop_back();

        setEnv(undo.back());
    } else {
        qDebug() << "\033[31mSession::onUndo() : last element, can't delete !\033[0m";
    }
}

void AnimTimelineSession::onRedo()
{
    if (redoHeap.empty()) {
        qDebug() << "\033[31mSession::onRedo() : empty stack !\033[0m";
        return;
    }

    undo.emplace_back(redoHeap.top());
    redoHeap.pop();

    setEnv(undo.back());
}

void AnimTimelineSession::onSaveRendering(void* anim, size_t bytes)
{

    while (!redoHeap.empty()) {
        emit renderDeleted(redoHeap.top().anim);
        qDebug() << "\033[35mrenderDeleted(" << redoHeap.top().anim << ")\033[0m";
        size -= redoHeap.top().bytes;
        redoHeap.pop();
    }

    undo.emplace_back(Env { *start, *end, *cursor, *duration, *keyPoses, anim, bytes });
    size += bytes;

    while (size > AnimTimeline::BUFFER_SESSION_MAX_SIZE) {
        qDebug() << "\033[31monSaveRendering : buffer overflow\033[0m";
        emit renderDeleted(undo.front().anim);
        qDebug() << "\033[35mrenderDeleted(" << undo.front().anim << ")\033[0m";
        size -= undo.front().bytes;
        undo.pop_front();
    }

    qDebug() << "AnimTimelineSession::onSaveRendering() : buff size (bytes) =" << size << "/" << AnimTimeline::BUFFER_SESSION_MAX_SIZE << "(" << size * 100.0 / AnimTimeline::BUFFER_SESSION_MAX_SIZE << "% )";
}

void AnimTimelineSession::setEnv(Env env)
{
    *start = env.start;
    selector->updateStartSpin();

    *end = env.end;
    selector->updateEndSpin();

    *keyPoses = env.keyPoses;
    nbKeyPosesSpin->setValue(static_cast<int>(keyPoses->size()));

    selector->updateCursorSpin();

    *duration = env.duration;
    selector->updateDurationSpin();

//    selector->redrawPlayZone();
    ruler->onDrawRuler(ruler->width());

    emit rendered(env.anim);
    qDebug() << "\033[35mrendered(" << env.anim << ")\033[0m";
}

#ifndef QT_NO_DEBUG_OUTPUT
void AnimTimelineSession::envSavedTrace()
{
    qDebug() << "\033[35menvSaved()\033[0m";
}
#endif

void AnimTimelineSession::setKeyPoses(std::set<double>* value)
{
    keyPoses = value;
}

void AnimTimelineSession::setDuration(double* value)
{
    duration = value;
}

void AnimTimelineSession::setCursor(double* value)
{
    cursor = value;
}

void AnimTimelineSession::setEnd(double* value)
{
    end = value;
}

void AnimTimelineSession::setStart(double* value)
{
    start = value;
}

void AnimTimelineSession::setDurationSpin(QDoubleSpinBoxSmart* value)
{
    durationSpin = value;
}

void AnimTimelineSession::setCursorSpin(QDoubleSpinBoxSmart* value)
{
    cursorSpin = value;
}

void AnimTimelineSession::setEndSpin(QDoubleSpinBoxSmart* value)
{
    endSpin = value;
}

void AnimTimelineSession::setStartSpin(QDoubleSpinBoxSmart* value)
{
    startSpin = value;
}

void AnimTimelineSession::setNbKeyPosesSpin(QSpinBoxSmart* value)
{
    nbKeyPosesSpin = value;
}

void AnimTimelineSession::setSelector(QFrameSelector* value)
{
    selector = value;
}

void AnimTimelineSession::setRuler(QWidgetRuler* value)
{
    ruler = value;
}

void AnimTimelineSession::setPlayButton(QToolButtonPlayPause* value)
{
    playButton = value;
}
