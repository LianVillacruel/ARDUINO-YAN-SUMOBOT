// Last Update: 09/22/2026
// *WITH AI ASSISTANCE

/* SUMOBOT PIN CONFIGURATION

L298 MOTOR DRIVER
ENA - D9
ENB - D6
IN1 - D2
IN2 - D3
IN3 - D4
IN4 - D5

12V MOTOR TO L298N DRIVER
LEFT:
(+) - OUT4
(-) - OUT3
RIGHT:
(+) - OUT1
(-) - OUT2

HC-SR04
FRONT: TRIG D10, ECHO D11
LEFT:  TRIG D8,  ECHO D7
RIGHT: TRIG D12, ECHO D13

IR
RIGHT - A1
LEFT  - A0

BUTTONS
SELECT - A2
START  - A3

OLED
SDA - A4
SCL - A5

HC-06
RXD - TX
TXD - RX
*/

// INCLUDED LIBRARIES
#include <NewPing.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ================= PIN CONFIGURATION =================

// Ultrasonic
#define TRIG_FRONT 10
#define ECHO_FRONT 11

#define TRIG_LEFT 8
#define ECHO_LEFT 7

#define TRIG_RIGHT 12
#define ECHO_RIGHT 13

#define MAX_DISTANCE 150
#define ATTACK_DISTANCE 40
#define SIDE_ATTACK_DISTANCE 50


// Motor driver
#define ENA 9
#define ENB 6

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5


// IR sensors
#define IR_RIGHT A1
#define IR_LEFT A0


// Buttons
#define BUTTON_SELECT A2
#define BUTTON_START A3


// Delayed start
#define START_DELAY 5000


// ================= AUTO SPEED SETTINGS =================

int autoAttackSpeed = 255; // Default Value
int autoSearchSpeed = 140; // Default Value

// =========== Bluetooth mode Motor Speed ============
int btMotorSpeed = 0;


// ================= OLED =================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1);


// ================= LOGO BITMAP =================

// KEEP YOUR ORIGINAL BITMAP DATA HERE

const unsigned char epd_bitmap_YAN_LOGO_FOR_OLED[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f,
  0xf8, 0x00, 0x3f, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x01, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x0f,
  0xfc, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x0f, 0xf8, 0x00, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xc0, 0x0f,
  0xff, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x1f, 0xf0, 0x00, 0x7f, 0xc0, 0x00, 0xff, 0xff, 0xc0, 0x0f,
  0xff, 0x80, 0x07, 0xff, 0xf8, 0x00, 0x3f, 0xe0, 0x00, 0x3f, 0xc0, 0x00, 0x7f, 0xff, 0xc0, 0x0f,
  0xff, 0xc0, 0x01, 0xff, 0xf0, 0x00, 0x7f, 0xc0, 0x00, 0x1f, 0xc0, 0x00, 0x1f, 0xff, 0xc0, 0x0f,
  0xff, 0xe0, 0x00, 0xff, 0xe0, 0x01, 0xff, 0x80, 0x00, 0x1f, 0xc0, 0x00, 0x0f, 0xff, 0xc0, 0x0f,
  0xff, 0xf0, 0x00, 0x7f, 0xc0, 0x03, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x07, 0xff, 0xc0, 0x0f,
  0xff, 0xf8, 0x00, 0x3f, 0x80, 0x07, 0xfe, 0x00, 0x20, 0x07, 0xc0, 0x00, 0x03, 0xff, 0xc0, 0x0f,
  0xff, 0xfc, 0x00, 0x1e, 0x00, 0x0f, 0xfc, 0x00, 0x70, 0x03, 0xc0, 0x00, 0x01, 0xff, 0xc0, 0x0f,
  0xff, 0xfe, 0x00, 0x0c, 0x00, 0x1f, 0xfc, 0x00, 0xf8, 0x01, 0xc0, 0x00, 0x00, 0x7f, 0xc0, 0x0f,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x01, 0xfc, 0x01, 0xc0, 0x00, 0x00, 0x3f, 0xc0, 0x0f,
  0xff, 0xff, 0x80, 0x00, 0x00, 0x7f, 0xf0, 0x03, 0xfc, 0x00, 0xc0, 0x04, 0x00, 0x1f, 0xc0, 0x0f,
  0xff, 0xff, 0xc0, 0x00, 0x00, 0xff, 0xe0, 0x07, 0xfe, 0x00, 0x40, 0x06, 0x00, 0x0f, 0xc0, 0x0f,
  0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xc0, 0x0f, 0xff, 0x00, 0x00, 0x07, 0x00, 0x03, 0xc0, 0x0f,
  0xff, 0xff, 0xf8, 0x00, 0x03, 0xff, 0x80, 0x1f, 0xff, 0x80, 0x00, 0x07, 0xc0, 0x01, 0xc0, 0x0f,
  0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0xc0, 0x0f,
  0xff, 0xff, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x40, 0x0f,
  0xff, 0xff, 0xff, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0xc0, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0x80, 0x1f, 0xff, 0xff, 0xff, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x07, 0xff, 0xfc, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7e, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xfe, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7c, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x78, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0f,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc1, 0xf9, 0x8f, 0x98, 0xfc, 0x3f, 0x83, 0xfc, 0x3f, 0x00, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x80, 0xf9, 0x8f, 0x88, 0xf8, 0x1f, 0x81, 0xf8, 0x1f, 0x01, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x9c, 0xf9, 0x8f, 0x80, 0xf9, 0x9f, 0x99, 0xf3, 0x9f, 0xe7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x8f, 0xf9, 0x8f, 0x80, 0xf1, 0x8f, 0x99, 0xf3, 0x9f, 0xe7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc3, 0xf9, 0x8f, 0x80, 0xf1, 0x8f, 0x81, 0xf3, 0x9f, 0xe7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0xf9, 0x8f, 0x84, 0xf1, 0x8f, 0x81, 0xf3, 0x9f, 0xe7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xbc, 0xf9, 0x8f, 0x9c, 0xf1, 0x8f, 0x98, 0xf3, 0x9f, 0xe7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x9c, 0xf9, 0x9f, 0x9c, 0xf9, 0x9f, 0x98, 0xf1, 0x9f, 0xe7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc0, 0xfc, 0x1f, 0x9c, 0xf8, 0x1f, 0x81, 0xf8, 0x3f, 0xc7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xe3, 0xfe, 0x3f, 0xbd, 0xfc, 0x7f, 0x83, 0xfc, 0x7f, 0xef, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff

};


