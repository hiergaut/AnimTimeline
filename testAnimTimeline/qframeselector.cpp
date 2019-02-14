#include "qframeselector.h"

#include <QDebug>
#include <QPainter>
#include <QWheelEvent>
//#include <QtMath>
//#include <QtGlobal>
#include <QTimer>
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
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTickTimer()));
}

QFrameSelector::~QFrameSelector()
{
    //    this->QFrame::~QFrame();
    delete timer;
}

void QFrameSelector::paintEvent(QPaintEvent* event)
{
    //    if (rulerChanged) {
    qDebug() << "QFrameSelector: paintEvent " << event->type() << iPaint++;
    //    double duration =10.0;
    //    double steps[] = {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
    //    int iStep =0;
    //    while (width() * steps[iStep] < 50 *duration) iStep++;

    QPainter painter(this);
    //    painter.begin(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

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
        painter.drawLine(x + *zero / 2, 0, x + *zero / 2, height());
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
    qDebug() << "QFrameSelector::mousePressEvent";
    //    event->ignore();
    if (event->button() == Qt::LeftButton) {
        double newCursor = qMax((event->x() - *zero) / *pixPerSec, 0.0);
        newCursor = int(newCursor / period) * period;

        if (qAbs(cursor - newCursor) > 1.0e-5) {
//        if (cursor != newCursor) {
            cursor = newCursor;
            emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
            update();
        }

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
        newCursor = int(newCursor / period) * period;

    if (qAbs(cursor - newCursor) > 1.0e-5) {
//        if (cursor != newCursor) {
            cursor = newCursor;
            emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
            update();
        }
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
    qDebug() << "keyPress";
    if (event->key() == Qt::Key_Space) {
        onAddKeyPose();
    }
}

//bool QFrameSelector::eventFilter(QObject *watched, QEvent *event)
//{
//    qDebug() << "QFrameSelector: eventFilter " << i++;

//}

void QFrameSelector::onRedrawScale()
{
}

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
    start = int(start / period) * period;
    emit changeStart(start);

    //    int newPos =leftSpacer->width() +deltaX +leftSlider->width();

    leftSpacer->setMinimumWidth(*zero + start * *pixPerSec - leftSlider->width());
    playZone->setMinimumWidth((end - start) * *pixPerSec);

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
    rightSlider->setStyleSheet("background-color: red");
    sliding = true;
    double newEnd = end + deltaX / *pixPerSec;
    end = qMin(qMax(newEnd, start), *maxDuration);
    end = int(end / period) * period;
    emit changeEnd(end);

    playZone->setMinimumWidth((end - start) * *pixPerSec);

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

void QFrameSelector::onAddKeyPose()
{
    //    if (cursor >= 0.0) {
    keyPoses.insert(cursor);
    emit changeNbKeyPoses(keyPoses.size());
    emit changeCursor(cursor, true);
    update();
    //    }
}

void QFrameSelector::onStartChanged(double time)
{
    start = int(time / period) * period;
    emit(changeStart(start));
    update();
}

void QFrameSelector::onEndChanged(double time)
{
    end = int(time / period) * period;
    emit changeEnd(end);
    update();
}

void QFrameSelector::onCursorChanged(double time)
{
    qDebug() << "QFrameSelector: onCursorChanged";
    double newCursor = int(time/ period) * period;

//    if (cursor != newCursor) {
    if (qAbs(cursor - newCursor) > 1.0e-5) {
        cursor = newCursor;
        emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
//        emit isOnKeyPose(keyPoses.find(cursor) != keyPoses.end());

        update();
    }
}

void QFrameSelector::onCursorStart()
{
    cursor = start;
    emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
    update();
}

void QFrameSelector::onCursorEnd()
{
    cursor = end;
    emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
    update();
}

void QFrameSelector::onCursorPreviousKeyPose()
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
        cursor = *it;

        emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
        update();
    }
}

void QFrameSelector::onCursorNextKeyPose()
{
    //    for (double d : keyPoses) {
    //        qDebug() << d;
    //    }

    auto it = keyPoses.begin();
    while (it != keyPoses.end() && *it <= cursor)
        it++;

    if (it != keyPoses.end()) {
        cursor = *it;

        emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
        update();
    }
}

void QFrameSelector::onPlay()
{
    //    qDebug() << "play";
    if (cursor < end)
        timer->start(period * 1000);
    else {
        changePauseMode();
    }

    //    while (cursor != end) {
    //        cursor += period;
    //        emit changeCursor(cursor);

    //        update();
    //    }
}

void QFrameSelector::onPause()
{
    //    qDebug() << "pause";
    timer->stop();
}

void QFrameSelector::onTickTimer()
{
    //    cursor += period;
    cursor = (int(cursor / period) + 1) * period;

    emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
    update();

    //    if (qAbs(cursor- end) < 1.0e-10) {
    if (cursor == end) {
        qDebug() << "tickTimer end";
        timer->stop();
        cursor = start;
        emit changeCursor(cursor, keyPoses.find(cursor) != keyPoses.end());
        emit changePauseMode();
        update();
    }
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
