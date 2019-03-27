#ifndef QSCROLLAREARULER_H
#define QSCROLLAREARULER_H

#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include "qwidgetruler.h"

class QScrollAreaRuler : public QScrollArea {
    Q_OBJECT
public:
    explicit QScrollAreaRuler(QWidget* parent = nullptr);

    bool * getShiftDown();

    void setRuler(QWidgetRuler *value);

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

public slots:

private:
    bool ctrlDown = false;
    bool shiftDown = false;
    bool midMouseDown = false;

    int mousePosX;
    int sliderPos;
    QWidgetRuler * ruler;
};

#endif // QSCROLLAREARULER_H
