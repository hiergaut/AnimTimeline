#include "qframeselector.h"

#include <QDebug>
#include <QPainter>
#include <QWheelEvent>
#include <QtGlobal>

#include "animtimeline.h"

QFrameSelector::QFrameSelector(QWidget* parent)
    : QFrame(parent)
{
    widgetRuler = static_cast<QWidgetRuler*>(parent);
    nbInterval = widgetRuler->getNbInterval();
    step = widgetRuler->getStep();
    pixPerSec = widgetRuler->getPixPerSec();
    zero = widgetRuler->getZero();
    maxDuration = widgetRuler->getMaxDuration();
    //    drawLock =widgetRuler->getSelectorLock();

    qDebug() << "end construct frameSelector, parent : " << parent;
}

double QFrameSelector::nearestStep(double time) const
{
    double deltaT = AUTO_SUGGEST_CURSOR_RADIUS / *pixPerSec;

    //    double pos = 2.0; // random initialize
    //    double
    double minDist = totalDurationSpin->maximum();
    double dist;

    double newCursor = time;
    //    cursor = time;

    for (double keyPose : keyPoses) {
        //        if (keyPose > cursor + AUTO_SUGGEST_CURSOR_RADIUS)
        //            break;

        dist = qAbs(keyPose - time);
        if (dist < deltaT && dist < minDist) {

            //                            pos = keyPose;
            minDist = dist;
            newCursor = keyPose;
            //            cursor = keyPose;
            //            break;
        }
        if (time < keyPose)
            break;
    }

    for (int i = 0; i < *nbInterval - 1; ++i) {
        double pos = i * *step;
        dist = qAbs(pos - time);
        if (dist < deltaT && dist < minDist) {
            minDist = dist;
            newCursor = pos;
        }

        pos = i * *step + 0.5 * *step;
        dist = qAbs(pos - time);
        if (dist < deltaT && dist < minDist) {
            minDist = dist;
            newCursor = pos;
        }

        if (time < pos)
            break;
    }

    //    return std::pair<bool, double>(qAbs(time -newCursor) > 1e-5, newCursor);
    //    bool found = qAbs(time -newCursor) > 1e-5;
    //    if (found)
    //        time =newCursor;

    return newCursor;
}

