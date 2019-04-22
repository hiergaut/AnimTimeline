#ifndef CONSTANTS_H
#define CONSTANTS_H


// fixed startup width of timeline
//  ---------------------------------
// |                                 |
// |                                 |
// |                                 |
// |                                 |
// |             ------------        |
// |            |  timeline  |       |
// |             ------------        |
// |            <--  width -->       |
// |                                 |
// |                                 |
//  ---------------------------------
//#define STARTUP_WIDTH 700

// first timeline position is fixed with these margins (right/bottom)
// parent area :
//  ---------------------------------
// |                                 |
// |                                 |
// |                                 |
// |                                 |
// |             ------------        |
// |            |  timeline  | <---> |
// |             ------------        |
// |                  ^              |
// |                  |              |
// |                  v              |
//  ---------------------------------
#define INIT_MARGIN 50


//  -----------------------     -----------------------
// |    |              |   |   |     |             |   |
//  -----------------------     -----------------------
//          ^                        ^ <- ^
// on ruler clicking, automove to nearest keyPose or step multiple if the distance is below this constant
// unit : pixel
#define AUTO_SUGGEST_CURSOR_RADIUS 5


// define default playzone start value
//#define INIT_START 0.0

// define default playZone end value
//#define INIT_END 20.0

// define default cursor value
//#define INIT_CURSOR 5.0

// define default duration
//#define INIT_DURATION 30.0

#endif // CONSTANTS_H
