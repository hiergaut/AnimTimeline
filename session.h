#ifndef SESSION_H
#define SESSION_H

#include <AnimTimeline/qdoublespinboxsmart.h>
#include <AnimTimeline/qframeselector.h>
#include <AnimTimeline/qspinboxsmart.h>
#include <AnimTimeline/qtoolbuttonplaypause.h>
#include <AnimTimeline/qwidgetruler.h>

#include <set>
#include <stack>

class Session : public QObject {
    Q_OBJECT
public:
    Session(QObject* parent = nullptr);
    virtual ~Session();

signals:
    void envSaved(); // EXTERNAL

    void rendered(void* render); // EXTERNAL
    void renderDeleted(void* render); // EXTERNAL

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

        void* anim;
        size_t bytes;
    } Env;

    std::deque<Env> undo;
    std::stack<Env> redoHeap;

    Env first;

    double* start;
    double* end;
    double* cursor;
    double* duration;
    std::set<double>* keyPoses;

    size_t size { 0 };

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

public: // setters
    void setStart(double* value);
    void setEnd(double* value);
    void setCursor(double* value);
    void setDuration(double* value);
    void setKeyPoses(std::set<double>* value);
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
