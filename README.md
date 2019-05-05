# AnimTimeline
Custom Qt widget like Blender timeline for animation

![timeline](timeline.png)

Released date : April 2019

## Build Instructions (Linux)
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run test application
```viml
./build/AnimTimeline
```

[![test timeline](testApplication.png)](https://youtu.be/0igq4E6YkDE "wiew on youtube")


## On your Qt application write
```c++
AnimTimeline * timeline = new AnimTimeline();
timeline->show(); // dialog window
```

