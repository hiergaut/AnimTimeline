#include "qscrollarearuler.h"

#include "animtimeline.h"
#include "ui_animtimeline.h"

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>

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
    qDebug() << "QScrollAreaRuler::keyPressEvent event : " << event;

    switch (event->key()) {

    case Qt::Key_Control:
        ctrlDown = true;
        break;

    case Qt::Key_Space:
        playPause->onChangeMode();
        break;

    case Qt::Key_I:
        if (shiftDown) {
            emit removeKeyPose();
        } else {
            emit addKeyPose();
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
    }
}

void QScrollAreaRuler::keyReleaseEvent(QKeyEvent* event)
{
    qDebug() << "QScrollAreaRuler::keyReleaseEvent event : " << event;
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
    if (shiftDown) {
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

        animTimeline->setGeometry(newX, newY, newWidth, newHeight);

        //            int diffWidth = curWidth - animTimeline->width();
        ruler->onDrawRuler(qMax(ruler->minimumWidth(), animTimeline->width() - 2));
        //        }
        return;
    }

    //    int gap = (ry > 0) ? (ruler->minimumWidth() / 4) : (-ruler->minimumWidth() / 4);
    //    int gap = (ry *ruler->minimumWidth()) / 500;
    int newRulerWidth = ruler->minimumWidth() + ry;
    if (newRulerWidth <= width() - 2) {
        if (ruler->minimumWidth() == width() - 2) {
            return;
        } else {
            newRulerWidth = width() - 2;
        }
    }
    qDebug() << "new ruler width : " << newRulerWidth;
    //    int newRulerWidth = ruler->width() + ry;

    // scroll left/right bar
    if (ctrlDown) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 3 * ry);

        // zoom in/out
    } else {
        emit changePrecision(newRulerWidth);

        //        double x = event->x();
        //        int endScroll = ruler->minimumWidth() -width();
        //        double ratio = x /width();
        //        horizontalScrollBar()->setValue(static_cast<int>(endScroll * ratio));
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

void QScrollAreaRuler::onKeyPress(QKeyEvent *event) {
    keyPressEvent(event);
}

void QScrollAreaRuler::onKeyRelease(QKeyEvent *event)
{
    keyReleaseEvent(event);
}
