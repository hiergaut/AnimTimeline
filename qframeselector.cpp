#include "qframeselector.h"

//#include <QDebug>
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
}

void QFrameSelector::paintEvent(QPaintEvent* event)
{
    (void)event;

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
}

void QFrameSelector::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        onChangeCursor(newCursor, true);
        emit cursorChanged(cursor);

        mouseLeftClicked = true;

    } else if (event->button() == Qt::RightButton) {
        double newPose = qMax((event->x() - *zero) / *pixPerSec, 0.0);

        auto it = keyPoses.find(cursor);

        // if already on keyPose, move current keyPose
        if (it != keyPoses.end()) {

            // if no keyPose under mouse, move keyPose to newPose
            if (keyPoses.find(newPose) == keyPoses.end()) {
                int num = static_cast<int>(std::distance(keyPoses.begin(), it));
                keyPoses.erase(it);
                keyPoses.insert(newPose);

                onChangeCursor(newPose);
                emit keyPoseMoved(num, newPose);
            }

        } else {
            // not on keyPose

            auto it = keyPoses.begin();
            int iRight = 0;
            while (it != keyPoses.end() && *it < newPose) {
                iRight++;
                it++;
            }

            // if keyPoses on the right, remove or insert time
            if (it != keyPoses.end()) {

                double right = *it;
                double dist = right - newPose;
                double gap = (*shiftDown) ? (-dist) : (dist);
                if (start > newPose) {
                    start += gap;
                    updateStartSpin();
                }

                if (end > newPose) {
                    end += gap;
                    updateEndSpin();
                }

                if (cursor > newPose) {
                    cursor += gap;
                    updateCursorSpin();
                }
                updateKeyPoses(gap, iRight);

                widgetRuler->setMaxDuration(*maxDuration + gap);
                updateDurationSpin();
            }
        }
        update();

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

        onChangeCursor(newCursor, true);
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

// external slot, warning on using external signal
void QFrameSelector::onAddingKeyPose(double time, bool internal)
{
    if (static_cast<int>(time) == -1)
        time = cursor;

    int nbKeyPoses = static_cast<int>(keyPoses.size());
    keyPoses.insert(time);
    if (static_cast<int>(keyPoses.size()) != nbKeyPoses) {
        updateCursorSpin();
        update();

        //        emit nbKeyPosesChanged(static_cast<int>(keyPoses.size()));
        if (internal)
            emit keyPoseAdded(time);

        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));
    } else {
        auto it = keyPoses.find(time);

        if (internal)
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

// external slot, warning on using external signal
void QFrameSelector::onClearKeyPoses()
{
    keyPoses.clear();
    nbKeyPosesSpin->setValue(0);

    updateCursorSpin();
    update();
}

// external slot, warning on using external signal
void QFrameSelector::onChangeStart(double time)
{
    start = qMax(qMin(time, end), 0.0);
    updateStartSpin();
    update();

    emit startChanged(start);
}

// external slot, warning on using external signal
void QFrameSelector::onChangeEnd(double time)
{
    end = qMin(qMax(time, start), *maxDuration);

    updateEndSpin();
    update();

    emit endChanged(end);
}

// external slot, warning on using external signal
void QFrameSelector::onChangeCursor(double time, bool findNearestKeyPose)
{
    if (findNearestKeyPose) {
        double pos = 2.0; // random initialize
        double min = 999.0;
        double dist;
        //    cursor = time;
        for (double keyPose : keyPoses) {
            //        if (keyPose > cursor + STICKY_KEYPOSE_DISTANCE)
            //            break;

            dist = qAbs(keyPose - time);
            if (dist < STICKY_KEYPOSE_DISTANCE) {

                if (min > dist) {
                    pos = keyPose;
                    min = dist;
                }
            }
        }

        cursor = (min != 999.0) ? (pos) : (time);
    } else {
        cursor = time;
    }
    updateCursorSpin();
    update();
}

void QFrameSelector::onChangeDuration()
{
    double newDuration = totalDurationSpin->value();

    end = qMin(qMax(endSpin->value(), start), newDuration);
    updateEndSpin();
    widgetRuler->setMaxDuration(newDuration);
}

void QFrameSelector::onSetCursorToStart()
{
    onChangeCursor(start);
    emit cursorChanged(cursor);
}

void QFrameSelector::onSetCursorToEnd()
{
    onChangeCursor(end);
    emit cursorChanged(cursor);
}

void QFrameSelector::onSetCursorToPreviousKeyPose()
{
    auto it = keyPoses.rbegin();
    while (it != keyPoses.rend() && *it >= cursor)
        it++;

    if (it != keyPoses.rend()) {
        onChangeCursor(*it);
        emit cursorChanged(cursor);
    }
}

void QFrameSelector::onSetCursorToNextKeyPose()
{
    auto it = keyPoses.begin();
    while (it != keyPoses.end() && *it <= cursor)
        it++;

    if (it != keyPoses.end()) {
        cursor = *it;

        updateCursorSpin();
        update();

        emit cursorChanged(cursor);
    }
}

void QFrameSelector::onPlay()
{
}

void QFrameSelector::onPause()
{
}

void QFrameSelector::onChangeCursorSpin()
{
    onChangeCursor(cursorSpin->value());
    emit cursorChanged(cursor);
}

void QFrameSelector::onChangeStartSpin()
{
    start = qMax(qMin(startSpin->value(), end), 0.0);
    updateStartSpin();
    update();

    emit startChanged(start);
}

void QFrameSelector::onChangeEndSpin()
{
    end = qMin(qMax(endSpin->value(), start), *maxDuration);
    updateEndSpin();
    update();

    emit endChanged(end);
}

void QFrameSelector::onStartIncPlus()
{
    double gap = startInc->value();

    start += gap;
    updateStartSpin();

    cursor += gap;
    updateCursorSpin();

    end += gap;
    updateEndSpin();

    updateKeyPoses(gap);
    widgetRuler->setMaxDuration(*maxDuration + gap);

    updateDurationSpin();
}

void QFrameSelector::onStartIncLess()
{
    double gap = startInc->value();

    start = qMax(0.0, start - gap);
    updateStartSpin();
    cursor = qMax(0.0, cursor - gap);
    updateCursorSpin();
    end = qMax(0.0, end - gap);
    updateEndSpin();

    updateKeyPoses(-gap);
    widgetRuler->setMaxDuration(qMax(0.0, *maxDuration - gap));
    updateDurationSpin();
}

void QFrameSelector::onEndIncPlus()
{
    double gap = endInc->value();

    widgetRuler->setMaxDuration(*maxDuration + gap);
    updateDurationSpin();
}

void QFrameSelector::onEndIncLess()
{
    double gap = endInc->value();

    end = qMin(*maxDuration - gap, end);
    updateEndSpin();

    cursor = qMin(*maxDuration - gap, cursor);
    updateCursorSpin();

    widgetRuler->setMaxDuration(qMax(0.0, *maxDuration - gap));
    updateDurationSpin();
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

void QFrameSelector::updateKeyPoses(double gap, int first)
{
    std::set<double> clone;
    int i = 0;
    for (double d : keyPoses) {
        if (i < first)
            clone.insert(d);
        else {
            clone.insert(d + gap);
        }
        i++;
    }

    keyPoses = clone;
    emit keyPosesMoved(gap, first);
}

void QFrameSelector::setShiftDown(bool* value)
{
    shiftDown = value;
}

void QFrameSelector::setNbKeyPosesSpin(QSpinBox* value)
{
    nbKeyPosesSpin = value;
}

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
    updateCursorSpin();
    update();
}

void QFrameSelector::updatePlayZone()
{
    start = 0;
    updateStartSpin();

    end = *maxDuration;
    updateEndSpin();
}

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
