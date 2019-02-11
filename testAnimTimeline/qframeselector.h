
#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include "qwidgetruler.h"
#include <QFrame>
#include <QSpacerItem>

class QFrameSelector : public QFrame {
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget* parent = nullptr);

    void setLeftSlider(QLabel* value);
    void setPlayZone(QFrame* value);
    void setRightSlider(QLabel* value);
    void setLeftSpacer(QFrame* value);
    //    void drawRulerScale();

signals:
    //    void changePrecision(int accuracy);
    //    void updatePlayZone(int xPos, int width);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    //    virtual bool eventFilter(QObject * watched, QEvent *event) override;
    //    virtual void wheelEvent(QWheelEvent * event) override;

public slots:
    void onRedrawScale();
    void onSlideLeftSlider(int deltaX);
    void onSlideRightSlider(int deltaX);
    void onSlideRelease();
    //    void onLeftSliderClicked(int);
    //    void onRulerChange(double step, int nbInterval, double pixPerSec);

private:
    QWidgetRuler* widgetRuler;

    //    QSpacerItem * leftSpacer;
    QFrame* leftSpacer;
    QLabel* leftSlider;
    QLabel* rightSlider;
    QFrame* playZone;

    double start = 0.0;
    double duration = 10.0;
    int i = 0;

    int leftSpacerX;

    int * nbInterval;
    double * step;
    double * pixPerSec;

    int iPaint = 0;
    //    bool rulerChanged =true;
    bool sliding = false;
};

#endif // QFRAMESELECTOR_H
