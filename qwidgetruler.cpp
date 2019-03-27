#include "qwidgetruler.h"

#include <QWheelEvent>
#include <QtMath>

QWidgetRuler::QWidgetRuler(QWidget* parent)
    : QWidget(parent)
{
    updateTimeline(500); // ui fixed width of ruler
}

int QWidgetRuler::updateTimeline(int newWidth)
{
    int iStep = 0;
    while (iStep < nbSteps && newWidth * steps[iStep] < 50 * maxDuration)
        iStep++;

    if (iStep == nbSteps) {
        //        qDebug() << "QWidgetRuler::updateTimeline: " << newWidth << " too short";
        return width();
    }

    step = steps[iStep];

    nbInterval = qCeil(maxDuration / step) + 2;
    pixPerSec = (newWidth / double(nbInterval)) / step;

    zero = pixPerSec * step;

    return newWidth;
}

void QWidgetRuler::onChangePrecision(int accuracy)
{
    int newWidth = updateTimeline(width() + accuracy);
    setMinimumWidth(newWidth);
}

void QWidgetRuler::setMaxDuration(double value)
{
    //    if (value != maxDuration) {
    if (qAbs(value - maxDuration) > 1e-5) {

        maxDuration = value;
        updateTimeline(width());
        update();
        emit durationChanged(value);
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
