#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

#include <AnimTimeline/configurations.h>
#include <AnimTimeline/qdoublespinboxsmart.h>

#include <QLabel>
#include <QSpacerItem>
#include <QWidget>

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
    bool* getTimescaleLock();
    bool* getSelectorLock();
    void setSpinStart(QDoubleSpinBoxSmart* value);
    void setSpinEnd(QDoubleSpinBoxSmart* value);
    void setSpinCursor(QDoubleSpinBoxSmart* value);
    void setSpinDuration(QDoubleSpinBoxSmart* value);

    void setShiftDown(bool* value);

    void setCtrlDown(bool* value);

signals:
    void rulerZoomed(QWheelEvent* event, double xr);

public slots:
    void onDrawRuler(int width);

private:
    static const int nbSteps = 11;
    const double steps[nbSteps] = { 0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0 };

    bool* ctrlDown;
    bool* shiftDown;

    double step;
    double pixPerSec;
    double zero;
    double maxDuration;

    QDoubleSpinBoxSmart* spinStart;
    QDoubleSpinBoxSmart* spinEnd;
    QDoubleSpinBoxSmart* spinCursor;
    QDoubleSpinBoxSmart* spinDuration;

    int nbInterval { 0 };
    bool timescaleLock { true };
    bool selectorLock { true };

    bool align[2];
};

#endif // QWIDGETRULER_H
