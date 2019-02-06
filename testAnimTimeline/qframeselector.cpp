#include "qframeselector.h"

#include <QPainter>
#include <QDebug>

QFrameSelector::QFrameSelector(QWidget *parent) : QFrame (parent)
{
    qDebug() << "parent =" << parent;

}

void QFrameSelector::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QPen linePen;
    linePen.setWidth(1);
    painter.setPen(linePen);

    painter.drawLine(0, 0, 10, 10);

    QRectF rect = this->rect();
    painter.fillRect(QRect(rect.left(), rect.top(), this->width(), 40), QColor(0, 255, 0));
}
