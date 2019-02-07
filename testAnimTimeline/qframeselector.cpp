#include "qframeselector.h"

#include <QPainter>
#include <QDebug>
#include <QWheelEvent>

QFrameSelector::QFrameSelector(QWidget *parent) : QFrame (parent)
{
    qDebug() << "parent =" << parent;

}

void QFrameSelector::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    int nbInterval =10;
    int wInterval =width() /nbInterval;
    for (int i =0; i <nbInterval; i++) {
        int x =wInterval *i;
        painter.drawLine(x, 0, x, height());
    }

//    QPen linePen;
//    linePen.setWidth(1);
//    painter.setPen(linePen);

//    painter.drawLine(0, 0, 100, 100);

//    QRectF rect = this->rect();
//    painter.fillRect(QRect(rect.left(), rect.top(), this->width(), 40), QColor(0, 255, 0));


}

bool QFrameSelector::eventFilter(QObject *watched, QEvent *event)
{

}

//void QFrameSelector::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "QFrameSelector: wheelEvent";
//    emit changePrecision(1);

//}
