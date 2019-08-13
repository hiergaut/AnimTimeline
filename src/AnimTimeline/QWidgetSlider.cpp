//#include <AnimTimeline/qlabelslider.h>
//#include "qlabelslider.h"
#include "QWidgetSlider.h"

#include <QMouseEvent>

QWidgetSlider::QWidgetSlider(QWidget* parent)
//    : QLabel(parent)
    : QWidget(parent)
{
}

void QWidgetSlider::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        clicked = true;
        emit slide(event->x());
    }
}

void QWidgetSlider::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        setStyleSheet("background-color: gray");

        clicked = false;
        emit slideRelease();
    }
}

void QWidgetSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (clicked) {
        emit slide(event->x());
    }
}
