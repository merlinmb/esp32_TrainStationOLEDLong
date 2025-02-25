// Call up the SPIFFS FLASH filing system
#include <FS.h>
#include <SPIFFS.h>

#include "AXS15231B.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

#include <Wire.h>

#include "time.h"
#include "connectionDetails.h"

#include <TimeLib.h>

// #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "merlinNetwork.h"
#include "merlinUpdateWebServer.h"

#include "merlinTrainData.h"
#include "Orbitron_Medium_20.h"
#include "NotoSansBold15.h"
#include "Monospaced_plain_12.h"
#include "Monospaced_plain_16.h"
#include "Orbitron_Bold_16.h"

#define BATTERY_PIN 2
/*
 * Touch Specific Calls & definition
 */
uint8_t ALS_ADDRESS = 0x3B;
#define TOUCH_IICSCL 10
#define TOUCH_IICSDA 15
#define TOUCH_INT 11
#define TOUCH_RES 16

#define AXS_TOUCH_TWO_POINT_LEN 14 // (AXS_TOUCH_ONE_POINT_LEN *  2) +  2  Bytes
#define AXS_TOUCH_ONE_POINT_LEN 6
#define AXS_TOUCH_BUF_HEAD_LEN 2

#define AXS_TOUCH_GESTURE_POS 0
#define AXS_TOUCH_POINT_NUM 1
#define AXS_TOUCH_EVENT_POS 2
#define AXS_TOUCH_X_H_POS 2
#define AXS_TOUCH_X_L_POS 3
#define AXS_TOUCH_ID_POS 4
#define AXS_TOUCH_Y_H_POS 4
#define AXS_TOUCH_Y_L_POS 5
#define AXS_TOUCH_WEIGHT_POS 6
#define AXS_TOUCH_AREA_POS 7

#define AXS_GET_POINT_NUM(buf) buf[AXS_TOUCH_POINT_NUM]
#define AXS_GET_GESTURE_TYPE(buf) buf[AXS_TOUCH_GESTURE_POS]
#define AXS_GET_POINT_X(buf, point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_X_H_POS] & 0x0F) << 8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_X_L_POS])
#define AXS_GET_POINT_Y(buf, point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_Y_H_POS] & 0x0F) << 8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_Y_L_POS])
#define AXS_GET_POINT_EVENT(buf, point_index) (buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_EVENT_POS] >> 6)
uint8_t read_touchpad_cmd[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};

/*
 * Fonts
 */
#define DEBUGFONT NotoSansBold15

#define CLOCKFONT Orbitron_Medium_20

#define TRAINLINEFONT Orbitron_Bold_16
#define TRAINLINETIMEFONT Monospaced_plain_16
#define TRAINLINEHEADINGFONT Orbitron_Bold_16 // Square digital

#define NODEPARTURESHEADINGFONT Orbitron_Medium_20 // Square digital

#define PLATFORMMINIHEADINGFONT Monospaced_plain_12
#define PLATFORMLABELFONT Orbitron_Medium_20
#define PLATFORMNUMBERFONT Orbitron_Light_32 // Orbitron_Light_32
#define PLATFORMFONTMEDIUM Orbitron_Bold_16
#define PLATFORMFONTSMALL Monospaced_plain_12

#define SYSINFOHEADINGFONT Orbitron_Medium_20 // Square digital
#define SYSINFOFONTMEDIUM Orbitron_Bold_16
#define SYSINFOLABELFONT Monospaced_plain_12

#define BRIGHTNESS_PIN 14
int _brightnesses[5] = {0, 51, 115, 192, 255};
int _selectedBrightness = 4;

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 180

#define LINERENDERVIEWPORTWIDTH DISPLAY_HEIGHT
#define LINERENDERVIEWPORTHEIGHT DISPLAY_HEIGHT - CLOCK_HEIGHT

#define NUMTOUCHCOLUMNS 3
#define TOUCHCOLUMNWIDTH int(DISPLAY_WIDTH / NUMTOUCHCOLUMNS)

#define NUMBERSCREENS 1
#define TIMEBOXMARGIN 5

#define location "51.39502, -1.3387" // 97 Enborne Road

// #include "merlinTrainsStationFonts.h"

#define MCMDVERSION 1.2

#define MAXBRIGHTNESS 255
#define MINBRIGHTNESS 20
#define BACKGROUNDCOLOR TFT_BLACK
#define CALLINGATBACKGROUNDCOLOR TFT_BLACK
#define CALLINGATMARGIN 60
#define CALLINGATSPRITEMOVEBY 8

bool _brightnessHigh;
byte _brightness = 100;

int px = 10;
int py = 10;

String _mqttPostFix = "";
float _batteryVoltage = 0;

#define HTTPSPORT 443
#define icon_width 25
#define icon_height 25

boolean _forceUpdate = false;
boolean _forceRender = false;

/* frames */
byte _currentFrame = 2;
int _currentSubFrame = 0;
byte _numTrains = 0;
const byte _renderNumTrainsMax = 4;
int _callingAtScroll = 0;
bool __scrollCallingAt = false;
bool _forceDrawClock = false;
int _callingAtStringWidth[_renderNumTrainsMax];

TFT_eSPI _display = TFT_eSPI();
TFT_eSprite _sprite = TFT_eSprite(&_display);
TFT_eSprite _allPlatformSprite = TFT_eSprite(&_display);
TFT_eSprite _platformSprites[_renderNumTrainsMax] = {TFT_eSprite(&_display), TFT_eSprite(&_display), TFT_eSprite(&_display), TFT_eSprite(&_display)};
TFT_eSprite _callingAtSprites[_renderNumTrainsMax] = {TFT_eSprite(&_display), TFT_eSprite(&_display), TFT_eSprite(&_display), TFT_eSprite(&_display)};
TFT_eSprite _lineRenderSprites[_renderNumTrainsMax] = {TFT_eSprite(&_display), TFT_eSprite(&_display), TFT_eSprite(&_display), TFT_eSprite(&_display)};
TFT_eSprite _callingAtBlankSprite = TFT_eSprite(&_display);

#define LINERENDERGAP 15
#define CLOCK_HEIGHT 20

#define UPDATE_WIFICHECK_INTERVAL_MILLISECS 60000 // Update every 1 min
#define UPDATE_CALLINGATSCROLL 80                 // Update every Xms
#define UPDATE_CALLINGATSCROLLPAUSE 2000          // Pause for x time if the end of the scrolling screen is reached
#define UPDATE_UI_FRAME_INTERVAL_MILLISECS 10000  // transition screen every ... milliseconds
#define UPDATE_TRAINS_INTERVAL_MILLISECS 300000   // Update every 5min
#define UPDATE_EMPTY_INTERVAL_MILLISECS 10000     // Update every 10seconds
#define UPDATE_TIME_INTERVAL_MILLISECS 900        // Update every <1sec
#define UPDATE_TOUCH_INTERVAL_MILLISECS 400       // Update every <1sec

unsigned long _runCurrent;
unsigned long _runScrollCallingAt;
unsigned long _runScrollCallingAtTicks;
unsigned long _runFrame;
unsigned long _runTime;
unsigned long _runEmptyFrame;
unsigned long _runTrains = 0;
unsigned long _runWiFiConnectionCheck = 0;
unsigned long _runBrightness = 0;
unsigned long _runTouch = 0;

bool _initComplete = false;
bool _displayInit = false;

const byte DEBUGBUFFERLENGTH = 8;
byte _debugBufferPosition = 0;
String _debugBuffer[DEBUGBUFFERLENGTH];

String _trainBuffer[4];
String _ip = "";

#define NTPTIMEOUTVAL 4500
const char *ntpServer = "pool.ntp.org";

