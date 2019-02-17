#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "animtimeline.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(AnimTimeline * timeline, QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
