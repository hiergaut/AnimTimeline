#include "qlabelslider.h"

#include <QDebug>
#include <QMouseEvent>

QLabelSlider::QLabelSlider(QWidget *parent) : QLabel(parent)
{

}

void QLabelSlider::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "QLabelSlider";

}

void QLabelSlider::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "QLabelSlider::mousePressEvent";
//    event->ignore();
    if (event->button() == Qt::LeftButton) {

        clicked =true;
        x =event->x();
    }

}

void QLabelSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        clicked =false;
    }
}

void QLabelSlider::mouseMoveEvent(QMouseEvent *event)
{
    if (clicked) {
//        int newX =event->x();
//        int deltaX =newX -x;

//        qDebug() << "QLabelSlider: deltaX = " << deltaX;
//        if (abs(deltaX) > 5) {
//            emit slide(deltaX);
            emit slide(event->x());

//            x =newX;
//        }
    }

}