//
// ------------------------------- PROTECTED ----------------------------------
void QFrameSelector::paintEvent(QPaintEvent*)
{
    //    (void)event;
    //    qDebug() << ev;
    //    qDebug() << "QFrameSelector::paintEvent";

    //    if (! *drawLock) {
    qDebug() << "QFrameSelector::paintEvent " << ++paintCounter;

    QPainter painter(this);
    int h = height();
    int w = width();

    if (!sliding) {

        leftSpacer->setMinimumWidth(static_cast<int>(*zero + start * *pixPerSec - leftSlider->width()));
        playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
    }

    int hDown = 2 * h / 3;
    //    painter.setPen(Qt::darkGray);
    painter.setPen(Qt::black);
    for (int i = 1; i < *nbInterval; i++) {
        int x = static_cast<int>(i * *step * *pixPerSec);
        painter.drawLine(x, hDown, x, h);
    }

    int hDown2 = 3 * h / 4;
    //    painter.setPen(QPen(Qt::lightGray));
    painter.setPen(Qt::darkGray);
    for (int i = 1; i < *nbInterval - 1; i++) {
        //        int x = static_cast<int>(i * *step * *pixPerSec);
        //        int middle = static_cast<int>(x + *zero / 2);
        int middle = static_cast<int>((i + 0.5) * *step * *pixPerSec);
        painter.drawLine(middle, hDown2, middle, h);
    }

    //    painter.setPen(QPen(Qt::blue));
    //    painter.setPen(QPen(Qt::white));
    painter.setPen(QPen(Qt::lightGray));
    double frameDuration = 1.0 / FPS;
    int hUp = h / 3;
    double nbFrame = *maxDuration / frameDuration;
    for (int i = 0; i < nbFrame; i++) {
        int x = static_cast<int>(i * frameDuration * *pixPerSec + *zero);
        painter.drawLine(x, 0, x, hUp);
    }

    int gap = 5;
    painter.setPen(Qt::white);
//    painter.drawLine(0, h / 2 + 1, w, h / 2 + 1);
    painter.drawLine(0, h / 2 - gap -1, w, h / 2 -gap -1);
    painter.drawLine(0, h / 2 + gap +2, w, h / 2 +gap +2);

    //    int xLine = 10;
    //    int radius = 5;
    //    painter.setPen(Qt::black);
    ////    painter.setPen(Qt::black);
    //    painter.setBrush(Qt::darkGray);
    //    painter.drawEllipse(xLine, h /2 - radius, 2 *radius, 2 * radius);

    painter.setPen(Qt::darkGray);
    //    painter.setPen(Qt::black);
    //    painter.drawLine(0, 0, 20, h /2 -gap);
    //    painter.drawLine(xLine +2 *radius, h /2 -gap, w, h /2 -gap);
    //    painter.drawLine(xLine +2 *radius, h /2 +gap, w, h /2 +gap);
    painter.drawLine(0, h / 2 - gap +1, w, h / 2 - gap +1);
//    painter.drawLine(0, h / 2 - gap + 2, w, h / 2 - gap + 2);
    painter.drawLine(0, h / 2 + gap + 1, w, h / 2 + gap + 1);

//    painter.setPen(Qt::lightGray);
//    painter.drawLine(0, h / 2 - gap +2, w, h / 2 - gap +2);

    painter.setPen(Qt::black);
    //    gap = 3;
    painter.drawLine(0, h / 2 - gap, w, h / 2 - gap);

//    painter.drawLine(0, h / 2 - gap +1, w, h / 2 - gap +1);
    //    painter.drawLine(0, h /2 +gap, w, h /2 +gap);

    //    painter.drawLine(0, h, 20, h /2 +gap);

    //    int hStep = h /10;
    //    int vMiddle = h /2;
    //    int vStep = 4;
    //    int hRight = 0.60 * *zero;
    //    for (int i =0; i <5; ++i) {
    //        int v = vMiddle +vStep *i;
    //        painter.drawLine(0, v, hRight, v);

    //        v = vMiddle -vStep *i;
    //        painter.drawLine(0, v, hRight, v);

    //    }

    painter.setPen(QPen(QColor(0, 0, 255, 255), 3));
    int xCursor = static_cast<int>(*zero + cursor * *pixPerSec);
    painter.drawLine(xCursor, 0, xCursor, h);

    painter.setPen(QPen(Qt::yellow, 3));
    for (double keyPose : keyPoses) {
        int xKeyPose = static_cast<int>(*zero + keyPose * *pixPerSec);
        painter.drawLine(xKeyPose, 30, xKeyPose, h);
    }

    //        *drawLock =true;
    //    }
}

