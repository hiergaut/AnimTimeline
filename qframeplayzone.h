#ifndef QFRAMEPLAYZONE_H
#define QFRAMEPLAYZONE_H

#include <QObject>
#include <QFrame>

class QFramePlayZone : public QFrame
{
    Q_OBJECT
public:
    explicit QFramePlayZone(QWidget *parent =nullptr);
};

#endif // QFRAMEPLAYZONE_H
