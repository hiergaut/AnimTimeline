#include <AnimTimeline/qwidgetruler.h>

#include <AnimTimeline/configurations.h>

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
#include <QtMath>

QWidgetRuler::QWidgetRuler(QWidget* parent)
    : QWidget(parent)
{
}

int QWidgetRuler::drawRuler(int width)
{
    int iStep = 0;
    while (iStep < nbSteps && width * steps[iStep] < 50 * maxDuration)
        iStep++;

    if (iStep == nbSteps) {
        qDebug() << "\033[31mQWidgetRuler::drawRuler : " << width << " too short step\033[0m";

        return this->width();
    }

    step = steps[iStep];
    spinStart->setSingleStep(0.5 * step);
    spinEnd->setSingleStep(0.5 * step);
    spinCursor->setSingleStep(0.5 * step);
    spinDuration->setSingleStep(0.5 * step);

    nbInterval = qCeil(maxDuration / step) + 2;
    pixPerSec = (width / double(nbInterval)) / step;

    zero = pixPerSec * step;
    setMinimumWidth(width);

    update();

    return width;
}

void QWidgetRuler::onDrawRuler(int width)
{
    drawRuler(width);
}

void QWidgetRuler::setCtrlDown(bool* value)
{
    ctrlDown = value;
}

void QWidgetRuler::setShiftDown(bool* value)
{
    shiftDown = value;
}

void QWidgetRuler::setSpinDuration(QDoubleSpinBoxSmart* value)
{
    spinDuration = value;
}

void QWidgetRuler::setSpinCursor(QDoubleSpinBoxSmart* value)
{
    spinCursor = value;
}

void QWidgetRuler::setSpinEnd(QDoubleSpinBoxSmart* value)
{
    spinEnd = value;
}

void QWidgetRuler::setSpinStart(QDoubleSpinBoxSmart* value)
{
    spinStart = value;
}

bool* QWidgetRuler::getSelectorLock()
{
    return &selectorLock;
}

bool* QWidgetRuler::getTimescaleLock()
{
    return &timescaleLock;
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
