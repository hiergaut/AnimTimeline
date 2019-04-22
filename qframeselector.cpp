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

void QFrameSelector::paintEvent(QPaintEvent*)
{
    //    (void)event;
    //    qDebug() << ev;
    //    qDebug() << "QFrameSelector::paintEvent";

    //    if (! *drawLock) {
    qDebug() << "QFrameSelector::paintEvent " << ++paintCounter;

    QPainter painter(this);

    if (!sliding) {

        leftSpacer->setMinimumWidth(static_cast<int>(*zero + start * *pixPerSec - leftSlider->width()));
        playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
    }

    painter.setPen(Qt::darkGray);
    for (int i = 1; i < *nbInterval; i++) {
        int x = static_cast<int>(i * *step * *pixPerSec);
        painter.drawLine(x, 0, x, height());
    }

    painter.setPen(QPen(Qt::lightGray));
    for (int i = 1; i < *nbInterval - 1; i++) {
        int x = static_cast<int>(i * *step * *pixPerSec);
        int middle = static_cast<int>(x + *zero / 2);
        painter.drawLine(middle, 0, middle, height());
    }

    painter.setPen(QPen(QColor(0, 0, 255, 255), 3));
    int xCursor = static_cast<int>(*zero + cursor * *pixPerSec);
    painter.drawLine(xCursor, 0, xCursor, height());

    painter.setPen(QPen(Qt::yellow, 3));
    for (double keyPose : keyPoses) {
        int xKeyPose = static_cast<int>(*zero + keyPose * *pixPerSec);
        painter.drawLine(xKeyPose, 30, xKeyPose, height());
    }

    //        *drawLock =true;
    //    }
}

void QFrameSelector::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        qDebug() << "QFrameSelector::mousePressEvent : newCursor = " << newCursor;
        //        onChangeCursor(newCursor, true);
        if (onInternalChangeCursor(newCursor))
            emit cursorChanged(cursor); // EXTERNAL SIGNAL

        mouseLeftClicked = true;

    } else if (event->button() == Qt::RightButton) {
        double newPose = qMax((event->x() - *zero) / *pixPerSec, 0.0);

        auto it = keyPoses.find(cursor);

        // if already on keyPose, move current keyPose
        if (it != keyPoses.end()) {

            if (*shiftDown) {
                // if no keyPose under mouse, move keyPose to newPose
                if (keyPoses.find(newPose) == keyPoses.end()) {

                    if (onInternalChangeCursor(newPose)) {

                        int num = static_cast<int>(std::distance(keyPoses.begin(), it));
                        keyPoses.erase(it);
                        keyPoses.insert(cursor);

                        updateCursorSpin();

                        //                onChangeCursor(newPose);
                        emit keyPoseMoved(num, cursor);
                    }
                }
            } else {
                // if no keyPose under mouse, move keyPose to newPose
                if (keyPoses.find(newPose) == keyPoses.end()) {
                    auto itLeft = --it;
                    double left = (it == keyPoses.begin()) ? (0.0) : (*itLeft);

                    if (newPose > left) {
                        double dist = newPose - cursor;
                        int num = static_cast<int>(std::distance(keyPoses.begin(), it));
                        updateKeyPoses(dist, num);

//                        widgetRuler->setMaxDuration(*maxDuration + dist);
//                        updateDurationSpin();
                    }
                }
            }

            // not on keyPose
        } else {

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
                    updateKeyPoses(dist, iRight);

//                    widgetRuler->setMaxDuration(*maxDuration + dist);
//                    updateDurationSpin();
                }

            } else {
                auto it = keyPoses.rbegin();
                size_t iLeft = keyPoses.size() - 1;
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
                    updateKeyPoses(dist, iLeft);

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
            emit cursorChanged(cursor);
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

void QFrameSelector::onSlideLeftSlider(int deltaX)
{

    if (!sliding) {
        leftSlider->setStyleSheet("background-color: #00ff00");
        sliding = true;
    }

    double newStart = start + deltaX / *pixPerSec;
    start = qMin(qMax(newStart, 0.0), end);

    onChangeStart(start);

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

    onChangeEnd(end);

    playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
}

void QFrameSelector::onSlideRelease()
{
    sliding = false;
}

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
        emit keyPoseAdded(time);

        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));

        // keyPose already here, change actual keyPose
    } else {
        auto it = keyPoses.find(time);

        emit keyPoseChanged(static_cast<int>(std::distance(keyPoses.begin(), it)));
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
        emit keyPoseDeleted(num);

        onSetCursorToNextKeyPose();
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
void QFrameSelector::onChangeStart(double time)
{
    start = qMax(qMin(time, end), 0.0);
    updateStartSpin();
    update();

    emit startChanged(start);
}

// EXTERNAL SLOT
void QFrameSelector::onChangeEnd(double time)
{
    end = qMin(qMax(time, start), *maxDuration);

    updateEndSpin();
    update();

    emit endChanged(end);
}

// EXTERNAL SLOT (warning on using EXTERNAL SIGNAL)
void QFrameSelector::onChangeCursor(double time)
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
    cursor = time;
    updateCursorSpin();
    update();
}

double QFrameSelector::nearestStep(double time)
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

    for (int i = 1; i < *nbInterval; i++) {
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

    return newCursor;
}

bool QFrameSelector::onInternalChangeCursor(double time)
{
    //    if (findNearestKeyPose) {
    //    int pixelTime =static_cast<int>(*zero +time * *pixPerSec);
    double newCursor = nearestStep(time);

    if (qAbs(cursor - newCursor) < 1e-5)
        return false;

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
        emit cursorChanged(cursor);
}

