#include "qframetimescale.h"

#include <QPainter>

QFrameTimescale::QFrameTimescale(QWidget* parent)
    : QFrame(parent)
{
}

void QFrameTimescale::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    int nbInterval = 10;
    int wInterval = width() / nbInterval;
    for (int i = 0; i < nbInterval; i++) {
        int x = wInterval * i;
//        painter.drawLine(x, 0, x, height());
        painter.drawText(x, 11, "0ms");
    }

    //    QPen linePen;
    //    linePen.setWidth(1);
    //    painter.setPen(linePen);

    //    painter.drawLine(0, 0, 100, 100);

    //    QRectF rect = this->rect();
    //    painter.fillRect(QRect(rect.left(), rect.top(), this->width(), 40), QColor(0, 255, 0));
}
