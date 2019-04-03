#ifndef CONSTANTS_H
#define CONSTANTS_H

// first timeline position is fixed with this margin (right/bottom)
// parent area :
//  -------------------------------
// |                               |
// |                               |
// |                               |
// |                               |
// |               ---------       |
// |              |         | <--> |
// |               ---------       |
// |                   ^           |
// |                   |           |
// |                   v           |
//  -------------------------------
#define INIT_MARGIN 50


//  -----------------------     -----------------------
// |    |              |   |   |     |             |   |
//  -----------------------     -----------------------
//          ^                        ^ <- ^
// on ruler clicking, automove to nearest keyPose if the distance is below this constant
// unit : seconde
#define STICKY_KEYPOSE_DISTANCE 0.2


// define default playzone start value
#define INIT_START 0.0

// define default playZone end value
#define INIT_END 20.0

// define default cursor value
#define INIT_CURSOR 5.0

// define default duration
#define INIT_DURATION 30.0

#endif // CONSTANTS_H
