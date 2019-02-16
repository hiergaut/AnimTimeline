#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

#include <QWidget>
//#include <set>
//#include <vector>

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

public:
    double getCursor();
    double getStart();
    double getEnd();

signals:
    void cursorChanged(double time);
    void keyPoseAdded(double time);
    void keyPoseChanged(int num);
    void playClicked();
    void pauseClicked();
    void removeKeyPose(int num);

public slots:
    void onChangeAnimDuration(double time);
    void onChangeCursor(double time);
    void onAddingKeyPose(double time);
    void onSetPauseMode();

    //    void onCursorChanged(double time, bool isOnKeyPose);
    //    void onAddKeyPose(double time);
    //    void onKeyPose(bool isOn);

    //private slots:
    //    void on_doubleSpinBox_cursor_editingFinished();

private:
    Ui::AnimTimeline* ui;
};

#endif // ANIMTIMELINE_H
