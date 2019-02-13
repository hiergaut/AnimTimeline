#ifndef QTOOLBUTTONPLAYPAUSE_H
#define QTOOLBUTTONPLAYPAUSE_H

#include <QObject>
#include <QToolButton>


class QToolButtonPlayPause : public QToolButton
{
    Q_OBJECT
public:
    explicit QToolButtonPlayPause(QWidget *parent = nullptr);
    ~QToolButtonPlayPause() override;


protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void playClicked();
    void pauseClicked();

public slots:

private:
    QIcon * playIcon;
    QIcon * pauseIcon;

    bool play =true;
};

#endif // QTOOLBUTTONPLAYPAUSE_H
