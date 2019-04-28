//#include "qwidgetruler.h"
#include <AnimTimeline/qwidgetruler.h>

//#include "ui_animtimeline.h"
//#include "animtimeline.h"

//#include "configurations.h"
#include <AnimTimeline/configurations.h>

#include <QDebug>
#include <QWheelEvent>
#include <QtMath>
//#include <QScrollArea>
#include <QScrollBar>

QWidgetRuler::QWidgetRuler(QWidget* parent)
    : QWidget(parent)
{
    //    updateTimeline(parent->width());
//    qDebug() << "end construct widgetRuler, parent : " << parent;
//    qDebug() << "ruler width " << width();
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
    //    qDebug() << "ruler parent : " << this->parent();

    //    setMinimumWidth(width -100);
    //    timescaleLock =selectorLock =false;
    setMinimumWidth(width);

    update();

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

void QWidgetRuler::setCtrlDown(bool* value)
{
    ctrlDown = value;
}

void QWidgetRuler::setShiftDown(bool* value)
{
    shiftDown = value;
}

//bool* QWidgetRuler::getCtrlDown() const
//{
//    return ctrlDown;
//}

//void QWidgetRuler::setAreaRuler(QScrollAreaRuler* value)
//{
//    areaRuler = value;
//}

void QWidgetRuler::setSpinDuration(QDoubleSpinBoxSmart* value)
{
    spinDuration = value;
}

//void QWidgetRuler::wheelEvent(QWheelEvent* event)
//{
//    // zoom in/out
//    if (!*ctrlDown && !*shiftDown) {
//        double xr = event->x();
////        Ui::ui->scrollAreaWidgetContents->wheelEvent(event);
//        emit rulerZoomed(event, xr);

//        event->accept();
//    }
//}
//        int ry = event->angleDelta().ry();

//        int newRulerWidth = minimumWidth() + ry * minimumWidth() / areaRuler->width();
//        //        qDebug() << "newRulerWidth : " << width();
//        if (newRulerWidth <= areaRuler->width() - 2) {
//            if (minimumWidth() == newRulerWidth) {
//                return;
//            } else {
//                newRulerWidth = areaRuler->width() - 2;
//            }
//        }
//        qDebug() << "new ruler width : " << newRulerWidth;
//        //    int newRulerWidth = ruler->width() + ry;

//        double hScroll = areaRuler->horizontalScrollBar()->value();
//        double w = areaRuler->width() - 2;
//        double zoomFactor = static_cast<double>(newRulerWidth) / minimumWidth();
//        double x = event->x();

//        double hScrollAfterProjection { 0.0 };
//        // zoom in
//        if (ry > 0) {
//            double d = w / zoomFactor;
//            double a = x - d / 2;

//            double c = qMax(qMin(a, w - d), 0.0);
//            qDebug() << "hScroll : " << hScroll << ", width() -2 : " << w << ", zoomFactor : " << zoomFactor;
//            qDebug() << "d : " << d << ", a : " << a << ", c : " << c;
//            hScrollAfterProjection = (hScroll + c) * zoomFactor;
//        }
//        // zoom out
//        else {

//            double d = w * zoomFactor;
//            double a = x - d / 2;
//            double c = a / zoomFactor;
//            hScrollAfterProjection = (hScroll - c) * zoomFactor;
//        }

//        //        emit changePrecision(newRulerWidth);
//        onDrawRuler(newRulerWidth);

//        //    int gap = (ry > 0) ? (ruler->minimumWidth() / 4) : (-ruler->minimumWidth() / 4);
//        //    int gap = (ry *ruler->minimumWidth()) / 500;

//        //        double x = event->x();
//        //        int endScroll = ruler->minimumWidth() -width();
//        //        double ratio = x /width();
//        //                horizontalScrollBar()->setValue(static_cast<int>(endScroll * ratio));
//        qDebug() << "scroll : " << hScrollAfterProjection;
//        areaRuler->horizontalScrollBar()->setValue(static_cast<int>(hScrollAfterProjection));

//        event->accept();
//    }
////    event->ignore();
//}

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

//bool * QWidgetRuler::getDrawLock()
//{
//    return &drawLock;
//}

// EXTERNAL SLOT
//void QWidgetRuler::setMaxDuration(double value)
//{
//    //    if (value != maxDuration) {
//    if (qAbs(value - maxDuration) > 1e-5) {

//        maxDuration = value;
//        drawRuler(width());
//        update();

////        emit durationChanged(value); //external signal
//    }
//}

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