void QFrameSelector::mousePressEvent(QMouseEvent* event)
{
    // ---------------------- LEFT CLICK --------------------------------------
    if (event->button() == Qt::LeftButton) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        qDebug() << "QFrameSelector::mousePressEvent : newCursor = " << newCursor;
        //        onChangeCursor(newCursor, true);
        if (onInternalChangeCursor(newCursor))
            emit cursorChanged(cursor); // EXTERNAL SIGNAL

        mouseLeftClicked = true;

        //
        // ------------------ RIGHT CLICK -------------------------------------
    } else if (event->button() == Qt::RightButton) {
        double newPose = qMax((event->x() - *zero) / *pixPerSec, 0.0);

        auto it = keyPoses.find(cursor);

        // if already on keyPose, move current keyPose
        // ------------------- CURSOR ON KEYPOSE -----------------------
        if (it != keyPoses.end()) {

            // -------------- SINGLE MOVE -------------------------------------
            if (*shiftDown) {
                double nearest = nearestStep(newPose);
                // if no keyPose under mouse, move keyPose to newPose
                if (keyPoses.find(nearest) == keyPoses.end()) {

                    if (onInternalChangeCursor(nearest, false)) {

                        int num = static_cast<int>(std::distance(keyPoses.begin(), it));
                        keyPoses.erase(it);
                        keyPoses.insert(cursor);

                        updateCursorSpin();

                        //                onChangeCursor(newPose);
                        emit keyPoseMoved(num, cursor); // EXTERNAL SIGNAL
                    }
                }

                // ---------- MULTIPLE MOVE -----------------------------------
            } else {
                // if no keyPose under mouse, move keyPose to newPose
                //                if (keyPoses.find(newPose) == keyPoses.end()) {
                auto itLeft = it;
                --itLeft;
                double left = (it == keyPoses.begin()) ? (0.0) : (*itLeft);

                if (newPose > left) {
                    double dist = newPose - cursor;
                    int num = static_cast<int>(std::distance(keyPoses.begin(), it));
                    moveKeyPoses(dist, num);

                    //                        widgetRuler->setMaxDuration(*maxDuration + dist);
                    //                        updateDurationSpin();
                }
                //                }
            }

            // ---------------- CURSOR NOT ON KEYPOSE --------------------------
        } else {

            // if shiftdown, slide first right keypose to the left
            // --------------- MOVE RIGHT KEYPOSE TO THE LEFT -----------------
            if (*shiftDown) {
                auto it = keyPoses.begin();
                int iRight = 0;
                while (it != keyPoses.end() && *it < newPose) {
                    ++it;
                    ++iRight;
                }

                // if keyPoses on the right, remove or insert time
                if (it != keyPoses.end()) {

                    double right = *it;
                    double dist = newPose - right;
                    //                    double gap = (*shiftDown) ? (-dist) : (dist);
                    //                    if (start > newPose) {
                    //                        start += dist;
                    //                        updateStartSpin();
                    //                    }

                    //                    if (end > newPose) {
                    //                        end += dist;
                    //                        updateEndSpin();
                    //                    }

                    //                    if (cursor > newPose) {
                    //                        cursor += dist;
                    //                        updateCursorSpin();
                    //                    }
                    moveKeyPoses(dist, iRight);

                    //                    widgetRuler->setMaxDuration(*maxDuration + dist);
                    //                    updateDurationSpin();
                }

                // if not shiftdown, slide first left keypose to the right
                // ---------------- MOVE LEFT KEYPOSE TO THE RIGHT -----------
            } else {
                auto it = keyPoses.rbegin();
                int iLeft = static_cast<int>(keyPoses.size() - 1);
                while (it != keyPoses.rend() && *it > newPose) {
                    ++it;
                    --iLeft;
                }

                if (it != keyPoses.rend()) {
                    double left = *it;
                    double dist = newPose - left;

                    //                    if (start > left) {
                    //                        start += dist;
                    //                        updateStartSpin();
                    //                    }

                    //                    if (end > left) {
                    //                        end += dist;
                    //                        updateEndSpin();
                    //                    }

                    //                    if (cursor > left) {
                    //                        cursor += dist;
                    //                        updateCursorSpin();
                    //                    }
                    moveKeyPoses(dist, iLeft);

                    //                    widgetRuler->setMaxDuration(*maxDuration + dist);
                    //                    updateDurationSpin();
                }
            }
        }
        //        update();

        // no catch mouse event
    } else {
        event->ignore();
        return;
    }
    event->accept();
}

void QFrameSelector::mouseMoveEvent(QMouseEvent* event)
{
    if (mouseLeftClicked) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);

        //        onChangeCursor(newCursor, true);
        if (onInternalChangeCursor(newCursor))
            emit cursorChanged(cursor); // EXTERNAL SIGNAL
    } else {
        event->ignore();
    }
}

void QFrameSelector::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mouseLeftClicked = false;
        event->accept();

    } else {
        event->ignore();
    }
}

//void QFrameSelector::wheelEvent(QWheelEvent* event)
//{
//    if (*shiftDown) {
//    }
//}

//void QFrameSelector::keyPressEvent(QKeyEvent *event)
//{
//    qDebug() << "keyPress" << endl;

//    if (event->key() == Qt::Key_Shift) {
//        shiftDown = true;
//    }
//}

//void QFrameSelector::keyReleaseEvent(QKeyEvent *event)
//{
//    if (event->key() == Qt::Key_Shift) {
//        shiftDown = false;
//    }