const long timezoneOffset = 0; // 0-23
const long gmtOffset_sec = timezoneOffset * 60 * 60;
const int daylightOffset_sec = 0;
unsigned long _epochTime;

long period = 1000;
long currentTime = 0;

String _configStation = "";
int _configFlipSreen = 999;

String _lastMQTTMessage = "";

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
  {                         \
    int16_t t = a;          \
    a = b;                  \
    b = t;                  \
  }
#endif

/***************************************************
  Screen Control Functions
****************************************************/

void setBrightness(byte brightnessValue)
{
  DEBUG_PRINTLN("setBrightness: " + String(brightnessValue));

  // ledcWrite(0, brightnessValue);
  hw_set_brightness(brightnessValue);

  // digitalWrite(TFT_BL, HIGH);

  for (int i = 0; i < 5; i++)
  {
    if (brightnessValue == _brightnesses[i])
    {
      _selectedBrightness = i;
      break;
    }
  }
}

void toggleBrightness(bool isBright)
{
  _brightness = (isBright) ? MAXBRIGHTNESS : MINBRIGHTNESS;

  //_display.setContrast(isBright ? 255 : 80);
  //_currentNeoPixelColour = pixelBrightness(_currentNeoPixelColour, _brightness);
  setBrightness(_brightness);

  _brightnessHigh = isBright;
}

void clearSprite()
{
  _sprite.fillSprite(BACKGROUNDCOLOR);
}

void clearAllPlatformSprite()
{
  _allPlatformSprite.fillSprite(BACKGROUNDCOLOR);
}

void clearPlatformSprite(byte line)
{
  _platformSprites[line].fillSprite(BACKGROUNDCOLOR);
}

void clearCallingAtSprite(byte line)
{
  _callingAtSprites[line].fillSprite(CALLINGATBACKGROUNDCOLOR);
}

void clearLineRenderSprite(byte line)
{
  _lineRenderSprites[line].fillSprite(CALLINGATBACKGROUNDCOLOR);
}

void clear_Display()
{
  _display.fillScreen(TFT_BLACK);
  // lcd_fill(0,0,180,640,0x00);       // clear screen
}

/***************************************************
  Clock Rendering
****************************************************/

#define TFT_GREY 0x5AEB

#define CENTERX int(DISPLAY_WIDTH / 2)
#define CENTERY int(DISPLAY_HEIGHT / 2)
#define CLOCKRADIUS 80

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0; // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = CENTERX, osy = CENTERY, omx = CENTERX, omy = CENTERY, ohx = CENTERX, ohy = CENTERY; // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0; // for next 1 second timeout

static uint8_t conv2d(const char *p);                                                // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

bool initial = 1;

/***************************************************
  SPIFFS functions
****************************************************/
bool parseConfigValue(String key, String value)
{
  DEBUG_PRINTLN("Parsing Config Value, " + key + ": " + value);

  key.toLowerCase();
  if (key == "station")
  {
    value.trim();
    value.toUpperCase();
    if (_configStation != value)
    {
      _configStation = value;
      _stationCode = _configStation;
      _forceUpdate = true;
    }
  }

  if (key == "flipscreen")
  {
    int __intValue = (value == "true" ? 3 : 1);
    if (_configFlipSreen != __intValue)
    {
      _configFlipSreen = __intValue;
      _display.setRotation(_configFlipSreen);
      //_forceRender = true;
    }
  }

  if (key == "brightness")
  {
    value.trim();
    if (_configStation != value)
    {
      _brightness = value.toInt();
      setBrightness(_brightness);
      //_forceRender = true;
    }
  }

  DEBUG_PRINTLN("parseConfigValue() - completed...");

  return true;
}

void setupSPIFFS()
{
  if (SPIFFS.begin())
  {
    DEBUG_PRINTLN("SPIFFS: Mounted file system");
  }
  else
  {
    DEBUG_PRINTLN("SPIFFS: FAILED to mount file system!");
  }
}
void loadCustomParamsSPIFFS()
{
  // read configuration from FS json
  DEBUG_PRINTLN("loadCustomParamsSPIFFS() - Open config file...");

  File __configFile = SPIFFS.open("/config.ini", FILE_READ);
  if (__configFile)
  {
    DEBUG_PRINTLN("Reading config file [" + String(__configFile.size()) + " bytes]");
    while (__configFile.available())
    {
      String __inString = __configFile.readStringUntil('\n');
      DEBUG_PRINTLN("Read line: " + __inString);
      int __equalsLoc = __inString.indexOf('=');

      String __key = __inString.substring(0, __equalsLoc);
      String __value = __inString.substring(__equalsLoc + 1, __inString.length());

      parseConfigValue(__key, __value);
    }

    DEBUG_PRINTLN("loadCustomParamsSPIFFS() - close config file...");
    __configFile.close();
    DEBUG_PRINTLN("... Done");
  }
}

void writeStrtoFile(File file, String key, String value)
{
  DEBUG_PRINTLN("    " + key + ": " + value);
  file.println(key + "=" + value);
}

void saveConfigValuesSPIFFS()
{
  DEBUG_PRINTLN("saveConfigValuesSPIFFS()");
  if (SPIFFS.remove("/config.ini"))
  {
    DEBUG_PRINTLN("Deleted old file");
  }

  DEBUG_PRINTLN("Open File in Write Mode");
  // open the file in write mode
  File __configFile = SPIFFS.open("/config.ini", FILE_WRITE);
  DEBUG_PRINTLN("Saving config to FS");

  writeStrtoFile(__configFile, "station", String(_configStation));
  writeStrtoFile(__configFile, "flipscreen", String(_configFlipSreen == 3));
  writeStrtoFile(__configFile, "brightness", String(_brightness));
  __configFile.close();
  DEBUG_PRINTLN("... Done");
  delay(250); // give SPIFFS chance to settle
}

static uint8_t conv2d(const char *p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void drawProgress(byte percentage, String label)
{
  // clearSprite();
  //_sprite.setTextAlignment(TEXT_ALIGN_CENTER);
  //_sprite.setFont(ArialMT_Plain_10);
  //_sprite.drawString(64, 10, label);
  //_sprite.drawProgressBar(2, 28, 124, 10, percentage);
  //_sprite.display();
}

void DisplayOut(String outStr)
{

  _debugBufferPosition++;
  if (_debugBufferPosition >= DEBUGBUFFERLENGTH)
  {
    for (byte i = 0; i < DEBUGBUFFERLENGTH - 1; i++) // StackArray - shift to left
    {
      _debugBuffer[i] = _debugBuffer[i + 1];
    }
    _debugBufferPosition = DEBUGBUFFERLENGTH - 1;
  }

  DEBUG_PRINTLN(outStr);

  // render
  clearSprite();

  _sprite.loadFont(DEBUGFONT);
  _sprite.setTextColor(TFT_WHITE, TFT_BLACK, true);
  //_sprite.setTextSize(12);
  _debugBuffer[_debugBufferPosition] = outStr;
  for (byte i = 0; i < _debugBufferPosition; i++) // StackArray - shift to left
  {
    _sprite.drawString(_debugBuffer[i], 5, 20 * i, 2);
  }
  _sprite.unloadFont();
  lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t *)_sprite.getPointer());
  // delay(100);
}

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t colour)
{
  _sprite.drawLine(x0, y0, x1, y1, colour);
  _sprite.drawLine(x1, y1, x2, y2, colour);
  _sprite.drawLine(x2, y2, x0, y0, colour);
}

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  drawTriangle(x0, y0, x1, y1, x2, y2, TFT_WHITE);
}

