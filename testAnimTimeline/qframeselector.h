
#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include <QFrame>
#include "qwidgetruler.h"

class QFrameSelector : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget *parent = nullptr);


signals:
    //    void changePrecision(int accuracy);

protected:
    virtual void paintEvent(QPaintEvent * event) override;
    virtual bool eventFilter(QObject * watched, QEvent *event) override;
//    virtual void wheelEvent(QWheelEvent * event) override;

public slots:
    void onRedrawScale();

private:
    QWidgetRuler * widgetRuler;
};

#endif // QFRAMESELECTOR_H
