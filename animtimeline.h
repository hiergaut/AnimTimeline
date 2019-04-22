#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

#include "constants.h"
#include <QDialog>

namespace Ui {
class AnimTimeline;
//static AnimTimeline * ui;
}

class AnimTimeline : public QDialog {
    Q_OBJECT

public:
    explicit AnimTimeline(QWidget* parent = nullptr);
    ~AnimTimeline() override;

    //protected:
    //    void showEvent(QShowEvent* ev) override;

    //public:
    //    double getCursor();
    //    double getStart();
    //    double getEnd();
    //    int getNbKeyPoses();
    //    double getKeyPose(int id);

signals:
    void startChanged(double time);
    void endChanged(double time);
    void cursorChanged(double time);
    void durationChanged(double time);

    void keyPoseAdded(double time);
    void keyPoseDeleted(int num);
    void keyPoseChanged(uint num);
    void keyPoseMoved(int num, double time);
    void keyPosesMoved(double gap, int first = 0);

    void playClicked();
    void pauseClicked();

public slots:
    void onChangeStart(double time);
    void onChangeEnd(double time);
    void onChangeCursor(double time);
    void onChangeAnimDuration(double time);

    void onAddingKeyPose(double time);
    void onClearKeyPoses();

    void onSetPlayMode();
    void onSetPauseMode();

private:
    Ui::AnimTimeline* ui;
};

#endif // ANIMTIMELINE_H
