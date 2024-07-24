#ifndef SCREEN_GEOMETRICS_HPP
#define SCREEN_GEOMETRICS_HPP

#include "AppState.hpp"

#define BIGSCREEN  //Use display in 128 x 64, if not set, fallback

#define SCREEN_WIDTH 128  // OLED display width, in pixels

#ifdef BIGSCREEN
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#else
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#endif                    //BIGSCREEN

#define ROTATE_SCREEN 2  // Screen rotation, 0 normal, 2 = rotated by 180°

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...

//The box configuration of the boxes
const int nBoxes = 4;

#ifdef BIGSCREEN  // 128 x 64
const byte boxes[nBoxes][4] = {
  { 0, 0, 64, 16 },
  { 64, 0, 64, 16 },
  { 0, 24, 128, 16 },
  { 0, 48, 128, 16 }
};
#define TRIANGLEOFFSET 3   //The horizontal size of the left and right triangle, its height is twice that measure
#define CIRCLEINSET 1      //The horizontal size of the left and right triangle, its height is twice that measure
#define CIRCLEFILLINSET 3  //The horizontal size of the left and right triangle, its height is twice that measure
#define SQUAREINSET 1      //The horizontal size of the left and right triangle, its height is twice that measure
#define SQUAREFILLINSET 4  //The horizontal size of the left and right triangle, its height is twice that measure
#define TEXTOFFSETX 4
#define TEXTOFFSETY 4

#define TEXTDEDAULTOFFSETX 0
#define TEXTDEDAULTOFFSETY 12

#else                      // 128 x 32
const byte boxes[nBoxes][4] = {
  { 0, 0, 64, 11 },
  { 64, 0, 64, 11 },
  { 0, 11, 128, 11 },
  { 0, 22, 128, 10 }
};
#define TRIANGLEOFFSET 2   //The horizontal size of the left and right triangle, its height is twice that measure
#define CIRCLEINSET 1      //The horizontal size of the left and right triangle, its height is twice that measure
#define CIRCLEFILLINSET 2  //The horizontal size of the left and right triangle, its height is twice that measure
#define SQUAREINSET 1      //The horizontal size of the left and right triangle, its height is twice that measure
#define SQUAREFILLINSET 4  //The horizontal size of the left and right triangle, its height is twice that measure
#define TEXTOFFSETX 3
#define TEXTOFFSETY 2

#endif  //BIGSCREEN




#endif
