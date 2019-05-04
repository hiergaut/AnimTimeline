#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

// fix timeline's width directly in animtimeline.ui

// first timeline position is fixed at startup with these margins (right/bottom)
// parent area :
//  ---------------------------------
// |                                 |
// |                                 |
// |                                 |
// |                                 |
// |             ------------        |
// |            |  timeline  | <---> |
// |             ------------        |
// |                                 |
// |                                 |
// |                                 |
//  ---------------------------------
#define MARGIN_RIGHT 50 // unit : pixel
//  ---------------------------------
// |                                 |
// |                                 |
// |                                 |
// |                                 |
// |             ------------        |
// |            |  timeline  |       |
// |             ------------        |
// |                  ^              |
// |                  |              |
// |                  v              |
//  ---------------------------------
#define MARGIN_DOWN 40 // unit : pixel

//  -----------------------     -----------------------
// |    |   timeline   ||  |   |   |   timeline    ||  |
//  -----------------------     -----------------------
//          ^                      ^ <--
//          |                      |
//        cursor                 cursor
// on ruler clicking, automove to nearest keyPose or ruler scale
// if the distance with cursor is below this constant
#define AUTO_SUGGEST_CURSOR_RADIUS 4 // unit : pixel

// zoom in/out speed factor (<wheelMouse> on ruler)
#define ZOOM_SPEED 1.0 // unit : double (0.0 .. oo)

// slide ruler speed factor (<ctrl> + <wheelMouse> on ruler)
#define SLIDE_SPEED 1.0 // unit : double (0.0 .. oo)

// todo : maybe set global static var
// frame per second to draw position of each frame in ruler
#define FPS 24

#define BUFFER_SESSION_MAX_SIZE 500000000 // 500M bytes in RAM, max bytes for saving user anim for undo/redo stack

#define DELAY_AUTO_SAVE 100 // millisecond, auto save environment after delay

#endif // CONFIGURATIONS_H
