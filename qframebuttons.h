#ifndef QFRAMEBUTTONS_H
#define QFRAMEBUTTONS_H

#include "animtimeline.h"
#include "qwidgetruler.h"
#include <QFrame>
#include <QPoint>
#include <QScrollArea>

class QFrameButtons : public QFrame {
    Q_OBJECT
public:
    explicit QFrameButtons(QWidget* parent = nullptr);

    void setAnimTimeline(AnimTimeline* value);

    void setRuler(QWidgetRuler* value);

    void setScrollArea(QScrollArea *value);

protected:
    //    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void keyPressed(QKeyEvent* event);
    void keyReleased(QKeyEvent* event);

public slots:
    void helpClicked();


private:
    bool clicked = false;
    AnimTimeline* animTimeline;
    QWidgetRuler* ruler;
    QPoint offset;
    QScrollArea * scrollArea;
};

#endif // QFRAMEBUTTONS_H
