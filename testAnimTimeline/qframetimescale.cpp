#include "qframetimescale.h"

#include <QPainter>

QFrameTimescale::QFrameTimescale(QWidget* parent)
    : QFrame(parent)
{
}

void QFrameTimescale::paintEvent(QPaintEvent* event)
{
    int animDuration =10.0f;

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.drawText(0, 11, "sec");
    int nbInterval = 20;
    int wInterval = width() / nbInterval;
    for (int i = 1; i < nbInterval; i++) {
        int x = wInterval * i;
        //        painter.drawLine(x, 0, x, height());
        QString time =QString::number(i/10.0);
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
