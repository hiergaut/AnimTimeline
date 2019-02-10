#include "qframetimescale.h"

#include <QPainter>
#include "qwidgetruler.h"
#include <QDebug>

QFrameTimescale::QFrameTimescale(QWidget* parent)
    : QFrame(parent)
{
}

void QFrameTimescale::paintEvent(QPaintEvent* event)
{

//    double duration =10.0;
//    double steps[] = {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
//    int iStep =0;
//    while (width() * steps[iStep] < 50 *duration) iStep++;

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.drawText(0, 11, "sec");
//    qDebug() << parent();
    int nbInterval = dynamic_cast<QWidgetRuler*>(parent())->getNbInterval();
    int wInterval = dynamic_cast<QWidgetRuler*>(parent())->getWInterval();
    double step = dynamic_cast<QWidgetRuler*>(parent())->getStep();
//    int nbInterval =10;
//    int wInterval =100;
//    double step = steps[iStep];
//    int nbInterval = duration /step +2;
//    int wInterval = width() / nbInterval;
    for (int i = 1; i < nbInterval; i++) {
        int x = wInterval * i;
        //        painter.drawLine(x, 0, x, height());
        QString time =QString::number((i -1) * step);
        int dec = time.size() * 5 / 2;
        painter.drawText(x -dec, 11, time);
    }

    //    QPen linePen;
    //    linePen.setWidth(1);
    //    painter.setPen(linePen);

    //    painter.drawLine(0, 0, 100, 100);

    //    QRectF rect = this->rect();
    //    painter.fillRect(QRect(rect.left(), rect.top(), this->width(), 40), QColor(0, 255, 0));
}
