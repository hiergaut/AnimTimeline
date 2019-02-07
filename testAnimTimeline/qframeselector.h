#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include <QFrame>

class QFrameSelector : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget *parent = nullptr);

signals:
    void changePrecision(int accuracy);

protected:
    virtual void paintEvent(QPaintEvent * event) override;
    virtual bool eventFilter(QObject * watched, QEvent *event) override;
//    virtual void wheelEvent(QWheelEvent * event) override;

public slots:
};

#endif // QFRAMESELECTOR_H
