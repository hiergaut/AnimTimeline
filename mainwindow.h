#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "dialog.h"
#include <AnimTimeline/FormAnimTimeline.h>
//#include <AnimTimeline/animsession.h>

typedef struct s_Anim {
    int slider;
    double b;

//    std::vector<double> *  keyPoses;
} Anim;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    // animTimeline signals
    void onPlay();
    void onPause();
    void onChangeDuration(double time);

    void onChangeCursor(double time);
    void onChangeStart(double time);
    void onChangeEnd(double time);

    void onAddKeyPose(double time);
    void onDeleteKeyPose(size_t num);
    void onChangeKeyPose(size_t num);
    void onMoveKeyPoses(double gap, size_t first);
    void onMoveKeyPose(size_t num, double time);

    void onSaveEnv();
    void onRendering(void * render);
    void onDeleteRender(void * render);
//    void onClearSession();
//    void onUndo();
//    void onRedo();

    //    void onAddCursorKeyPose();

private slots:

    //    void on_pushButton_clicked();

    //    void on_pushButton_2_clicked();

//    void on_changeCursor_clicked();
//    void on_changeDuration_clicked();
//    void on_start_clicked();
//    void on_end_clicked();
    void on_play_clicked();
    void on_pause_clicked();
    void on_durationOut_editingFinished();

    void on_cursorOut_editingFinished();
    void on_doubleSpinBox_startOut_editingFinished();
    void on_doubleSpinBox_endOut_editingFinished();

    void on_keyPoseOut_editingFinished();
    void on_pushButton_clearKeyPoses_clicked();



    void on_pushButton_showTimeline_clicked();
    void on_pushButton_hideTimeline_clicked();

private:
    Ui::MainWindow* ui;
    //    Dialog * dialog;
//    AnimTimeline* timeline;
//    AnimTimelineWithSession * timeline;
//    AnimSession * session;
};

#endif // MAINWINDOW_H
