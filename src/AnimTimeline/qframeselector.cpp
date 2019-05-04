#include <AnimTimeline/animtimeline.h>
#include <AnimTimeline/qframeselector.h>

#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QtGlobal>

QFrameSelector::QFrameSelector(QWidget* parent)
    : QFrame(parent)
{
    widgetRuler = static_cast<QWidgetRuler*>(parent);
    nbInterval = widgetRuler->getNbInterval();
    step = widgetRuler->getStep();
    pixPerSec = widgetRuler->getPixPerSec();
    zero = widgetRuler->getZero();
    duration = widgetRuler->getMaxDuration();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //    qDebug() << "end construct frameSelector, parent : " << parent;
}

QFrameSelector::~QFrameSelector()
{
    delete timer;
}

double QFrameSelector::nearestStep(double time) const
{
    double deltaT = AUTO_SUGGEST_CURSOR_RADIUS / *pixPerSec;

    double minDist = durationSpin->maximum();
    double dist;

    double newCursor = time;

    for (double keyPose : keyPoses) {
        dist = qAbs(keyPose - time);
        if (dist < deltaT && dist < minDist) {

            minDist = dist;
            newCursor = keyPose;
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

    return newCursor;
}

// ------------------------------- PROTECTED ----------------------------------
void QFrameSelector::paintEvent(QPaintEvent*)
{
    qDebug() << "QFrameSelector::paintEvent " << ++paintCounter;

    QPainter painter(this);
    int h = height();
    int w = width();

    if (!sliding) {
        redrawPlayZone();
    }

    // DRAW FRAME SCALE
    painter.setPen(QPen(Qt::lightGray));
    double frameDuration = 1.0 / FPS;
    int hUp = h / 3;
    double nbFrame = *duration / frameDuration;
    for (int i = 0; i < nbFrame; i++) {
        int x = static_cast<int>(i * frameDuration * *pixPerSec + *zero);
        painter.drawLine(x, 0, x, hUp);
    }

    // DRAW CURSOR
    painter.setPen(QPen(QColor(0, 0, 255, 255), 3));
    int xCursor = static_cast<int>(*zero + cursor * *pixPerSec);
    painter.drawLine(xCursor, 0, xCursor, h);

    // DRAW KEYPOSES
    painter.setPen(QPen(QColor(255, 255, 0, 255), 3));
    int hTemp = h / 3 + 2;
    for (double keyPose : keyPoses) {
        int xKeyPose = static_cast<int>(*zero + keyPose * *pixPerSec);
        painter.drawLine(xKeyPose, hTemp, xKeyPose, h);
    }

    // DRAW TIME SCALE
    int hDown = 2 * h / 3;
    painter.setPen(Qt::black);
    for (int i = 1; i < *nbInterval; i++) {
        int x = static_cast<int>(i * *step * *pixPerSec);
        painter.drawLine(x, hDown, x, h);
    }
    int hDown2 = 3 * h / 4;
    painter.setPen(Qt::darkGray);
    for (int i = 1; i < *nbInterval - 1; i++) {
        int middle = static_cast<int>((i + 0.5) * *step * *pixPerSec);
        painter.drawLine(middle, hDown2, middle, h);
    }

    if (updateKeyPoseFlash > 0) {

        if (updateKeyPoseFlash % 2 == 0) {
            painter.setPen(QPen(QColor(0, 0, 255, 255), 3));
            int xKeyPose = static_cast<int>(*zero + keyPoseFlash * *pixPerSec);
            painter.drawLine(xKeyPose, hTemp, xKeyPose, h);
        }

        if (--updateKeyPoseFlash == 0)
            timer->stop();
    }

    // DRAW MIDDLE RULER SEPARATOR
    int gap = 5;
    painter.setPen(Qt::white);
    painter.drawLine(0, h / 2 + gap + 2, w, h / 2 + gap + 2);
    painter.drawLine(0, h / 2 + gap + 1, w, h / 2 + gap + 1);

    painter.setPen(Qt::darkGray);
    painter.drawLine(0, h / 2 - gap - 1, w, h / 2 - gap - 1);
    painter.drawLine(0, h / 2 + gap, w, h / 2 + gap);

    painter.setPen(Qt::black);
    painter.drawLine(0, h / 2 - gap - 2, w, h / 2 - gap - 2);
}

void QFrameSelector::mousePressEvent(QMouseEvent* event)
{
    // ---------------------- LEFT CLICK --------------------------------------
    if (event->button() == Qt::LeftButton) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);

        // move cursor without render
        if (*ctrlDown) {
            onChangeCursor(newCursor, false);
        }
        // delete keyPoses between cursor and newCursor
        else if (*shiftDown) {
            deleteZone(cursor, newCursor);
        }
        // move cursor and update renderer
        else {
            onChangeCursor(newCursor);
            mouseLeftClicked = true;
        }

        // ------------------ RIGHT CLICK -------------------------------------
    } else if (event->button() == Qt::RightButton) {
        double newPose = qMax((event->x() - *zero) / *pixPerSec, 0.0);

        auto it = keyPoses.find(cursor);

        // if already on keyPose, move current keyPose
        // ------------------- CURSOR ON KEYPOSE -----------------------
        if (it != keyPoses.end()) {

            double nearest = nearestStep(newPose);
            // -------------- SINGLE MOVE -------------------------------------
            if (*shiftDown) {
                // if no keyPose under mouse, move keyPose to newPose
                if (keyPoses.find(nearest) == keyPoses.end()) {

                    if (qAbs(cursor - nearest) > 1e-5) {
                        cursor = nearest;

                        size_t id = static_cast<size_t>(std::distance(keyPoses.begin(), it));
                        keyPoses.erase(it);
                        keyPoses.insert(cursor);

                        updateCursorSpin(); // to find keyPose here, yellow spinBox
                        update();

                        emit keyPoseMoved(id, cursor); // EXTERNAL SIGNAL
                        qDebug() << "\033[35mkeyPoseMoved(" << id << ", " << cursor << ")\033[0m";
                    }
                }

                // ---------- MULTIPLE MOVE -----------------------------------
            } else {
                auto itLeft = it;
                --itLeft;
                double left = (it == keyPoses.begin()) ? (0.0) : (*itLeft);

                if (nearest > left) {
                    double dist = nearest - cursor;
                    size_t id = static_cast<size_t>(std::distance(keyPoses.begin(), it));
                    moveKeyPoses(dist, id);
                }
            }

            // ---------------- CURSOR NOT ON KEYPOSE --------------------------
        } else {

            // if shiftdown, slide first right keypose to the left
            // --------------- MOVE RIGHT KEYPOSE TO THE LEFT -----------------
            if (*shiftDown) {
                auto it = keyPoses.begin();
                size_t iRight = 0;
                while (it != keyPoses.end() && *it < newPose) {
                    ++it;
                    ++iRight;
                }

                // if keyPoses on the right, remove or insert time
                if (it != keyPoses.end()) {

                    double right = *it;
                    double dist = newPose - right;
                    moveKeyPoses(dist, iRight);
                }

                // if not shiftdown, slide first left keypose to the right
                // ---------------- MOVE LEFT KEYPOSE TO THE RIGHT -----------
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

                    moveKeyPoses(dist, iLeft);
                }
            }
        }

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

        onChangeCursor(newCursor);
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

// -------------------------- EXTERNAL SLOTS ----------------------------------
// EXTERNAL SLOT
void QFrameSelector::onAddingKeyPose(double time, bool internal /* = true */)
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

        if (internal) {
            emit keyPoseAdded(time); // EXTERNAL SIGNAL
            qDebug() << "\033[35mkeyPoseAdded(" << time << ")\033[0m";
        }

        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));

        // keyPose already here, change actual keyPose
    } else {
        auto it = keyPoses.find(time);
        size_t id = static_cast<size_t>(std::distance(keyPoses.begin(), it));

        if (internal) {
            emit keyPoseChanged(id); // EXTERNAL SIGNAL
            qDebug() << "\033[35mkeyPoseChanged(" << id << ")\033[0m";
        }

        updateKeyPoseFlash = 6;
        keyPoseFlash = time;

        timer->start(50);

        update();
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

    bool out = qAbs(newStart - time) > 1e-5;
    bool change = qAbs(newStart - start) > 1e-5;

    if (change) {
        start = newStart;
        updateStartSpin();
        redrawPlayZone();

        // emit signal if time of emitter is internal changed due of limits
        if (internal || out) {
            emit startChanged(start); // EXTERNAL SIGNAL
            qDebug() << "\033[35mstartChanged(" << start << ")\033[0m";
        }
    } else {

        if (out) {
            updateStartSpin();
        }
    }
}

