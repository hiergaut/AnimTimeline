
#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include "qwidgetruler.h"
#include <QFrame>
#include <QSpacerItem>
//#include <QSet>
#include <set>

class QFrameSelector : public QFrame {
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget* parent = nullptr);
    ~QFrameSelector() override;

    void setLeftSlider(QLabel* value);
    void setPlayZone(QFrame* value);
    void setRightSlider(QLabel* value);
    void setLeftSpacer(QFrame* value);
    //    void drawRulerScale();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void keyPressEvent(QKeyEvent * event) override;
    //    virtual bool eventFilter(QObject * watched, QEvent *event) override;
    //    virtual void wheelEvent(QWheelEvent * event) override;


signals:
    void changeStart(double time);
    void changeEnd(double time);
    void changeCursor(double time, bool isOnKeyPose);
    void changeNbKeyPoses(int nbEl);
    void changePauseMode();
    void isOnKeyPose(bool isOn);

    //    void changePrecision(int accuracy);
    //    void updatePlayZone(int xPos, int width);

public slots:
    void onRedrawScale();
    void onSlideLeftSlider(int deltaX);
    void onSlideRightSlider(int deltaX);
    void onSlideRelease();
    void onAddKeyPose();
    void onStartChanged(double time);
    void onEndChanged(double time);
    void onCursorChanged(double time);
    void onCursorStart();
    void onCursorEnd();
    void onCursorPreviousKeyPose();
    void onCursorNextKeyPose();
    void onPlay();
    void onPause();
    void onTickTimer();
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
//    double maxDuration = 11.0;
    double end = 10.0;
    double cursor = 0.0;
//    double period = 1.0 /24.0;
    double period = 1.0 /4.0;
//    double period = 1.0;
//    int i = 0;

//    int leftSpacerX;

    int * nbInterval;
    double * step;
    double * pixPerSec;
    double * zero;
    double * maxDuration;

    // question : why QSet is unordered
//    QSet<double> keyPoses;
    std::set<double> keyPoses;


    int iPaint = 0;
    //    bool rulerChanged =true;
    bool sliding = false;
    bool clicked = false;

    QTimer * timer;
};

#endif // QFRAMESELECTOR_H
