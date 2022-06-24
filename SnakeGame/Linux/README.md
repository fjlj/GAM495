## What is this?
This is a linux compilable version of my snake game, this has a few different features and some notably missing features as well.. The bigger star of this implementation is actually the half precision float library I made [TinyMatrix](/SnakeGame/Linux/TinyMatrix.cpp), which is very much in a hacked together prototype phase and in need of much refactoring... what has been tested seems functional.. however I would not go trusting it with anything important. I am utilizing it for a neural network implemention, I wanted to save on space using half sized floating point matricies.

## Building
```g++ SnakePos.cpp TinyMatrix.cpp TinyMatrix.h -pthread -lm -I. -o SnakeGame```
