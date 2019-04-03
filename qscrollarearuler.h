#ifndef QSCROLLAREARULER_H
#define QSCROLLAREARULER_H

#include "qtoolbuttonplaypause.h"
#include "qwidgetruler.h"
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

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void changePrecision(int accuracy);
    void addKeyPose();
    void removeKeyPose();
    void previousKeyPose();
    void nextKeyPose();

public slots:

private:
    bool ctrlDown = false;
    bool shiftDown = false;
    bool midMouseDown = false;

    int mousePosX;
    int sliderPos;
    QWidgetRuler* ruler;
    QToolButtonPlayPause* playPause;
};

#endif // QSCROLLAREARULER_H
