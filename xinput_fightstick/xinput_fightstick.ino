/*!
 * @file xinput_fighter.ino
 * @brief Arduino sketch for Micro/Leonardo fight stick using XInput
 * 
 * Uses standard button mapping based off of Hori
 * Fighting Stick Mini 4.  Allows switching between
 * using the joystick as DPAD/Left Stick/Right Stick.
 * 
 * @author byt3swap
 * @date   10/22/2020
 */

#include <XInput.h>

// different modes for joystick
#define MODE_DPAD 0
#define MODE_LEFT_STICK 1
#define MODE_RIGHT_STICK 2

// time to hold until mode switch is enabled
#define MODE_SWITCH_INIT_MS 3000

// button pin defs
const int pin_BtnA = 2;
const int pin_BtnB = 3;
const int pin_BtnX = 4;
const int pin_BtnY = 5;
const int pin_BtnRT = 6;
const int pin_BtnLT = 7;
const int pin_BtnRB = 8;
const int pin_BtnLB = 9;
const int pin_BtnBack = A0;
const int pin_BtnStart = A1;

// joystick pin defs
const int pin_joyUp = 10;
const int pin_joyDown = 16;
const int pin_joyRight = 14;
const int pin_joyLeft = 15;

// extra mode switch items
unsigned long modeSwitchTimerMS;
bool modeSwitchInit = false;

// button value bools
bool btnA, btnB, btnX, btnY, btnRT, btnLT, btnRB, btnLB, btnBack, btnStart;
bool joyUp, joyDown, joyRight, joyLeft;

// map the mode switch activation buttons
bool *modeSwitch0 = &btnLB;
bool *modeSwitch1 = &btnLT;

// default mode is dpad
int currentMode = MODE_DPAD;

/**
 * @brief Arduino setup call
 * 
 * Just configures the pins and basic XInput settings.
 * Sets all potential button pins as inputs with an
 * internal pullup.  Default state for all non pressed
 * buttons is high.  2nd terminal on all wired buttons
 * and joystick should be tied to common ground.
 * 
 * Keeps joystick range at 1 since in all modes, there
 * is no notion of analog input.
 */
void setup()
{
  pinMode(pin_BtnA, INPUT_PULLUP);
  pinMode(pin_BtnB, INPUT_PULLUP);
  pinMode(pin_BtnX, INPUT_PULLUP);
  pinMode(pin_BtnY, INPUT_PULLUP);
  pinMode(pin_BtnRT, INPUT_PULLUP);
  pinMode(pin_BtnLT, INPUT_PULLUP);
  pinMode(pin_BtnRB, INPUT_PULLUP);
  pinMode(pin_BtnLB, INPUT_PULLUP);  
  pinMode(pin_BtnBack, INPUT_PULLUP);
  pinMode(pin_BtnStart, INPUT_PULLUP);

  pinMode(pin_joyUp, INPUT_PULLUP);
  pinMode(pin_joyDown, INPUT_PULLUP);
  pinMode(pin_joyRight, INPUT_PULLUP);
  pinMode(pin_joyLeft, INPUT_PULLUP);

  XInput.setJoystickRange(0,1);         // joystick is all or nothing
  XInput.setAutoSend(false);            // wait for all inputs before sending
}

/**
 * @brief Handle switching between joystick modes
 * 
 * No args, everythign is global in true arduino
 * fashion... keeps the stack low I guess...
 */
void modeSwitchHandler()
{
  if (*modeSwitch0 && *modeSwitch1)
  {
    if (!modeSwitchInit)
    {
      modeSwitchTimerMS = millis();
      modeSwitchInit = true;
    }
    else
    {
      // if we have held the buttons long enough
      if ((millis() - modeSwitchTimerMS) >= MODE_SWITCH_INIT_MS)
      {
        // loop until we receive a valid joystick input
        while(true)
        {
          if (!digitalRead(pin_joyDown))      // down sets mode to DPAD
          {
            currentMode = MODE_DPAD;
            break;
          }
          else if(!digitalRead(pin_joyLeft))  // left sets mode to Left Stick
          {
            currentMode = MODE_LEFT_STICK;
            break;
          }
          else if(!digitalRead(pin_joyRight)) // right sets mode to Right Stick
          {
            currentMode = MODE_RIGHT_STICK;
            break;
          }
        }
        // clear mode switch flag once new mode is set
        modeSwitchInit = false;
      }
    }
  }
  else
  {
    // clear mode switch flag if we stopped holding before timeout
    modeSwitchInit = false;
  }
}

/**
 * @brief Main Arduino program loop
 * 
 * Reads all of the current button / joystick states,
 * handles mode switching checks, and writes out the
 * XInput values to the host.
 */
void loop()
{
  // read the face buttons
  btnA = !digitalRead(pin_BtnA);
  btnB = !digitalRead(pin_BtnB);
  btnX = !digitalRead(pin_BtnX);
  btnY = !digitalRead(pin_BtnY);
  btnRT = !digitalRead(pin_BtnRT);
  btnLT = !digitalRead(pin_BtnLT);
  btnRB = !digitalRead(pin_BtnRB);
  btnLB = !digitalRead(pin_BtnLB);
  btnBack = !digitalRead(pin_BtnBack);
  btnStart = !digitalRead(pin_BtnStart);

  // read the joystick
  joyUp = !digitalRead(pin_joyUp);
  joyDown = !digitalRead(pin_joyDown);
  joyRight = !digitalRead(pin_joyRight);
  joyLeft = !digitalRead(pin_joyLeft);

  // set the face buttons
  XInput.setButton(BUTTON_A, btnA);
  XInput.setButton(BUTTON_B, btnB);
  XInput.setButton(BUTTON_X, btnX);
  XInput.setButton(BUTTON_Y, btnY);  
  XInput.setButton(TRIGGER_RIGHT, btnRT);
  XInput.setButton(TRIGGER_LEFT, btnLT);
  XInput.setButton(BUTTON_RB, btnRB);
  XInput.setButton(BUTTON_LB, btnLB);
  XInput.setButton(BUTTON_BACK, btnBack);
  XInput.setButton(BUTTON_START, btnStart);

  // keep stick clicks unpressed
  XInput.setButton(BUTTON_R3, false);
  XInput.setButton(BUTTON_L3, false);

  // handle mode switching
  modeSwitchHandler();

  // set the joystick by mode
  switch(currentMode)
  {
    case MODE_DPAD:
      // set joystick input as DPAD
      XInput.setDpad(joyUp, joyDown, joyLeft, joyRight);

      // keep the joysticks clear
      XInput.setJoystick(JOY_LEFT, false, false, false, false);
      XInput.setJoystick(JOY_RIGHT, false, false, false, false);
      break;
    case MODE_LEFT_STICK:
      // set joystick input as Left Stick
      XInput.setJoystick(JOY_LEFT, joyUp, joyDown, joyLeft, joyRight);
    
      // keep dpad + right stick clear
      XInput.setDpad(false, false, false, false);      
      XInput.setJoystick(JOY_RIGHT, false, false, false, false);
      break;
    case MODE_RIGHT_STICK:
      // set joystick input as Right Stick
      XInput.setJoystick(JOY_RIGHT, joyUp, joyDown, joyLeft, joyRight);
    
      // keep dpad + left stick clear
      XInput.setDpad(false, false, false, false);
      XInput.setJoystick(JOY_LEFT, false, false, false, false);
      break;
  }

  // send it off
  XInput.send();
}