// ================= ULTRASONIC =================

NewPing sonarFront(
  TRIG_FRONT,
  ECHO_FRONT,
  MAX_DISTANCE);

NewPing sonarLeft(
  TRIG_LEFT,
  ECHO_LEFT,
  MAX_DISTANCE);

NewPing sonarRight(
  TRIG_RIGHT,
  ECHO_RIGHT,
  MAX_DISTANCE);


int frontDistance;
int leftDistance;
int rightDistance;


// ================= MODES =================

#define INSTANT_AUTO 1
#define DELAYED_AUTO 2
#define BLUETOOTH 3

// ================= BT CURRENT COMMAND =================

const char* currentCommand = "STOP";

// ============= BT CREDENTIALS =========
const char btName[] = "yanSumobot";
String btPassword = "1227";


// ================= LINE TYPES =================

#define WHITE_LINE 1
#define BLACK_LINE 2

byte selectedMode = INSTANT_AUTO;
byte selectedLine = WHITE_LINE;


// ================= ROBOT STATE =================

bool started = false;


// ================= MENU STATES =================

#define MENU_MAIN 0
#define MENU_SETTINGS 1
#define MENU_MODE 2
#define MENU_LINE 3
#define MENU_TEST 4
#define MENU_AUTO_SPEED 5
#define MENU_SPEED_ADJUST 6

byte settingsScroll = 0;
byte autoSpeedSelection = 0;

byte speedAdjustTarget = 0;

byte currentMenu = MENU_MAIN;
byte menuSelection = 0;


// ================= TEST MENU =================

#define TEST_MENU 0
#define SENSOR_TEST 1
#define MOTOR_TEST 2

byte currentTest = TEST_MENU;


// ================= SETUP =================

void setup() {

  Serial.begin(9600);


  // ================= MOTOR PINS =================

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  // ================= IR PINS =================

  pinMode(IR_RIGHT, INPUT_PULLUP);
  pinMode(IR_LEFT, INPUT_PULLUP);


  // ================= BUTTON PINS =================

  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_START, INPUT_PULLUP);


  // Stop motors
  stopMove();


  // ================= OLED =================

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println(F("OLED NOT FOUND"));

  }

  else {

    Serial.println(F("OLED FOUND"));

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);


    // ================= STARTUP LOGO =================

    display.invertDisplay(true);

    display.drawBitmap(
      0,
      0,
      epd_bitmap_YAN_LOGO_FOR_OLED,
      128,
      64,
      SSD1306_WHITE);

    display.display();

    delay(1500);


    // ================= READY SCREEN =================

    display.invertDisplay(false);

    display.clearDisplay();

    display.setTextSize(2);

    display.setCursor(18, 5);
    display.println(F("SCRAP v2"));

    display.drawLine(
      30,
      28,
      98,
      28,
      SSD1306_WHITE);

    display.setCursor(34, 40);
    display.println(F("READY"));

    display.display();

    delay(1000);

    showMainMenu();
  }
}


// ================= MAIN LOOP =================

void loop() {

  // ================= SYSTEM STOP =================

  if (started) {

    if (stopSystem()) {
      return;
    }
  }


  // ================= MENU =================

  if (!started) {

    menuLoop();

    return;
  }


  // ================= BLUETOOTH =================

  if (selectedMode == BLUETOOTH) {

    bluetoothMode();

    return;
  }


  // ================= AUTONOMOUS =================

  autonomousMode();
}


