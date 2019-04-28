//#include "qscrollarearuler.h"
#include <AnimTimeline/qscrollarearuler.h>

//#include "animtimeline.h"
//#include "ui_animtimeline.h"

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
#include <QtMath>

QScrollAreaRuler::QScrollAreaRuler(QWidget* parent)
    : QScrollArea(parent)
{

    horizontalScrollBar()->setStyleSheet("\
                    QScrollBar:horizontal {\
                        background: transparent;\
                    }\
                    \
                    QScrollBar::handle:horizontal {\
                        background: white;\
                    }\
                    \
                    QScrollBar::add-line:horizontal {\
                        border: none;\
                        background: transparent;\
                    }\
                    \
                    QScrollBar::sub-line:horizontal {\
                        border: none;\
                        background: transparent;\
                    }");

    //    qDebug() << "end construct scrollAreaRuler, parent : " << parent;
    //    qDebug() << "scroolAreaRuler width : " << width();
}

void QScrollAreaRuler::keyPressEvent(QKeyEvent* event)
{
    //    qDebug() << "QScrollAreaRuler::keyPressEvent event : " << event;

    switch (event->key()) {

    case Qt::Key_Control:
        ctrlDown = true;
        break;

    case Qt::Key_Space:
        playPause->onChangeMode();
        break;

    case Qt::Key_Delete:
        emit removeKeyPose();
        break;

    case Qt::Key_I:
        if (shiftDown) {
            emit removeKeyPose();
        } else {
            //            emit keyPoseOnMouseAdded();
            emit addKeyPose();
            // TODO : add key pose on mouse if no key pose selected
        }
        break;

    case Qt::Key_Shift:
        shiftDown = true;
        break;

    case Qt::Key_Left:
        emit previousKeyPose();
        break;

    case Qt::Key_Right:
        emit nextKeyPose();
        break;

    case Qt::Key_Escape:
        animTimeline->hide();
        break;

    case Qt::Key_Up:
        //        spinDuration->setValue(spinDuration->value() + spinDuration->singleStep());
        emit durationChanged(spinDuration->value() + spinDuration->singleStep());
        break;

    case Qt::Key_Down:
        //        spinDuration->setValue(spinDuration->value() - spinDuration->singleStep());
        emit durationChanged(spinDuration->value() - spinDuration->singleStep());
        break;

    case Qt::Key_Z:
        if (ctrlDown) {
            if (shiftDown) {
                emit redo();
            } else {
                emit undo();
            }
        }
        break;

    case Qt::Key_U:
        emit undo();
        break;

    case Qt::Key_R:
//        if (ctrlDown) {
            emit redo();
//        }
    }
}

void QScrollAreaRuler::keyReleaseEvent(QKeyEvent* event)
{
    //    qDebug() << "QScrollAreaRuler::keyReleaseEvent event : " << event;

    switch (event->key()) {

    case Qt::Key_Control:
        ctrlDown = false;
        break;

    case Qt::Key_Shift:
        shiftDown = false;
        break;
    }
}

