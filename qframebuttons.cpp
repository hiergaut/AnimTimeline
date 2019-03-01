#include "qframebuttons.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QWheelEvent>

QFrameButtons::QFrameButtons(QWidget* parent)
    : QFrame(parent)
{
}

void QFrameButtons::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        clicked = true;
        offset = event->pos() + animTimeline->pos();
        //        dialogTop =animTimeline->y();
        //        dialogLeft =animTimeline->x();
    }
}

void QFrameButtons::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        //        int leftInc = event->x();
        //        int topInc =event->y();
        //        animTimeline->move(dialogLeft +leftInc, dialogTop +topInc);
        //        this->move(mapToParent(event->pos() -offset));
        animTimeline->move(event->pos() - offset);
        //        animTimeline->update();
        //        animTimeline->move(event->pos());
    }
}

void QFrameButtons::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        clicked = false;
    }
}

void QFrameButtons::wheelEvent(QWheelEvent* event)
{
    int gap = event->angleDelta().ry();

    //    animTimeline->move(animTimeline->x() -ry, animTimeline->y());
    //    animTimeline->setMinimumWidth(animTimeline->width() +gap);
    //    if (animTimeline->width() >= 500 || gap >= 0) {
    int previousWidth = animTimeline->width();
    animTimeline->setGeometry(animTimeline->x() + 1, animTimeline->y() + 1, animTimeline->width() + 1 + gap, animTimeline->height());

    int diffWidth = previousWidth - animTimeline->width();
    animTimeline->move(animTimeline->x() + diffWidth, animTimeline->y());
    ruler->onChangePrecision(-diffWidth);

    //    }
    //    animTimeline->update();
}

void QFrameButtons::setRuler(QWidgetRuler *value)
{
    ruler = value;
}

void QFrameButtons::setAnimTimeline(AnimTimeline* value)
{
    animTimeline = value;
}
