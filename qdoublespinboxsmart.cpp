//#include "qdoublespinboxsmart.h"
#include <AnimTimeline/qdoublespinboxsmart.h>

#include <QDebug>
#include <QWheelEvent>

QDoubleSpinBoxSmart::QDoubleSpinBoxSmart(QWidget* parent)
    : QDoubleSpinBox(parent)
{
}

void QDoubleSpinBoxSmart::wheelEvent(QWheelEvent* event)
{
    QDoubleSpinBox::wheelEvent(event);

    //    qDebug() << "QDoubleSpinBoxSmart::wheelEvent(" << event << ")";
    //    int ry = (event->angleDelta().ry() > 0) ?(1) :(-1);

    emit editingFinished();
    //    emit valueChanged(value());

    //    setValue(value() + ry *singleStep());
    //    event->ignore();

    event->accept();
}

void QDoubleSpinBoxSmart::keyPressEvent(QKeyEvent* event)
{
    QDoubleSpinBox::keyPressEvent(event);
    //    qDebug() << "QDoubleSpinBoxSmart::keyPressEvent(" << event << ")";
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
        emit editingFinished();
        break;

        //    case Qt::Key_Left:
        //    case Qt::Key_Right:
        ////        event->accept();
        //        event->ignore();
        //        break;
        //    case Qt::Key_Return:
        //        emit valueChanged(value());
        //        event->accept();
        //        break;

        //    default:
        //        event->ignore();
        //        break;
    }

    event->ignore(); // Esc
    //    event->accept();
}

//void QDoubleSpinBoxSmart::onChangeValue(double time)
//{
//}
