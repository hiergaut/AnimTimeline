#include "qframeselector.h"

#include <QPainter>
#include <QDebug>
#include <QWheelEvent>

QFrameSelector::QFrameSelector(QWidget *parent) : QFrame (parent)
{
    widgetRuler =static_cast<QWidgetRuler*>(parent);
//    qDebug() << "parent =" << parent;
//    qDebug() << "QFrameSelector created";
//    leftSpacer = findChild<QSpacerItem*>("horizontalSpacer");
//    leftSlider = findChild<QLabel*>("label_leftSlider");
//    playZone = findChild<QFrame*>("frame_playzone");

//    qDebug() << leftSlider;
//    qDebug() << playZone;
//    assert(leftSpacer != nullptr);
//    assert(leftSlider != nullptr);
//    assert(playZone != nullptr);
//    this->installEventFilter(this);

}

void QFrameSelector::paintEvent(QPaintEvent *event)
{
    qDebug() << "QFrameSelector: paintEvent " << event;
//    double duration =10.0;
//    double steps[] = {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
//    int iStep =0;
//    while (width() * steps[iStep] < 50 *duration) iStep++;

    QPainter painter(this);
//    painter.begin(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    int nbInterval = widgetRuler->getNbInterval();
    double step = widgetRuler->getStep();
    double pixPerSec = widgetRuler->getPixPerSec();
    int wInterval = pixPerSec *step;

//    qDebug() << "QFrameSelector: nbInterval =" << nbInterval;
//    qDebug() << "QFrameSelector: wwInterval =" << wInterval;


//    double step = static_cast<QWidgetRuler*>(parent())->getStep();

//    double step = steps[iStep];
//    int nbInterval =duration /step +2;
//    int wInterval =width() /nbInterval;

//    qDebug() << leftSpacer;
//    leftSpacerX =wInterval -leftSlider->width() +start *pixPerSec;
//    leftSpacer->changeSize(leftSpacerX, 0);
//    playZone->setMinimumWidth(duration *pixPerSec);
//    repaint();

    painter.setPen(Qt::darkGray);
    for (int i =1; i <nbInterval; i++) {
//        int x =wInterval *i;
        int x =i *step *pixPerSec;
        painter.drawLine(x, 0, x, height());
    }

    painter.setPen(QPen(Qt::lightGray));
    for (int i =1; i <nbInterval -1; i++) {
//        int x =wInterval *i;
        int x =i *step *pixPerSec;
        painter.drawLine(x +wInterval /2, 0, x +wInterval /2, height());
    }

//    painter.end();

//    QPen linePen;
//    linePen.setWidth(1);
//    painter.setPen(linePen);

//    painter.drawLine(0, 0, 100, 100);

//    QRectF rect = this->rect();
//    painter.fillRect(QRect(rect.left(), rect.top(), this->width(), 40), QColor(0, 255, 0));



}

//bool QFrameSelector::eventFilter(QObject *watched, QEvent *event)
//{
//    qDebug() << "QFrameSelector: eventFilter " << i++;


//}

void QFrameSelector::onRedrawScale()
{
}

void QFrameSelector::onSlideLeftSlider(int deltaX)
{
    qDebug() << "QFrameSelector::onSlideLeftSlider " << deltaX;
//    leftSpacerX +=deltaX;
//    leftSpacer->changeSize(leftSpacerX, 0);
//    leftSpacer->changeSize(200 +deltaX, 0);
    leftSpacer->setMinimumWidth(leftSpacer->width() +deltaX);
//    update();
//    repaint();
//    playZone->setMinimumWidth(playZone->width());

}

void QFrameSelector::onSlideRightSlider(int deltaX)
{
    qDebug() << "QFrameSelector::onSlideRightSlider " << deltaX;
    playZone->setMinimumWidth(playZone->width() +deltaX);
//    playZone->setMinimumWidth(100);
//    duration += deltaX *0.1;
//    update();

}

void QFrameSelector::setLeftSpacer(QFrame *value)
{
    leftSpacer = value;
}

void QFrameSelector::setRightSlider(QLabel *value)
{
    rightSlider = value;
}

void QFrameSelector::setPlayZone(QFrame *value)
{
    playZone = value;
}

void QFrameSelector::setLeftSlider(QLabel *value)
{
    leftSlider = value;
}


//void QFrameSelector::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "QFrameSelector: wheelEvent";
//    emit changePrecision(1);

//}
