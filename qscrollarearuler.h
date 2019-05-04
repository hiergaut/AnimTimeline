#ifndef QSCROLLAREARULER_H
#define QSCROLLAREARULER_H

//#include "animtimeline.h"
#include <AnimTimeline/animtimeline.h>

//#include "qtoolbuttonplaypause.h"
#include <AnimTimeline/animtimeline.h>
//#include "qwidgetruler.h"
#include <AnimTimeline/qwidgetruler.h>
//#include "qdoublespinboxsmart.h"
#include <AnimTimeline/qdoublespinboxsmart.h>
//#include "qframeselector.h"
#include <AnimTimeline/qframeselector.h>

#include <QObject>
#include <QScrollArea>
#include <QWidget>

class QScrollAreaRuler : public QScrollArea {
    Q_OBJECT
public:
    explicit QScrollAreaRuler(QWidget* parent = nullptr);

    bool* getShiftDown();

    void setRuler(QWidgetRuler* value);

    void setPlayPause(QToolButtonPlayPause* value);

    void setAnimTimeline(AnimTimeline* value);

    void setSpinDuration(QDoubleSpinBoxSmart* value);

    bool* getCtrlDown();

    //    void setZero(double *value);

    void setSelector(QFrameSelector* value);

    bool* getMidMouseDown();
    void setCursorSpin(QDoubleSpinBoxSmart* value);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    //    void scrollContentsBy(int dx, int dy) override;

signals:
    //    void changePrecision(int width);
    void keyPoseOnMouseAdded();
    void removeKeyPose();
    void previousKeyPose();
    void nextKeyPose();
    void durationChanged(double time);
    void addKeyPose();

    void undo();
    void redo();

public slots:
    void onKeyPress(QKeyEvent* event);
    void onKeyRelease(QKeyEvent* event);
    //    void onZoomRuler(QWheelEvent *event, double xr);

private:
    bool ctrlDown = false;
    bool shiftDown = false;
    bool midMouseDown = false;

    int mousePosX;
    int sliderPos;

    QWidgetRuler* ruler;
    QToolButtonPlayPause* playPause;
    AnimTimeline* animTimeline;
    QDoubleSpinBoxSmart* spinDuration;

    QDoubleSpinBoxSmart* cursorSpin;

    QFrameSelector* selector;
    //    double * zero;
};

#endif // QSCROLLAREARULER_H
