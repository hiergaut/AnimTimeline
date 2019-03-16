#include "qframeplayzone.h"

QFramePlayZone::QFramePlayZone(QWidget* parent)
    : QFrame(parent)
{
    setStyleSheet("\
border: 0px solid rgb(255, 255, 255);\
background-color: qlineargradient(spread:pad, x1: 0 y1: 0, x2: 0 y2: 1, stop:0 rgba(0,255, 0, 30), stop:1 rgba(0, 255, 0, 30));\
");
}
