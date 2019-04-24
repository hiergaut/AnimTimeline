#include "qspinboxsmart.h"
#include <QWheelEvent>

QSpinBoxSmart::QSpinBoxSmart(QWidget *parent) : QSpinBox(parent)
{

}

void QSpinBoxSmart::wheelEvent(QWheelEvent *event)
{

    int ry = (event->angleDelta().ry() > 0) ?(1) :(-1);
    if (ry > 0) {
        emit nextKeyPose();
    }
    else {
        emit previousKeyPose();
    }
    event->accept();
}

void QSpinBoxSmart::keyPressEvent(QKeyEvent *event)
{
    emit deleteKeyPose();

}
