#include "qwidgetruler.h"

//#include "ui_animtimeline.h"
//#include "animtimeline.h"

#include "constants.h"

#include <QDebug>
#include <QWheelEvent>
#include <QtMath>

QWidgetRuler::QWidgetRuler(QWidget* parent)
    : QWidget(parent)
{
//    updateTimeline(parent->width());
    qDebug() << "end construct widgetRuler, parent : " << parent;
    qDebug() << "ruler width " << width();
//    drawRuler(width()); // ui fixed width of ruler
//    setSizePolicy(QSizePolicy::Fixed, sizePolicy().verticalPolicy());
//    setMinimumWidth(1000);
//    update();
}

int QWidgetRuler::drawRuler(int width)
{
    int iStep = 0;
    while (iStep < nbSteps && width * steps[iStep] < 50 * maxDuration)
        iStep++;

    if (iStep == nbSteps) {
                qDebug() << "QWidgetRuler::drawRuler: " << width << " too short";

        return this->width();
    }

    step = steps[iStep];

    nbInterval = qCeil(maxDuration / step) + 2;
    pixPerSec = (width / double(nbInterval)) / step;

    zero = pixPerSec * step;
    qDebug() << "ruler parent : " << this->parent();

//    setMinimumWidth(width -100);
//    timescaleLock =selectorLock =false;
    setMinimumWidth(width);

//    update();

    return width;
}

void QWidgetRuler::onDrawRuler(int width)
{
//    int width = updateTimeline(width() + accuracy);
    drawRuler(width);
//    drawLock =true;
//    update();
//    setMinimumWidth(width);
}

bool * QWidgetRuler::getSelectorLock()
{
    return &selectorLock;
}

bool * QWidgetRuler::getTimescaleLock()
{
    return &timescaleLock;
}

//bool * QWidgetRuler::getDrawLock()
//{
//    return &drawLock;
//}

void QWidgetRuler::setMaxDuration(double value)
{
    //    if (value != maxDuration) {
    if (qAbs(value - maxDuration) > 1e-5) {

        maxDuration = value;
        drawRuler(width());
        update();

        emit durationChanged(value); //external signal
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

int* QWidgetRuler::getNbInterval()
{
    return &nbInterval;
}

double* QWidgetRuler::getStep()
{
    return &step;
}
