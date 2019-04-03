
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
    void updatePlayZone();
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
    void updateDurationSpin();
    void setNbKeyPosesSpin(QSpinBox* value);

    void setShiftDown(bool* value);

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
    // all external signal
    void cursorChanged(double time);
    void startChanged(double time);
    void endChanged(double time);
    //    void nbKeyPosesChanged(int nbEl);
    void keyPoseAdded(double time);
    void keyPoseChanged(int num);
    void keyPoseMoved(int num, double time);
    void keyPosesMoved(double gap, int first);
    void keyPoseDeleted(int num);

public slots:
    void onSlideLeftSlider(int deltaX);
    void onSlideRightSlider(int deltaX);
    void onSlideRelease();

    // by default, add keyPose on cursor
    void onAddingKeyPose(double time = -1.0, bool internal = true);
    void onDeleteKeyPose();
    void onClearKeyPoses();

    void onChangeStart(double time);
    void onChangeEnd(double time);
    void onChangeCursor(double time, bool findNearestKeyPose = false);
    void onChangeDuration();

    void onSetCursorToStart();
    void onSetCursorToEnd();
    void onSetCursorToPreviousKeyPose();
    void onSetCursorToNextKeyPose();

    void onPlay();
    void onPause();
    void onChangeCursorSpin();
    void onChangeStartSpin();
    void onChangeEndSpin();

    void onStartIncPlus();
    void onStartIncLess();
    void onEndIncPlus();
    void onEndIncLess();

private:
    void updateCursorSpin();
    void updateStartSpin();
    void updateEndSpin();
    void updateKeyPoses(double gap, int first = 0);

private:
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

    double start = INIT_START;
    double end = INIT_END;
    double cursor = INIT_CURSOR;

    int* nbInterval;
    double* step;
    double* pixPerSec;
    double* zero;
    double* maxDuration;

    // question : why QSet is unordered
    //    QSet<double> keyPoses;
    std::set<double> keyPoses;

    int iPaint = 0;
    bool sliding = false;
    bool mouseLeftClicked = false;
    bool* shiftDown;
};

#endif // QFRAMESELECTOR_H
