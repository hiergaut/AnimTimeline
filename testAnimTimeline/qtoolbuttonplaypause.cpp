#include "qtoolbuttonplaypause.h"

#include <QMouseEvent>
#include <QDebug>

QToolButtonPlayPause::QToolButtonPlayPause(QWidget *parent) : QToolButton(parent)
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

void QToolButtonPlayPause::mousePressEvent(QMouseEvent *event)
{
//    this->setCheckable(true);
//    update();
    if (event->button() == Qt::LeftButton) {
        if (play) {
            this->setIcon(*pauseIcon);
            play =false;
            emit pauseClicked();
        }
        else {
            this->setIcon(*playIcon);
            play = true;
            emit playClicked();
        }
//        this->icon().addPixmap(QPixmap("images/pause.png"), QIcon::Normal, QIcon::On);
//        this->setIcon(*pauseIcon);
//        update();
    }
//    emit clicked();
//    event->ignore();

}
