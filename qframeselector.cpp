#include "qframeselector.h"

#include <QDebug>
#include <QPainter>
//#include <QTimer>
#include <QWheelEvent>

//#include <QtMath>
#include <QtGlobal>
//#include <cmath>

QFrameSelector::QFrameSelector(QWidget* parent)
    : QFrame(parent)
{
    widgetRuler = static_cast<QWidgetRuler*>(parent);
    nbInterval = widgetRuler->getNbInterval();
    step = widgetRuler->getStep();
    pixPerSec = widgetRuler->getPixPerSec();
    zero = widgetRuler->getZero();
    maxDuration = widgetRuler->getMaxDuration();
    //    qDebug() << "parent =" << parent;
    //    qDebug() << "QFrameSelector created";
    //    leftSpacer = findChild<QSpacerItem*>("horizontalSpacer");
    //    leftSlider = findChild<QLabel*>("label_leftSlider");
    //    playZone = findChild<QFrame*>("frame_playzone");

    //    qDebug() << leftSlider;
    //    qDebug() << playZone;
    //    assert(leftSpacer != nullptr);
    //    assert(leftSlider != nullptr);
    //    assert(playZone != nullptr);
    //    this->installEventFilter(this);
    //        setUpdatesEnabled(false);
    //    repaint();
    //    int wInterval = *pixPerSec * *step;
    //    leftSpacer->setMinimumWidth(wInterval - leftSlider->width() + start * *pixPerSec);
    //    playZone->setMinimumWidth(duration * *pixPerSec);
    //    setAttribute(Qt::WA_OpaquePaintEvent);
    //    repaint();
    //    timer = new QTimer(this);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(onTickTimer()));
}

//QFrameSelector::~QFrameSelector()
//{
//    //    this->QFrame::~QFrame();
//    //    delete timer;
//}

void QFrameSelector::paintEvent(QPaintEvent* event)
{
    //    if (end > *maxDuration) {
    //        end = *maxDuration;
    //    }
    //    if (rulerChanged) {
    //    qDebug() << "QFrameSelector: paintEvent " << event->type() << iPaint++;
    //    double duration =10.0;
    //    double steps[] = {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
    //    int iStep =0;
    //    while (width() * steps[iStep] < 50 *duration) iStep++;

    QPainter painter(this);
    //    painter.begin(this);
    //    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    //    int nbInterval = widgetRuler->getNbInterval();
    //    double step = widgetRuler->getStep();
    //    double pixPerSec = widgetRuler->getPixPerSec();
    //    int wInterval = *pixPerSec * *step;

    //    qDebug() << "QFrameSelector: nbInterval =" << nbInterval;
    //    qDebug() << "QFrameSelector: wwInterval =" << wInterval;

    //    double step = static_cast<QWidgetRuler*>(parent())->getStep();

    //    double step = steps[iStep];
    //    int nbInterval =duration /step +2;
    //    int wInterval =width() /nbInterval;

    //    qDebug() << leftSpacer;
    //        int leftSpacerX =wInterval -leftSlider->width() +start *pixPerSec;
    //        leftSpacer->changeSize(leftSpacerX, 0);
    if (!sliding) {
        //        leftSpacer->setMinimumWidth(*zero - leftSlider->width() + start * *pixPerSec);
        //        playZone->setMinimumWidth(end * *pixPerSec);

        leftSpacer->setMinimumWidth(*zero + start * *pixPerSec - leftSlider->width());
        playZone->setMinimumWidth((end - start) * *pixPerSec);
    }
    //        repaint();

    painter.setPen(Qt::darkGray);
    for (int i = 1; i < *nbInterval; i++) {
        //        int x =wInterval *i;
        int x = i * *step * *pixPerSec;
        painter.drawLine(x, 0, x, height());
    }

    painter.setPen(QPen(Qt::lightGray));
    for (int i = 1; i < *nbInterval - 1; i++) {
        //        int x =wInterval *i;
        int x = i * *step * *pixPerSec;
        double middle = x + *zero / 2;
        painter.drawLine(middle, 0, middle, height());
    }

    painter.setPen(QPen(QColor(0, 0, 255, 255), 3));
    //    if (cursor > 0.0) {
    int xCursor = *zero + cursor * *pixPerSec;
    painter.drawLine(xCursor, 0, xCursor, height());
    //    } else {
    //        painter.drawLine(*zero + 3, 0, *zero + 3, height());
    //    }

    painter.setPen(QPen(Qt::yellow, 3));
    for (double keyPose : keyPoses) {
        int xKeyPose = *zero + keyPose * *pixPerSec;
        painter.drawLine(xKeyPose, 30, xKeyPose, height());
    }
    //    }
    //    painter.end();

    //    QPen linePen;
    //    linePen.setWidth(1);
    //    painter.setPen(linePen);

    //    painter.drawLine(0, 0, 100, 100);
    //    sliding =false;

    //    QRectF rect = this->rect();
    //    painter.fillRect(QRect(rect.left(), rect.top(), this->width(), 40), QColor(0, 255, 0));

    //        rulerChanged = false;
    //    }
}

