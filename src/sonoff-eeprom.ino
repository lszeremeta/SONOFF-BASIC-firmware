/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-eeprom.h"

SonoffEEPROM::SonoffEEPROM() {
  EEPROM.begin(EEPROM_size);

  if (read(0, 8)[0] == '\0' || ( read(140, 2)[0] != 'e' &&  read(140, 2)[0] != 'p'))  {
    erase();
    saveMode(1);
  }
}


SONOFFCONFIG SonoffEEPROM::getConfiguration() {
  SONOFFCONFIG _temp;

  read(0, 8).toCharArray(_temp.version, sizeof(_temp.version));
  _temp.mode = read(104, 1).toInt();

  // If there is no version in EPPROM this a first launch
  if (_temp.version[0] == '\0')  {
    erase();
    _temp.mode = MODE_ACCESSPOINT;
  }

  read(119, 6).toCharArray(_temp.id, sizeof(_temp.id));
  read(140, 2).toCharArray(_temp.language, sizeof(_temp.language));
  read(142, 32).toCharArray(_temp.device_name, sizeof(_temp.device_name));
  read(232, 32).toCharArray(_temp.wifi_ssid, sizeof(_temp.wifi_ssid));
  read(264, 32).toCharArray(_temp.wifi_password, sizeof(_temp.wifi_password));
  
  read(296, 32).toCharArray(_temp.mqtt_host, sizeof(_temp.mqtt_host));
  _temp.mqtt_port = read(328, 5).toInt();
  read(333, 32).toCharArray(_temp.mqtt_user, sizeof(_temp.mqtt_user));
  read(365, 32).toCharArray(_temp.mqtt_password, sizeof(_temp.mqtt_password));
  read(397, 32).toCharArray(_temp.mqtt_topic, sizeof(_temp.mqtt_topic));

  _temp.ds18b20_present = (read(138, 1).toInt() == 1 ? true : false);  
  _temp.ds18b20_correction = read(105, 5).toFloat();  
  _temp.ds18b20_interval = read(110, 8).toInt();

  _temp.switch_present = (read(126, 1).toInt() == 1 ? true : false);  
  _temp.switch_sensitiveness = read(129, 1).toInt(); 
  _temp.switch_gpio = read(127, 2).toInt();

  _temp.debugger = (read(130, 1).toInt() == 1 ? true : false); 

  return _temp;
}

boolean SonoffEEPROM::debuggable() {
  return (read(130, 1).toInt() == 1 ? true : false);
}

boolean SonoffEEPROM::isDS18B20Present() {
  return (read(138, 1).toInt() == 1 ? true : false);
}

float SonoffEEPROM::DS18B20Correction() {
  return read(105, 5).toFloat();
}

unsigned int SonoffEEPROM::DS18B20ReadInterval() {
  return read(110, 8).toInt();
}

uint8_t SonoffEEPROM::getRelayState() {
  return read(118, 1).toInt();
}

uint8_t SonoffEEPROM::getRelayStateAfterPowerRestored() {
  return read(139, 1).toInt();
}

uint8_t SonoffEEPROM::getRelayStateAfterConnectionRestored() {
  return read(125, 1).toInt();
}


boolean SonoffEEPROM::isSwitchPresent() {
  return (read(126, 1).toInt() == 1 ? true : false);
}

uint8_t SonoffEEPROM::getSwitchGPIO() {
  return read(127, 2).toInt();
}

uint8_t SonoffEEPROM::getSwitchSensitiveness() {
  return read(129, 1).toInt();
}


void SonoffEEPROM::saveDeviceName(String in) {
  write(142, 32, in);
}

void SonoffEEPROM::saveVersion(String in) {
  write(0, 8, in);
}

void SonoffEEPROM::saveLanguage(String in) {
  write(140, 2, in);
}

void SonoffEEPROM::saveMode(uint8_t in) {
  write(104, 1, String(in));
}

void SonoffEEPROM::saveTemperatureCorrection(float in) {
  write(105, 5, String(in));
}