// =================================================
// ================= MENU SYSTEM ===================
// =================================================

void menuLoop() {


  // ================= SELECT BUTTON =================

  if (buttonPressed(BUTTON_SELECT)) {


    // ================= MAIN =================

    if (currentMenu == MENU_MAIN) {

      menuSelection++;

      if (menuSelection > 1) {
        menuSelection = 0;
      }

      showMainMenu();
    }


    // ================= SETTINGS =================

    else if (currentMenu == MENU_SETTINGS) {
      menuSelection++;

      if (menuSelection > 4) {
        menuSelection = 0;
      }

      // Scroll down when selection reaches
      // the bottom visible item
      if (menuSelection >= settingsScroll + 4) {

        settingsScroll = menuSelection - 3;
      }

      // Reset scroll when returning to first item
      if (menuSelection == 0) {
        settingsScroll = 0;
      }

      showSettingsMenu();
    }


    // ================= MODE =================

    else if (currentMenu == MENU_MODE) {

      menuSelection++;

      if (menuSelection > 3) {
        menuSelection = 0;
      }

      showModeMenu();
    }


    // ================= LINE =================

    else if (currentMenu == MENU_LINE) {

      menuSelection++;

      if (menuSelection > 2) {
        menuSelection = 0;
      }

      showLineMenu();
    }


    // ================= TEST =================

    else if (currentMenu == MENU_TEST) {

      menuSelection++;

      if (menuSelection > 2) {
        menuSelection = 0;
      }

      showTestMenu();
    }

    // ================= AUTO SPEED =================

    else if (currentMenu == MENU_AUTO_SPEED) {

      autoSpeedSelection++;

      if (autoSpeedSelection > 2) {
        autoSpeedSelection = 0;
      }

      showAutoSpeedMenu();
    }

    else if (currentMenu == MENU_SPEED_ADJUST) {

      menuSelection++;

      if (menuSelection > 2) {
        menuSelection = 0;
      }

      showSpeedAdjustMenu();
    }
  }




  // ================= START BUTTON =================

  if (buttonPressed(BUTTON_START)) {


    // =================================================
    // MAIN MENU
    // =================================================

    if (currentMenu == MENU_MAIN) {

      // START
      if (menuSelection == 0) {

        startRobot();
      }


      // SETTINGS
      else if (menuSelection == 1) {

        currentMenu = MENU_SETTINGS;
        menuSelection = 0;

        showSettingsMenu();
      }
    }


    // =================================================
    // SETTINGS MENU
    // =================================================

    else if (currentMenu == MENU_SETTINGS) {

      // MODE
      if (menuSelection == 0) {

        currentMenu = MENU_MODE;
        menuSelection = 0;

        showModeMenu();
      }


      // LINE TYPE
      else if (menuSelection == 1) {

        currentMenu = MENU_LINE;
        menuSelection = 0;

        showLineMenu();
      }


      // AUTO SPEED
      else if (menuSelection == 2) {

        currentMenu = MENU_AUTO_SPEED;

        autoSpeedSelection = 0;

        showAutoSpeedMenu();
      }


      // ROBOT TEST
      else if (menuSelection == 3) {

        currentMenu = MENU_TEST;
        menuSelection = 0;

        showTestMenu();
      }


      // BACK
      else if (menuSelection == 4) {

        currentMenu = MENU_MAIN;
        menuSelection = 0;
        settingsScroll = 0;

        showMainMenu();
      }
    }

    // =================================================
    // MODE MENU
    // =================================================

    else if (currentMenu == MENU_MODE) {


      // INSTANT AUTO
      if (menuSelection == 0) {

        selectedMode = INSTANT_AUTO;

        showSaved(F("INSTANT AUTO"));
      }


      // DELAYED AUTO
      else if (menuSelection == 1) {

        selectedMode = DELAYED_AUTO;

        showSaved(F("DELAYED AUTO"));
      }


      // BLUETOOTH
      else if (menuSelection == 2) {

        selectedMode = BLUETOOTH;

        showSaved(F("BLUETOOTH"));
      }


      // BACK
      else if (menuSelection == 3) {

        currentMenu = MENU_SETTINGS;
        menuSelection = 0;

        showSettingsMenu();
      }
    }


    // =================================================
    // LINE MENU
    // =================================================

    else if (currentMenu == MENU_LINE) {


      // WHITE LINE
      if (menuSelection == 0) {

        selectedLine = WHITE_LINE;

        showSaved(F("WHITE LINE"));
      }


      // BLACK LINE
      else if (menuSelection == 1) {

        selectedLine = BLACK_LINE;

        showSaved(F("BLACK LINE"));
      }


      // BACK
      else if (menuSelection == 2) {

        currentMenu = MENU_SETTINGS;
        menuSelection = 0;

        showSettingsMenu();
      }
    }


    // =================================================
    // TEST MENU
    // =================================================

    else if (currentMenu == MENU_TEST) {


      // SENSORS
      if (menuSelection == 0) {

        currentTest = SENSOR_TEST;

        sensorTest();
      }


      // MOTORS
      else if (menuSelection == 1) {

        currentTest = MOTOR_TEST;

        motorTest();
      }


      // BACK
      else if (menuSelection == 2) {

        stopMove();

        currentTest = TEST_MENU;

        currentMenu = MENU_SETTINGS;
        menuSelection = 0;

        showSettingsMenu();
      }
    }

    // =================================================
    // AUTO SPEED MENU
    // =================================================

    else if (currentMenu == MENU_AUTO_SPEED) {

      // ATTACK SPEED
      if (autoSpeedSelection == 0) {

        speedAdjustTarget = 0;

        menuSelection = 0;

        currentMenu = MENU_SPEED_ADJUST;

        showSpeedAdjustMenu();
      }


      // SEARCH SPEED
      else if (autoSpeedSelection == 1) {

        speedAdjustTarget = 1;

        menuSelection = 0;

        currentMenu = MENU_SPEED_ADJUST;

        showSpeedAdjustMenu();
      }


      // BACK
      else if (autoSpeedSelection == 2) {

        currentMenu = MENU_SETTINGS;

        menuSelection = 2;
        settingsScroll = 0;

        showSettingsMenu();
      }
    }

    // =================================================
    // SPEED ADJUST MENU
    // =================================================

    else if (currentMenu == MENU_SPEED_ADJUST) {

      // INCREASE
      if (menuSelection == 0) {

        if (speedAdjustTarget == 0) {
          autoAttackSpeed += 10;

          if (autoAttackSpeed > 255) {
            autoAttackSpeed = 255;
          }
        } else {
          autoSearchSpeed += 10;

          if (autoSearchSpeed > 255) {
            autoSearchSpeed = 255;
          }
        }

        showSpeedAdjustMenu();
      }


      // DECREASE
      else if (menuSelection == 1) {

        if (speedAdjustTarget == 0) {

          if (autoAttackSpeed >= 10) {
            autoAttackSpeed -= 10;
          } else {
            autoAttackSpeed = 0;
          }
        }

        else {

          if (autoSearchSpeed >= 10) {
            autoSearchSpeed -= 10;
          } else {
            autoSearchSpeed = 0;
          }
        }

        showSpeedAdjustMenu();
      }


      // BACK
      else if (menuSelection == 2) {

        currentMenu = MENU_AUTO_SPEED;

        // Return to the speed we were editing
        autoSpeedSelection = speedAdjustTarget;

        showAutoSpeedMenu();
      }
    }
  }
}

