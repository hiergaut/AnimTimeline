#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

#include <QWidget>

class QWidgetRuler : public QWidget {
    Q_OBJECT
public:
    explicit QWidgetRuler(QWidget* parent = nullptr);

    double getStep() const;
    int getNbInterval() const;
    int getWInterval() const;

    int updateTimeline(int newWidth);

signals:
    //    void changeScale();

protected:
    //    void wheelEvent(QWheelEvent * event) override;

public slots:
    void onChangePrecision(int accuracy);

private:
    double duration = 10.0;
    const double steps[7] = { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0 };
    double step;
    int nbInterval;
    int wInterval;
};

#endif // QWIDGETRULER_H
