#ifndef QTOOLBUTTONPLAYPAUSE_H
#define QTOOLBUTTONPLAYPAUSE_H

#include <QObject>
#include <QToolButton>

class QToolButtonPlayPause : public QToolButton {
    Q_OBJECT
public:
    explicit QToolButtonPlayPause(QWidget* parent = nullptr);
    ~QToolButtonPlayPause() override;

    bool * getPlay();

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void playClicked(); // EXTERNAL SIGNAL
    void pauseClicked(); // EXTERNAL SIGNAL

public slots:
    void onPlayMode(); // EXTERNAL SLOT
    void onPauseMode(); // EXTERNAL SLOT

    void onChangeMode();

private:
    QIcon* playIcon;
    QIcon* pauseIcon;

    bool play = false;
};

#endif // QTOOLBUTTONPLAYPAUSE_H
