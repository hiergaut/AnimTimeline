
#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include <QFrame>
#include "qwidgetruler.h"
#include <QSpacerItem>

class QFrameSelector : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget *parent = nullptr);


    void setLeftSpacer(QSpacerItem *value);

    void setLeftSlider(QLabel *value);

    void setPlayZone(QFrame *value);

signals:
    //    void changePrecision(int accuracy);
//    void updatePlayZone(int xPos, int width);

protected:
    virtual void paintEvent(QPaintEvent * event) override;
    virtual bool eventFilter(QObject * watched, QEvent *event) override;
//    virtual void wheelEvent(QWheelEvent * event) override;

public slots:
    void onRedrawScale();
//    void onLeftSliderClicked(int);

private:
    QWidgetRuler * widgetRuler;

    QSpacerItem * leftSpacer;
    QLabel * leftSlider;
    QFrame * playZone;

    double start =0.0;
    double duration =10.0;
    int i =0;

};

#endif // QFRAMESELECTOR_H
