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
    char module_pwd[32] = "Apolon1106";
    char sta_ssid[32] = "MobileWiFi-b30f";
    char sta_pwd[32] = "421ed916";

    char static_ip_mode[32] = "0";
    char static_ip[32] = "192.168.1.55";
    char static_gateway[32] = "192.168.1.1";
    char static_subnet[32] = "255.255.255.0";

    char get_data_delay[32] = "10";
    char reboot_delay[32] = "3";

    char sensor_bmp180_on[32] = "0";
    char sensor_dht22_on[32] = "1";
    char sensor_ds18b20_on[32] = "0";
    char sensor_analog_on[32] = "0";

    char thing_speak_api_key[32] = "6NSWJ0EQD330HG4E";

private:
};

#endif