void QFrameSelector::mousePressEvent(QMouseEvent* event)
{
    //    qDebug() << "QFrameSelector: mousePressEvent";
    //    event->ignore();
    if (event->button() == Qt::LeftButton) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        onChangeCursor(newCursor);
        //        double leftStep = int(cursor / period) * period;

        //        emit cursorChanged(cursor);
        //        cursorSpin->setValue(cursor);
        //        update();

        //        if (cursor - leftStep > 1.0e-5) {
        //            //        if (cursor != leftStep) {
        //            cursor = leftStep;
        //        }

        clicked = true;
        event->accept();
    } else {
        event->ignore();
    }
}

void QFrameSelector::mouseMoveEvent(QMouseEvent* event)
{
    if (clicked) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        //        newCursor = int(newCursor / period) * period;

        //        cursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        onChangeCursor(newCursor);
        //        emit cursorChanged(cursor);
        //        updateCursorSpin();
        //        update();

        //        if (qAbs(cursor - newCursor) > 1.0e-5) {
        //            //        if (cursor != newCursor) {
        //            cursor = newCursor;
        //            emit cursorChanged(cursor, keyPoses.find(cursor) != keyPoses.end());
        //            cursorSpin->setValue(cursor);
        //            update();
        //        }
    } else {
        event->ignore();
    }
}

void QFrameSelector::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        clicked = false;
        event->accept();
    } else {
        event->ignore();
    }
}

void QFrameSelector::keyPressEvent(QKeyEvent* event)
{
    //    qDebug() << "keyPress";
    if (event->key() == Qt::Key_Space) {
        onAddingKeyPose(cursor);
    }
}

//bool QFrameSelector::eventFilter(QObject *watched, QEvent *event)
//{
//    qDebug() << "QFrameSelector: eventFilter " << i++;

//}

//void QFrameSelector::onRedrawScale()
//{
//}

void QFrameSelector::onSlideLeftSlider(int deltaX)
{
    //    setAttribute(Qt::WA_OpaquePaintEvent);
    //    qDebug() << "QFrameSelector::onSlideLeftSlider " << deltaX;
    //    leftSpacerX +=deltaX;
    //    leftSpacer->changeSize(leftSpacerX, 0);
    //    leftSpacer->changeSize(200 +deltaX, 0);
    //    double zero =*pixPerSec **step;

    if (!sliding) {
        leftSlider->setStyleSheet("background-color: #00ff00");
        sliding = true;
    }

    //    double newStart =(leftSpacer->width() +deltaX -leftSlider->width()) / *pixPerSec;
    double newStart = start + deltaX / *pixPerSec;
    start = qMin(qMax(newStart, 0.0), end);
    //    start = int(start / period) * period;
    //    emit startChanged(start);
    onChangeStart(start);

    //    int newPos =leftSpacer->width() +deltaX +leftSlider->width();

    leftSpacer->setMinimumWidth(*zero + start * *pixPerSec - leftSlider->width());
    playZone->setMinimumWidth((end - start) * *pixPerSec);

    //    updateStartSpin();
    //    if (newPos >= *zero) {

    //        if (newPos < *zero +end * *pixPerSec) {
    //            leftSpacer->setMinimumWidth(leftSpacer->width() + deltaX);
    //            playZone->setMinimumWidth(playZone->width() -deltaX);
    //        }
    //    }
    //    else {
    //        leftSpacer->setMinimumWidth(*zero -leftSlider->width());
    //        playZone->setMinimumWidth(*pixPerSec *(end -start));
    //    }

    //    start +=deltaX;
    //    update();
    //    repaint();
    //    playZone->setMinimumWidth(playZone->width());
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
    //    end = int(end / period) * period;
    //    emit endChanged(end);

    playZone->setMinimumWidth((end - start) * *pixPerSec);

    //    updateEndSpin();
    //    double endLimit = (*nbInterval - 1) * *step * *pixPerSec;
    //    qDebug() << "QFrameSelector::onSlideRightSlider " << deltaX;
    //    if (*zero + playZone->width() + deltaX <= endLimit) {
    //        playZone->setMinimumWidth(qMax(playZone->width() + deltaX, 0));
    //    } else {
    //        playZone->setMinimumWidth(endLimit - *zero);
    //    }
    //    playZone->setMinimumWidth(100);
    //    duration += deltaX *0.1;
    //    update();
}

