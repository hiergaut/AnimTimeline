#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
//#include "animtimeline.h"
//#include "dialog.h"
#include <set>
#include <vector>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ui->widget->onChangeDuration(40);
//    ui->widget->onChangeCursor(5);
//    ui->widget->hide();
//    ui->widget->setVisible(false);
//    ui->widget->setEnabled(false);
//    ui->widget->show();


//    dialog->setModal(true);
//    dialog->exec();

//    timeline = new AnimTimeline(this);
//    dialog = new Dialog(this);
//    dialog->show();
//    timeline = new AnimTimelineWithSession();
//    session = new AnimSession(timeline);
//    session->setAnimType<Anim>();
//    timeline->show();
//    timeline->resize(100, 100);
    // signals
//    connect(timeline, &AnimTimeline::cursorChanged, ui->cursorIn, &QDoubleSpinBox::setValue);

    // --------------------------- ANIM_TIMELINE SIGNALS ----------------------
    connect(ui->widget_timeline, SIGNAL(playClicked()), this, SLOT(onPlay()));
    connect(ui->widget_timeline, SIGNAL(pauseClicked()), this, SLOT(onPause()));
//    connect(timeline, SIGNAL(durationChanged(double)), ui->durationIn, SLOT(setValue(double)));
    connect(ui->widget_timeline, SIGNAL(durationChanged(double)), this, SLOT(onChangeDuration(double)));

//    connect(timeline, SIGNAL(durationChanged(double)), this, SLOT(onChangeDuration(double)));

//    connect(timeline, SIGNAL(cursorChanged(double)), ui->cursorIn, SLOT(setValue(double)));
    connect(ui->widget_timeline, SIGNAL(cursorChanged(double)), this, SLOT(onChangeCursor(double)));
//    connect(timeline, SIGNAL(startChanged(double)), ui->doubleSpinBox_startIn, SLOT(setValue(double)));
    connect(ui->widget_timeline, SIGNAL(startChanged(double)), this, SLOT(onChangeStart(double)));
//    connect(timeline, SIGNAL(endChanged(double)), ui->doubleSpinBox_endIn, SLOT(setValue(double)));
    connect(ui->widget_timeline, SIGNAL(endChanged(double)), this, SLOT(onChangeEnd(double)));

    connect(ui->widget_timeline, SIGNAL(keyPoseAdded(double)), this, SLOT(onAddKeyPose(double)));
    connect(ui->widget_timeline, SIGNAL(keyPoseDeleted(size_t)), this, SLOT(onDeleteKeyPose(size_t)));
    connect(ui->widget_timeline, SIGNAL(keyPoseChanged(size_t)), this, SLOT(onChangeKeyPose(size_t)));
    connect(ui->widget_timeline, SIGNAL(keyPosesMoved(double, size_t)), this, SLOT(onMoveKeyPoses(double, size_t)));
    connect(ui->widget_timeline, SIGNAL(keyPoseMoved(size_t, double)), this, SLOT(onMoveKeyPose(size_t, double)));

    connect(ui->widget_timeline, SIGNAL(envSaved()), this, SLOT(onSaveEnv()));
    connect(ui->widget_timeline, SIGNAL(rendered(void *)), this, SLOT(onRendering(void *)));
//    connect(timeline, &AnimTimeline::rendered, this, &MainWindow::onRendering);
    connect(ui->widget_timeline, &FormAnimTimelineWithSession::renderDeleted, this, &MainWindow::onDeleteRender);
//    connect(timeline, )

//    connect(timeline, SIGNAL(envSaved()), this, SLOT(onSaveEnv()));
//    connect(timeline, SIGNAL(sessionCleared()), this, SLOT(onClearSession()));
//    connect(timeline, SIGNAL(undid()), this, SLOT(onUndo()));
//    connect(timeline, SIGNAL(redid()), this, SLOT(onRedo()));


    // --------------------------- ANIM_TIMELINE SLOTS ------------------------
//    connect(ui->play, SIGNAL(clicked()), timeline, SLOT(onSetPlayMode()));
//    connect(ui->pause, SIGNAL(clicked()), timeline, SLOT(onSetPauseMode()));
//    connect(ui->durationOut, SIGNAL(valueChanged(double)), timeline, SLOT(onChangeAnimDuration(double)));

//    connect(ui->cursorOut, SIGNAL(valueChanged(double)), timeline, SLOT(onChangeCursor(double)));

//    connect(ui->addKeyPose, SIGNAL(clicked()), this, SLOT(onAddCursorKeyPose()));

//    std::vector<double> vect{1, 5};
//    timeline->onAddingKeyPose(6);

    ui->widget_timeline->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}


// -------------------------- ANIM_TIMELINE SIGNALS ---------------------------
void MainWindow::onPlay()
{
//    qDebug() << "\033[35mplayClicked()\033[0m";

}

void MainWindow::onPause()
{
//    qDebug() << "\033[35mpauseClicked()\033[0m";

}

void MainWindow::onChangeDuration(double time)
{
//    qDebug() << "\033[35mdurationChanged(" << time << ")\033[0m";
    ui->durationIn->setValue(time);

}


void MainWindow::onChangeCursor(double time)
{
//    qDebug() << "\033[35mcursorChanged(" << time << ")\033[0m";
    ui->cursorIn->setValue(time);

}

void MainWindow::onChangeStart(double time)
{
//    qDebug() << "\033[35mstartChanged(" << time << ")\033[0m";
    ui->doubleSpinBox_startIn->setValue(time);

}

void MainWindow::onChangeEnd(double time)
{
//    qDebug() << "\033[35mendChanged(" << time << ")\033[0m";
    ui->doubleSpinBox_endIn->setValue(time);

}


