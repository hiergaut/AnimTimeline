#include "qframebuttons.h"

#include <QMessageBox>
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

void QFrameButtons::helpClicked()
{
    QMessageBox msgBox;
    msgBox.setText("List of Shortcuts :\n"
                   "\n"
                   "keyBoard :\n"
                   "i : insert/replace keyPose on cursor\n"
                   "<shift> + i : delete keyPose on cursor\n"
                   "<left> : previous keyPose\n"
                   "<right> : next keyPose\n"
                   "<space> : play/pause mode\n"
                   "\n"
                   "mouse :\n"
                   "<wheelMouse[Up/Down]> : zoom on ruler\n"
                   "<ctrl>+<wheelMouse[Up/Down] : move right/left ruler\n"
                   "<leftClick> : move cursor\n"
                   "<rightClick> (cursor on keyPose) : move keyPose\n"
                   "<rightClick> : insert time on mouse\n"
                   "<shift>+<rightClick> : remove time on mouse\n");
    msgBox.exec();
}
