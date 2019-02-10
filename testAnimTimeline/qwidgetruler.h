#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

#include <QWidget>
#include <QSpacerItem>
#include <QLabel>
//#include "qframeselector.h"

class QWidgetRuler : public QWidget {
    Q_OBJECT
public:
    explicit QWidgetRuler(QWidget* parent = nullptr);

    double getStep() const;
    int getNbInterval() const;
    int getWInterval() const;

//    void setPlayZone(QFrame *value);
//    void setLeftSpacer(QSpacerItem *value);
//    void setLeftSlider(QLabel *value);

    int updateTimeline(int newWidth);

//    void setFrameSelector(QFrameSelector *value);

signals:
    //    void changeScale();

protected:
    //    void wheelEvent(QWheelEvent * event) override;

public slots:
    void onChangePrecision(int accuracy);

private:
    double duration = 10.0;
    static const int nbSteps =7;
    const double steps[nbSteps] = { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0 };
    double step;
    int nbInterval;
    int wInterval;

    double start =0.0;
    double end =duration;

//    QSpacerItem * leftSpacer;
//    QLabel * leftSlider;
//    QFrame * playZone;
//    QFrameSelector * frameSelector;

};

#endif // QWIDGETRULER_H
