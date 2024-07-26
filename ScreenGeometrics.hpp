#ifndef SCREEN_GEOMETRICS_HPP
#define SCREEN_GEOMETRICS_HPP

#include "AppState.hpp"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define ROTATE_SCREEN 2  // Screen rotation, 0 normal, 2 = rotated by 180°, which is the default for the bicolored display

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...

//The box configuration of the boxes
const int nBoxes = 4;

const byte boxes[nBoxes][4] = {
  { 0, 0, 64, 20 },
  { 64, 0, 64, 20 },
  { 0, 24, 128, 20 },
  { 0, 48, 128, 16 }
};
#define TRIANGLEOFFSET 3   //The horizontal size of the left and right triangle, its height is twice that measure
#define CIRCLEINSET 1      //The horizontal size of the left and right triangle, its height is twice that measure
#define CIRCLEFILLINSET 3  //The horizontal size of the left and right triangle, its height is twice that measure
#define SQUAREINSET 1      //The horizontal size of the left and right triangle, its height is twice that measure
#define SQUAREFILLINSET 4  //The horizontal size of the left and right triangle, its height is twice that measure

#define TEXTDEDAULTOFFSETY 13

#define PREFIXOFFSETX 2  // The offset for Prefix Elements, like checkboxs
#define PREFIXOFFSETY 14  // The offset for Prefix Elements, like checkboxes

#endif //SCREEN_GEOMETRICS_HPP