// Fill a triangle
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1)
  {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2)
  {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1)
  {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2)
  { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    _sprite.drawFastHLine(a, y0, b - a + 1, TFT_WHITE);
    return;
  }

  int16_t
      dx01 = x1 - x0,
      dy01 = y1 - y0,
      dx02 = x2 - x0,
      dy02 = y2 - y0,
      dx12 = x2 - x1,
      dy12 = y2 - y1;
  int32_t
      sa = 0,
      sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it
  for (y = y0; y <= last; y++)
  {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
      a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
      b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    _sprite.drawFastHLine(a, y, b - a + 1, TFT_WHITE);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++)
  {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
      a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
      b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    // writeFastHLine(a, y, b - a + 1);
    // display->drawHorizontalLine(a, y, b - a + 1);
    _sprite.drawFastHLine(a, y, b - a + 1, TFT_WHITE);
  }
}

/***************************************************
  Train Data Parsing
****************************************************/
String fixTimeFormatting(String _shortTime)
{
  // DEBUG_PRINTLN("ETA: " + __eta);
  _shortTime.trim();
  int __strLen = _shortTime.length();

  if (__strLen > 0)
    return _shortTime.substring(0, 2) + ":" + _shortTime.substring(2);
  else
    return "tba";
}

String returnPostfix(int number)
{

  String postfix;

  if (number >= 11 && number <= 13)
  {
    postfix = "th";
  }
  else
  {
    int lastDigit = number % 10;
    switch (lastDigit)
    {
    case 1:
      postfix = "st";
      break;
    case 2:
      postfix = "nd";
      break;
    case 3:
      postfix = "rd";
      break;
    default:
      postfix = "th";
      break;
    }
  }

  return postfix;
}

boolean isDigit(char c)
{
  return ((c >= '0') && (c <= '9'));
}

// check a string to see if it is numeric
bool isNumeric(String str)
{
  for (byte i = 0; i < str.length(); i++)
  {
    if (isDigit(str.charAt(i)))
      return true;
  }
  return false;
}

String getLiveTrainStatusFromInput(String input)
{
  if (isNumeric(input))
    return (fixTimeFormatting(input));
  return input;
}

u_int16_t getLiveTrainStatusColorFromInput(int __train)
{
  String __input = _mainStation.departures[__train].departures_all_item_status;
  __input.toUpperCase();

  // DEBUG_PRINTLN("getLiveTrainStatusColorFromInput: Status - "+__input);

  if (__input == "LATE" || __input == "DELAYED" || isNumeric(__input))
  {
    return TFT_ORANGE;
  }

  else if (__input == "CANCELLED")
  {
    return TFT_RED;
  }

  else if (__input == "ON TIME")
  {
    return TFT_GREEN;
  }

  return TFT_WHITE;
}

String getETAFromTrainlineStruct(int __train)
{
  return _mainStation.departures[__train].timeTable.trainStops[_mainStation.departures[__train].timeTableLength - 1].stop_aimed_arrival_time;
}

String getCallingAtFromTrainLineStruct(int __train)
{
  String __stopString = "";
  if (_mainStation.departures[__train].timeTableLength > 0)
  {
    __stopString = "";
  }
  else
  {
    return "";
  }

  for (byte i = 0; i < _mainStation.departures[__train].timeTableLength; i++)
  {
    __stopString += _mainStation.departures[__train].timeTable.trainStops[i].stop_station_name;
    if (i < _mainStation.departures[__train].timeTableLength - 1)
      __stopString = __stopString + ", ";
  }
  /*
  if (__stopString.length() > 38) //38 characters fit in this screen
    __stopString = __stopString.substring(0, 38) + "...";
  */
  return __stopString;
}

/***************************************************
  Display & Rendering
****************************************************/
void fillandDrawRect(int x1, int y1, int x2, int y2, int radius, uint8_t bgColor, uint8_t lineColor)
{
  _sprite.fillRect(x1, y1, x2, y2, bgColor);
  _sprite.fillRoundRect(x1 - TIMEBOXMARGIN - 3, y1 - TIMEBOXMARGIN - 3, x2 - x1 + TIMEBOXMARGIN * 2 + 6, y2 - y1 + TIMEBOXMARGIN * 2 + 6, radius, lineColor);
}

void drawClockBase(int centerX, int centerY, u_int16_t clockColor = TFT_YELLOW)
{
  _sprite.fillRect(centerX - 40, centerY - CLOCK_HEIGHT, centerX + 60, centerY, TFT_BLACK);

  _sprite.setTextColor(clockColor);
  _sprite.setFreeFont(&CLOCKFONT);
  _sprite.setTextDatum(BL_DATUM);

  _sprite.drawString(String(timeHour) + ":" + String(timeMin) + ":" + String(timeSec), centerX - 40, centerY + 20);
}

#define COL1 2
#define COL2 COL1 + 72
#define COL3 COL2 + 135
#define COL4 COL3 + 35
#define COL5 DISPLAY_WIDTH - 20

#define ROW1 2
#define ROW2 ROW1 + 20
#define ROW3 ROW2 + 35
#define ROW4 ROW3 + 140
#define ROW5 ROW4 + 70

#define ROWHEIGHT 25
#define HEADINGMARGIN 12
#define HEADINGHEIGHT 25

String outputTrainLine(int __y, int arrivalOrder, String _col1, String _col2, String _col3, String _col4, String _col5)
{

  _sprite.setFreeFont(&TRAINLINETIMEFONT);
  _sprite.drawString(_col1.c_str(), COL1, __y);
  _sprite.drawString(_col3.c_str(), COL3, __y);
  _sprite.drawString(_col4.c_str(), COL4, __y);
  _sprite.drawString(_col5.c_str(), COL5, __y);
  _sprite.unloadFont();

  _sprite.setFreeFont(&TRAINLINEFONT);
  _sprite.drawString(_col2.c_str(), COL2, __y - 4);
  _sprite.unloadFont();

  String __line = _col1 + " " + _col2 + " " + _col3 + " " + _col4 + " " + _col5;
  return __line;
}

String outputPlatform(int __y, int __train)
{
  String _col1 = fixTimeFormatting(_mainStation.departures[__train].departures_all_item_aimed_departure_time);
  String _col2 = _mainStation.departures[__train].departures_all_item_destination_name;
  String _col3 = "";
  String _col4 = "";
  String _col5 = _mainStation.departures[__train].departures_all_item_platform;

  _allPlatformSprite.setFreeFont(&TRAINLINETIMEFONT);
  _allPlatformSprite.drawString(_col1.c_str(), COL1, __y);
  _allPlatformSprite.drawString(_col3.c_str(), COL3, __y);
  _allPlatformSprite.drawString(_col4.c_str(), COL4, __y);
  _allPlatformSprite.drawString(_col5.c_str(), COL5, __y);
  _allPlatformSprite.unloadFont();

  _allPlatformSprite.setFreeFont(&TRAINLINEFONT);
  _allPlatformSprite.drawString(_col2.c_str(), COL2, __y - 4);
  _allPlatformSprite.unloadFont();

  String __line = _col1 + " " + _col2 + " " + _col3 + " " + _col4 + " " + _col5;
  return __line;
}