//}

//
// -------------------------- EXTERNAL SLOTS ----------------------------------
// EXTERNAL SLOT
void QFrameSelector::onAddingKeyPose(double time)
{
    // by default (time = -1.0), add keyPose on cursor
    if (static_cast<int>(time) == -1)
        time = cursor;

    int nbKeyPoses = static_cast<int>(keyPoses.size());
    keyPoses.insert(time);

    // if keyPose not already here
    if (static_cast<int>(keyPoses.size()) != nbKeyPoses) {
        updateCursorSpin();
        update();

        //        emit nbKeyPosesChanged(static_cast<int>(keyPoses.size()));

        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));

        // keyPose already here, do not changing actual keyPose
    } else {
        qDebug() << "\033[31mQFrameSelector::onAddingKeyPose(" << time << ") : can't insert keyPose, keyPose already here\033[0m";
    }
}

// EXTERNAL SLOT
void QFrameSelector::onClearKeyPoses()
{
    keyPoses.clear();
    nbKeyPosesSpin->setValue(0);

    updateCursorSpin();
    update();
}

// EXTERNAL SLOT
void QFrameSelector::onChangeStart(double time, bool internal /* = true */)
{
    double newStart = qMax(qMin(time, end), 0.0);

    if (qAbs(newStart - start) > 1e-5) {
        start = newStart;
        updateStartSpin();
        update();

        // emit signal if time of emitter is internal changed due of limits
        if (internal || qAbs(time -start) > 1e-5)
            emit startChanged(start); // EXTERNAL SIGNAL
    }
}

// EXTERNAL SLOT
void QFrameSelector::onChangeEnd(double time, bool internal /* = true */)
{
    double newEnd = qMin(qMax(time, start), *maxDuration);

    // emit signal only if new value of end
    if (qAbs(newEnd - end) > 1e-5) {
        end = newEnd;
        updateEndSpin();
        update();

        if (internal || qAbs(time -end) > 1e-5)
            emit endChanged(end); // EXTERNAL SIGNAL
    }
}

// EXTERNAL SLOT (warning on using EXTERNAL SIGNAL)
void QFrameSelector::onChangeCursor(double time, bool internal /* = true */)
{
    //    if (findNearestKeyPose) {
    //        double pos = 2.0; // random initialize
    //        double min = 999.0;
    //        double dist;
    //        //    cursor = time;
    //        for (double keyPose : keyPoses) {
    //            //        if (keyPose > cursor + AUTO_SUGGEST_CURSOR_RADIUS)
    //            //            break;

    //            dist = qAbs(keyPose - time);
    //            if (dist < AUTO_SUGGEST_CURSOR_RADIUS) {

    //                if (min > dist) {
    //                    pos = keyPose;
    //                    min = dist;
    //                }
    //            }
    //        }

    //        cursor = (min != 999.0) ? (pos) : (time);
    //    } else {
    //        cursor = time;
    //    }
    double newCursor = qMax(0.0, time);
    if (qAbs(newCursor - cursor) > 1e-5) {
        cursor = newCursor;
        updateCursorSpin();
        update();

        if (internal || qAbs(time -end) > 1e-5)
            emit cursorChanged(cursor); // EXTERNAL SIGNAL
    }
}

// EXTERNAL SLOT
void QFrameSelector::onChangeDuration(double time, bool internal /* = true */)
{
    //    double newDuration = totalDurationSpin->value();

    double newStart = qMin(start, time);
    if (qAbs(newStart - start) > 1e-5) {
        start = newStart;
        updateStartSpin();

        if (internal || qAbs(time -start) > 1e-5)
            emit startChanged(start); // EXTERNAL SIGNAL
    }

    double newEnd = qMin(qMax(endSpin->value(), end), time);
    if (qAbs(newEnd - end) > 1e-5) {
        end = newEnd;
        updateEndSpin();

        if (internal || qAbs(time -end) > 1e-5)
            emit endChanged(end); // EXTERNAL SIGNAL
    }

    widgetRuler->setMaxDuration(time); // emit external signal
    // auto update
}

