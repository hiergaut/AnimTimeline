#ifndef SESSION_H
#define SESSION_H

//#include "qdoublespinboxsmart.h"
#include <AnimTimeline/qdoublespinboxsmart.h>
//#include "qframeselector.h"
#include <AnimTimeline/qframeselector.h>
//#include "qspinboxsmart.h"
#include <AnimTimeline/qspinboxsmart.h>
//#include "qtoolbuttonplaypause.h"
#include <AnimTimeline/qtoolbuttonplaypause.h>
//#include "qwidgetruler.h"
#include <AnimTimeline/qwidgetruler.h>

//#include <QObject>
#include <set>
#include <stack>
//#include "animtimeline.h"

//typedef struct s_Env Env;

//template <class T>
class Session : public QObject {
    Q_OBJECT
public:
    //    typedef struct s_Env Env;
    Session(QObject* parent = nullptr);
    //    virtual ~Session() = default;
    virtual ~Session();

signals:
    void envSaved(); // EXTERNAL

    void rendered(void* render); // EXTERNAL
    void renderDeleted(void* render); // EXTERNAL
    //    void sessionCleared();
    //    void undid();
    //    void redid();

public slots:
    void onChangeEnv();

    void onClearSession();
    void onUndo();
    void onRedo();

    void onSaveRendering(void* anim, size_t bytes); // EXTERNAL

private:
    typedef struct s_Env {
        double start;
        double end;
        double cursor;
        double duration;

        std::set<double> keyPoses;

        //    bool play;
        void* anim;
        size_t bytes;
        //    int c;
    } Env;

    std::deque<Env> undo;
    std::stack<Env> redoHeap;

    Env first;

    double* start;
    double* end;
    double* cursor;
    double* duration;
    std::set<double>* keyPoses;
    //    bool * play;

    int size { 0 };

    QDoubleSpinBoxSmart* startSpin;
    QDoubleSpinBoxSmart* endSpin;
    QDoubleSpinBoxSmart* cursorSpin;
    QDoubleSpinBoxSmart* durationSpin;

    QToolButtonPlayPause* playButton;

    QWidgetRuler* ruler;

    QTimer* saveDelay;
    QFrameSelector* selector;
    QSpinBoxSmart* nbKeyPosesSpin;

private:
    void setEnv(Env env);

#ifndef QT_NO_DEBUG_OUTPUT
    void envSavedTrace();
#endif
    //    void saveFirst();
    //    int size();

public: // setters
    void setStart(double* value);
    void setEnd(double* value);
    void setCursor(double* value);
    void setDuration(double* value);
    void setKeyPoses(std::set<double>* value);
    //    void setPlay(bool *value);
    void setStartSpin(QDoubleSpinBoxSmart* value);
    void setEndSpin(QDoubleSpinBoxSmart* value);
    void setCursorSpin(QDoubleSpinBoxSmart* value);
    void setDurationSpin(QDoubleSpinBoxSmart* value);
    void setPlayButton(QToolButtonPlayPause* value);
    void setRuler(QWidgetRuler* value);
    void setSelector(QFrameSelector* value);
    void setNbKeyPosesSpin(QSpinBoxSmart* value);
};

#endif // SESSION_H
