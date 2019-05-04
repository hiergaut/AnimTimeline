#include <AnimTimeline/qtoolbuttonplaypause.h>

#include <QDebug>
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
        onChangeMode();
        event->accept();
    }
}

// EXTERNAL SLOT
void QToolButtonPlayPause::onPlayMode()
{
    if (play) {
        qDebug() << "\033[31mQToolButtonPlayPause::onPlayMode() : already on play mode\033[0m";
        return;
    }

    this->setIcon(*pauseIcon);
    play = true;
}

// EXTERNAL SLOT
void QToolButtonPlayPause::onPauseMode()
{
    if (!play) {
        qDebug() << "\033[31mQToolButtonPlayPause::onPauseMode() : already on pause mode\033[0m";
        return;
    }

    this->setIcon(*playIcon);
    play = false;
}

void QToolButtonPlayPause::onChangeMode()
{
    if (play) {
        onPauseMode();
        emit pauseClicked();
        qDebug() << "\033[35mpauseClicked()\033[0m";
    } else {
        onPlayMode();
        emit playClicked();
        qDebug() << "\033[35mplayClicked()\033[0m";
    }
}

bool* QToolButtonPlayPause::getPlay()
{
    return &play;
}
