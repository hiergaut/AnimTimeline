#include <AnimTimeline/qframebuttons.h>

#include <QDebug>
#include <QMessageBox>
#include <QWheelEvent>

QFrameButtons::QFrameButtons(QWidget* parent)
    : QFrame(parent)
{
}

void QFrameButtons::setRuler(QWidgetRuler* value)
{
    ruler = value;
}

void QFrameButtons::keyPressEvent(QKeyEvent* event)
{
    emit keyPressed(event);
}

void QFrameButtons::keyReleaseEvent(QKeyEvent* event)
{
    emit keyReleased(event);
}

void QFrameButtons::setAnimTimeline(AnimTimeline* value)
{
    animTimeline = value;
}

void QFrameButtons::helpClicked()
{
    QMessageBox msgBox;
    msgBox.setText(helpButton->toolTip());
    msgBox.exec();
}

void QFrameButtons::setHelpButton(QToolButton* value)
{
    helpButton = value;
}

void QFrameButtons::setScrollArea(QScrollArea* value)
{
    scrollArea = value;
}