//
// -------------------------- INTERNAL SLOTS ----------------------------------
void QFrameSelector::onSlideLeftSlider(int deltaX)
{

    if (!sliding) {
        leftSlider->setStyleSheet("background-color: #00ff00");
        sliding = true;
    }

    double newStart = start + deltaX / *pixPerSec;
    start = qMin(qMax(newStart, 0.0), end);

    onChangeStart(start); // EXTERNAL SLOT

    leftSpacer->setMinimumWidth(static_cast<int>(*zero + start * *pixPerSec - leftSlider->width()));
    playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
}

void QFrameSelector::onSlideRightSlider(int deltaX)
{
    if (!sliding) {
        rightSlider->setStyleSheet("background-color: red");
        sliding = true;
    }
    double newEnd = end + deltaX / *pixPerSec;
    end = qMin(qMax(newEnd, start), *maxDuration);

    onChangeEnd(end); // EXTERNAL SLOT

    playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
}

void QFrameSelector::onSlideRelease()
{
    sliding = false;
}

void QFrameSelector::onInternalAddingKeyPose(double time)
{
    // by default (time = -1.0), add keyPose on cursor
    if (static_cast<int>(time) == -1)
        time = cursor;

    int nbKeyPoses = static_cast<int>(keyPoses.size());
    keyPoses.insert(time);

    // if keyPose not already here
    if (static_cast<int>(keyPoses.size()) != nbKeyPoses) {
        updateCursorSpin();
        update();

        //        emit nbKeyPosesChanged(static_cast<int>(keyPoses.size()));
        emit keyPoseAdded(time); // EXTERNAL SIGNAL

        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));

        // keyPose already here, change actual keyPose
    } else {
        auto it = keyPoses.find(time);

        emit keyPoseChanged(static_cast<int>(std::distance(keyPoses.begin(), it))); // EXTERNAL SIGNAL
    }
}

void QFrameSelector::onDeleteKeyPose()
{
    auto it = keyPoses.find(cursor);

    if (it != keyPoses.end()) {

        int num = static_cast<int>(std::distance(keyPoses.begin(), it));
        keyPoses.erase(it);

        updateCursorSpin();
        update();

        //        emit nbKeyPosesChanged(static_cast<int>(keyPoses.size()));
        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));
        emit keyPoseDeleted(num); // EXTERNAL SIGNAL

        onSetCursorToNextKeyPose();
    }
}

bool QFrameSelector::onInternalChangeCursor(double time, bool findNearestStep /* = true */)
{
    //    if (findNearestKeyPose) {
    //    int pixelTime =static_cast<int>(*zero +time * *pixPerSec);
    double newCursor { time };

    if (findNearestStep)
        newCursor = nearestStep(time);

    if (qAbs(cursor - newCursor) < 1e-5)
        return false;

    //    if (! nearestStep(time))
    //        return false;

    cursor = newCursor;
    //    cursor = (min != 999.0) ? (pos) : (time);
    //    } else {
    //        cursor = time;
    //    }
    updateCursorSpin();
    update();
    return true;
}

void QFrameSelector::onSetCursorToStart()
{
    //    onChangeCursor(start);
    if (onInternalChangeCursor(start))
        emit cursorChanged(cursor); // EXTERNAL SIGNAL
}

void QFrameSelector::onSetCursorToEnd()
{
    //    onChangeCursor(end);
    if (onInternalChangeCursor(end))
        emit cursorChanged(cursor); // EXTERNAL SIGNAL
}

void QFrameSelector::onSetCursorToPreviousKeyPose()
{
    auto it = keyPoses.rbegin();
    while (it != keyPoses.rend() && *it >= cursor)
        it++;

    if (it != keyPoses.rend()) {
        //        onChangeCursor(*it);
        if (onInternalChangeCursor(*it))
            emit cursorChanged(cursor); // EXTERNAL SIGNAL
    }
}

