#include "qwidgetruler.h"

#include <QWheelEvent>
#include <QDebug>

QWidgetRuler::QWidgetRuler(QWidget *parent) : QWidget(parent)
{
//    qDebug() << "QWidgetRuler created";
//    emit onChangePrecision(0);
//    setMinimumWidth(500);
    updateTimeline(500);
}

int QWidgetRuler::updateTimeline(int newWidth) {
//    int newWidth = 500 +accuracy;

    int iStep =0;
    while (iStep < sizeof(steps) && newWidth * steps[iStep] < 50 * duration) iStep++;

    if (iStep == sizeof (steps)) {
        return width();
    }

    step =steps[iStep];

    nbInterval = duration / step +2;
    wInterval = newWidth / nbInterval;

    return newWidth;
}

//void QWidgetRuler::wheelEvent(QWheelEvent *event)
//{
////    qDebug() << "QWidgetRuler: wheelEvent " << event->angleDelta().ry();
////    setMinimumWidth(width() +event->angleDelta().ry() /2);
//    //    event->accept();
//}

void QWidgetRuler::onChangePrecision(int accuracy)
{
//    qDebug() << "QWidgetRuler: onChangePrecision";

//    int newWidth =width() +accuracy;

    int newWidth = updateTimeline(width() + accuracy);
    setMinimumWidth(newWidth);
//    update();

//    updateTimeline();

//    emit changeScale();

}

int QWidgetRuler::getWInterval() const
{
    return wInterval;
}

int QWidgetRuler::getNbInterval() const
{
    return nbInterval;
}

double QWidgetRuler::getStep() const
{
    return step;
}
