#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <set>
#include <stack>
#include "qdoublespinboxsmart.h"
#include "qframeselector.h"
#include "qspinboxsmart.h"
#include "qtoolbuttonplaypause.h"
#include "qwidgetruler.h"

//typedef struct s_Env Env;
typedef struct s_Env {
    double start;
    double end;
    double cursor;
    double duration;

    std::set<double> keyPoses;

//    bool play;
} Env;

class Session : public QObject
{
        Q_OBJECT
public:
    Session();
//    virtual ~Session() = default;
    virtual ~Session();


signals:
    void envSaved();
    void sessionCleared();
    void undid();
    void redid();

public slots:
    void onSaveEnv();
    void onClearSession();
    void onUndo();
    void onRedo();

    void saveEnv();

private:
    std::stack<Env> undoHeap;
    std::stack<Env> redoHeap;

    double * start;
    double * end;
    double * cursor;
    double * duration;
    std::set<double> * keyPoses;
//    bool * play;

    QDoubleSpinBoxSmart * startSpin;
    QDoubleSpinBoxSmart * endSpin;
    QDoubleSpinBoxSmart * cursorSpin;
    QDoubleSpinBoxSmart * durationSpin;

    QToolButtonPlayPause * playButton;

    QWidgetRuler * ruler;

    QTimer * saveDelay;
    QFrameSelector * selector;
    QSpinBoxSmart * nbKeyPosesSpin;

private:
    void setEnv(Env env);

public: // setters
    void setStart(double *value);
    void setEnd(double *value);
    void setCursor(double *value);
    void setDuration(double *value);
    void setKeyPoses(std::set<double> *value);
//    void setPlay(bool *value);
    void setStartSpin(QDoubleSpinBoxSmart *value);
    void setEndSpin(QDoubleSpinBoxSmart *value);
    void setCursorSpin(QDoubleSpinBoxSmart *value);
    void setDurationSpin(QDoubleSpinBoxSmart *value);
    void setPlayButton(QToolButtonPlayPause *value);
    void setRuler(QWidgetRuler *value);
    void setSelector(QFrameSelector *value);
    void setNbKeyPosesSpin(QSpinBoxSmart *value);
};

#endif // SESSION_H
