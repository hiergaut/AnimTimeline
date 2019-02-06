#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include <QFrame>

class QFrameSelector : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget *parent = nullptr);

signals:

protected:
    virtual void paintEvent(QPaintEvent * event) override;

public slots:
};

#endif // QFRAMESELECTOR_H
