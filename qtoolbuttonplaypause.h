#ifndef QTOOLBUTTONPLAYPAUSE_H
#define QTOOLBUTTONPLAYPAUSE_H

#include <QObject>
#include <QToolButton>

class QToolButtonPlayPause : public QToolButton {
    Q_OBJECT
public:
    explicit QToolButtonPlayPause(QWidget* parent = nullptr);
    ~QToolButtonPlayPause() override;

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void playClicked();
    void pauseClicked();

public slots:
    void onPlayMode();
    void onPauseMode();
    void onChangeMode();

private:
    QIcon* playIcon;
    QIcon* pauseIcon;

    bool play = false;
};

#endif // QTOOLBUTTONPLAYPAUSE_H
