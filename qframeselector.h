
#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

//#include "configurations.h"
#include <AnimTimeline/configurations.h>
//#include "qwidgetruler.h"
#include <AnimTimeline/qwidgetruler.h>

#include <QDoubleSpinBox>
#include <QFrame>
#include <QSpacerItem>
#include <QToolButton>
#include <set>

class QFrameSelector : public QFrame {
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget* parent = nullptr);
    ~QFrameSelector() override;

    //    void updatePlayZone();

    double nearestStep(double time) const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    //    void wheelEvent(QWheelEvent* event) override;

    // keyPress event on area but not here
    //    void keyPressEvent(QKeyEvent* event) override;
    //    void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void cursorChanged(double time); // EXTERNAL SIGNAL
    void startChanged(double time); // EXTERNAL SIGNAL
    void endChanged(double time); // EXTERNAL SIGNAL
    void durationChanged(double time); // EXTERNAL SIGNAL

    void keyPoseAdded(double time); // EXTERNAL SIGNAL
    void keyPoseDeleted(size_t id); // EXTERNAL SIGNAL
    void keyPoseChanged(size_t id); // EXTERNAL SIGNAL
    void keyPosesMoved(double gap, size_t first); // EXTERNAL SIGNAL
    void keyPoseMoved(size_t id, double time); // EXTERNAL SIGNAL

public slots:
    // ---------------------- EXTERNAL SLOTS ----------------------------------
    void onAddingKeyPose(double time = -1.0, bool internal = true); // EXTERNAL SLOT
    void onClearKeyPoses(); // EXTERNAL SLOT

    void onChangeStart(double time, bool internal = true); // EXTERNAL SLOT
    void onChangeEnd(double time, bool internal = true); // EXTERNAL SLOT
    void onChangeCursor(double time, bool internal = true); // EXTERNAL SLOT
    void onChangeDuration(double time, bool internal = true); // EXTERNAL SLOT

    //
    // ---------------------- INTERNAL SLOTS ----------------------------------
    void onSlideLeftSlider(int deltaX);
    void onSlideRightSlider(int deltaX);
    //    void onSlideRelease();
    void onLeftSlideRelease();
    void onRightSlideRelease();

    // by default (time = -1.0), add keyPose on cursor
    //    void onInternalAddingKeyPose(double time = -1.0);

    void onDeleteKeyPose();
    //    bool onInternalChangeCursor(double time, bool findNearestStep = true);

    void onSetCursorToStart();
    void onSetCursorToEnd();
    void onSetCursorToPreviousKeyPose();
    void onSetCursorToNextKeyPose();

    //    void onPlay();
    //    void onPause();
    void onChangeStartSpin();
    void onChangeEndSpin();
    void onChangeCursorSpin();
    void onChangeDurationSpin();

    //    void onAddingKeyPoseOnMouse();

    //    void onStartIncPlus();
    //    void onStartIncLess();
    //    void onEndIncPlus();
    //    void onEndIncLess();

    //    void onCursorChanged(double time);
    void updateCursorSpin();
    void updateStartSpin();
    void updateEndSpin();
    void updateDurationSpin();
    void redrawPlayZone();

private:
    // ---------------------- PRIVATE FUNCTIONS --------------------------------
    void moveKeyPoses(double gap, size_t first = 0);
    void deleteZone(double time, double time2);

private:
    int paintCounter { 0 };

    double start;
    double end;
    double cursor;

    int* nbInterval;
    double* step;
    double* pixPerSec;
    double* zero;
    double* duration;

    // question : why QSet is unordered
    //    QSet<double> keyPoses;
    std::set<double> keyPoses;

    //    int iPaint = 0;
    bool sliding { false };
    bool mouseLeftClicked { false };

    bool* midMouseDown;

    bool* shiftDown;
    bool* ctrlDown;

    //    bool * drawLock;
    int updateKeyPoseFlash { 0 };
    double keyPoseFlash;

    QTimer* timer;

    // ---------------------- REFERENCES --------------------------------------
    QWidgetRuler* widgetRuler;

    QFrame* leftSpacer;
    QLabel* leftSlider;
    QFrame* playZone;
    QLabel* rightSlider;

    QDoubleSpinBox* cursorSpin;
    QDoubleSpinBox* startSpin;
    QDoubleSpinBox* endSpin;
    QDoubleSpinBox* durationSpin;

    QToolButton* removeKeyPoseButton;
    QDoubleSpinBox* startInc;
    QDoubleSpinBox* endInc;
    QSpinBox* nbKeyPosesSpin;

public:
    // --------------------------- GETTERS ------------------------------------
    double getStart() const;
    double* getStart();
    double* getEnd();
    double* getCursor();
    double getEnd() const;
    double getCursor() const;
    int getNbKeyPoses() const;
    double getKeyPose(int id) const;
    std::set<double> getKeyPoses() const;
    std::set<double>* getKeyPoses();

    //
    // --------------------------- SETTERS ------------------------------------
    void setCursor(double time);
    void setKeyPoses(const std::set<double>& value);
    void setShiftDown(bool* value);
    //    void setDrawLock(bool *value);
    void setStart(double value);
    void setEnd(double value);
    void setDuration(double time);

    //
    // ---------------------- REFERENCES SETTERS ------------------------------
    void setLeftSpacer(QFrame* value);
    void setLeftSlider(QLabel* value);
    void setPlayZone(QFrame* value);
    void setRightSlider(QLabel* value);

    void setCursorSpin(QDoubleSpinBox* value);
    void setStartSpin(QDoubleSpinBox* value);
    void setEndSpin(QDoubleSpinBox* value);
    void setDurationSpin(QDoubleSpinBox* value);

    void setRemoveKeyPoseButton(QToolButton* value);
    void setStartInc(QDoubleSpinBox* value);
    void setEndInc(QDoubleSpinBox* value);
    void setNbKeyPosesSpin(QSpinBox* value);
    void setMidMouseDown(bool* value);
    void setCtrlDown(bool* value);
};

#endif // QFRAMESELECTOR_H
