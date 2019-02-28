#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

//#include <QWidget>
#include <QDialog>
//#include <set>
//#include <vector>
#include <set>

namespace Ui {
class AnimTimeline;
}

class AnimTimeline : public QDialog {
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
    int getNbKeyPoses();
    double getKeyPose(int id);

signals:
    void cursorChanged(double time);
    void keyPoseAdded(double time);
    void removeKeyPose(int num);
    void keyPoseChanged(int num);
    void playClicked();
    void pauseClicked();

public slots:
    void onChangeAnimDuration(double time);
    void onChangeCursor(double time);
    void onAddingKeyPose(double time);
    void onSetPauseMode();
    void onSetPlayMode();

    //    void onCursorChanged(double time, bool isOnKeyPose);
    //    void onAddKeyPose(double time);
    //    void onKeyPose(bool isOn);

    //private slots:
    //    void on_doubleSpinBox_cursor_editingFinished();


private:
    Ui::AnimTimeline* ui;
};

#endif // ANIMTIMELINE_H
