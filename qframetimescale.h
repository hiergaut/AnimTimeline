#ifndef QFRAMETIMESCALE_H
#define QFRAMETIMESCALE_H

#include "qwidgetruler.h"
#include <QFrame>
#include <QObject>

class QFrameTimescale : public QFrame {
    Q_OBJECT
public:
    explicit QFrameTimescale(QWidget* parent = nullptr);

signals:

protected:
    virtual void paintEvent(QPaintEvent* event) override;

public slots:

private:
    QWidgetRuler* widgetRuler;

    int* nbInterval;
    double* step;
    double* pixPerSec;
};

#endif // QFRAMETIMESCALE_H
