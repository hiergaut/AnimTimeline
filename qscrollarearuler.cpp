#include "qscrollarearuler.h"

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
//#include <QtMath>

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
//    event->ignore();
//    event->accept(); // avoid Esc, QDialog::reject()
}

void QScrollAreaRuler::keyReleaseEvent(QKeyEvent* event)
{
//    qDebug() << "QScrollAreaRuler::keyReleaseEvent";
    if (event->key() == Qt::Key_Control) {
        ctrlDown = false;
    }
    if (event->key() == Qt::Key_Shift) {
        shiftDown = false;
    }
}

void QScrollAreaRuler::wheelEvent(QWheelEvent* event)
{
    //  qDebug() << "QScrollAreaRuler: wheelEvent " << event->angleDelta().ry();
    //  setMinimumWidth(width() + event->angleDelta().ry() / 2);
    if (ctrlDown) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + event->angleDelta().ry());

    } else {
        emit changePrecision(event->angleDelta().ry());
    }
    //    if (event->type() == QEvent::)
    event->accept();
}

void QScrollAreaRuler::mousePressEvent(QMouseEvent* event)
{
//    qDebug() << "QScrollAreaRuler::mousePressEvent";
    if (event->button() == Qt::MiddleButton) {
        setCursor(Qt::SplitHCursor);
        mousePosX = event->x();
        //        prevMouseX = horizontalScrollBar()->value();
        //        prevMouseX = 0;
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
        //        int newMouseX = horizontalScrollBar()->value() + event->x() - mousePosX;
        //        int ratio = log10( horizontalScrollBar()->maximum());
        //        int ratio = horizontalScrollBar()->maximum() /100 +1;
        //        horizontalScrollBar()->setValue((sliderPos +mousePosX -event->x()) *ratio);
        horizontalScrollBar()->setValue((sliderPos + mousePosX - event->x()));
        //        qDebug() << horizontalScrollBar()->maximum();
        //        prevMouseX = newMouseX;
    }
}