void showSpeedAdjustMenu() {

  display.clearDisplay();

  display.setTextSize(1);

  // ================= HEADER =================

  display.setCursor(0, 0);

  if (speedAdjustTarget == 0) {
    display.println(F("ATTACK SPEED:"));
  } else {
    display.println(F("SEARCH SPEED:"));
  }

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);


  // ================= CURRENT SPEED =================

  display.setCursor(0, 15);

  display.print(F("SPEED: "));

  if (speedAdjustTarget == 0) {
    display.println(autoAttackSpeed);
  } else {
    display.println(autoSearchSpeed);
  }


  // ================= INCREASE =================

  display.setCursor(0, 29);

  if (menuSelection == 0) {
    display.println(F("> INCREASE"));
  } else {
    display.println(F("  INCREASE"));
  }


  // ================= DECREASE =================

  display.setCursor(0, 41);

  if (menuSelection == 1) {
    display.println(F("> DECREASE"));
  } else {
    display.println(F("  DECREASE"));
  }


  // ================= BACK =================

  display.setCursor(0, 53);

  if (menuSelection == 2) {
    display.println(F("> BACK"));
  } else {
    display.println(F("  BACK"));
  }

  display.display();
}

void showAutoSpeedMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // ================= HEADER =================

  display.setCursor(0, 0);
  display.println(F("AUTO SPEED ADJUST:"));


  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);


  // ================= ATTACK =================

  display.setCursor(0, 15);

  if (autoSpeedSelection == 0) {
    display.print(F("> ATTACK: "));
  }

  else {
    display.print(F("  ATTACK: "));
  }

  display.println(autoAttackSpeed);


  // ================= SEARCH =================

  display.setCursor(0, 29);

  if (autoSpeedSelection == 1) {
    display.print(F("> SEARCH: "));
  }

  else {
    display.print(F("  SEARCH: "));
  }

  display.println(autoSearchSpeed);


  // ================= BACK =================

  display.setCursor(0, 43);

  if (autoSpeedSelection == 2) {
    display.println(F("> BACK"));
  }

  else {
    display.println(F("  BACK"));
  }


  display.display();
}


