#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

#include <QDialog>

namespace Ui {
class AnimTimeline;
}

class AnimTimeline : public QDialog {
    Q_OBJECT

public:
    explicit AnimTimeline(QWidget* parent = nullptr);
    ~AnimTimeline() override;

protected:
    void showEvent(QShowEvent* ev) override;

public:
    double getCursor();
    double getStart();
    double getEnd();
    int getNbKeyPoses();
    double getKeyPose(int id);

signals:
    void playClicked();
    void pauseClicked();
    void durationChanged(double time);

    void cursorChanged(double time);
    void startChanged(double time);
    void endChanged(double time);
    void keyPoseAdded(double time);
    void keyPoseDeleted(int num);
    void keyPoseChanged(int num, double time);
    void keyPosesChanged(double gap);

public slots:
    void onSetPlayMode();
    void onSetPauseMode();
    void onChangeAnimDuration(double time);

    void onChangeCursor(double time);
    void onChangeStart(double time);
    void onChangeEnd(double time);
    void onAddingKeyPose(double time);
    void onClearKeyPoses();

private:
    Ui::AnimTimeline* ui;
};

#endif // ANIMTIMELINE_H