void RenderAllStationStrings()
{
  DEBUG_PRINTLN("RenderAllStationStrings");
  clearAllPlatformSprite();

  _allPlatformSprite.fillRoundRect(0, 0, DISPLAY_WIDTH, HEADINGHEIGHT, 4, TFT_WHITE);
  _allPlatformSprite.setTextDatum(TC_DATUM);
  _allPlatformSprite.setFreeFont(&TRAINLINEHEADINGFONT);
  _allPlatformSprite.setTextColor(TFT_BLACK);
  _allPlatformSprite.drawString(_mainStation.station_name + " Departures", CENTERX, 2);
  _allPlatformSprite.unloadFont();

  _allPlatformSprite.setTextDatum(TL_DATUM);
  _allPlatformSprite.setFreeFont(&TRAINLINEFONT);

  _allPlatformSprite.setTextColor(TFT_WHITE);
  byte _renderNumTrains = min(_numTrains, _renderNumTrainsMax);
  for (int __nextDepartingTrainNumber = 0; __nextDepartingTrainNumber < _renderNumTrains; __nextDepartingTrainNumber++)
  {
    _trainBuffer[__nextDepartingTrainNumber] = outputPlatform(HEADINGHEIGHT + HEADINGMARGIN + ROWHEIGHT * (__nextDepartingTrainNumber), __nextDepartingTrainNumber);
    _allPlatformSprite.fillCircle(COL2 - 9, HEADINGHEIGHT + HEADINGMARGIN + ROWHEIGHT * (__nextDepartingTrainNumber) + (ROWHEIGHT / 2) - 5, 3, getLiveTrainStatusColorFromInput(__nextDepartingTrainNumber));
    DEBUG_PRINTLN(_trainBuffer[__nextDepartingTrainNumber]);
  }

  _allPlatformSprite.unloadFont();
}

#define P_ROWHEIGHTSMALL 8
#define P_ROWHEIGHTBIG 28
#define P_ROWFONTOFFSET 2

#define P_ROW1 2
#define P_ROW2 P_ROW1 + P_ROWHEIGHTSMALL
#define P_ROW3 P_ROW2 + P_ROWHEIGHTBIG
#define P_ROW4 P_ROW3 + P_ROWHEIGHTSMALL + P_ROWHEIGHTSMALL
#define P_ROW5 P_ROW4 + P_ROWHEIGHTSMALL
#define P_ROW6 P_ROW5 + P_ROWHEIGHTBIG
#define P_ROW7 P_ROW6 + P_ROWHEIGHTBIG
#define P_ROW8 P_ROW7 + P_ROWHEIGHTSMALL

#define P_COLWIDTHSMALL 8
#define P_COLWIDTHBIG 65

#define P_COL1 2
#define P_COL2 P_COL1 + P_COLWIDTHSMALL
#define P_COL3 P_COL2 + P_COLWIDTHBIG
#define P_COL4 P_COL3 + P_COLWIDTHBIG
#define P_COL5 P_COL4 + P_COLWIDTHBIG + P_COLWIDTHSMALL
#define P_COL6 P_COL5 + (P_COLWIDTHBIG / 2) + 6
#define P_COL7 P_COL6 + P_COLWIDTHBIG / 2
#define P_COL8 P_COL7 + P_COLWIDTHBIG + P_COLWIDTHSMALL
#define P_COL9 P_COL8 + P_COLWIDTHBIG / 3

#define P_COL1_REV DISPLAY_WIDTH - 2
#define P_COL2_REV P_COL1_REV - P_COLWIDTHSMALL
#define P_COL3_REV P_COL2_REV - P_COLWIDTHBIG
#define P_COL4_REV P_COL3_REV - P_COLWIDTHBIG
#define P_COL5_REV P_COL4_REV - P_COLWIDTHBIG - P_COLWIDTHSMALL
#define P_COL6_REV P_COL5_REV - (P_COLWIDTHBIG / 2) - 6
#define P_COL7_REV P_COL6_REV - P_COLWIDTHBIG / 2
#define P_COL8_REV P_COL7_REV - P_COLWIDTHBIG - P_COLWIDTHSMALL
#define P_COL9_REV P_COL8_REV - P_COLWIDTHBIG / 3

void RenderPlatform(int _line)
{

  DEBUG_PRINTLN("RenderPlatform: Train " + String(_line));

  if (_line < 0 || _line > _numTrains)
    return;

  DEBUG_PRINTLN("RenderPlatform: render headings");

  clearPlatformSprite(_line);
  _platformSprites[_line].setTextDatum(TL_DATUM);
  _platformSprites[_line].setFreeFont(&PLATFORMMINIHEADINGFONT);

  _platformSprites[_line].setTextColor(TFT_DARKGREY);
  _platformSprites[_line].drawString("leaving", P_COL1, P_ROW1);
  _platformSprites[_line].setTextColor(TFT_YELLOW);
  _platformSprites[_line].drawString(String(_line + 1) + returnPostfix(_line + 1), P_COL1 + 60, P_ROW1);

  _platformSprites[_line].setTextColor(TFT_DARKGREY);
  _platformSprites[_line].drawString("from:", P_COL1 + 85, P_ROW1);

  if (_mainStation.departures[_line].departures_all_item_platform.length() > 0)
  {
    _platformSprites[_line].setTextDatum(TR_DATUM);
    _platformSprites[_line].drawString("platform", P_COL3_REV, P_ROW1);
  }

  _platformSprites[_line].setTextDatum(TL_DATUM);
  _platformSprites[_line].drawString("expected", P_COL4, P_ROW3);
  _platformSprites[_line].drawString("departs:", P_COL1, P_ROW3);

  _platformSprites[_line].drawString("to:", P_COL1, P_ROW4);
  _platformSprites[_line].drawString("arrival", P_COL1, P_ROW6);
  _platformSprites[_line].drawString("calling at:", P_COL1, P_ROW7);

  _platformSprites[_line].unloadFont();

  _platformSprites[_line].setTextColor(TFT_WHITE);
  _platformSprites[_line].setTextDatum(TL_DATUM);

  // from
  if (_mainStation.station_name.length() > 0)
  {
    _platformSprites[_line].setFreeFont(&PLATFORMLABELFONT);
    _platformSprites[_line].drawString(_mainStation.station_name, P_COL2, P_ROW2);
    _platformSprites[_line].unloadFont();
  }

  // platform
  if (_mainStation.departures[_line].departures_all_item_platform.length() > 0)
  {
#define PLATFORMBOXSIZE (P_COLWIDTHBIG / 2)
#define PLATFORMBOXLEFT P_COL3_REV + P_COLWIDTHSMALL
#define PLATFORMBOXWIDTH DISPLAY_WIDTH - (P_COL3_REV + P_COLWIDTHSMALL)

    _platformSprites[_line].fillRoundRect(PLATFORMBOXLEFT, P_ROW1, PLATFORMBOXWIDTH, PLATFORMBOXSIZE, 4, TFT_MAGENTA);
    _platformSprites[_line].setFreeFont(&PLATFORMNUMBERFONT);
    _platformSprites[_line].setTextDatum(TC_DATUM);
    _platformSprites[_line].drawString(_mainStation.departures[_line].departures_all_item_platform, PLATFORMBOXLEFT + PLATFORMBOXSIZE, P_ROW1 - 5);
    _platformSprites[_line].unloadFont();
  }

  // departs
  if (_mainStation.departures[_line].departures_all_item_aimed_departure_time.length() > 0)
  {
    _platformSprites[_line].setTextDatum(TL_DATUM);
    _platformSprites[_line].setFreeFont(&PLATFORMFONTMEDIUM);
    _platformSprites[_line].drawString(fixTimeFormatting(_mainStation.departures[_line].departures_all_item_aimed_departure_time), P_COL3, P_ROW3 - P_ROWFONTOFFSET);
  }

  // expected
  if (_mainStation.departures[_line].departures_all_item_status.length() > 0)
  {
    _platformSprites[_line].setTextColor(getLiveTrainStatusColorFromInput(_line));
    _platformSprites[_line].drawString(getLiveTrainStatusFromInput(_mainStation.departures[_line].departures_all_item_status), P_COL5, P_ROW3 - P_ROWFONTOFFSET);
    _platformSprites[_line].unloadFont();
  }

  // to
  if (_mainStation.departures[_line].departures_all_item_destination_name.length() > 0)
  {
    _platformSprites[_line].setFreeFont(&PLATFORMLABELFONT);
    _platformSprites[_line].setTextColor(TFT_WHITE);
    _platformSprites[_line].drawString(_mainStation.departures[_line].departures_all_item_destination_name, P_COL2, P_ROW5);
    _platformSprites[_line].unloadFont();
  }

  // arrival
  if (_mainStation.departures[_line].timeTable.trainStops[_mainStation.departures[_line].timeTableLength - 1].stop_aimed_arrival_time.length() > 0)
  {
    _platformSprites[_line].setFreeFont(&PLATFORMFONTMEDIUM);
    _platformSprites[_line].drawString(fixTimeFormatting(_mainStation.departures[_line].timeTable.trainStops[_mainStation.departures[_line].timeTableLength - 1].stop_aimed_arrival_time), P_COL3, P_ROW6 - P_ROWFONTOFFSET);
    _platformSprites[_line].unloadFont();
  }

  // Calling at
  String _callingAtString = getCallingAtFromTrainLineStruct(_line); // stopping at
  clearCallingAtSprite(_line);

  if (_callingAtString.length() == 0)
  {
    _callingAtString = _mainStation.departures[_line].departures_all_item_destination_name;
  }
  else if (_callingAtString.length() > 0)
  {
    DEBUG_PRINTLN("Calling at: " + _callingAtString.length());

    _callingAtString = _callingAtString + ".";
    _callingAtSprites[_line].setFreeFont(&PLATFORMMINIHEADINGFONT);
    _callingAtSprites[_line].setTextColor(TFT_WHITE);
    _callingAtSprites[_line].setTextDatum(TL_DATUM);
    _callingAtSprites[_line].fillScreen(BACKGROUNDCOLOR);

    _callingAtStringWidth[_line] = _callingAtSprites[_line].textWidth(_callingAtString.c_str()) + (CALLINGATMARGIN);

    _callingAtSprites[_line].deleteSprite();
    _callingAtSprites[_line].createSprite(_callingAtStringWidth[_line], 14);

    DEBUG_PRINTLN("Calling at render width: " + String(_callingAtStringWidth[_line]));
    _callingAtSprites[_line].setTextColor(TFT_WHITE, TFT_BLACK);
    _callingAtSprites[_line].drawString(_callingAtString.c_str(), 0, 0);

    _callingAtSprites[_line].unloadFont();

    /*
      int __currentClock = (String(timeHour) + String(timeMin)).toInt();
      DEBUG_PRINTLN("Current at clock: "+String(__currentClock));
      int __totalstops = _mainStation.departures[_line].timeTableLength-1;
      bool __midpointFound = false;
      for (byte i = 0; i < __totalstops; i++)
      {

        String __stopAimedDepartureTime =_mainStation.departures[_line].timeTable.trainStops[i].stop_aimed_departure_time;
        String __stationCode =_mainStation.departures[_line].timeTable.trainStops[i].stop_station_code;

        __stopAimedDepartureTime.replace(":","");
        //DEBUG_PRINTLN(__stationCode+" departure time: "+String(__stopAimedDepartureTime));
        int __stopClock = __stopAimedDepartureTime.toInt();
        DEBUG_PRINTLN(__stationCode+" aimed departure time: "+String(__stopClock));

        int __centerX = LINERENDERVIEWPORTWIDTH/2;
        int __centerY = LINERENDERVIEWPORTHEIGHT/2;

        _lineRenderSprites[_line].deleteSprite();
        _lineRenderSprites[_line].createSprite(LINERENDERVIEWPORTWIDTH, LINERENDERVIEWPORTHEIGHT-2);
        clearLineRenderSprite(_line);



        if (__currentClock < __stopClock)
        {

          if (i==0) //start of line
          {

          }
          if (i==__totalstops) //end of line
          {

          }
          if ((i-1)>0) //not first stop
          {

          }
          if ((i+1)<__totalstops)
          {

          }

          __midpointFound = true;
        }

        //render the train arror in the middle of the viewport
        _lineRenderSprites[_line].fillTriangle(__centerX-5,__centerY-5, __centerX,__centerY,  __centerX+5,__centerY-5, getLiveTrainStatusColorFromInput(_line));

        if (__midpointFound)       {break;}

      }
    */
  }
}