// =================================================
// ================= START ROBOT ===================
// =================================================

void startRobot() {

  started = true;

  display.clearDisplay();


  // ================= INSTANT AUTO =================

  if (selectedMode == INSTANT_AUTO) {

    display.setTextSize(2);

    display.setCursor(5, 10);
    display.println(F("INSTANT"));

    display.setCursor(5, 35);
    display.println(F("START!"));

    display.display();

    Serial.println(F("INSTANT AUTO START"));
  }


  // ================= DELAYED AUTO =================

  else if (selectedMode == DELAYED_AUTO) {

    Serial.println(F("DELAY MODE: COUNTDOWN"));


    // 5
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(55, 15);
    display.println(F("5"));
    display.display();

    delay(1000);


    // 4
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(55, 15);
    display.println(F("4"));
    display.display();

    delay(1000);


    // 3
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(55, 15);
    display.println(F("3"));
    display.display();

    delay(1000);


    // 2
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(55, 15);
    display.println(F("2"));
    display.display();

    delay(1000);


    // 1
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(55, 15);
    display.println(F("1"));
    display.display();

    delay(1000);


    // RUNNING
    display.clearDisplay();

    display.setTextSize(2);

    display.setCursor(5, 10);
    display.println(F("DELAY AUTO"));

    display.setCursor(5, 35);
    display.println(F("RUNNING!"));

    display.display();

    Serial.println(F("DELAY MODE START"));
  }


  // ================= BLUETOOTH =================

  else if (selectedMode == BLUETOOTH) {

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    // Header
    display.setCursor(0, 0);
    display.println(F("BLUETOOTH RC MODE"));

    display.drawLine(0, 9, 127, 9, SSD1306_WHITE);

    // Bluetooth name
    display.setCursor(0, 14);
    display.print(F("NAME: "));
    display.println(btName);

    // Bluetooth password
    display.setCursor(0, 25);
    display.print(F("PIN: "));
    display.println(btPassword);

    // Command
    display.setCursor(0, 42);
    display.print(F("CMD: "));
    display.println(currentCommand);

    // Speed
    display.setCursor(0, 54);
    display.print(F("SPEED: "));
    display.println(btMotorSpeed);

    display.display();

    Serial.println(F("BLUETOOTH RC START"));
  }
}


// =================================================
// ================= STOP SYSTEM ===================
// =================================================

bool stopSystem() {

  if (digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_START) == LOW) {

    stopMove();


    display.clearDisplay();

    display.setTextSize(2);

    display.setCursor(20, 10);
    display.println(F("STOPPED"));

    display.setTextSize(1);

    display.setCursor(30, 40);
    display.println(F("Returning..."));

    display.display();

    delay(1000);


    started = false;

    currentMenu = MENU_MAIN;
    menuSelection = 0;


    while (digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_START) == LOW) {
    }

    delay(50);

    showMainMenu();

    Serial.println(F("SYSTEM STOPPED"));

    return true;
  }

  return false;
}


// =================================================
// ================= MAIN MENU =====================
// =================================================

void showMainMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // START
  display.setCursor(0, 0);

  if (menuSelection == 0) {
    display.println(F("> START"));
  }

  else {
    display.println(F("  START"));
  }


  // SETTINGS
  display.setCursor(0, 16);

  if (menuSelection == 1) {
    display.println(F("> SETTINGS"));
  }

  else {
    display.println(F("  SETTINGS"));
  }


  // Separator
  display.drawLine(
    0,
    28,
    127,
    28,
    SSD1306_WHITE);


  // MODE
  display.setCursor(0, 36);

  display.print(F("MODE: "));

  if (selectedMode == INSTANT_AUTO) {
    display.println(F("INSTANT"));
  }

  else if (selectedMode == DELAYED_AUTO) {
    display.println(F("DELAYED (5s)"));
  }

  else if (selectedMode == BLUETOOTH) {
    display.println(F("BT"));
  }


  // LINE
  display.setCursor(0, 50);

  display.print(F("LINE: "));

  if (selectedLine == WHITE_LINE) {
    display.println(F("WHITE"));
  }

  else if (selectedLine == BLACK_LINE) {
    display.println(F("BLACK"));
  }

  display.display();
}


// =================================================
// ================= SETTINGS MENU =================
// =================================================
void showSettingsMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // ================= HEADER =================

  display.setCursor(0, 0);
  display.println(F("SETTINGS:"));


  // Separator
  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);


  // ================= MENU =================

  for (int i = 0; i < 4; i++) {

    int item = settingsScroll + i;

    if (item > 4) {
      break;
    }


    display.setCursor(0, 14 + (i * 12));


    // Selection arrow
    if (menuSelection == item) {
      display.print(F("> "));
    }

    else {
      display.print(F("  "));
    }


    // Item text
    switch (item) {

      case 0:
        display.println(F("MODE"));
        break;

      case 1:
        display.println(F("LINE TYPE"));
        break;

      case 2:
        display.println(F("AUTO SPEED ADJUST"));
        break;

      case 3:
        display.println(F("ROBOT TEST"));
        break;

      case 4:
        display.println(F("BACK"));
        break;
    }
  }


  display.display();
}

