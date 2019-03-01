#ifndef QFRAMEBUTTONS_H
#define QFRAMEBUTTONS_H

#include <QFrame>
#include "animtimeline.h"
#include <QPoint>
#include "qwidgetruler.h"

class QFrameButtons : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameButtons(QWidget *parent = nullptr);

    void setAnimTimeline(AnimTimeline *value);

    void setRuler(QWidgetRuler *value);

protected:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void wheelEvent(QWheelEvent * event) override;

signals:

public slots:

private:
    bool clicked =false;
    AnimTimeline * animTimeline;
    QWidgetRuler * ruler;
    QPoint offset;

};

#endif // QFRAMEBUTTONS_H