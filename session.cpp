#include "session.h"

#include <QDebug>
#include <QTimer>
#include <set>
//#include <boost/circular_buffer.hpp> // need boost lib

Session::Session()
{
    saveDelay = new QTimer(this);
    connect(saveDelay, SIGNAL(timeout()), this, SLOT(saveEnv()));
    saveDelay->setSingleShot(true);
}

Session::~Session()
{
    delete saveDelay;
}

// timeline changed, save timeline and anim environment
void Session::onSaveEnv()
{
    saveDelay->start(100);
}

void Session::onClearSession()
{
    while (!undoHeap.empty())
        undoHeap.pop();
    while (!redoHeap.empty())
        redoHeap.pop();

    emit sessionCleared();
}

void Session::onUndo()
{
    if (undoHeap.empty()) {
        qDebug() << "\033[31mSession::onUndo() : empty stack !\033[0m";
        return;
    }
    //    if (redoHeap.empty()) {
    //        redoHeap.push(undoHeap.top());
    //        undoHeap.pop();
    //    }

    if (undoHeap.size() > 1) {
        redoHeap.push(undoHeap.top());
        undoHeap.pop();

        setEnv(undoHeap.top());
        emit undid();
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

    undoHeap.push(redoHeap.top());
    redoHeap.pop();

    //    if (!redoHeap.empty()) {
    setEnv(undoHeap.top());
    emit redid();
    //    } else {
    //        qDebug() << "\033[31mSession::onRedo() : empty stack !\033[0m";
    //    }

    //    if (undoHeap.empty()) {
    //        undoHeap.push(redoHeap.top());
    //        redoHeap.pop();
    //    }

    //    if (!redoHeap.empty()) {
    //        undoHeap.push(redoHeap.top());
    //        redoHeap.pop();

    //        setEnv(undoHeap.top());
    //        emit redid();
    //    }
}

void Session::saveEnv()
{
    //    undoHeap.push({ *start, *end, *cursor, *duration, *keyPoses, *play });
    undoHeap.push({ *start, *end, *cursor, *duration, *keyPoses });
    while (!redoHeap.empty())
        redoHeap.pop();

    emit envSaved();
}

void Session::setNbKeyPosesSpin(QSpinBoxSmart *value)
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
}

// ############################### SETTERS
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