// =================================================
// ================= MODE MENU =====================
// =================================================

void showModeMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // HEADER
  display.setCursor(0, 0);
  display.println(F("SELECT MODE:"));


  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);


  // INSTANT
  display.setCursor(0, 15);

  if (menuSelection == 0) {
    display.println(F("> INSTANT AUTO"));
  }

  else {
    display.println(F("  INSTANT AUTO"));
  }


  // DELAYED
  display.setCursor(0, 27);

  if (menuSelection == 1) {
    display.println(F("> DELAYED AUTO (5s)"));
  }

  else {
    display.println(F("  DELAYED AUTO (5s)"));
  }


  // BLUETOOTH
  display.setCursor(0, 39);

  if (menuSelection == 2) {
    display.println(F("> BLUETOOTH"));
  }

  else {
    display.println(F("  BLUETOOTH"));
  }


  // BACK
  display.setCursor(0, 51);

  if (menuSelection == 3) {
    display.println(F("> BACK"));
  }

  else {
    display.println(F("  BACK"));
  }

  display.display();
}


// =================================================
// ================= LINE MENU =====================
// =================================================

void showLineMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // HEADER
  display.setCursor(0, 0);
  display.println(F("SELECT LINE TYPE:"));


  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);


  // WHITE
  display.setCursor(0, 15);

  if (menuSelection == 0) {
    display.println(F("> WHITE LINE"));
  }

  else {
    display.println(F("  WHITE LINE"));
  }


  // BLACK
  display.setCursor(0, 30);

  if (menuSelection == 1) {
    display.println(F("> BLACK LINE"));
  }

  else {
    display.println(F("  BLACK LINE"));
  }


  // BACK
  display.setCursor(0, 45);

  if (menuSelection == 2) {
    display.println(F("> BACK"));
  }

  else {
    display.println(F("  BACK"));
  }

  display.display();
}


// =================================================
// ================= TEST MENU =====================
// =================================================

void showTestMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // HEADER
  display.setCursor(0, 0);
  display.println(F("SELECT TEST:"));


  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);


  // SENSORS
  display.setCursor(0, 15);

  if (menuSelection == 0) {
    display.println(F("> SENSOR TEST"));
  }

  else {
    display.println(F("  SENSOR TEST"));
  }


  // MOTORS
  display.setCursor(0, 29);

  if (menuSelection == 1) {
    display.println(F("> MOTOR TEST"));
  }

  else {
    display.println(F("  MOTOR TEST"));
  }


  // BACK
  display.setCursor(0, 43);

  if (menuSelection == 2) {
    display.println(F("> BACK"));
  }

  else {
    display.println(F("  BACK"));
  }

  display.display();
}


// =================================================
// ================= SAVED =========================
// =================================================

void showSaved(const __FlashStringHelper* text) {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 10);
  display.println(F("SAVED:"));

  display.setTextSize(2);

  display.setCursor(0, 30);
  display.println(text);

  display.display();

  delay(700);


  currentMenu = MENU_SETTINGS;
  menuSelection = 0;

  showSettingsMenu();
}


// =================================================
// ================= SENSOR TEST ===================
// =================================================

void sensorTest() {

  // Motors must remain stopped
  stopMove();


  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println(F("SENSOR TEST"));

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);

  display.setCursor(0, 51);
  display.println(F("PRESS BUTTON TO EXIT"));

  display.display();


  delay(300);


  while (true) {


    // ================= EXIT =================

    if (digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_START) == LOW) {

      stopMove();


      while (digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_START) == LOW) {
      }

      delay(50);


      currentTest = TEST_MENU;

      currentMenu = MENU_TEST;
      menuSelection = 0;

      showTestMenu();

      return;
    }


    // ================= ULTRASONIC =================

    frontDistance = sonarFront.ping_cm();

    delay(5);

    leftDistance = sonarLeft.ping_cm();

    delay(5);

    rightDistance = sonarRight.ping_cm();


    // ================= IR =================

    int irLeft = digitalRead(IR_LEFT);
    int irRight = digitalRead(IR_RIGHT);

    // 0 is WHITE LINE
    // 1 is BLACK LINE

    // ================= SERIAL =================

    Serial.print(F("F: "));
    Serial.print(frontDistance);

    Serial.print(F("  L: "));
    Serial.print(leftDistance);

    Serial.print(F("  R: "));
    Serial.print(rightDistance);

    Serial.print(F("  IR L: "));
    Serial.print(irLeft);

    Serial.print(F("  IR R: "));
    Serial.println(irRight);


    // ================= OLED =================

    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println(F("SENSOR TEST"));

    display.drawLine(
      0,
      10,
      127,
      10,
      SSD1306_WHITE);


    // FRONT
    display.setCursor(0, 15);
    display.print(F("F: "));

    if (frontDistance == 0) {
      display.println(F("---"));
    }

    else {
      display.print(frontDistance);
      display.println(F(" cm"));
    }


    // LEFT
    display.setCursor(0, 27);
    display.print(F("L: "));

    if (leftDistance == 0) {
      display.println(F("---"));
    }

    else {
      display.print(leftDistance);
      display.println(F(" cm"));
    }


    // RIGHT
    display.setCursor(0, 39);
    display.print(F("R: "));

    if (rightDistance == 0) {
      display.println(F("---"));
    }

    else {
      display.print(rightDistance);
      display.println(F(" cm"));
    }


    // IR
    display.setCursor(0, 51);

    display.print(F("IR L: "));


    if (irLeft == 0) {
      display.print("WHT");
    } else {
      display.print("BLK");
    }

    display.print(F(" R: "));

    if (irRight == 0) {
      display.print("WHT");
    } else {
      display.print("BLK ");
    }

    display.display();


    delay(50);
  }
}


