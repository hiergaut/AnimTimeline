#ifndef QFRAMEBUTTONS_H
#define QFRAMEBUTTONS_H

#include <AnimTimeline/animtimeline.h>
#include <AnimTimeline/qwidgetruler.h>

#include <QFrame>
#include <QPoint>
#include <QScrollArea>
#include <QToolButton>

class QFrameButtons : public QFrame {
    Q_OBJECT
public:
    explicit QFrameButtons(QWidget* parent = nullptr);

    void setAnimTimeline(AnimTimeline* value);
    void setRuler(QWidgetRuler* value);
    void setScrollArea(QScrollArea* value);
    void setHelpButton(QToolButton* value);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void keyPressed(QKeyEvent* event);
    void keyReleased(QKeyEvent* event);

public slots:
    void helpClicked();

private:
    AnimTimeline* animTimeline;
    QWidgetRuler* ruler;
    QPoint offset;
    QScrollArea* scrollArea;

    QToolButton* helpButton;
    bool clicked = false;

    bool align[7];
};

#endif // QFRAMEBUTTONS_H
