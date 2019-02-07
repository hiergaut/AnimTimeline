#include "qwidgetruler.h"

#include <QWheelEvent>
#include <QDebug>

QWidgetRuler::QWidgetRuler(QWidget *parent) : QWidget(parent)
{

}

//void QWidgetRuler::wheelEvent(QWheelEvent *event)
//{
////    qDebug() << "QWidgetRuler: wheelEvent " << event->angleDelta().ry();
////    setMinimumWidth(width() +event->angleDelta().ry() /2);
//    //    event->accept();
//}

void QWidgetRuler::onChangePrecision(int accuracy)
{
    setMinimumWidth(width() + accuracy);
}