// =================================================
// ================= MOTOR TEST ====================
// =================================================

void motorTest() {

  // Always stop when entering motor test
  stopMove();

  menuSelection = 0;

  showMotorTestMenu();


  while (true) {

    // ================= SELECT =================

    if (buttonPressed(BUTTON_SELECT)) {

      menuSelection++;

      // 0-5
      if (menuSelection > 5) {
        menuSelection = 0;
      }

      showMotorTestMenu();
    }


    // ================= START =================

    if (buttonPressed(BUTTON_START)) {

      // FORWARD
      if (menuSelection == 0) {

        pushForward(autoAttackSpeed);
      }


      // BACKWARD
      else if (menuSelection == 1) {

        moveBackward(autoAttackSpeed);
      }


      // LEFT
      else if (menuSelection == 2) {

        turnLeft(autoAttackSpeed);
      }


      // RIGHT
      else if (menuSelection == 3) {

        turnRight(autoAttackSpeed);
      }


      // STOP
      else if (menuSelection == 4) {

        stopMove();
      }


      // BACK
      else if (menuSelection == 5) {

        // SAFETY FIRST
        stopMove();

        delay(100);

        currentTest = TEST_MENU;
        currentMenu = MENU_TEST;
        menuSelection = 0;

        showTestMenu();

        return;
      }


      showMotorTestMenu();
    }
  }
}


// =================================================
// ================= MOTOR TEST MENU ===============
// =================================================

void showMotorTestMenu() {

  display.clearDisplay();

  display.setTextSize(1);


  // HEADER
  display.setCursor(0, 0);
  display.println(F("MOTOR TEST:"));


  // SEPARATOR
  display.drawLine(
    0,
    8,
    127,
    8,
    SSD1306_WHITE);


  // FORWARD
  display.setCursor(0, 15);

  if (menuSelection == 0)
    display.println(F("> FORWARD"));
  else
    display.println(F("  FORWARD"));


  // BACKWARD
  display.setCursor(0, 23);

  if (menuSelection == 1)
    display.println(F("> BACKWARD"));
  else
    display.println(F("  BACKWARD"));


  // LEFT
  display.setCursor(0, 31);

  if (menuSelection == 2)
    display.println(F("> LEFT"));
  else
    display.println(F("  LEFT"));


  // RIGHT
  display.setCursor(0, 39);

  if (menuSelection == 3)
    display.println(F("> RIGHT"));
  else
    display.println(F("  RIGHT"));


  // STOP
  display.setCursor(0, 47);

  if (menuSelection == 4)
    display.println(F("> STOP"));
  else
    display.println(F("  STOP"));


  // BACK
  display.setCursor(0, 55);

  if (menuSelection == 5)
    display.println(F("> BACK"));
  else
    display.println(F("  BACK"));


  display.display();
}


// =================================================
// ================= AUTONOMOUS MODE ===============
// =================================================

