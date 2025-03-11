#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

#include <math.h>
#include <map>
#include <list>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include "GLVector.h"
#include <GL/glut.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef unsigned char uchar;

using namespace std;

// If M_PI has not been defined in math.h add it now
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define value to convert angles in degrees to radians
#define DEG2RAD (M_PI/180)

#endif