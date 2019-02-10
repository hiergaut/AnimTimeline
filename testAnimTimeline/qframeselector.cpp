#include "qframeselector.h"

#include <QPainter>
#include <QDebug>
#include <QWheelEvent>

QFrameSelector::QFrameSelector(QWidget *parent) : QFrame (parent), widgetRuler(static_cast<QWidgetRuler*>(parent))
{
//    qDebug() << "parent =" << parent;
//    qDebug() << "QFrameSelector created";
}

void QFrameSelector::paintEvent(QPaintEvent *event)
{
//    qDebug() << "QFrameSelector: paintEvent";
//    double duration =10.0;
//    double steps[] = {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
//    int iStep =0;
//    while (width() * steps[iStep] < 50 *duration) iStep++;


    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    int nbInterval = widgetRuler->getNbInterval();
    int wInterval = widgetRuler->getWInterval();

//    qDebug() << "QFrameSelector: nbInterval =" << nbInterval;
//    qDebug() << "QFrameSelector: wwInterval =" << wInterval;


//    double step = static_cast<QWidgetRuler*>(parent())->getStep();

//    double step = steps[iStep];
//    int nbInterval =duration /step +2;
//    int wInterval =width() /nbInterval;


    painter.setPen(Qt::darkGray);
    for (int i =1; i <nbInterval; i++) {
        int x =wInterval *i;
        painter.drawLine(x, 0, x, height());
    }

    painter.setPen(QPen(Qt::lightGray));
    for (int i =1; i <nbInterval -1; i++) {
        int x =wInterval *i;
        painter.drawLine(x +wInterval /2, 0, x +wInterval /2, height());
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

void QFrameSelector::onRedrawScale()
{
}

//void QFrameSelector::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "QFrameSelector: wheelEvent";
//    emit changePrecision(1);

//}
