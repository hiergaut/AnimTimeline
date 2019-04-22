
#ifndef QFRAMESELECTOR_H
#define QFRAMESELECTOR_H

#include "constants.h"
#include "qwidgetruler.h"
#include <QDoubleSpinBox>
#include <QFrame>
#include <QSpacerItem>
#include <QToolButton>
#include <set>

class QFrameSelector : public QFrame {
    Q_OBJECT
public:
    explicit QFrameSelector(QWidget* parent = nullptr);

    void setLeftSlider(QLabel* value);
    void setPlayZone(QFrame* value);
    void setRightSlider(QLabel* value);
    void setLeftSpacer(QFrame* value);

    void setCursorSpin(QDoubleSpinBox* value);
    void setStartSpin(QDoubleSpinBox* value);
    void setEndSpin(QDoubleSpinBox* value);

    void setCursor(double time);
//    void updatePlayZone();
    double getCursor();

    double getStart() const;

    double getEnd() const;
    int getNbKeyPoses() const;
    double getKeyPose(int id) const;

    void setRemoveKeyPoseButton(QToolButton* value);

    std::set<double> getKeyPoses() const;
    void setKeyPoses(const std::set<double>& value);

    void setStartInc(QDoubleSpinBox* value);
    void setEndInc(QDoubleSpinBox* value);

    void setTotalDurationSpin(QDoubleSpinBox* value);
    void updateDurationSpin(); // EXTERNAL SLOT
    void setNbKeyPosesSpin(QSpinBox* value);

    void setShiftDown(bool* value);

//    void setDrawLock(bool *value);

    void setStart(double value);

    void setEnd(double value);

    double nearestStep(double time);

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

    void keyPoseAdded(double time); // EXTERNAL SIGNAL
    void keyPoseDeleted(int num); // EXTERNAL SIGNAL
    void keyPoseChanged(int num); // EXTERNAL SIGNAL
    void keyPosesMoved(double gap, size_t first); // EXTERNAL SIGNAL
    void keyPoseMoved(int num, double time); // EXTERNAL SIGNAL

public slots:
    void onSlideLeftSlider(int deltaX);
    void onSlideRightSlider(int deltaX);
    void onSlideRelease();

    void onAddingKeyPose(double time); // EXTERNAL SLOT
    // by default (time = -1.0), add keyPose on cursor
    void onInternalAddingKeyPose(double time = -1.0);


    void onDeleteKeyPose();
    void onClearKeyPoses(); // EXTERNAL SLOT

    void onChangeStart(double time); // EXTERNAL SLOT
    void onChangeEnd(double time); // EXTERNAL SLOT

    void onChangeCursor(double time); // EXTERNAL SLOT
    bool onInternalChangeCursor(double time);


    void onSetCursorToStart();
    void onSetCursorToEnd();
    void onSetCursorToPreviousKeyPose();
    void onSetCursorToNextKeyPose();

//    void onPlay();
//    void onPause();
    void onChangeStartSpin();
    void onChangeEndSpin();
    void onChangeCursorSpin();
    void onChangeDuration();

//    void onStartIncPlus();
//    void onStartIncLess();
//    void onEndIncPlus();
//    void onEndIncLess();

    void onCursorChanged(double time);

private:
    void updateCursorSpin();
    void updateStartSpin();
    void updateEndSpin();
    void updateKeyPoses(double gap, size_t first = 0);

private:
    int paintCounter {0};

    QWidgetRuler* widgetRuler;

    QFrame* leftSpacer;
    QLabel* leftSlider;
    QLabel* rightSlider;
    QFrame* playZone;

    QDoubleSpinBox* cursorSpin;
    QDoubleSpinBox* startSpin;
    QDoubleSpinBox* endSpin;
    QDoubleSpinBox* totalDurationSpin;

    QSpinBox* nbKeyPosesSpin;

    QDoubleSpinBox* startInc;
    QDoubleSpinBox* endInc;

    QToolButton* removeKeyPoseButton;

    double start;
    double end;
    double cursor;

    int* nbInterval;
    double* step;
    double* pixPerSec;
    double* zero;
    double* maxDuration;

    // question : why QSet is unordered
    //    QSet<double> keyPoses;
    std::set<double> keyPoses;

//    int iPaint = 0;
    bool sliding {false};
    bool mouseLeftClicked {false};

    bool* shiftDown;
//    bool * drawLock;
};

#endif // QFRAMESELECTOR_H
