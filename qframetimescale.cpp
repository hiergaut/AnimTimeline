#include "qframetimescale.h"

#include "qwidgetruler.h"
#include <QPainter>

QFrameTimescale::QFrameTimescale(QWidget* parent)
    : QFrame(parent)
{
    widgetRuler = static_cast<QWidgetRuler*>(parent);

    nbInterval = widgetRuler->getNbInterval();
    step = widgetRuler->getStep();
    pixPerSec = widgetRuler->getPixPerSec();
}

void QFrameTimescale::paintEvent(QPaintEvent* event)
{
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.drawText(0, 11, "sec");
    for (int i = 1; i < *nbInterval; i++) {
        int x = static_cast<int>(*pixPerSec * *step * i);
        QString time = QString::number((i - 1) * *step);
        int dec = time.size() * 6 / 2;
        painter.drawText(x - dec, 11, time);
    }
}