void QFrameSelector::onSetCursorToEnd()
{
    //    onChangeCursor(end);
    if (onInternalChangeCursor(end))
        emit cursorChanged(cursor);
}

void QFrameSelector::onSetCursorToPreviousKeyPose()
{
    auto it = keyPoses.rbegin();
    while (it != keyPoses.rend() && *it >= cursor)
        it++;

    if (it != keyPoses.rend()) {
        //        onChangeCursor(*it);
        if (onInternalChangeCursor(*it))
            emit cursorChanged(cursor);
    }
}

void QFrameSelector::onSetCursorToNextKeyPose()
{
    auto it = keyPoses.begin();
    while (it != keyPoses.end() && *it <= cursor)
        it++;

    if (it != keyPoses.end()) {
        if (onInternalChangeCursor(*it))
            emit cursorChanged(cursor);
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
    double newStart = qMax(qMin(startSpin->value(), end), 0.0);
    double diff = qAbs(start - newStart);

    start = newStart;
    updateStartSpin();

    if (diff > 1e-5) {
        update();
        emit startChanged(start);
    }
}

void QFrameSelector::onChangeEndSpin()
{
    double newEnd = qMin(qMax(endSpin->value(), start), *maxDuration);

    double diff = qAbs(end - newEnd);
    //    if (qAbs(end -newEnd) < 1e-5) {
    //        return;
    //    }

    end = newEnd;
    updateEndSpin();

    if (diff > 1e-5) {
        update();

        emit endChanged(end);
    }
}

void QFrameSelector::onChangeCursorSpin()
{
    //    onChangeCursor(cursorSpin->value());
    if (onInternalChangeCursor(cursorSpin->value()))
        emit cursorChanged(cursor);
}

void QFrameSelector::onChangeDuration()
{
    double newDuration = totalDurationSpin->value();

    double newStart = qMin(start, newDuration);
    if (qAbs(newStart - start) > 1e-5) {
        start = newStart;
        updateStartSpin();
        emit startChanged(start);
    }

    double newEnd = qMin(qMax(endSpin->value(), end), newDuration);
    if (qAbs(newEnd - end) > 1e-5) {
        end = newEnd;
        updateEndSpin();
        emit endChanged(end);
    }

    widgetRuler->setMaxDuration(newDuration); // emit external signal
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

//    updateKeyPoses(gap);
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

//    updateKeyPoses(-gap);
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

void QFrameSelector::onCursorChanged(double time)
{
    cursor = time;
    updateCursorSpin();
    update();
}

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

void QFrameSelector::updateKeyPoses(double gap, size_t iFirst)
{
    std::set<double> clone;
    size_t i = 0;
    double first;
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

    double left = (gap > 0) ? (first) : (first - gap);

    if (start > left) {
        start += gap;
        updateStartSpin();
    }

    if (end > left) {
        end += gap;
        updateEndSpin();
    }

    if (cursor > left) {
        cursor += gap;
        updateCursorSpin();
    }

    widgetRuler->setMaxDuration(*maxDuration + gap);
    updateDurationSpin();

    emit keyPosesMoved(gap, iFirst);
}

void QFrameSelector::setEnd(double value)
{
    end = value;
}

void QFrameSelector::setStart(double value)
{
    start = value;
}

//void QFrameSelector::setDrawLock(bool *value)
//{
//    drawLock = value;
//}

void QFrameSelector::setShiftDown(bool* value)
{
    shiftDown = value;
}

void QFrameSelector::setNbKeyPosesSpin(QSpinBox* value)
{
    nbKeyPosesSpin = value;
}

// EXTERNAL SLOT
void QFrameSelector::updateDurationSpin()
{
    totalDurationSpin->setValue(*maxDuration);
}

void QFrameSelector::setTotalDurationSpin(QDoubleSpinBox* value)
{
    totalDurationSpin = value;
}

void QFrameSelector::setEndInc(QDoubleSpinBox* value)
{
    endInc = value;
}

void QFrameSelector::setStartInc(QDoubleSpinBox* value)
{
    startInc = value;
}

std::set<double> QFrameSelector::getKeyPoses() const
{
    return keyPoses;
}

void QFrameSelector::setKeyPoses(const std::set<double>& value)
{
    keyPoses = value;
}

void QFrameSelector::setRemoveKeyPoseButton(QToolButton* value)
{
    removeKeyPoseButton = value;
}

double QFrameSelector::getEnd() const
{
    return end;
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

double QFrameSelector::getStart() const
{
    return start;
}

void QFrameSelector::setEndSpin(QDoubleSpinBox* value)
{
    endSpin = value;
}

void QFrameSelector::setCursor(double time)
{
    cursor = time;
    //    updateCursorSpin();
    //    update();
}

//void QFrameSelector::updatePlayZone()
//{
//    start = 0;
//    updateStartSpin();

//    end = *maxDuration;
//    updateEndSpin();
//}

double QFrameSelector::getCursor()
{
    return cursor;
}

void QFrameSelector::setStartSpin(QDoubleSpinBox* value)
{
    startSpin = value;
}

void QFrameSelector::setCursorSpin(QDoubleSpinBox* value)
{
    cursorSpin = value;
}

void QFrameSelector::setLeftSpacer(QFrame* value)
{
    leftSpacer = value;
}

void QFrameSelector::setRightSlider(QLabel* value)
{
    rightSlider = value;
}

void QFrameSelector::setPlayZone(QFrame* value)
{
    playZone = value;
}

void QFrameSelector::setLeftSlider(QLabel* value)
{
    leftSlider = value;
}
