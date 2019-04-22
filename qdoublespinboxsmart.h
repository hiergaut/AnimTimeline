#ifndef QDOUBLESPINBOXSMART_H
#define QDOUBLESPINBOXSMART_H

#include <QObject>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QKeyEvent>

class QDoubleSpinBoxSmart : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit QDoubleSpinBoxSmart(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;

//signals:
//    void valueChanged(double time);

//public slots:
//    void onChangeValue(double time);

};

#endif // QDOUBLESPINBOXSMART_H
