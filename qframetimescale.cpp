//#include "qframetimescale.h"
#include <AnimTimeline/qframetimescale.h>
//#include "qwidgetruler.h"
#include <AnimTimeline/qwidgetruler.h>

#include <QDebug>
#include <QPainter>

QFrameTimescale::QFrameTimescale(QWidget* parent)
    : QFrame(parent)
{
    widgetRuler = static_cast<QWidgetRuler*>(parent);

    nbInterval = widgetRuler->getNbInterval();
    step = widgetRuler->getStep();
    pixPerSec = widgetRuler->getPixPerSec();

    drawLock = widgetRuler->getTimescaleLock();
}

void QFrameTimescale::paintEvent(QPaintEvent* event)
{
    (void)event;

    //    if (! *drawLock) {
    qDebug() << "QFrameTimescale::paintEvent " << ++counter;

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.drawText(0, 11, "sec");
    for (int i = 1; i < *nbInterval; i++) {
        int x = static_cast<int>(*pixPerSec * *step * i);
        QString time = QString::number((i - 1) * *step);
        int dec = time.size() * 6 / 2;
        painter.drawText(x - dec, 11, time);
    }

    //        *drawLock =true;
    //    }
}

void QFrameTimescale::setDrawLock(bool* value)
{
    drawLock = value;
}