void QFrameSelector::onSetCursorToNextKeyPose()
{
    auto it = keyPoses.begin();
    while (it != keyPoses.end() && *it <= cursor)
        it++;

    if (it != keyPoses.end()) {
        if (onInternalChangeCursor(*it))
            emit cursorChanged(cursor); // EXTERNAL SIGNAL
        //        cursor = *it;

        //        updateCursorSpin();
        //        update();
    }
}

//void QFrameSelector::onPlay()
//{
//}

//void QFrameSelector::onPause()
//{
//}

void QFrameSelector::onChangeStartSpin()
{
    onChangeStart(startSpin->value());
//    double newStart = qMax(qMin(startSpin->value(), end), 0.0);
//    double diff = qAbs(start - newStart);

//    start = newStart;
//    updateStartSpin();

//    if (diff > 1e-5) {
//        update();
//        emit startChanged(start); // EXTERNAL SIGNAL
//    }
}

void QFrameSelector::onChangeEndSpin()
{
    onChangeEnd(endSpin->value());
    //    double newEnd = qMin(qMax(endSpin->value(), start), *maxDuration);

    //    double diff = qAbs(end - newEnd);
    //    //    if (qAbs(end -newEnd) < 1e-5) {
    //    //        return;
    //    //    }

    //    end = newEnd;
    //    updateEndSpin();

    //    if (diff > 1e-5) {
    //        update();

    //        emit endChanged(end); // EXTERNAL SIGNAL
    //    }
}

void QFrameSelector::onChangeCursorSpin()
{
    onChangeCursor(cursorSpin->value());
    //    onChangeCursor(cursorSpin->value());
//    if (onInternalChangeCursor(cursorSpin->value()))
//        emit cursorChanged(cursor); // EXTERNAL SIGNAL
}

void QFrameSelector::onChangeDurationSpin()
{
    onChangeDuration(totalDurationSpin->value());
//    totalDurationSpin->setValue(*maxDuration);
}

//void QFrameSelector::onStartIncPlus()
//{
//    double gap = startInc->value();

//    start += gap;
//    updateStartSpin();

//    cursor += gap;
//    updateCursorSpin();

//    end += gap;
//    updateEndSpin();

//    moveKeyPoses(gap);
//    widgetRuler->setMaxDuration(*maxDuration + gap);

//    updateDurationSpin();
//}

//void QFrameSelector::onStartIncLess()
//{
//    double gap = startInc->value();

//    start = qMax(0.0, start - gap);
//    updateStartSpin();
//    cursor = qMax(0.0, cursor - gap);
//    updateCursorSpin();
//    end = qMax(0.0, end - gap);
//    updateEndSpin();

//    moveKeyPoses(-gap);
//    widgetRuler->setMaxDuration(qMax(0.0, *maxDuration - gap));
//    updateDurationSpin();
//}

//void QFrameSelector::onEndIncPlus()
//{
//    double gap = endInc->value();

//    widgetRuler->setMaxDuration(*maxDuration + gap);
//    updateDurationSpin();
//}

//void QFrameSelector::onEndIncLess()
//{
//    double gap = endInc->value();

//    end = qMin(*maxDuration - gap, end);
//    updateEndSpin();

//    cursor = qMin(*maxDuration - gap, cursor);
//    updateCursorSpin();

//    widgetRuler->setMaxDuration(qMax(0.0, *maxDuration - gap));
//    updateDurationSpin();
//}

//void QFrameSelector::onCursorChanged(double time)
//{
//    cursor = time;
//    updateCursorSpin();
//    update();
//}

//
// -------------------------- PRIVATE FUNCTIONS -------------------------------
void QFrameSelector::updateCursorSpin()
{
    if (keyPoses.find(cursor) != keyPoses.end()) {
        cursorSpin->setStyleSheet("background-color: yellow");
        removeKeyPoseButton->setEnabled(true);
    } else {
        cursorSpin->setStyleSheet("background-color: #5555ff");
        removeKeyPoseButton->setEnabled(false);
    }
    cursorSpin->setValue(cursor);
}

void QFrameSelector::updateStartSpin()
{
    startSpin->setValue(start);
}

void QFrameSelector::updateEndSpin()
{
    endSpin->setValue(end);
}

