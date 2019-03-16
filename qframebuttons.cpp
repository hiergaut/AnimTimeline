#include "qframebuttons.h"

#include <QWheelEvent>

QFrameButtons::QFrameButtons(QWidget* parent)
    : QFrame(parent)
{
}

void QFrameButtons::wheelEvent(QWheelEvent* event)
{
    int gap = event->angleDelta().ry();

    if (animTimeline->width() >= 650 || gap >= 0) {
        int previousWidth = animTimeline->width();

        int newX = animTimeline->x() - gap;
        int newY = animTimeline->y() + 1;
        int newWidth = animTimeline->width() + 1 + gap;
        int newHeight = animTimeline->height();

        animTimeline->setGeometry(newX, newY, newWidth, newHeight);

        int diffWidth = previousWidth - animTimeline->width();
        ruler->onChangePrecision(-diffWidth);
    }
}

void QFrameButtons::setRuler(QWidgetRuler* value)
{
    ruler = value;
}

void QFrameButtons::setAnimTimeline(AnimTimeline* value)
{
    animTimeline = value;
}
