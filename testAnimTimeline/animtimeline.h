#ifndef ANIMTIMELINE_H
#define ANIMTIMELINE_H

#include <QWidget>

namespace Ui {
class AnimTimeline;
}

class AnimTimeline : public QWidget
{
    Q_OBJECT

public:
    explicit AnimTimeline(QWidget *parent = nullptr);
    ~AnimTimeline();

private:
    Ui::AnimTimeline *ui;
};

#endif // ANIMTIMELINE_H
