#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

//#include "configurations.h"
#include <AnimTimeline/configurations.h>
//#include "qdoublespinboxsmart.h"
#include <AnimTimeline/qdoublespinboxsmart.h>

#include <QLabel>
#include <QSpacerItem>
#include <QWidget>
//#include "qscrollarearuler.h"

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
//    void setMaxDuration(double value); // EXTERNAL SLOT
//    bool * getDrawLock();
    bool * getTimescaleLock();
    bool * getSelectorLock();
    void setSpinStart(QDoubleSpinBoxSmart *value);
    void setSpinEnd(QDoubleSpinBoxSmart *value);
    void setSpinCursor(QDoubleSpinBoxSmart *value);
    void setSpinDuration(QDoubleSpinBoxSmart *value);

//    void setAreaRuler(QScrollAreaRuler *value);

//    bool *getCtrlDown() const;

    void setShiftDown(bool *value);

    void setCtrlDown(bool *value);

protected:
//    void wheelEvent(QWheelEvent* event) override;

signals:
//    void durationChanged(double time); // EXTERNAL SIGNAL
    void rulerZoomed(QWheelEvent *event, double xr);

public slots:
    void onDrawRuler(int width);

private:
    static const int nbSteps = 11;
    const double steps[nbSteps] = { 0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0 };

    bool * ctrlDown;
    bool * shiftDown;

    int nbInterval {0};
    double step;
    double pixPerSec;
    double zero;
    double maxDuration;

    bool timescaleLock {true};
    bool selectorLock {true};

    QDoubleSpinBoxSmart * spinStart;
    QDoubleSpinBoxSmart * spinEnd;
    QDoubleSpinBoxSmart * spinCursor;
    QDoubleSpinBoxSmart * spinDuration;

//    QScrollAreaRuler * areaRuler;

};

#endif // QWIDGETRULER_H
