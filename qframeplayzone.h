#ifndef QFRAMEPLAYZONE_H
#define QFRAMEPLAYZONE_H

#include <QFrame>
#include <QObject>

class QFramePlayZone : public QFrame {
    Q_OBJECT
public:
    explicit QFramePlayZone(QWidget* parent = nullptr);
};

#endif // QFRAMEPLAYZONE_H