void SonoffEEPROM::saveTemperatureInterval(unsigned int in) {
  write(110, 8, String(in));
}

void SonoffEEPROM::saveTemperatureSensorPresent(uint8_t in) {
  write(138, 1, String(in));
}

void SonoffEEPROM::saveRelayState(uint8_t in) {
  write(118, 1, String(in));
}

void SonoffEEPROM::saveRelayStateAfterPowerRestored(uint8_t in) {
  write(139, 1, String(in));
}

void SonoffEEPROM::saveRelayStateAfterConnectionRestored(uint8_t in) {
  write(125, 1, String(in));
}

void SonoffEEPROM::saveWiFiSSID(String in) {
  write(232, 32, in);
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  write(264, 32, in);
}

void SonoffEEPROM::saveMQTTHost(String in) {
  write(296, 32, in);
}

void SonoffEEPROM::saveMQTTPort(unsigned int in) {
  write(328, 5, String(in));
}

void SonoffEEPROM::saveMQTTUser(String in) {
  write(333, 32, in);
}

void SonoffEEPROM::saveMQTTPassword(String in) {
  write(365, 32, in);
}

void SonoffEEPROM::saveMQTTTopic(String in) {
  write(397, 32, in);
}

void SonoffEEPROM::saveSwitchPresent(uint8_t in) {
  write(126, 1, String(in));
}

void SonoffEEPROM::saveSwitchGPIO(uint8_t in) {
  write(127, 2, String(in));
}

void SonoffEEPROM::saveSwitchSensitiveness(uint8_t in) {
  write(129, 1, String(in));
}

void SonoffEEPROM::saveDebuggable(byte in) {
  write(130, 1, String(in));
}

void SonoffEEPROM::erase() {
  if (Configuration.debugger) Serial << "Erasing EEPROM" << endl;
  clear(0, EEPROM_size);
  setDefaults();
}

void SonoffEEPROM::setDefaults() {

  if (Configuration.debugger) Serial << "Setting default values" << endl;

  char _id[6] = {0};
  char _device_name[13] = {0};
  char _mqtt_topic[32] = {0};

  sprintf(_id, "%06X", ESP.getChipId());
  sprintf(_device_name, "SONOFF_%s", _id);
  sprintf(_mqtt_topic, "/sonoff/%s/", _id);

  write(119, 6, _id);
  write(142, 32, _device_name);

  saveVersion(sonoffDefault.version);

  saveTemperatureSensorPresent(sonoffDefault.temp_present);
  saveTemperatureCorrection(sonoffDefault.temp_correction);
  saveTemperatureInterval(sonoffDefault.temp_interval);

  saveMQTTTopic(_mqtt_topic);
  saveMQTTPort(sonoffDefault.mqtt_port);

  saveMode(MODE_SWITCH);
  saveRelayState(0);
  saveRelayStateAfterPowerRestored(sonoffDefault.relay_state_after_power_restored);
  saveRelayStateAfterConnectionRestored(sonoffDefault.relay_state_after_connection_restored);
  saveLanguage(sonoffDefault.language);

  saveSwitchPresent(sonoffDefault.switch_present);
  saveSwitchGPIO(sonoffDefault.switch_gpio);
  saveSwitchSensitiveness(sonoffDefault.switch_sensitiveness); 

  saveDebuggable(0); 
  
}


void SonoffEEPROM::write(unsigned int address, unsigned int size, String in) {
  clear(address, size);
  for (int i = 0; i < in.length(); ++i)
  {
    EEPROM.write(address + i, in[i]);
  }
  EEPROM.commit();
}

String SonoffEEPROM::read(unsigned int address, unsigned int size) {
  String _return;
  for (int i = address; i < address + size; ++i)
  {
    if (EEPROM.read(i) != 255) {
      _return += char(EEPROM.read(i));
    }
  }
  return _return;
}

void SonoffEEPROM::clear(unsigned int address, unsigned int size) {
  for (int i = 0; i < size; ++i) {
    EEPROM.write(i + address, 255);
  }
  EEPROM.commit();
}


