#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

//#include "configurations.h"
#include "session.h"

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

protected:
    virtual void resizeEvent(QResizeEvent* ev) override;
    //    void showEvent(QShowEvent* ev) override;

signals:
    void startChanged(double time);
    void endChanged(double time);
    void cursorChanged(double time);
    void durationChanged(double time);

    void keyPoseAdded(double time);
    void keyPoseDeleted(int num);
    void keyPoseChanged(int num); // skeleton changed
    void keyPoseMoved(int num, double time);
    void keyPosesMoved(double gap, int first = 0); // first ith keyPose to move

    void playClicked();
    void pauseClicked();


public slots:
    void onChangeStart(double time);
    void onChangeEnd(double time);
    void onChangeCursor(double time);
    void onChangeDuration(double time);

    void onAddingKeyPose(double time);
    void onClearKeyPoses();

    void onSetPlayMode(); // useless : why use it ?
    void onSetPauseMode(); // useless : why use it ?


signals:
    // session (undo/redo)
    void envSaved();
    void sessionCleared();
    void undid();
    void redid();

    // getters
    //public:
    //    double getCursor();
    //    double getStart();
    //    double getEnd();
    //    int getNbKeyPoses();
    //    double getKeyPose(int id);

    //private:
    //    virtual void resizeEvent(QResizeEvent* ev) override;

private:
    Ui::AnimTimeline* ui;
//    static const Session & session;
//    const Session & session =new Session();
    Session session;
};

//const Session & AnimTimeline::session{};


#endif // ANIMTIMELINE_H
