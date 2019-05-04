#include <AnimTimeline/qscrollarearuler.h>

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
        emit durationChanged(spinDuration->value() + spinDuration->singleStep());
        break;

    case Qt::Key_Down:
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
        emit redo();
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
//    qDebug() << "scroll area ruler width : " << width();
    int ry = event->angleDelta().ry();

    // change dialog width
    if (shiftDown && ctrlDown) {
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

        ruler->onDrawRuler(newWidth - 2);
        animTimeline->setGeometry(newX, newY, newWidth, newHeight);

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
        horizontalScrollBar()->setValue(static_cast<int>(horizontalScrollBar()->value() + ry * SLIDE_SPEED));

    }
    // zoom in/out
    else {
        int newRulerWidth = static_cast<int>(ruler->minimumWidth() + ry * ZOOM_SPEED * ruler->minimumWidth() / width());
        if (newRulerWidth <= width() - 2) {
            if (ruler->minimumWidth() == width() - 2) {
                return;
            } else {
                newRulerWidth = width() - 2;
            }
        }
//        qDebug() << "new ruler width : " << newRulerWidth;

        double hScroll = horizontalScrollBar()->value();
        double x = event->x();

        double* zero = ruler->getZero();
        double* pixPerSec = ruler->getPixPerSec();

        double time = (hScroll + x - *zero) / *pixPerSec;
        time = selector->nearestStep(time);
//        qDebug() << "TIME = " << time;

        ruler->onDrawRuler(newRulerWidth);

        double a = time * *pixPerSec + *zero;

        double hScrollAfterProjection = a - x;

//        qDebug() << "scroll : " << hScrollAfterProjection;
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

void QScrollAreaRuler::setCursorSpin(QDoubleSpinBoxSmart* value)
{
    cursorSpin = value;
}

bool* QScrollAreaRuler::getMidMouseDown()
{
    return &midMouseDown;
}

void QScrollAreaRuler::setSelector(QFrameSelector* value)
{
    selector = value;
}

bool* QScrollAreaRuler::getCtrlDown()
{
    return &ctrlDown;
}

void QScrollAreaRuler::setSpinDuration(QDoubleSpinBoxSmart* value)
{
    spinDuration = value;
}
