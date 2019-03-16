#include "qscrollarearuler.h"

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
}

void QScrollAreaRuler::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlDown = true;
    }
    if (event->key() == Qt::Key_I) {
        if (shiftDown) {
            emit removeKeyPose();
        } else {
            emit addKeyPose();
        }
    }
    if (event->key() == Qt::Key_Shift) {
        shiftDown = true;
    }
}

void QScrollAreaRuler::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlDown = false;
    }
    if (event->key() == Qt::Key_Shift) {
        shiftDown = false;
    }
}

void QScrollAreaRuler::wheelEvent(QWheelEvent* event)
{
    if (ctrlDown) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + event->angleDelta().ry());

    } else {
        emit changePrecision(event->angleDelta().ry());
    }
    event->accept();
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