void MainWindow::onAddKeyPose(double time)
{
//    qDebug() << "\033[35mkeyPoseAdded(" << time << ")\033[0m";
//    ui->nbKeyPose->setValue(timeline->getNbKeyPoses());
    ui->nbKeyPose->setValue(ui->nbKeyPose->value() +1);
}

void MainWindow::onDeleteKeyPose(size_t num)
{
//    qDebug() << "\033[35mkeyPoseDeleted(" << num << ")\033[0m";
//    ui->nbKeyPose->setValue(timeline->getNbKeyPoses());
    ui->nbKeyPose->setValue(ui->nbKeyPose->value() -1);
}

void MainWindow::onChangeKeyPose(size_t num)
{
//    qDebug() << "\033[35mkeyPoseChanged(" << num << ")\033[0m";
}


void MainWindow::onMoveKeyPoses(double gap, size_t first)
{
//    qDebug() << "\033[35mkeyPosesMoved(" << gap << ", " << first << ")\033[0m";
}

void MainWindow::onMoveKeyPose(size_t num, double time)
{
//    qDebug() << "\033[35mkeyPoseMoved(" << num << ", " << time << ")\033[0m";

}

void MainWindow::onSaveEnv()
{
//    qDebug() << "\033[35menvSaved()\033[0m";

//    std::vector<double> keyPoses{0.0};
//    keyPoses.push_back(0.0);
//    keyPoses.push_back(1.0);
//    Env env;
//    struct s_Env env;
//    Anim a {0, 1.0, &keyPoses};

    Anim * anim = static_cast<Anim*>(malloc(sizeof(struct s_Anim)));
//    *anim = a;
    anim->slider = ui->anim->value();
//    anim->a = 0;
//    anim->b = 1.0;
//    anim->keyPoses = keyPoses;
//            anim{0, 1.0, keyPoses};
//    Render * render = new Render(0, 0.0, keyPoses);
//    Render * render = (Render *)malloc(sizeof (struct s_render));
//    render->a = 0;
//    render->b = 1.0;
//    render->keyPoses = {0, 1.0, 2};

    ui->widget_timeline->onSaveRendering(anim, sizeof(*anim));
}

void MainWindow::onRendering(void *render)
{
    Anim * anim = static_cast<Anim*>(render);
//    qDebug() << "\033[35mrendered(" << anim << ")\033[0m";

    ui->anim->setValue(anim->slider);
}

void MainWindow::onDeleteRender(void *render)
{
    Anim * anim = static_cast<Anim*>(render);
//    qDebug() << "\033[35mrenderDeleted(" << anim << ")\033[0m";

    free(anim);
//    delete(anim);
}

//void MainWindow::onRendering()
//{

//}

//void MainWindow::onClearSession()
//{
//    qDebug() << "\033[35menvsCleared()\033[0m";

//}

//void MainWindow::onUndo()
//{
//    qDebug() << "\033[35mundid()\033[0m";

//}

//void MainWindow::onRedo()
//{
//    qDebug() << "\033[35mredid()\033[0m";

//}



// ------------------------------- OTHERS SLOTS -------------------------------

//void MainWindow::onAddCursorKeyPose()
//{
//    qDebug() << "\033[35madding keyPose current cursor \033[0m";
//    timeline->onAddingKeyPose(ui->keyPose->value());
//}


//void MainWindow::on_pushButton_clicked()
//{
//   ui->widget->onAddingKeyPose(ui->doubleSpinBox_4->value());
//}

//void MainWindow::on_pushButton_2_clicked()
//{
////    Dialog dialog;
////    dialog.setModal(true);
////    dialog.exec();
////    ui->widget->hide();
////    dialog = new Dialog(ui->widget, this);
////    dialog->show();

//}

void MainWindow::on_play_clicked()
{
    ui->widget_timeline->onSetPlayMode();
}

void MainWindow::on_pause_clicked()
{
    ui->widget_timeline->onSetPauseMode();
}

void MainWindow::on_durationOut_editingFinished()
{
//    ui->widget_timeline->onChangeAnimDuration(ui->durationOut->value());
    ui->widget_timeline->onChangeDuration(ui->durationOut->value());
}


void MainWindow::on_cursorOut_editingFinished()
{
    ui->widget_timeline->onChangeCursor(ui->cursorOut->value());
}

void MainWindow::on_doubleSpinBox_startOut_editingFinished()
{
    ui->widget_timeline->onChangeStart(ui->doubleSpinBox_startOut->value());
}

void MainWindow::on_doubleSpinBox_endOut_editingFinished()
{
    ui->widget_timeline->onChangeEnd(ui->doubleSpinBox_endOut->value());
}


void MainWindow::on_keyPoseOut_editingFinished()
{
    ui->widget_timeline->onAddingKeyPose(ui->keyPoseOut->value());
}

void MainWindow::on_pushButton_clearKeyPoses_clicked()
{
    ui->widget_timeline->onClearKeyPoses();
    ui->nbKeyPose->setValue(0);
}



void MainWindow::on_pushButton_showTimeline_clicked()
{
    ui->widget_timeline->show();
}

void MainWindow::on_pushButton_hideTimeline_clicked()
{
    ui->widget_timeline->hide();
}

//void MainWindow::on_changeCursor_clicked()
//{
//    timeline->onChangeCursor(ui->cursorOut->value());
//}

//void MainWindow::on_changeDuration_clicked()
//{
//    timeline->onChangeAnimDuration(ui->durationOut->value());

//}

//void MainWindow::on_start_clicked()
//{
//    timeline->onChangeStart(ui->doubleSpinBox_startOut->value());
//}

//void MainWindow::on_end_clicked()
//{
//    timeline->onChangeEnd(ui->doubleSpinBox_endOut->value());

//}



