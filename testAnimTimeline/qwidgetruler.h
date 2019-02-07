#ifndef QWIDGETRULER_H
#define QWIDGETRULER_H

#include <QWidget>

class QWidgetRuler : public QWidget
{
    Q_OBJECT
public:
    explicit QWidgetRuler(QWidget *parent = nullptr);

signals:

protected:
//    void wheelEvent(QWheelEvent * event) override;

public slots:
    void onChangePrecision(int accuracy);


};

#endif // QWIDGETRULER_H