// EXTERNAL SLOT
void QFrameSelector::onChangeEnd(double time, bool internal /* = true */)
{
    double newEnd = qMin(qMax(time, start), *duration);

    bool out = qAbs(newEnd - time) > 1e-5;
    bool change = qAbs(newEnd - end) > 1e-5;

    // emit signal only if new value of end
    if (change) {
        end = newEnd;
        updateEndSpin();
        update();

        if (internal || out) {
            emit endChanged(end); // EXTERNAL SIGNAL
            qDebug() << "\033[35mendChanged(" << end << ")\033[0m";
        }
    } else {

        if (out) {
            updateEndSpin();
        }
    }
}

// EXTERNAL SLOT (warning on using EXTERNAL SIGNAL)
void QFrameSelector::onChangeCursor(double time, bool internal /* = true */)
{
    double newCursor = qMax(0.0, time);

    if (internal) {
        newCursor = nearestStep(newCursor);
    }

    bool out = qAbs(newCursor - time) > 1e-5;
    bool change = qAbs(newCursor - cursor) > 1e-5;

    if (change) {
        cursor = newCursor;
        updateCursorSpin();
        update();

        if (internal || out) {
            emit cursorChanged(cursor); // EXTERNAL SIGNAL
            qDebug() << "\033[35mcursorChanged(" << time << ")\033[0m";
        }
    } else {

        if (out) {
            updateCursorSpin();
        }
    }
}

