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

![test timeline](testApplication.png)

![video test timeline](testApplication.mp4)


## On your Qt application write
```c++
AnimTimeline * timeline = new AnimTimeline();
timeline->show(); // dialog window
```

<!-- [![](https://img.youtube.com/vi/)](https://www.youtube.com/watch/ "view on youtube") -->