void QFrameSelector::moveKeyPoses(double gap, int iFirst)
{
    std::set<double> clone;
    int i = 0;
    double first { 0 };
    for (double d : keyPoses) {
        if (i < iFirst)
            clone.insert(d);
        else {
            if (i == iFirst)
                first = d;

            clone.insert(d + gap);
        }
        i++;
    }
    keyPoses = clone;

    double left = (gap > 0) ? (first) : (first + gap);

    // emit keyPosesMoved before emitting cursorChanged to render the truly pose on cursor
    emit keyPosesMoved(gap, iFirst); // EXTERNAL SIGNAL

    if (start >= left) {
        //        start += gap;
        start = qMax(qMin(start + gap, *maxDuration), 0.0);
        updateStartSpin();

        emit startChanged(start);
    }

    if (end >= left) {
        //        end += gap;
        end = qMax(qMin(end + gap, *maxDuration), 0.0);
        updateEndSpin();

        emit endChanged(end);
    }

    if (cursor >= left) {
        bool cursorOnKeyPose = qAbs(cursor - first) < 1e-5;

        cursor = qMax(cursor + gap, 0.0);
        updateCursorSpin();

        if (!cursorOnKeyPose)
            emit cursorChanged(cursor);
    }

    //    widgetRuler->setMaxDuration(*maxDuration + gap);
    //    updateDurationSpin();
    update();
}

//void QFrameSelector::updatePlayZone()
//{
//    start = 0;
//    updateStartSpin();

//    end = *maxDuration;
//    updateEndSpin();
//}

// -------------------------- GETTERS -----------------------------------------
double QFrameSelector::getStart() const
{
    return start;
}

double QFrameSelector::getEnd() const
{
    return end;
}

double QFrameSelector::getCursor()
{
    return cursor;
}

int QFrameSelector::getNbKeyPoses() const
{
    return static_cast<int>(keyPoses.size());
}

double QFrameSelector::getKeyPose(int id) const
{
    auto it = keyPoses.begin();
    while (it != keyPoses.end() && id-- > 0)
        it++;

    return *it;
}

std::set<double> QFrameSelector::getKeyPoses() const
{
    return keyPoses;
}

//
// -------------------------- SETTERS -----------------------------------------
void QFrameSelector::setCursor(double time)
{
    cursor = time;
    //    updateCursorSpin();
    //    update();
}

void QFrameSelector::setKeyPoses(const std::set<double>& value)
{
    keyPoses = value;
}

void QFrameSelector::setShiftDown(bool* value)
{
    shiftDown = value;
}

//void QFrameSelector::setDrawLock(bool *value)
//{
//    drawLock = value;
//}

void QFrameSelector::setStart(double value)
{
    start = value;
}

void QFrameSelector::setEnd(double value)
{
    end = value;
}

//
// -------------------------- REFERENCES SETTERS ------------------------------
void QFrameSelector::setLeftSpacer(QFrame* value)
{
    leftSpacer = value;
}

void QFrameSelector::setLeftSlider(QLabel* value)
{
    leftSlider = value;
}

void QFrameSelector::setPlayZone(QFrame* value)
{
    playZone = value;
}

void QFrameSelector::setRightSlider(QLabel* value)
{
    rightSlider = value;
}
//

void QFrameSelector::setCursorSpin(QDoubleSpinBox* value)
{
    cursorSpin = value;
}

void QFrameSelector::setStartSpin(QDoubleSpinBox* value)
{
    startSpin = value;
}

void QFrameSelector::setEndSpin(QDoubleSpinBox* value)
{
    endSpin = value;
}

void QFrameSelector::setTotalDurationSpin(QDoubleSpinBox* value)
{
    totalDurationSpin = value;
}
//

void QFrameSelector::setRemoveKeyPoseButton(QToolButton* value)
{
    removeKeyPoseButton = value;
}

void QFrameSelector::setStartInc(QDoubleSpinBox* value)
{
    startInc = value;
}

void QFrameSelector::setEndInc(QDoubleSpinBox* value)
{
    endInc = value;
}

void QFrameSelector::setNbKeyPosesSpin(QSpinBox* value)
{
    nbKeyPosesSpin = value;
}
