#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

#include <QLabel>
#include <QSpacerItem>
#include <QWidget>
#include "constants.h"

class QWidgetRuler : public QWidget {
    Q_OBJECT
public:
    explicit QWidgetRuler(QWidget* parent = nullptr);

    double* getStep();
    int* getNbInterval();
    double* getPixPerSec();

    int updateTimeline(int newWidth);

    double* getZero();
    double* getMaxDuration();

    void setMaxDuration(double value);

signals:
    void durationChanged(double time);

public slots:
    void onChangePrecision(int accuracy);

private:
    double maxDuration = INIT_DURATION;
    static const int nbSteps = 7;
    const double steps[nbSteps] = { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0 };

    int nbInterval;
    double step;
    double pixPerSec;
    double zero;
};

#endif // QWIDGETRULER_H