void QScrollAreaRuler::wheelEvent(QWheelEvent* event)
{
    qDebug() << "scroll area ruler width : " << width();
    int ry = event->angleDelta().ry();

    // change dialog width
    if (shiftDown && ctrlDown) {
        //        if (animTimeline->width() >= 650 || ry >= 0) {
        int curWidth = animTimeline->width();
        int minWidth = animTimeline->minimumWidth();

        if (curWidth == minWidth && ry <= 0)
            return;

        if (ry + curWidth <= minWidth)
            ry = minWidth - curWidth;

        int newX = animTimeline->x() - ry;
        int newY = animTimeline->y() + 1;
        int newWidth = animTimeline->width() + 1 + ry;
        int newHeight = animTimeline->height();

        //        bool wholeRuler = animTimeline->width() -2 == ruler->minimumWidth();

        ruler->onDrawRuler(newWidth - 2);
        //        emit changePrecision(newWidth -2);
        animTimeline->setGeometry(newX, newY, newWidth, newHeight);

        //            int diffWidth = curWidth - animTimeline->width();
        //        ruler->onDrawRuler(qMax(ruler->minimumWidth(), animTimeline->width() - 2));
        //        ruler->onDrawRuler(animTimeline->width() - 2);
        //        }
    }
    // next/previous keyPose
    else if (shiftDown) {
        if (ry > 0) {
            emit nextKeyPose();
        } else {
            emit previousKeyPose();
        }

    }
    // scroll left/right bar
    else if (ctrlDown) {
        //    if (ctrlDown) {
        horizontalScrollBar()->setValue(static_cast<int>(horizontalScrollBar()->value() + ry * SLIDE_SPEED));

    }
    // zoom in/out
    else {
        //        QWidgetRuler * ruler = Ui::ui->scrollAreaWidgetContents;
        //        int newRulerWidth = ruler->minimumWidth() + ry;
        int newRulerWidth = static_cast<int>(ruler->minimumWidth() + ry * ZOOM_SPEED * ruler->minimumWidth() / width());
        //        qDebug() << "newRulerWidth : " << width();
        if (newRulerWidth <= width() - 2) {
            if (ruler->minimumWidth() == width() - 2) {
                return;
            } else {
                newRulerWidth = width() - 2;
            }
        }
        qDebug() << "new ruler width : " << newRulerWidth;
        //    int newRulerWidth = ruler->width() + ry;

        double hScroll = horizontalScrollBar()->value();
        //        double w = width() - 2;
        //        double zoomFactor = static_cast<double>(newRulerWidth) / ruler->minimumWidth();
        double x = event->x();

        double* zero = ruler->getZero();
        double* pixPerSec = ruler->getPixPerSec();

        double time = (hScroll + x - *zero) / *pixPerSec;
        time = selector->nearestStep(time);
        qDebug() << "TIME = " << time;

        //        double hScrollAfterProjection { 0.0 };
        //        // zoom in
        //        if (ry > 0) {
        //            //            double d = w / zoomFactor;
        //            //            double a = x - d / 2;

        //            //            double c = qMax(qMin(a, w - d), 0.0);
        //            //            qDebug() << "hScroll : " << hScroll << ", width() -2 : " << w << ", zoomFactor : " << zoomFactor;
        //            //            qDebug() << "d : " << d << ", a : " << a << ", c : " << c;
        //            //            hScrollAfterProjection = (hScroll + c) * zoomFactor;
        //            double X = x * zoomFactor;

        //            hScrollAfterProjection = hScroll * zoomFactor + X - x;
        //        }
        //        // zoom out
        //        else {

        //            //            double d = w *zoomFactor;
        //            //            double a =x -d /2;
        //            //            double c =a /zoomFactor;
        //            //            hScrollAfterProjection = (hScroll -c)*zoomFactor;

        //            hScrollAfterProjection = x * zoomFactor - x;
        //        }

        //        double temp = *zero;
        //        double prevZero = *zero;
        //        double prevPixPerSec = *pixPerSec;

        //                emit changePrecision(newRulerWidth);
        ruler->onDrawRuler(newRulerWidth);

        double a = time * *pixPerSec + *zero;

        //        double hScrollAfterProjection = (hScroll +x) *zoomFactor -x;
        double hScrollAfterProjection = a - x;

        //        double time = (hScroll +x - *zero) / *pixPerSec;
        //        hScrollAfterProjection = qCeil(((hScroll +x - prevZero) * *pixPerSec) /prevPixPerSec + *zero -x);
        //        hScrollAfterProjection = ((hScroll +x - prevZero) * *pixPerSec) /prevPixPerSec + *zero -x + 0.6;
        //        double diff = *zero -temp;

        //    int gap = (ry > 0) ? (ruler->minimumWidth() / 4) : (-ruler->minimumWidth() / 4);
        //    int gap = (ry *ruler->minimumWidth()) / 500;

        //        double x = event->x();
        //        int endScroll = ruler->minimumWidth() -width();
        //        double ratio = x /width();
        //                horizontalScrollBar()->setValue(static_cast<int>(endScroll * ratio));
        qDebug() << "scroll : " << hScrollAfterProjection;
        horizontalScrollBar()->setValue(static_cast<int>(hScrollAfterProjection));
    }
    event->accept(); // parent is animTimeline (root) with non event catching
}