void renderEmpty()
{
  if (_currentFrame != 1 && _currentFrame != 0)
    clearSprite();

  _sprite.fillRoundRect(0, 0, DISPLAY_WIDTH, (ROWHEIGHT * 2) + 10, 5, TFT_LIGHTGREY);
  _sprite.setTextColor(TFT_BLACK);
  _sprite.setFreeFont(&SYSINFOHEADINGFONT);

  /*
      TL_DATUM = 0 = Top left
      TC_DATUM = 1 = Top centre
      TR_DATUM = 2 = Top right
      ML_DATUM = 3 = Middle left
      MC_DATUM = 4 = Middle centre
      MR_DATUM = 5 = Middle right
      BL_DATUM = 6 = Bottom left
      BC_DATUM = 7 = Bottom centre
      BR_DATUM = 8 = Bottom right
  */
  _sprite.setTextDatum(1);
  _sprite.drawString("No more departures are", CENTERX, 6);
  _sprite.drawString("scheduled for today", CENTERX, ROWHEIGHT);
  _sprite.unloadFont();
}

#define SI_ROWFONTOFFSET 6
#define SI_COLFONTOFFSET 15
void renderSystemInfo()
{
  DEBUG_PRINTLN("renderSystemInfo()");
  clearSprite();

  _sprite.fillRoundRect(0, 0, DISPLAY_WIDTH, HEADINGHEIGHT + 2, 4, TFT_CYAN);

  _sprite.setFreeFont(&SYSINFOHEADINGFONT);
  _sprite.setTextDatum(TC_DATUM);
  _sprite.setTextColor(TFT_BLACK);
  _sprite.drawString(getWiFIAPName(), CENTERX, 1);

  _sprite.setTextDatum(TL_DATUM);
  _sprite.setTextColor(TFT_WHITE);
  _sprite.fillRoundRect(P_COL3 - 20, HEADINGHEIGHT * 4 + 10, DISPLAY_WIDTH - P_COL3 - 5, HEADINGHEIGHT + 2, 4, TFT_BLUE);
  _sprite.drawString("markbeets@gmail.com", P_COL3 - 10, HEADINGHEIGHT * 4 + 10);

  _sprite.unloadFont();

  _sprite.setTextColor(TFT_DARKGREY);
  _sprite.setTextDatum(TL_DATUM);
  _sprite.setFreeFont(&SYSINFOLABELFONT);

  _sprite.drawString("IP", P_COL1, P_ROW3);
  _sprite.drawString("Power", P_COL1, P_ROW5);
  _sprite.drawString("Memory", P_COL1, P_ROW6);
  _sprite.drawString("By", P_COL1, P_ROW7);

  _sprite.unloadFont();

  _sprite.setTextColor(TFT_WHITE);

  /*
    _sprite.setFreeFont(&SYSINFOFONTMEDIUM);
    if (_mainStation.station_name.length() > 0)
    {
      _sprite.drawString(_mainStation.station_name, P_COL3 - SI_COLFONTOFFSET, P_ROW3 - SI_ROWFONTOFFSET);
    }
    else
    {
      _sprite.drawString("None specified", P_COL3 - SI_COLFONTOFFSET, P_ROW3 - SI_ROWFONTOFFSET);
    }
  */
  _sprite.drawString(IpAddress2String(WiFi.localIP()), P_COL3 - SI_COLFONTOFFSET, P_ROW3 - SI_ROWFONTOFFSET);
  _batteryVoltage = (analogRead(BATTERY_PIN) * 2 * 3.3 * 1000) / 4096;
  String __batteryVoltage = String(_batteryVoltage / 1000, 1) + "V " + String(_batteryVoltage / 1000 / 5.0 * 100, 0) + "%";
  _sprite.drawString(__batteryVoltage, P_COL3 - SI_COLFONTOFFSET, P_ROW5 - SI_ROWFONTOFFSET);

  _sprite.drawString(String(ESP.getFreeHeap()), P_COL3 - SI_COLFONTOFFSET, P_ROW6 - SI_ROWFONTOFFSET);

  _sprite.unloadFont();
}

