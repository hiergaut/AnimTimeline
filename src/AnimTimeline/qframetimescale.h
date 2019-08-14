#ifndef QFRAMETIMESCALE_H
#define QFRAMETIMESCALE_H

//#include <AnimTimeline/qwidgetruler.h>
#include "qwidgetruler.h"

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
//    virtual void resizeEvent(QResizeEvent * event) override;

public slots:
    void onRulerChange();

private:
    QWidgetRuler* widgetRuler;

    QPixmap * m_pixmapBackground = nullptr;

    bool m_rulerChanged = false;

    int* nbInterval;
    double* step;
    double* pixPerSec;

    bool* drawLock;
//    int counter { 0 };

    int align;
};

#endif // QFRAMETIMESCALE_H
