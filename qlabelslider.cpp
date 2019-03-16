#include "qlabelslider.h"

#include <QMouseEvent>

QLabelSlider::QLabelSlider(QWidget* parent)
    : QLabel(parent)
{
}

void QLabelSlider::keyPressEvent(QKeyEvent* event)
{
    (void)event;
}

void QLabelSlider::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        clicked = true;
        emit slide(event->x());
    }
}

void QLabelSlider::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        setStyleSheet("background-color: gray");

        clicked = false;
        emit slideRelease();
    }
}

void QLabelSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (clicked) {
        emit slide(event->x());
    }
}
