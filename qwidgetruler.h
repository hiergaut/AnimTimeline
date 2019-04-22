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

    int drawRuler(int newWidth);

    double* getZero();
    double* getMaxDuration();

    void setMaxDuration(double value); // EXTERNAL SLOT

//    bool * getDrawLock();

    bool * getTimescaleLock();

    bool * getSelectorLock();

signals:
    void durationChanged(double time); // EXTERNAL SIGNAL

public slots:
    void onDrawRuler(int width);

private:
    static const int nbSteps = 10;
    const double steps[nbSteps] = { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0 };

    int nbInterval {0};
    double step;
    double pixPerSec;
    double zero;
    double maxDuration;

    bool timescaleLock {true};
    bool selectorLock {true};
};

#endif // QWIDGETRULER_H