/***************************************************
  Web Services
****************************************************/
bool updateTrainStationData()
{
  bool __ret = false;
  DEBUG_PRINT(F("Free Heap: "));
  DEBUG_PRINTLN(ESP.getFreeHeap());
  DEBUG_PRINTLN("Updating Train Schedule");

  DisplayOut("Updating Train Schedule");
  // Get data with an API call and place response in a String
  int __hours = _timeClient.getHours();
  int __minute = _timeClient.getMinutes();
  String __time = (__hours < 10 ? "0" : "") + String(__hours) + ":" + (__minute < 10 ? "0" : "") + String(__minute);
  String __date = _timeClient.getFormattedDate().substring(0, 10);

  DisplayOut("Reading Train Schedule for " + __date + " " + __time);

  __ret = GetTrainConditionsAndForecast(__time, __date); /// v3/uk/train/station/NBY/2022-08-03/18:11/timetable.json?app_id=4744c1ef&amp;app_key=9094b940701879dd648a76b95991a787&amp;train_status=passenger

  if (__ret)
  {
    _numTrains = _mainStation.departuresLength;
    DEBUG_PRINTLN("Number of trains parsed: " + String(_numTrains));
  }

  DEBUG_PRINT(F("updateTrainStationData Free Heap: "));
  DEBUG_PRINTLN(ESP.getFreeHeap());
  return __ret;
}
/***************************************************
  MQTT
****************************************************/
void mqttTransmitCustomSubscribe() {}
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  char message_buff[100];
  int i = 0;
  for (i = 0; i < length; i++)
  {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String __payloadString = String(message_buff);

  DEBUG_PRINTLN(__payloadString);

  String __incomingTopic = String(topic);

  _lastMQTTMessage = __incomingTopic + " " + __payloadString;

  if (__incomingTopic == "cmnd/" + String(MQTT_CLIENTNAME) + "/reset")
  {
    DEBUG_PRINTLN("Resetting ESP");
    ESP.restart();
  }
  if (__incomingTopic == "cmnd/" + String(MQTT_CLIENTNAME) + "/info")
  {
    mqttTransmitInitStat();
  }
  if (__incomingTopic == "cmnd/mcmddevices/brightness")
  {
    _brightness = __payloadString.toInt();
    _brightness = map(_brightness, 0, 100, 0, 255);
    DEBUG_PRINTLN("Setting Brightness to: " + String(_brightness));
    setBrightness(_brightness);
  }
}
void mqttCustomSubscribe() {}
void mqttTransmitCustomStat() {}
/***************************************************
  Setup
****************************************************/
void initTouch()
{
  DEBUG_PRINTLN("Initialising Touch");

  pinMode(TOUCH_INT, INPUT_PULLUP);

  // ini touch screen
  pinMode(TOUCH_RES, OUTPUT);
  digitalWrite(TOUCH_RES, HIGH);
  delay(2);
  digitalWrite(TOUCH_RES, LOW);
  delay(10);
  digitalWrite(TOUCH_RES, HIGH);
  delay(2);
  Wire.begin(TOUCH_IICSDA, TOUCH_IICSCL);
}

