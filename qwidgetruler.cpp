#include "qwidgetruler.h"

#include <QDebug>
#include <QWheelEvent>
#include <QtMath>

QWidgetRuler::QWidgetRuler(QWidget* parent)
    : QWidget(parent)
{
    //    qDebug() << "QWidgetRuler created";
    //    emit onChangePrecision(0);
    //    setMinimumWidth(500);
    updateTimeline(500); // ui fixed width of ruler
}

int QWidgetRuler::updateTimeline(int newWidth)
{
    //    int newWidth = 500 +accuracy;

    int iStep = 0;
    while (iStep < nbSteps && newWidth * steps[iStep] < 50 * maxDuration)
        iStep++;

    if (iStep == nbSteps) {
        qDebug() << "can't compress more ruler";
        return width();
    }

    step = steps[iStep];

    nbInterval = qCeil(maxDuration / step) + 2;
    //    wInterval = newWidth / nbInterval;
    pixPerSec = (newWidth / float(nbInterval)) / step;

    zero = pixPerSec * step;

    //    emit rulerChange(step, nbInterval, pixPerSec);
    //    leftSpacer->changeSize(0, 0);

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
    //    int sign =(accuracy > 0) ?(2) :(0.5);

    //    int newWidth = updateTimeline(width() + accuracy *log10(width()));
    int newWidth = updateTimeline(width() + accuracy);
    //    int newWidth = updateTimeline(width() * sign);
    setMinimumWidth(newWidth);
    //    update();

    //    leftSpacer->changeSize(wInterval +wInterval *start / maxDuration, 0);
    //    leftSpacer->minimumSize(0);
    //    updateTimeline();
    //    qDebug() << leftSpacer;
    //    playZone->setMinimumWidth((nbInterval -2) *wInterval * (end -start) / maxDuration);
    //    frameSelector->update();

    //    emit changeScale();
}

void QWidgetRuler::setMaxDuration(double value)
{
    if (value != maxDuration) {
        qDebug() << "QWidgetRuler::setMaxDuration: " << value;

        maxDuration = value;
        updateTimeline(width());
        update();
        emit durationChanged(value);
        //    onChangePrecision(0);
    }
}

double* QWidgetRuler::getMaxDuration()
{
    return &maxDuration;
}

double* QWidgetRuler::getZero()
{
    return &zero;
}

double* QWidgetRuler::getPixPerSec()
{
    return &pixPerSec;
}

//void QWidgetRuler::setFrameSelector(QFrameSelector *value)
//{
//    frameSelector = value;
//}

//int* QWidgetRuler::getWInterval() const
//{
//    return &wInterval;
//}

int* QWidgetRuler::getNbInterval()
{
    return &nbInterval;
}

double* QWidgetRuler::getStep()
{
    return &step;
}

//void QWidgetRuler::setLeftSlider(QLabel *value)
//{
//    leftSlider = value;
//}

//void QWidgetRuler::setLeftSpacer(QSpacerItem *value)
//{
//    leftSpacer = value;
//}

//void QWidgetRuler::setPlayZone(QFrame *value)
//{
//    playZone = value;
//}
