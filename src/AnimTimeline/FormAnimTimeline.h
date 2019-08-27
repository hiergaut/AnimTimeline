/*!
 * \brief Custom Qt widget for animation (timeline)
 * \author Gauthier Bouyjou (email : gauthierbouyjou@aol.com)
 * \date april 2019
 *
 * For question send me mail or add issue to initial
 * github repo : https://github.com/hiergaut/AnimTimeline.git
 * current commit id : 93854c5c91578430aa6efb665b7b63773ade4619
 */

#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

//#include <AnimTimeline/session.h>
#include "AnimTimelineSession.h"

#include <QDialog>
#include <QObject>

namespace Ui {
class FormAnimTimeline;
}

/*!
 * \brief The FormAnimTimeline class is a minimal specification of timeline abilities
 */
class FormAnimTimeline : public QWidget {
    Q_OBJECT

public:
    explicit FormAnimTimeline(QWidget* parent = nullptr);
    ~FormAnimTimeline() override;

protected:
    virtual void resizeEvent(QResizeEvent* ev) override;

//signals:
//    void startChanged(double time);
//    void endChanged(double time);
//    void cursorChanged(double time);
//    void durationChanged(double time);

//    void keyPoseAdded(double time);
//    void keyPoseDeleted(size_t id);
//    void keyPoseChanged(size_t id);
//    void keyPoseMoved(size_t id, double time);
//    void keyPosesMoved(double gap, size_t first = 0);

//    void playClicked();
//    void pauseClicked();

//public slots:
//    virtual void onChangeStart(double time);
//    virtual void onChangeEnd(double time);
//    virtual void onChangeCursor(double time);
//    virtual void onChangeDuration(double time);

//    virtual void onAddingKeyPose(double time);
//    virtual void onClearKeyPoses();
//    virtual void onSetPlayMode(); // use it if external play button
//    virtual void onSetPauseMode(); // use it if external pause button

//protected:
//    Ui::FormAnimTimeline* ui;

signals:
    /*!
     * \brief startChanged is emitted when user move left slider of playzone or set new value in start spin (green)
     * \param time is the new start time for playzone
     */
    void startChanged(double time);
    /*!
     * \brief endChanged is emitted when user move right slider of playzone or set new value in end spin (red)
     * \param time is the new end time for playzone
     */
    void endChanged(double time);
    /*!
     * \brief cursorChanged is emitted when user move cursor
     * \param time is the new time of cursor to render in engine
     */
    void cursorChanged(double time);
    /*!
     * \brief durationChanged is emitted when user change duration time in top right spin
     * \param time is the new time for animation
     */
    void durationChanged(double time);

    /*!
     * \brief keyPoseAdded is emitted when user add new keyPose
     * \param time is the time of new keyPose, if a keyPose is already here so signal keyPoseChanged is called
     */
    void keyPoseAdded(double time);
    /*!
     * \brief keyPoseDeleted is emitted when user remove keyPose
     * \param id is the ith keyPose to remove (chronological order)
     */
    void keyPoseDeleted(size_t id);
    /*!
     * \brief keyPoseChanged is emitted when user insert keyPose in a known keyPose position
     * \param id is the ith keyPose to change
     */
    void keyPoseChanged(size_t id);
    /*!
     * \brief keyPoseMoved is emitted when user move keyPose on cursor to new position (on mouse)
     * \param id is the ith keyPose to move
     * \param time is the time of keyPose, move current keyPose in other keyPose is not possible
     */
    void keyPoseMoved(size_t id, double time);
    /*!
     * \brief keyPosesMoved is emitted when user move keyPoses
     * \param gap is the sliding distance for moving
     * \param first is the first keyPose to move with its right brothers
     */
    void keyPosesMoved(double gap, size_t first = 0);

    /*!
     * \brief playClicked is emitted when user click on play button
     */
    void playClicked();
    /*!
     * \brief pauseClicked is emitted when user click on pause button
     */
    void pauseClicked();

public slots:
    /*!
     * \brief onChangeStart change start in timeline
     * \param time is the new start
     */
    virtual void onChangeStart(double time);
    /*!
     * \brief onChangeEnd change end in timeline
     * \param time is the new end
     */
    virtual void onChangeEnd(double time);
    /*!
     * \brief onChangeCursor change cursor in timeline
     * \param time is the new cursor
     */
    virtual void onChangeCursor(double time);
    /*!
     * \brief onChangeDuration change duration in timeline
     * \param time is the new duration
     */
    virtual void onChangeDuration(double time);

    /*!
     * \brief onAddingKeyPose add keyPose in timeline
     * \param time is the new keyPose time
     */
    virtual void onAddingKeyPose(double time);
    /*!
     * \brief onClearKeyPoses remove all keyPoses in timeline
     */
    virtual void onClearKeyPoses();

    /*!
     * \brief onSetPlayMode set play mode in timeline
     */
    virtual void onSetPlayMode(); // use it if external play button
    /*!
     * \brief onSetPauseMode set pause mode in timeline
     */
    virtual void onSetPauseMode(); // use it if external pause button

protected:
    Ui::FormAnimTimeline* ui;
};

/*!
 * \brief The FormAnimTimelineWithSession class is a FormAnimTimeline with session, permit (undo/redo)
 */
class FormAnimTimelineWithSession : public FormAnimTimeline {
    Q_OBJECT

public:
    explicit FormAnimTimelineWithSession(QWidget* parent = nullptr);

signals:
    /*!
     * \brief envSaved is emitted for session, after receive this signal, user must call
     * onSaveRendering slot of FormAnimTimeline to save client anim to render later due of undo/redo envent
     */
    void envSaved();
    /*!
     * \brief rendered is emmited when undo/redo envent is calling and need to render previous environment
     * \param is the environment to render, anim is void* because Qt Q_OBJECT no accept template class
     * and signal and slots are needed to send signal to user to render
     */
    void rendered(void* anim);
    /*!
     * \brief renderDeleted is emmited when environment is not needed for future
     * \param anim is the environment to delete
     */
    void renderDeleted(void* anim);

public slots:
    /*!
     * \brief onSaveRendering must be called by user after envSaved receive
     * \param anim is the minimal environment to save in timeline session to render later
     * \param bytes is the size of minimal environment, permit to save RAM
     */
    void onSaveRendering(void* anim, size_t bytes);

public slots:
    void onChangeStart(double time) override;
    void onChangeEnd(double time) override;
    //    void onChangeCursor(double time);
    void onChangeDuration(double time) override;

    void onAddingKeyPose(double time) override;
    void onClearKeyPoses() override;

    //    void onSetPlayMode();
    //    void onSetPauseMode();

private:
    AnimTimelineSession session { this };
};

#endif // ANIMTIMELINE_H
