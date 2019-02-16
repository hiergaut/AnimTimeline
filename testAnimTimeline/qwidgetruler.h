#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

#include <QLabel>
#include <QSpacerItem>
#include <QWidget>
//#include "qframeselector.h"

class QWidgetRuler : public QWidget {
    Q_OBJECT
public:
    explicit QWidgetRuler(QWidget* parent = nullptr);

    double* getStep();
    int* getNbInterval();
    double* getPixPerSec();
    //    int* getWInterval() const;

    //    void setPlayZone(QFrame *value);
    //    void setLeftSpacer(QSpacerItem *value);
    //    void setLeftSlider(QLabel *value);

    int updateTimeline(int newWidth);

    //    void setFrameSelector(QFrameSelector *value);

    double * getZero();
    double * getMaxDuration();

    void setMaxDuration(double value);

signals:
    //    void rulerChange(double step, int nbInterval, double pixPerSec);

protected:
    //    void wheelEvent(QWheelEvent * event) override;

public slots:
    void onChangePrecision(int accuracy);

private:
    double maxDuration = 30.0;
    static const int nbSteps = 7;
    const double steps[nbSteps] = { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0 };

    int nbInterval;
    double step;
    double pixPerSec;
    double zero;

//    int wInterval;


//    double start = 0.0;
//    double end = maxDuration;

    //    QSpacerItem * leftSpacer;
    //    QLabel * leftSlider;
    //    QFrame * playZone;
    //    QFrameSelector * frameSelector;
};

#endif // QWIDGETRULER_H
