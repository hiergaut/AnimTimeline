#ifndef QDOUBLESPINBOXSMART_H
#define QDOUBLESPINBOXSMART_H

#include <QDoubleSpinBox>
#include <QEvent>
#include <QKeyEvent>
#include <QObject>
#include <QWidget>

class QDoubleSpinBoxSmart : public QDoubleSpinBox {
    Q_OBJECT
public:
    explicit QDoubleSpinBoxSmart(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
};

#endif // QDOUBLESPINBOXSMART_H
