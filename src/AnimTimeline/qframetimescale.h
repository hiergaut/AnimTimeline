#ifndef QFRAMETIMESCALE_H
#define QFRAMETIMESCALE_H

#include <AnimTimeline/qwidgetruler.h>

#include <QFrame>
#include <QObject>

class QFrameTimescale : public QFrame {
    Q_OBJECT
public:
    explicit QFrameTimescale(QWidget* parent = nullptr);

    void setDrawLock(bool* value);

signals:

protected:
    virtual void paintEvent(QPaintEvent* event) override;

public slots:

private:
    QWidgetRuler* widgetRuler;

    int* nbInterval;
    double* step;
    double* pixPerSec;

    bool* drawLock;
    int counter { 0 };

    int align;
};

#endif // QFRAMETIMESCALE_H
