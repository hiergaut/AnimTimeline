#include "qtoolbuttonplaypause.h"

#include <QMouseEvent>

QToolButtonPlayPause::QToolButtonPlayPause(QWidget* parent)
    : QToolButton(parent)
{

    playIcon = new QIcon();
    playIcon->addPixmap(QPixmap(":/images/play.png"));
    pauseIcon = new QIcon();
    pauseIcon->addPixmap(QPixmap(":/images/pause.png"));

    this->setIcon(*playIcon);
}

QToolButtonPlayPause::~QToolButtonPlayPause()
{
    delete playIcon;
    delete pauseIcon;
}

void QToolButtonPlayPause::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        if (play) {
            this->setIcon(*playIcon);
            play = false;
            emit pauseClicked();
        } else {
            this->setIcon(*pauseIcon);
            play = true;
            emit playClicked();
        }
    }
}

void QToolButtonPlayPause::onPlayMode()
{
    this->setIcon(*pauseIcon);
    play = true;
}

void QToolButtonPlayPause::onPauseMode()
{
    this->setIcon(*playIcon);
    play = false;
}

void QToolButtonPlayPause::onChangeMode()
{
    if (play) {
        onPauseMode();
        emit pauseClicked();
    } else {
        onPlayMode();
        emit playClicked();
    }
}
