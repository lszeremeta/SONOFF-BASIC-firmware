/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-03-18 tschaban https://github.com/tschaban
*/


#include "sonoff-switch.h"


SonoffSwitch::SonoffSwitch() {
}

void SonoffSwitch::init(uint8_t gpio) {
  _gpio = gpio;
  if (_gpio != GPIO_14) { /* Turn off Serial */
    Serial.swap();
  }
  pinMode(_gpio, INPUT_PULLUP);
  state = !digitalRead(_gpio);
  switchTimer.attach(0.05, callbackSwitch);
}

boolean SonoffSwitch::stateChange() {
  boolean _ret = false;
  if (state != !digitalRead(_gpio) and !delay()) {
    counter++;
  } else if (state != !digitalRead(_gpio) and delay()) {
    counter = 0;
    state = !state;
    _ret = true;
  } else {
    counter = 0;
  }
  return _ret;
}

boolean SonoffSwitch::delay() {
  return counter == (Configuration.switch_sensitiveness == SWITCH_SENSITIVENESS_NORMAL ? SWITCH_SENSITIVENESS_NORMAL_VALUE : Configuration.switch_sensitiveness == SWITCH_SENSITIVENESS_LOW ? SWITCH_SENSITIVENESS_LOW_VALUE : SWITCH_SENSITIVENESS_HIGH_VALUE)  ? true : false;
}



void callbackSwitch() {
  if (Switch.stateChange()) {
    Relay.toggle();
  }
}

