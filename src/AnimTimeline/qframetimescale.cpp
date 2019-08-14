//#include <AnimTimeline/qframetimescale.h>
#include "qframetimescale.h"
//#include <AnimTimeline/qwidgetruler.h>
#include "qwidgetruler.h"

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

void QFrameTimescale::paintEvent(QPaintEvent*)
{
    if (m_rulerChanged) {
        int w = width();
        int h = height();

        if (m_pixmapBackground != nullptr)
            delete m_pixmapBackground;
        m_pixmapBackground = new QPixmap(this->size());
        m_pixmapBackground->fill(QColor(255, 255, 255, 0));
        //    m_pixmapBackground->fill(QColor(255, 0, 0, 255));

        QPainter painter(m_pixmapBackground);
        //        int h = height;
        painter.setRenderHint(QPainter::HighQualityAntialiasing);

        painter.drawText(0, 11, "sec");
        for (int i = 1; i < *nbInterval; i++) {
            int x = static_cast<int>(*pixPerSec * *step * i);
            QString time = QString::number((i - 1) * *step);
            int dec = time.size() * 6 / 2;
            painter.drawText(x - dec, 11, time);
        }

        m_rulerChanged = false;
    }

    QPainter painter(this);
    painter.drawPixmap(rect(), *m_pixmapBackground);
}

//void QFrameTimescale::resizeEvent(QResizeEvent *event)
//{
////    qDebug() << "QFrameTimescale::resizeEvent " << ++counter;

////    QSize size = event->size();

//}

void QFrameTimescale::onRulerChange()
{
    m_rulerChanged = true;
}

void QFrameTimescale::setDrawLock(bool* value)
{
    drawLock = value;
}
