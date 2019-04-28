#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

//#include "configurations.h"
//#include "session.h"
#include <AnimTimeline/session.h>

#include <QDialog>
#include <QObject>

namespace Ui {
class AnimTimeline;
//static AnimTimeline * ui;
}

class AnimTimeline : public QDialog {
    Q_OBJECT

public:
    explicit AnimTimeline(QWidget* parent = nullptr);
    ~AnimTimeline() override;



//    Ui::AnimTimeline *getUi() const;

protected:
    virtual void resizeEvent(QResizeEvent* ev) override;
    //    void showEvent(QShowEvent* ev) override;

signals:
    void startChanged(double time);
    void endChanged(double time);
    void cursorChanged(double time);
    void durationChanged(double time);

    void keyPoseAdded(double time);
    void keyPoseDeleted(size_t id);
    void keyPoseChanged(size_t id); // save client anim
    void keyPoseMoved(size_t id, double time);
    void keyPosesMoved(double gap, size_t first = 0); // first : first ith keyPose to move

    void playClicked();
    void pauseClicked();


public slots:
    void onChangeStart(double time);
    void onChangeEnd(double time);
    void onChangeCursor(double time);
    void onChangeDuration(double time);

    void onAddingKeyPose(double time);
    void onClearKeyPoses();

    void onSetPlayMode(); // use it if external play button
    void onSetPauseMode(); // use it if external pause button


    //signals:
    //    void sessionCleared();
    //    void undid();
    //    void redid();
//signals:
//    void undid();
//    void redid();


    // getters
    //public:
    //    double getCursor();
    //    double getStart();
    //    double getEnd();
    //    int getNbKeyPoses();
    //    double getKeyPose(int id);

    //private:
    //    virtual void resizeEvent(QResizeEvent* ev) override;

//private:
protected:
    Ui::AnimTimeline* ui;
    //    static const Session & session;
    //    const Session & session =new Session();
//    Session session;
};

//template <class T>
//class AnimTimelineWithSession : public AnimTimeline {
//    Q_OBJECT

//public:
////    explicit AnimTimeline(QWidget* parent = nullptr);
//    explicit AnimTimelineWithSession(QWidget * parent = nullptr);

//signals:
//    // session (undo/redo)
//    void envSaved();
//    void rendered(T anim);
//    void renderDeleted(T anim);

//public slots:
//    void onChangeStart(double time);
//    void onChangeEnd(double time);
////    void onChangeCursor(double time);
//    void onChangeDuration(double time);

//    void onAddingKeyPose(double time);
//    void onClearKeyPoses();

////    void onSetPlayMode(); // useless : why use it ?
////    void onSetPauseMode(); // useless : why use it ?
//    // undo/redo, must be called before envSaved receive
//    void onSaveRendering(T anim);

//private:
//    Session session;
//};


//typedef struct s_Env Env;
class AnimTimelineWithSession : public AnimTimeline {
    Q_OBJECT

public:
//    explicit AnimTimeline(QWidget* parent = nullptr);
    explicit AnimTimelineWithSession(QWidget * parent = nullptr);
//    ~AnimTimelineWithSession();

signals:
    // for session, after receive signal, user must call onSaveRendering
    // AnimTimeline slot to save client anim to render later due undo/redo event
    void envSaved();

    // use void * because qt Q_OBJECT no accept template class
    // and signals and slots are needed to send signal to user to render
    void rendered(void * anim);
    void renderDeleted(void * anim);

public slots:
    // undo/redo, must be called by user after envSaved receive
    void onSaveRendering(void * anim, size_t bytes);

    // overload parent slots
public slots:
    void onChangeStart(double time);
    void onChangeEnd(double time);
//    void onChangeCursor(double time);
    void onChangeDuration(double time);
//    void onUndo();
//    void onRedo();
    void onAddingKeyPose(double time);
    void onClearKeyPoses();
//    void onSetPlayMode();
//    void onSetPauseMode();


private:
//    QTimer * saveDelay;
    Session session{this};
};

//const Session & AnimTimeline::session{};

#endif // ANIMTIMELINE_H
