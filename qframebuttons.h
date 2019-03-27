#ifndef QFRAMEBUTTONS_H
#define QFRAMEBUTTONS_H

#include "animtimeline.h"
#include "qwidgetruler.h"
#include <QFrame>
#include <QPoint>

class QFrameButtons : public QFrame {
    Q_OBJECT
public:
    explicit QFrameButtons(QWidget* parent = nullptr);

    void setAnimTimeline(AnimTimeline* value);

    void setRuler(QWidgetRuler* value);

protected:
    void wheelEvent(QWheelEvent* event) override;

signals:

public slots:
    void helpClicked();

private:
    bool clicked = false;
    AnimTimeline* animTimeline;
    QWidgetRuler* ruler;
    QPoint offset;
};

#endif // QFRAMEBUTTONS_H