void QFrameSelector::onSlideRelease()
{
    sliding = false;
    //    qDebug() << "slide released";
}

void QFrameSelector::onAddingKeyPose(double time)
{
    if (time == -1.0)
        time = cursor;
    //    if (cursor >= 0.0) {
    int nbKeyPoses = keyPoses.size();
    keyPoses.insert(time);
    if (keyPoses.size() != nbKeyPoses) {
        updateCursorSpin();
        update();

        emit nbKeyPosesChanged(keyPoses.size());
        emit keyPoseAdded(time);
    } else {
        //        int num =0;
        auto it = keyPoses.find(time);
        emit keyPoseChanged(std::distance(keyPoses.begin(), it));
    }
    //    emit cursorChanged(cursor);
    //    }
}

void QFrameSelector::onDeleteKeyPose()
{
    auto it = keyPoses.find(cursor);
//    assert(it != keyPoses.end());
    if (it != keyPoses.end()) {

        int num = std::distance(keyPoses.begin(), it);
        keyPoses.erase(it);

        updateCursorSpin();
        update();

        emit nbKeyPosesChanged(keyPoses.size());
        emit removeKeyPose(num);
    }
}

void QFrameSelector::onChangeStart(double time)
{
    //    start = int(time / period) * period;
//    start = time;
    start = qMax(qMin(time, end), 0.0);
    updateStartSpin();
    update();

    //    emit startChanged(start);
}

void QFrameSelector::onChangeEnd(double time)
{
    //    end = int(time / period) * period;
//    qDebug() << "onChangedEnd " << time;
    end = qMin(qMax(time, start), *maxDuration);
//    end = time;
    updateEndSpin();
    update();

    //    emit endChanged(end);
}

void QFrameSelector::onChangeCursor(double time)
{
//        qDebug() << "QFrameSelector: onChangeCursor " << time;
    //    double newCursor = int(time / period) * period;

    //    emit cursorChanged(cursor);
    cursor = time;
    updateCursorSpin();
    update();

    //    cursorSpin->setValue(cursor);
    //    if (cursor != newCursor) {
    //    if (qAbs(cursor - newCursor) > 1.0e-5) {
    //        cursor = newCursor;
    //        emit cursorChanged(cursor);
    //        cursorSpin->setValue(cursor);
    //        emit isOnKeyPose(keyPoses.find(cursor) != keyPoses.end());

    //        update();
    //    }
    emit cursorChanged(cursor);
}

void QFrameSelector::onSetCursorToStart()
{
    onChangeCursor(start);
    //    cursor = start;
    //    updateCursorSpin();
    //    update();

    //    emit cursorChanged(cursor);
    //    cursorSpin->setValue(cursor);
}

void QFrameSelector::onSetCursorToEnd()
{
    onChangeCursor(end);
    //    cursor = end;
    //    updateCursorSpin();
    //    update();

    //    emit cursorChanged(cursor);
    //    cursorSpin->setValue(cursor);
}

void QFrameSelector::onSetCursorToPreviousKeyPose()
{
    //    for (double d : keyPoses) {
    //        qDebug() << d;
    //    }
    //    auto it2 = keyPoses.begin();
    //    while (it2 != keyPoses.end()) {
    //        qDebug() << *it2;
    //        it2++;
    //    }

    auto it = keyPoses.rbegin();
    while (it != keyPoses.rend() && *it >= cursor)
        it++;

    if (it != keyPoses.rend()) {
        onChangeCursor(*it);
        //        cursor = *it;

        //        updateCursorSpin();
        ////        cursorSpin->setValue(cursor);
        //        update();

        //        emit cursorChanged(cursor);
    }
}