void QScrollAreaRuler::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) {
        setCursor(Qt::SplitHCursor);
        mousePosX = event->x();
        sliderPos = horizontalScrollBar()->value();

        midMouseDown = true;
    }
}

void QScrollAreaRuler::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) {
        midMouseDown = false;
        setCursor(Qt::ArrowCursor);
    }
}

void QScrollAreaRuler::mouseMoveEvent(QMouseEvent* event)
{
    if (midMouseDown) {
        horizontalScrollBar()->setValue((sliderPos + mousePosX - event->x()));
    }
}

//void QScrollAreaRuler::scrollContentsBy(int dx, int dy)
//{
//    //    qDebug() << "QScrollAreaRuler::scrollContentsBy";
//    qDebug() << "scroll : " << horizontalScrollBar()->value();
//}

void QScrollAreaRuler::setAnimTimeline(AnimTimeline* value)
{
    animTimeline = value;
}

void QScrollAreaRuler::setPlayPause(QToolButtonPlayPause* value)
{
    playPause = value;
}

void QScrollAreaRuler::setRuler(QWidgetRuler* value)
{
    ruler = value;
}

bool* QScrollAreaRuler::getShiftDown()
{
    return &shiftDown;
}

void QScrollAreaRuler::onKeyPress(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Right:
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_Down:
        return;
    }
    keyPressEvent(event);
}

void QScrollAreaRuler::onKeyRelease(QKeyEvent* event)
{
    keyReleaseEvent(event);
}

bool* QScrollAreaRuler::getMidMouseDown()
{
    return &midMouseDown;
}

void QScrollAreaRuler::setSelector(QFrameSelector* value)
{
    selector = value;
}

//void QScrollAreaRuler::setZero(double *value)
//{
//    zero = value;
//}

//void QScrollAreaRuler::onZoomRuler(QWheelEvent* event, double xr)
//{
//    qDebug() << "onZoomRuler : xr = " << xr;
//    int ry = event->angleDelta().ry();
//    // zoom in/out
//    //        QWidgetRuler * ruler = Ui::ui->scrollAreaWidgetContents;
//    //        int newRulerWidth = ruler->minimumWidth() + ry;
//    int newRulerWidth = ruler->minimumWidth() + ry * ruler->minimumWidth() / width();
//    //        qDebug() << "newRulerWidth : " << width();
//    if (newRulerWidth <= width() - 2) {
//        if (ruler->minimumWidth() == newRulerWidth) {
//            return;
//        } else {
//            newRulerWidth = width() - 2;
//        }
//    }
//    qDebug() << "new ruler width : " << newRulerWidth;
//    //    int newRulerWidth = ruler->width() + ry;

//    double hScroll = horizontalScrollBar()->value();
//    double w = width() - 2;
//    double zoomFactor = static_cast<double>(newRulerWidth) / ruler->minimumWidth();
//    qDebug() << "onZoomRuler : zoomFactor = " << zoomFactor;
//    double x = event->x();
//    qDebug() << "onZoomRuler : x = " << x;

//    double hScrollAfterProjection { 0.0 };

//    hScrollAfterProjection = (hScroll +x) *zoomFactor -x;

//    //                emit changePrecision(newRulerWidth);
//    ruler->onDrawRuler(newRulerWidth);

//    //    int gap = (ry > 0) ? (ruler->minimumWidth() / 4) : (-ruler->minimumWidth() / 4);
//    //    int gap = (ry *ruler->minimumWidth()) / 500;

//    //        double x = event->x();
//    //        int endScroll = ruler->minimumWidth() -width();
//    //        double ratio = x /width();
//    //                horizontalScrollBar()->setValue(static_cast<int>(endScroll * ratio));
//    qDebug() << "scroll : " << hScrollAfterProjection;
//    horizontalScrollBar()->setValue(static_cast<int>(hScrollAfterProjection));
//}

bool* QScrollAreaRuler::getCtrlDown()
{
    return &ctrlDown;
}

void QScrollAreaRuler::setSpinDuration(QDoubleSpinBoxSmart* value)
{
    spinDuration = value;
}