void initDisplay()
{
  DEBUG_PRINTLN("Initialising Display");
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); // turn off backlight asap to minimise power on artifacts

  DEBUG_PRINTLN("axs15231_init()");
  axs15231_init(); //_display.init();
  delay(100);

  _sprite.createSprite(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  _sprite.setTextColor(TFT_WHITE, TFT_BLACK);

  _allPlatformSprite.createSprite(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  _allPlatformSprite.setTextColor(TFT_WHITE, TFT_BLACK);

  _callingAtBlankSprite.createSprite(DISPLAY_WIDTH, 14);
  _callingAtBlankSprite.fillScreen(BACKGROUNDCOLOR);

  for (byte i = 0; i < _renderNumTrainsMax; i++)
  {
    _platformSprites[i].createSprite(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  }

  DEBUG_PRINTLN("clear_Display()");
  clear_Display();
  // lcd_fill(0,0,180,640,0x00);       // clear screen

  DEBUG_PRINTLN("Backlight on");
  digitalWrite(TFT_BL, HIGH); // turn on backlight

  DEBUG_PRINTLN("Init Display Complete");
}

void setupWebServer()
{

  DEBUG_PRINTLN("Handling Web Request...");

  _httpServer.on("/", []()
                 {
					   String __infoStr = "<html><head>"+style;
             __infoStr += "<script>  ";
             __infoStr += "function checkFlipped() {      document.getElementById('flipscreen').value=document.getElementById('flipscreenHidden').checked;  }";
             __infoStr += "function submitForm() { checkFlipped();    document.getElementById('myForm').submit(); }";
             __infoStr +="</script>";
             __infoStr += "</head>";
					   __infoStr += "<div align=left><H1><i>" + String(MQTT_CLIENTNAME) + "</i></H1>";
             __infoStr += loginIndex+loginIndex2;

					   __infoStr += "<hr class='new5'>";
             __infoStr += "<form action='/set' id='myForm'>";
             
             
             __infoStr += "<input for='station' data-lpignore='true' name='station' type='text' value='"+_stationCode+"' width=40%><br>";
					   __infoStr += "Vertically Flip Screen:&nbsp;&nbsp;<input id='flipscreenHidden' onclick='checkFlipped()' data-lpignore='true' name='flipscreenHidden' type='checkbox' value='true' width=20% ";
             __infoStr +=  String(_configFlipSreen==3?"checked":"")+"><input type='hidden' name='flipscreen' id='flipscreen' value='false' /><br>";

            __infoStr += "Screen brightness:&nbsp;&nbsp;";
            __infoStr += "<select id='brightness' name='brightness'>";
            for (int i = 0; i < 5; i++)
            {
                __infoStr += "<option value='"+String(_brightnesses[i])+"'"+ (_selectedBrightness==i?"selected='selected'":"") +">"+String(map(_brightnesses[i], 0, 255, 0, 100))+"%</option>";
            }
            
            __infoStr += "</select><br>";


             __infoStr += "<input type='submit' class='btn' value='Save setting(s)'>";
             __infoStr += "</form>";


					   __infoStr += "<hr  class='new5'>";
             for (byte i = 0; i < _numTrains; i++)
             {
                __infoStr += _trainBuffer[i] + "\n<br>";
             }           
					   __infoStr += "<hr class='new5'>Connected to: " + String(SSID) + " (" + _rssiQualityPercentage + "%)<br>";
					   __infoStr += "Last Message Received:  <br><i>" + _lastMQTTMessage;
					   __infoStr += "<br>Last Message Published: <br><i>" + _lastPublishedMQTTMessage;

					   __infoStr += "<hr  class='new5'>IP Address: " + IpAddress2String(WiFi.localIP());
					   __infoStr += "<br>MAC Address: " + WiFi.macAddress();
					   __infoStr += "<br>" + String(MQTT_CLIENTNAME) + " - Firmware version: " + String(MCMDVERSION,1);
					   
					   __infoStr += "</div>";

					   String __retStr = __infoStr+"</html>";

					   _httpServer.sendHeader("Connection", "close");
					   _httpServer.send(200, "text/html", __retStr); });

  _httpServer.on("/serverIndex", HTTP_GET, []()
                 {
					   _httpServer.sendHeader("Connection", "close");
					   _httpServer.send(200, "text/html", serverIndex); });

  _httpServer.on("/reset", []()
                 {
					   String _webClientReturnString = "Resetting device";
					   _httpServer.send(200, "text/plain", _webClientReturnString);
					   ESP.restart();
					   delay(1000); });
  _httpServer.on("/resetSettings", []()
                 {
                   String _webClientReturnString = "Resetting Settings";
                   _httpServer.send(200, "text/plain", _webClientReturnString);

                   if (SPIFFS.exists("/config.ini"))
                   {
                     DEBUG_PRINTLN("Removing Configuration files from SPIFFS");
                     SPIFFS.remove("/config.ini");
                   } });

  _httpServer.on("/defaults", []()
                 {
                    String _webClientReturnString = "Resetting device to defaults";
                    _httpServer.send(200, "text/plain", _webClientReturnString); });

  /*handling uploading firmware file */
  _httpServer.on(
      "/update", HTTP_POST, []()
      {
			_httpServer.sendHeader("Connection", "close");
			_httpServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
			ESP.restart(); },
      []()
      {
        HTTPUpload &upload = _httpServer.upload();
        if (upload.status == UPLOAD_FILE_START)
        {
          DisplayOut("Updating Firmware");
          DEBUG_PRINT("Update: ");
          DEBUG_PRINTLN(upload.filename.c_str());
          if (!Update.begin(UPDATE_SIZE_UNKNOWN))
          { // start with max available size
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
          //_updatingFirmware = true;
          /* flashing firmware to ESP*/
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          {
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_END)
        {
          if (Update.end(true))
          { // true to set the size to the current progress
            DEBUG_PRINTLN("Update Success:" + String(upload.totalSize) + "\nRebooting...\n");
            //_updatingFirmware = false;
          }
          else
          {
            Update.printError(Serial);
          }
        }
      });

  _httpServer.on("/set", HTTP_GET, []()
                 {
			String __retMessage = "";
			String __val = "";
			bool __update = false;

			for (uint8_t i = 0; i < _httpServer.args(); i++) {
				__val = _httpServer.arg(i);
				String __key = _httpServer.argName(i);
				__update = parseConfigValue(__key, __val);
				__retMessage += " " + _httpServer.argName(i) + ": " + _httpServer.arg(i) + (__update ? " set." : " not set.") + "\n";
			}
			_httpServer.send(200, "text/plain", __retMessage);

			if (__update) {
				saveConfigValuesSPIFFS();
			} });

  _httpServer.onNotFound(handleSendToRoot);

  _httpServer.begin();

  DEBUG_PRINTLN("Web Request Completed...");
}

void updateLocalTime()
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    return;
  }

  adjustBST(&timeinfo);

  strftime(timeHour, 3, "%H", &timeinfo);
  strftime(timeMin, 3, "%M", &timeinfo);
  strftime(timeSec, 3, "%S", &timeinfo);
  /*
   strftime(timeWeekDay,10, "%A", &timeinfo);
   strftime(timeday, 3, "%d", &timeinfo);
   strftime(timemonth, 10, "%B", &timeinfo);
   strftime(timeyear, 5, "%Y", &timeinfo);
  */
}

void rebootESP()
{
  DEBUG_PRINTLN("rebootESP");
  delay(100);
  ESP.restart();
}

void toggleSysInfoFrame()
{
  DEBUG_PRINTLN("toggleSysInfoFrame");
  _currentFrame = (_currentFrame == 0 ? 2 : 0);
  _forceRender = true;
}

void rotateBrightness()
{
  DEBUG_PRINTLN("rotateBrightness");
  _selectedBrightness--;
  if (_selectedBrightness < 0)
    _selectedBrightness = 4;

  setBrightness(_brightnesses[_selectedBrightness]);
}

void advanceFrame()
{
  DEBUG_PRINTLN("advanceFrame");
  if (_currentFrame == 2)
  {
    _forceRender = true;
    _runFrame = millis();
  }
}

void reverseFrame()
{
  DEBUG_PRINTLN("reverseFrame");

  if (_currentFrame == 2)
  {
    DEBUG_PRINTLN("_currentSubFrame: " + String(_currentSubFrame));
    _currentSubFrame--;
    _currentSubFrame--;

    if (_currentSubFrame < 0)
      _currentSubFrame = 0;

    _forceRender = true;
    _runFrame = millis();
  }
}

void updateTrainDataRenderSprites()
{

  clearSprite();
  _sprite.pushSprite(0, 0);

  DisplayOut("Updating train departures");

  updateLocalTime();
  updateTrainStationData();

  _runTrains = millis();
  for (byte i = 0; i < _numTrains; i++)
  {
    DisplayOut("Rendering arrival #" + String(i));
    RenderPlatform(i);
  }
  DisplayOut("Rendering all departures screen");
  RenderAllStationStrings();
}

void checkTouch()
{
  uint8_t __touchBuffer[20] = {0};
  Wire.beginTransmission(ALS_ADDRESS);
  Wire.write(read_touchpad_cmd, 8);
  Wire.endTransmission();
  Wire.requestFrom(ALS_ADDRESS, AXS_TOUCH_TWO_POINT_LEN);
  while (!Wire.available())
  {
  }

  Wire.readBytes(__touchBuffer, AXS_TOUCH_TWO_POINT_LEN);

  uint8_t __numTouchPoints = AXS_GET_POINT_NUM(__touchBuffer);
  uint16_t __gestureType = AXS_GET_GESTURE_TYPE(__touchBuffer);

  if (__numTouchPoints && !__gestureType)
  {
    // Serial.printf("type:%d \n", num);
    DEBUG_PRINTLN("Touch Detected");
    // Serial.printf("num:%d \n", num);
    int __pointX, __pointY = 0;
    for (int i = 0; i < __numTouchPoints; ++i)
    {
      __pointX = AXS_GET_POINT_X(__touchBuffer, i);
      __pointY = AXS_GET_POINT_Y(__touchBuffer, i);
      DEBUG_PRINTLN(String(__numTouchPoints) + " touch @ (" + String(__pointX) + "," + String(__pointY) + ")");
    }

    if (__numTouchPoints == 2)
    {
      if (_currentFrame != 0)
        toggleSysInfoFrame();
    }
    else
    {
      if (_currentFrame == 0)
      {
        toggleSysInfoFrame();
      }
      else
      {
        if (__pointX > 0 && __pointX < TOUCHCOLUMNWIDTH)
        {
          advanceFrame();
        }
        else if (__pointX > TOUCHCOLUMNWIDTH && __pointX < TOUCHCOLUMNWIDTH * 2)
        {
          rotateBrightness();
        }
        else if (__pointX > TOUCHCOLUMNWIDTH * 2 && __pointX < DISPLAY_WIDTH)
        {
          reverseFrame();
        }
      }
    }
  }
}

void printRAM()
{

  Serial.print(F("Total heap  "));
  Serial.println(ESP.getHeapSize());
  Serial.print(F("Free heap   "));
  Serial.println(ESP.getFreeHeap());
  Serial.print(F("Total psram "));
  Serial.println(ESP.getPsramSize());
  Serial.print(F("Free psram  "));
  Serial.println(ESP.getFreePsram());
}

void setupWifi()
{

  DisplayOut("Initialising WiFi: 1st AP");

  if (!isWiFiConnected(_mqttClientId))
  {
    DisplayOut("Initialising WiFi: 2nd AP");
    flipAPDetails();
    if (!isWiFiConnected(_mqttClientId))
    {
      DisplayOut("WiFi connection failed");
      DisplayOut("Restarting device");
      rebootESP();
    }
  }
}

/*
void setupWifiManager()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;


  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  String __wmAPName = "Train_Monitor_Long";

  DisplayOut("Connect to: " + __wmAPName);
  delay(150);

  //wm.resetSettings();
  //wm.erase();

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect(__wmAPName.c_str()); // password protected ap

  if (!res)
  {
    DisplayOut("Failed to connect");
    DisplayOut("Restarting Train Monitor");
    delay(1500);
    ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    DisplayOut("Connected :)");
  }
}
*/

void setup()
{
  Serial.begin(115200);

  DEBUG_PRINTLN("Starting...");
  _mqttPostFix = String(random(0xffff), HEX);
  _mqttClientId = MQTT_CLIENTNAME;
  _deviceClientName = MQTT_CLIENTNAME;

  initTouch();
  initDisplay();

  DisplayOut("Starting TrainMonitor");
  DisplayOut("----------------------------------");

  setupWifi();
  // setupWifiManager();

  // DEBUG_PRINT(F("********** Free Heap: "));   DEBUG_PRINTLN(ESP.getFreeHeap());
  DisplayOut("Web Server config");
  setupWebServer();

  // Start the server
  // DEBUG_PRINT(F("********** Free Heap: "));   DEBUG_PRINTLN(ESP.getFreeHeap());
  DisplayOut("Web Server starting");
  _httpServer.begin();

  // DEBUG_PRINT(F("********** Free Heap: "));   DEBUG_PRINTLN(ESP.getFreeHeap());
  DisplayOut("OTA Firmware Setup");
  setupOTA();

  DisplayOut("Configuring MQTT");
  setupMQTT();
  mqttReconnect(_mqttClientId);
  mqttCustomSubscribe();
  mqttSendInitStat();

  DisplayOut("Setup Time Server");
  checkBST();

  DisplayOut("Attempting MQTT: ");
  DisplayOut(String(MQTT_SERVERADDRESS) + ":1883");
  _mqttClient.setServer(MQTT_SERVERADDRESS, 1883);
  _mqttClient.setCallback(mqttCallback);

  DisplayOut("Free Heap Memory: " + String(ESP.getFreeHeap()));

  DisplayOut("DNS Setup");
  if (MDNS.begin(_deviceClientName))
  {
    DisplayOut("Connect to:");
    DisplayOut(" http://" + String(_deviceClientName) + ".local");
    MDNS.addService("http", "tcp", 80);
  }
  else
  {
    DisplayOut("DNS Setup failed");
  }
  _ip = WiFi.localIP().toString();
  DisplayOut("IP:" + _ip);

  DisplayOut("Opening Filesystem");
  setupSPIFFS();
  loadCustomParamsSPIFFS();

  DisplayOut("Updating local time");
  setupTimeClient();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  updateLocalTime();

  printRAM();
  _forceUpdate = true;
  _forceRender = true;
  _initComplete = true;
}

void loop()
{
  //_sprite.fillSprite(TFT_CYAN);
  // lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t*)_sprite.getPointer());
  // delay(500);
  //_sprite.fillSprite(TFT_ORANGE);
  // lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t*)_sprite.getPointer());
  // delay(500);

  if ((_initComplete && !_updatingFirmware))
  {

    _runCurrent = millis(); // sets the counter

    if (_runCurrent - _runTouch >= UPDATE_TOUCH_INTERVAL_MILLISECS)
    {
      checkTouch();
      _runTouch = _runCurrent;
    }

    if (_runCurrent - _runTrains >= UPDATE_TRAINS_INTERVAL_MILLISECS || _forceUpdate)
    {
      updateTrainDataRenderSprites();
    }

    if (_runCurrent - _runWiFiConnectionCheck >= UPDATE_WIFICHECK_INTERVAL_MILLISECS)
    {
      isWiFiConnected(); // make sure we're still connected
      _runWiFiConnectionCheck = millis();
    }
    /*
        if ((_runCurrent - _runEmptyFrame >= UPDATE_EMPTY_INTERVAL_MILLISECS))
        {


          if (_currentFrame != 0)
            _currentFrame = 1;

          _runEmptyFrame = millis();
        }
    */
    if ((_runCurrent - _runFrame >= UPDATE_UI_FRAME_INTERVAL_MILLISECS) || _forceUpdate || _forceRender)
    {

      if (_forceUpdate)
        DEBUG_PRINTLN("_forceUpdate is True");
      if (_forceRender)
        DEBUG_PRINTLN("_forceRender is True");

      __scrollCallingAt = false;

      DEBUG_PRINTLN("Current Frame: " + String(_currentFrame));

      if (_numTrains == 0)
      {
        renderEmpty();
        _sprite.pushSprite(0, 0);
      }
      else if (_currentFrame == 0) // debug info
      {
        renderSystemInfo();
      }
      else if (_numTrains > 0)
      {
        _currentFrame = 2;
        DEBUG_PRINTLN("_currentSubFrame: " + String(_currentSubFrame));

        if (_currentSubFrame > _numTrains)
          _currentSubFrame = 0;

        if (_currentSubFrame == 0)
        {
          _allPlatformSprite.pushToSprite(&_sprite, 0, 0);
        }
        else
        {
          DEBUG_PRINTLN("Pushing platform sprite [" + String(_currentSubFrame - 1) + "] to main sprite");
          _platformSprites[_currentSubFrame - 1].pushToSprite(&_sprite, 0, 0);
          //_lineRenderSprites[_currentSubFrame - 1].pushToSprite(&_sprite, DISPLAY_WIDTH-LINERENDERVIEWPORTWIDTH, 0);
          _callingAtSprites[_currentSubFrame - 1].pushToSprite(&_sprite, P_COL2, P_ROW8 + P_ROWFONTOFFSET * 2);

          _runScrollCallingAtTicks = millis();
          __scrollCallingAt = true;
          _forceDrawClock = true;
          _callingAtScroll = 0;
        }
        _currentSubFrame++;
      }

      _forceUpdate = false;
      _forceRender = false;
      _runFrame = millis();
    }

    if (__scrollCallingAt && (_runCurrent >= _runScrollCallingAtTicks + UPDATE_CALLINGATSCROLLPAUSE) && (_runCurrent - _runScrollCallingAt >= UPDATE_CALLINGATSCROLL))
    {
      _runScrollCallingAt = _runCurrent;

      if (_callingAtStringWidth[_currentSubFrame - 2] > (DISPLAY_WIDTH - P_COL2))
      {
        // clear the space behind the scroller
        _callingAtBlankSprite.pushToSprite(&_sprite, 0, P_ROW8 + P_ROWFONTOFFSET * 2);

        // increase the leftmost point of the 1st sprite
        _callingAtScroll += CALLINGATSPRITEMOVEBY;

        int _nextScrollingAtX = 0;
        int _totalwidth = P_COL2 - _callingAtScroll;

        while (_totalwidth < DISPLAY_WIDTH)
        {
          _callingAtSprites[_currentSubFrame - 2].pushToSprite(&_sprite, P_COL2 - _callingAtScroll + _nextScrollingAtX, P_ROW8 + P_ROWFONTOFFSET * 2);

          _nextScrollingAtX += _callingAtStringWidth[_currentSubFrame - 2] + CALLINGATMARGIN;
          _totalwidth += _callingAtStringWidth[_currentSubFrame - 2] + CALLINGATMARGIN;
        }
      }
    }

    if ((_runCurrent - _runTime >= UPDATE_TIME_INTERVAL_MILLISECS) || _forceDrawClock)
    {
      updateLocalTime();
      drawClockBase(CENTERX, DISPLAY_HEIGHT - CLOCK_HEIGHT, (_currentFrame == 0 ? TFT_CYAN : TFT_YELLOW));
      _runTime = millis();
      _forceDrawClock = false;
    }

    lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t *)_sprite.getPointer());
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    _mqttClient.loop();
    _httpServer.handleClient(); //// Check if a client has connected

    // ArduinoOTA.handle();        /* this function will handle incomming chunk of SW, flash and respond sender */
  }
}
