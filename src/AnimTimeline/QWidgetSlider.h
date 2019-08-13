#ifndef QLABELSLIDER_H
#define QLABELSLIDER_H

//#include <QLabel>
#include <QWidget>

//class QLabelSlider : public QLabel {
class QWidgetSlider : public QWidget {
    Q_OBJECT
public:
    explicit QWidgetSlider(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void slide(int deltaX);
    void slideRelease();

public slots:

private:
    int x;
    bool clicked = false;

    bool align[3];
};

#endif // QLABELSLIDER_H