// EXTERNAL SLOT
void QFrameSelector::onChangeDuration(double time, bool internal /* = true */)
{
    double newDuration = qMax(time, 0.0);

    bool out = qAbs(newDuration - time) > 1e-5;
    bool change = qAbs(newDuration - *duration) > 1e-5;

    if (change) {
        *duration = newDuration;
        widgetRuler->drawRuler(widgetRuler->minimumWidth());
        updateDurationSpin();

        // emit signal if time of emitter is internal changed due of limits
        if (internal || out) {
            emit durationChanged(*duration);
            qDebug() << "\033[35mdurationChanged(" << *duration << ")\033[0m";
        }
    } else {

        if (out) {
            updateDurationSpin();
        }
    }

    if (*duration < start)
        onChangeStart(*duration);

    if (*duration < end)
        onChangeEnd(*duration);

    // auto update
}

// -------------------------- INTERNAL SLOTS ----------------------------------
void QFrameSelector::onSlideLeftSlider(int deltaX)
{

    if (!sliding) {
        leftSlider->setStyleSheet("background-color: #00ff00");
        sliding = true;
    }

    double newStart = start + deltaX / *pixPerSec;

    onChangeStart(newStart); // EXTERNAL SLOT

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

    onChangeEnd(newEnd); // EXTERNAL SLOT

    playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
}

void QFrameSelector::onLeftSlideRelease()
{
    sliding = false;
}

void QFrameSelector::onRightSlideRelease()
{
    sliding = false;
}

void QFrameSelector::onDeleteKeyPose()
{
    auto it = keyPoses.find(cursor);

    if (it != keyPoses.end()) {

        size_t id = static_cast<size_t>(std::distance(keyPoses.begin(), it));
        keyPoses.erase(it);

        updateCursorSpin();
        update();

        nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));
        emit keyPoseDeleted(id); // EXTERNAL SIGNAL
        qDebug() << "\033[35mkeyPoseDeleted(" << id << ")\033[0m";

        onSetCursorToNextKeyPose();
    }
}

void QFrameSelector::onSetCursorToStart()
{
    onChangeCursor(start);
}

void QFrameSelector::onSetCursorToEnd()
{
    onChangeCursor(end);
}

void QFrameSelector::onSetCursorToPreviousKeyPose()
{
    auto it = keyPoses.rbegin();
    while (it != keyPoses.rend() && *it >= cursor)
        it++;

    if (it != keyPoses.rend()) {
        onChangeCursor(*it);
    }
}

void QFrameSelector::onSetCursorToNextKeyPose()
{
    auto it = keyPoses.begin();
    while (it != keyPoses.end() && *it <= cursor)
        it++;

    if (it != keyPoses.end()) {

        onChangeCursor(*it);
    }
}

void QFrameSelector::onChangeStartSpin()
{
    onChangeStart(startSpin->value());
}

void QFrameSelector::onChangeEndSpin()
{
    onChangeEnd(endSpin->value());
}

void QFrameSelector::onChangeCursorSpin()
{
    onChangeCursor(cursorSpin->value());
}

void QFrameSelector::onChangeDurationSpin()
{
    onChangeDuration(durationSpin->value());
}

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

