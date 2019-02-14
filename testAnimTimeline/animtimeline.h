#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

#include <QWidget>

namespace Ui {
class AnimTimeline;
}

class AnimTimeline : public QWidget {
    Q_OBJECT

public:
    explicit AnimTimeline(QWidget* parent = nullptr);
    ~AnimTimeline();

protected:
    //  virtual void paintEvent(QPaintEvent *event) override;
    //  virtual void wheelEvent(QWheelEvent *event) override;

signals:
    void render(double time);

public slots:
    void onCursorChanged(double time, bool isOnKeyPose);
//    void onKeyPose(bool isOn);


private:
    Ui::AnimTimeline* ui;
};

#endif // ANIMTIMELINE_H