void autonomousMode() {


  // ================= LINE DETECTION =================

  if (lineDetected()) {

    stopMove();

    moveBackward(autoAttackSpeed);

    delay(300);


    bool left = digitalRead(IR_LEFT);
    bool right = digitalRead(IR_RIGHT);

    // MAKE A TURN BASED ON THE SELECTED LINE MODE
    // WHITE LINE
    if (selectedLine == WHITE_LINE) {

      if (right == LOW && left != LOW) {

        turnLeft(autoAttackSpeed);

        delay(220);
      }

      else if (left == LOW && right != LOW) {

        turnRight(autoAttackSpeed);

        delay(220);
      }

      else {

        turnLeft(autoAttackSpeed);

        delay(220);
      }
    }


    // BLACK LINE
    else {

      if (right == HIGH && left != HIGH) {

        turnLeft(autoAttackSpeed);

        delay(220);
      }

      else if (left == HIGH && right != HIGH) {

        turnRight(autoAttackSpeed);

        delay(220);
      }

      else {

        turnLeft(autoAttackSpeed);

        delay(220);
      }
    }

    return;
  }


  // ================= ULTRASONIC =================

  frontDistance = sonarFront.ping_cm();

  delay(5);

  leftDistance = sonarLeft.ping_cm();

  delay(5);

  rightDistance = sonarRight.ping_cm();


  // ================= SERIAL =================

  Serial.print(F("F:"));
  Serial.print(frontDistance);

  Serial.print(F(" L:"));
  Serial.print(leftDistance);

  Serial.print(F(" R:"));
  Serial.println(rightDistance);


  // ================= FRONT =================

  if (frontDistance > 0 && frontDistance <= ATTACK_DISTANCE) {

    pushForward(autoAttackSpeed);

    return;
  }


  // ================= LEFT =================

  else if (leftDistance > 0 && leftDistance <= SIDE_ATTACK_DISTANCE) {

    turnLeft(autoAttackSpeed);

    return;
  }


  // ================= RIGHT =================

  else if (rightDistance > 0 && rightDistance <= SIDE_ATTACK_DISTANCE) {

    turnRight(autoAttackSpeed);

    return;
  }


  // ================= SEARCH =================

  else {

    search();
  }
}


// =================================================
// ================= LINE DETECTION =================
// =================================================

bool lineDetected() {

  int left = digitalRead(IR_LEFT);
  int right = digitalRead(IR_RIGHT);


  // WHITE
  if (selectedLine == WHITE_LINE) {

    return (
      left == LOW || right == LOW);
  }


  // BLACK
  else {

    return (
      left == HIGH || right == HIGH);
  }
}


// =================================================
// ================= SEARCH =========================
// =================================================

void search() {

  pushForward(autoSearchSpeed);
}


// =================================================
// ================= BLUETOOTH =====================
// =================================================

void bluetoothMode() {

  if (Serial.available()) {

    char command = Serial.read();


    switch (command) {


      // FORWARD
      case 'F':

        pushForward(btMotorSpeed);
        currentCommand = "FORWARD";


        break;


      // BACKWARD
      case 'B':

        moveBackward(btMotorSpeed);
        currentCommand = "BACKWARD";


        break;


      // LEFT
      case 'L':

        turnLeft(btMotorSpeed);
        currentCommand = "LEFT";

        break;


      // RIGHT
      case 'R':

        turnRight(btMotorSpeed);
        currentCommand = "RIGHT";

        break;

        // Forward Left
      case 'G':
        forwardLeft(btMotorSpeed);
        currentCommand = "FORWARD LEFT";

        break;

        // Forward Right
      case 'H':
        forwardRight(btMotorSpeed);
        currentCommand = "FORWARD RIGHT";

        break;

        // Backward Left
      case 'I':
        backwardLeft(btMotorSpeed);
        currentCommand = "BACKWARD LEFT";

        break;

        // Backward Right
      case 'J':
        backwardRight(btMotorSpeed);
        currentCommand = "BACKWARD RIGHT";

        break;


      // STOP
      case 'S':

        stopMove();
        currentCommand = "STOP";


        break;

        // Set Motor Speed using the app controller
      case '0': setSpeed(0); break;
      case '1': setSpeed(25); break;
      case '2': setSpeed(50); break;
      case '3': setSpeed(75); break;
      case '4': setSpeed(100); break;
      case '5': setSpeed(125); break;
      case '6': setSpeed(150); break;
      case '7': setSpeed(175); break;
      case '8': setSpeed(200); break;
      case '9': setSpeed(255); break;


      // UNKNOWN
      default:

        stopMove();

        break;
    }

    startRobot();
  }
}

void setSpeed(int val) {
  btMotorSpeed = val;
}


// =================================================
// ================= BUTTON HANDLING ===============
// =================================================

bool buttonPressed(byte button) {

  if (digitalRead(button) == LOW) {

    delay(50);


    if (digitalRead(button) == LOW) {


      while (digitalRead(button) == LOW) {
      }

      delay(50);

      return true;
    }
  }

  return false;
}


// =================================================
// ================= MOTOR FUNCTIONS ===============
// =================================================

// STOP
void stopMove() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// FORWARD
void pushForward(int speed) {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


// BACKWARD
void moveBackward(int speed) {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


// LEFT
void turnLeft(int speed) {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


// RIGHT
void turnRight(int speed) {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


// FORWARD-RIGHT
void forwardRight(int speed) {
  analogWrite(ENA, speed * 0.5);
  analogWrite(ENB, speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// FORWARD-LEFT
void forwardLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed * 0.5);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// BACKWARD-LEFT
void backwardLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed * 0.5);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


// BACKWARD-RIGHT
void backwardRight(int speed) {
  analogWrite(ENA, speed * 0.5);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