void QFrameSelector::onSetCursorToNextKeyPose()
{
    //    for (double d : keyPoses) {
    //        qDebug() << d;
    //    }

    auto it = keyPoses.begin();
    while (it != keyPoses.end() && *it <= cursor)
        it++;

    if (it != keyPoses.end()) {
        cursor = *it;

        updateCursorSpin();
        //        cursorSpin->setValue(cursor);
        update();

        emit cursorChanged(cursor);
    }
}

void QFrameSelector::onPlay()
{

    //        qDebug() << "play";
    //    //    if (cursor < end)
    //    //        timer->start(period * 1000);
    //    //    else {
    //    //        changePauseMode();
    //    //    }

    //    //    while (cursor != end) {
    //    //        cursor += period;
    //    //        emit cursorChanged(cursor);

    //    //        update();
    //    //    }
}

void QFrameSelector::onPause()
{
    //        qDebug() << "pause";
    //    timer->stop();
}

//void QFrameSelector::onTickTimer()
//{
//    //    cursor += period;
//    cursor = (int((cursor + period / 2) / period) + 1) * period;

//    emit cursorChanged(cursor);
//    cursorSpin->setValue(cursor);
//    update();

//    if (qAbs(cursor - end) < 1.0e-5) {
//        //    if (cursor == end) {
//        qDebug() << "tickTimer end";
////        timer->stop();
//        cursor = start;
//        emit cursorChanged(cursor);
//        cursorSpin->setValue(cursor);
//        emit changePauseMode();
//        update();
//    }
//}

void QFrameSelector::onChangeCursorSpin()
{
    onChangeCursor(cursorSpin->value());
    //    cursor = cursorSpin->value();
    //    update();

    //    emit cursorChanged(cursor);
}

void QFrameSelector::onChangeStartSpin()
{
//    start = startSpin->value();
    start = qMax(qMin(startSpin->value(), end), 0.0);
    updateStartSpin();
    update();
}

void QFrameSelector::onChangeEndSpin()
{
//    end = endSpin->value();
    end = qMin(qMax(endSpin->value(), start), *maxDuration);
    updateEndSpin();
    update();
}

void QFrameSelector::onStartIncPlus()
{
    double gap = startInc->value();

    start +=gap;
    updateStartSpin();

    cursor +=gap;
    updateCursorSpin();

    end += gap;
    updateEndSpin();

    updateKeyPoses(gap);
    widgetRuler->setMaxDuration(*maxDuration + gap);
//    *maxDuration += gap;
//    update();
}

void QFrameSelector::onStartIncLess()
{
    double gap = startInc->value();

    start = qMax(0.0, start -gap);
    updateStartSpin();
    cursor =qMax(0.0, cursor -gap);
    updateCursorSpin();
    end = qMax(0.0, end -gap);
    updateEndSpin();

    updateKeyPoses(-gap);
    widgetRuler->setMaxDuration(qMax(0.0, *maxDuration - gap));
}

void QFrameSelector::onEndIncPlus()
{
    double gap = endInc->value();

    widgetRuler->setMaxDuration(*maxDuration + gap);
}

void QFrameSelector::onEndIncLess()
{
    double gap = endInc->value();

    end = qMin(*maxDuration -gap, end);
    updateEndSpin();

    cursor = qMin(*maxDuration -gap, cursor);
    updateCursorSpin();

    widgetRuler->setMaxDuration(qMax(0.0, *maxDuration - gap));
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

void QFrameSelector::updateKeyPoses(double gap)
{
    std::set<double> clone;
    for (double d : keyPoses) {
        clone.insert(d +gap);
    }

    keyPoses =clone;
}

void QFrameSelector::setEndInc(QDoubleSpinBox *value)
{
    endInc = value;
}

void QFrameSelector::setStartInc(QDoubleSpinBox *value)
{
    startInc = value;
}

std::set<double> QFrameSelector::getKeyPoses() const
{
    return keyPoses;
}

void QFrameSelector::setKeyPoses(const std::set<double> &value)
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
    return keyPoses.size();
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
    //    if (end > *maxDuration) {
    //        end = *maxDuration;
    //    }
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

//void QFrameSelector::onRulerChange(double step, int nbInterval, double pixPerSec)
//{

//    this->step = step;
//    this->nbInterval = nbInterval;
//    this->pixPerSec = pixPerSec;
////    rulerChanged = true;
//}

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

//void QFrameSelector::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "QFrameSelector: wheelEvent";
//    emit changePrecision(1);

//}
