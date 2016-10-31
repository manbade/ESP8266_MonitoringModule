#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"

class SensorData
{
public:
    float temp;
    String tempStr;

    float humidity;
    String humidityStr;

    float pressure;
    String pressureStr;

    int adc;
    String adcStr;
};

class WiFiData
{
public:
    String ssid;
    long rssi;
    byte encryptionType;
    bool isSet;
};

class JsonConfig
{
public:
    bool saveConfig();
    bool loadConfig();
    bool printConfig();

    char module_id[32] = "1";
    char module_name[32] = "MonitoringSystem";
    char module_pwd[32] = "somepass123";
    char sta_ssid[32] = "ASUS3";
    char sta_pwd[32] = "Zeus1106";
    char hidden_toogle[32] = "0";
    char deepsleep_toogle[32] = "0";
    
    char static_ip_toogle[32] = "0";
    char static_ip[32] = "192.168.1.55";
    char static_gateway[32] = "192.168.1.1";
    char static_subnet[32] = "255.255.255.0";

    char get_data_delay[32] = "300";
    char reboot_delay[32] = "3";

    char sensor_bmp180_on[32] = "0";
    char sensor_dht_on[32] = "0";
    char sensor_ds18b20_on[32] = "0";
    char sensor_analog_on[32] = "0";

    char narodmon_toogle[32] = "0";
    char ts_toogle[32] = "0";
    char thing_speak_api_key[32] = "6NSWJ0EQD330HG4F";

//    char deepsleep_timer[32] = "0";

private:
};

#endif