void QFrameSelector::updateDurationSpin()
{
    durationSpin->setValue(*duration);
}

void QFrameSelector::moveKeyPoses(double gap, size_t iFirst)
{
    std::set<double> clone;
    size_t i = 0;
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
    qDebug() << "\033[35mkeyPosesMoved(" << gap << ", " << iFirst << ")\033[0m";

    if (start >= left) {
        //        start += gap;
        start = qMax(qMin(start + gap, *duration), 0.0);
        updateStartSpin();

        emit startChanged(start);
        qDebug() << "\033[35mstartChanged(" << start << ")\033[0m";
    }

    if (end >= left) {
        //        end += gap;
        end = qMax(qMin(end + gap, *duration), 0.0);
        updateEndSpin();

        emit endChanged(end);
        qDebug() << "\033[35mendChanged(" << end << ")\033[0m";
    }

    if (cursor >= left) {
        bool cursorOnKeyPose = qAbs(cursor - first) < 1e-5;

        cursor = qMax(cursor + gap, 0.0);
        updateCursorSpin();

        if (!cursorOnKeyPose) {
            emit cursorChanged(cursor);
            qDebug() << "\033[35mcursorChanged(" << cursor << ")\033[0m";
        }
    }

    update();
}

void QFrameSelector::deleteZone(double time, double time2)
{
    double left = qMin(time, time2);
    double right = qMax(time, time2);

    double dist = right - left;

    auto it = keyPoses.begin();
    size_t id = 0;
    bool first = true;
    while (it != keyPoses.end()) {
        double keyPose = *it;

        if (keyPose >= left) {
            keyPoses.erase(it++);

            if (keyPose > right) {
                if (first) {
                    emit keyPosesMoved(-dist, id);
                    qDebug() << "\033[35mkeyPosesMoved(" << -dist << ", " << id << ")\033[0m";
                    first = false;
                }
                keyPoses.insert(keyPose - dist);
            } else {
                emit keyPoseDeleted(id);
                qDebug() << "\033[35mkeyPoseDeleted(" << id << ")\033[0m";
            }
        } else {
            ++it;
        }

        ++id;
    }
    nbKeyPosesSpin->setValue(static_cast<int>(keyPoses.size()));

    double newStart = qMax(qMax(qMin(start, left), start - dist), 0.0);
    if (qAbs(newStart - start) > 1e-5) {
        start = newStart;
        updateStartSpin();

        emit startChanged(start);
        qDebug() << "\033[35mstartChanged(" << start << ")\033[0m";
    }

    double newEnd = qMax(qMax(qMin(end, left), end - dist), 0.0);
    if (qAbs(newEnd - end) > 1e-5) {
        end = newEnd;
        updateEndSpin();

        emit endChanged(end);
        qDebug() << "\033[35mendChanged(" << end << ")\033[0m";
    }

    update();
}

void QFrameSelector::redrawPlayZone()
{
    leftSpacer->setMinimumWidth(static_cast<int>(*zero + start * *pixPerSec - leftSlider->width()));
    playZone->setMinimumWidth(static_cast<int>((end - start) * *pixPerSec));
}

// -------------------------- GETTERS -----------------------------------------
double QFrameSelector::getStart() const
{
    return start;
}

double* QFrameSelector::getStart()
{
    return &start;
}

double* QFrameSelector::getEnd()
{
    return &end;
}

double* QFrameSelector::getCursor()
{
    return &cursor;
}

double QFrameSelector::getEnd() const
{
    return end;
}

double QFrameSelector::getCursor() const
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

std::set<double>* QFrameSelector::getKeyPoses()
{
    return &keyPoses;
}

// -------------------------- SETTERS -----------------------------------------
void QFrameSelector::setCursor(double time)
{
    cursor = time;
}

void QFrameSelector::setKeyPoses(const std::set<double>& value)
{
    keyPoses = value;
}

void QFrameSelector::setShiftDown(bool* value)
{
    shiftDown = value;
}

void QFrameSelector::setStart(double value)
{
    start = value;
}

void QFrameSelector::setEnd(double value)
{
    end = value;
}

void QFrameSelector::setDuration(double time)
{
    *duration = time;
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

void QFrameSelector::setDurationSpin(QDoubleSpinBox* value)
{
    durationSpin = value;
}

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
void QFrameSelector::setCtrlDown(bool* value)
{
    ctrlDown = value;
}

void QFrameSelector::setMidMouseDown(bool* value)
{
    midMouseDown = value;
}
