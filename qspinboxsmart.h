#ifndef QSPINBOXSMART_H
#define QSPINBOXSMART_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>

class QSpinBoxSmart : public QSpinBox
{
    Q_OBJECT
public:
    explicit QSpinBoxSmart(QWidget * parent = nullptr);

protected:
        void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
        void nextKeyPose();
        void previousKeyPose();
        void deleteKeyPose();
};

#endif // QSPINBOXSMART_H
