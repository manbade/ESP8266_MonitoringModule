#include "JsonConfig.h"

bool JsonConfig::printConfig()
{
    Serial.println("\r\nConfig: printing");

    Serial.print(F("module_id     : "));   Serial.println(module_id);
    Serial.print(F("module_name   : "));   Serial.println(module_name);
    Serial.print(F("sta_ssid      : "));   Serial.println(sta_ssid);
    Serial.print(F("sta_pwd       : "));   Serial.println(sta_pwd);

    Serial.print(F("static_ip_mode: "));   Serial.println(static_ip_mode);
    Serial.print(F("static_ip     : "));   Serial.println(static_ip);
    Serial.print(F("static_gateway: "));   Serial.println(static_gateway);
    Serial.print(F("static_subnet : "));   Serial.println(static_subnet);

    Serial.print(F("get_data_delay: "));   Serial.println(get_data_delay);
    Serial.print(F("reboot_delay  : "));   Serial.println(reboot_delay);

    Serial.print(F("ts_toogle  : "));   Serial.println(ts_toogle);
    Serial.print(F("narodmon_toogle  : "));   Serial.println(narodmon_toogle);
    Serial.print(F("thing_speak_api_key  : "));   Serial.println(thing_speak_api_key);

    Serial.print(F("sensor_bmp180_on  : "));   Serial.println(sensor_bmp180_on);
    Serial.print(F("sensor_dht22_on   : "));   Serial.println(sensor_dht22_on);
    Serial.print(F("sensor_ds18b20_on  : "));   Serial.println(sensor_ds18b20_on);
    Serial.print(F("sensor_analog_on  : "));   Serial.println(sensor_analog_on);
    

    Serial.println("\r\nConfig: printed");

    return true;
}

bool JsonConfig::loadConfig()
{
    Serial.println("\r\nConfig: loading");

    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println("Config: failed to open config file for reading");
        return false;
    }

    size_t size = configFile.size();
    if (size > 2048)
    {
        Serial.println("Config: file size is too large");
        SPIFFS.remove("/config.json");
        saveConfig();
        return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);

    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());

    if (!json.success())
    {
        Serial.println("Failed to parse config file");
        SPIFFS.remove("/config.json");
        saveConfig();
        return false;
    }

    if (json.containsKey("module_id")) { const char* module_id_char = json["module_id"]; sprintf_P(module_id, ("%s"), module_id_char); }
    if (json.containsKey("module_name")) { const char* module_name_char = json["module_name"]; sprintf_P(module_name, ("%s"), module_name_char); }
    if (json.containsKey("module_pwd")) { const char* module_pwd_char = json["module_pwd"]; sprintf_P(module_pwd, ("%s"), module_pwd_char); }
    if (json.containsKey("sta_ssid")) { const char* sta_ssid_char = json["sta_ssid"]; sprintf_P(sta_ssid, ("%s"), sta_ssid_char); }
    if (json.containsKey("sta_pwd")) { const char* sta_pwd_char = json["sta_pwd"]; sprintf_P(sta_pwd, ("%s"), sta_pwd_char); }

    if (json.containsKey("static_ip_mode")) { const char* static_ip_mode_char = json["static_ip_mode"]; sprintf_P(static_ip_mode, ("%s"), static_ip_mode_char); }
    if (json.containsKey("static_ip")) { const char* static_ip_char = json["static_ip"]; sprintf_P(static_ip, ("%s"), static_ip_char); }
    if (json.containsKey("static_gateway")) { const char* static_gateway_char = json["static_gateway"]; sprintf_P(static_gateway, ("%s"), static_gateway_char); }
    if (json.containsKey("static_subnet")) { const char* static_subnet_char = json["static_subnet"]; sprintf_P(static_subnet, ("%s"), static_subnet_char); }

    if (json.containsKey("get_data_delay")) { const char* get_data_delay_char = json["get_data_delay"]; sprintf_P(get_data_delay, ("%s"), get_data_delay_char); }
    if (json.containsKey("reboot_delay")) { const char* reboot_delay_char = json["reboot_delay"]; sprintf_P(reboot_delay, ("%s"), reboot_delay_char); }

    if (json.containsKey("narodmon_toogle")) { const char* narodmon_toogle_char = json["narodmon_toogle"]; sprintf_P(narodmon_toogle, ("%s"), narodmon_toogle_char); }
    if (json.containsKey("ts_toogle")) { const char* ts_toogle_char = json["ts_toogle"]; sprintf_P(ts_toogle, ("%s"), ts_toogle_char); }
    if (json.containsKey("thing_speak_api_key")) { const char* thing_speak_api_key_char = json["thing_speak_api_key"]; sprintf_P(thing_speak_api_key, ("%s"), thing_speak_api_key_char); }

    if (json.containsKey("sensor_bmp180_on")) { const char* sensor_bmp180_on_char = json["sensor_bmp180_on"]; sprintf_P(sensor_bmp180_on, ("%s"), sensor_bmp180_on_char); }
    if (json.containsKey("sensor_dht22_on")) { const char* sensor_dht22_on_char = json["sensor_dht22_on"]; sprintf_P(sensor_dht22_on, ("%s"), sensor_dht22_on_char); }
    if (json.containsKey("sensor_ds18b20_on")) { const char* sensor_ds18b20_on_char = json["sensor_ds18b20_on"]; sprintf_P(sensor_ds18b20_on, ("%s"), sensor_ds18b20_on_char); }
    if (json.containsKey("sensor_analog_on")) { const char* sensor_analog_on_char = json["sensor_analog_on"]; sprintf_P(sensor_analog_on, ("%s"), sensor_analog_on_char); }


    configFile.close();

    Serial.println("Config: loaded");

    return true;
}

bool JsonConfig::saveConfig()
{
    Serial.println("\r\nConfig: saving");

    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
        Serial.println("Config: failed to open config file for writing");
        return false;
    }

    json["module_id"] = module_id;
    json["module_name"] = module_name;
    json["module_pwd"] = module_pwd;
    json["sta_ssid"] = sta_ssid;
    json["sta_pwd"] = sta_pwd;

    json["static_ip_mode"] = static_ip_mode;
    json["static_ip"] = static_ip;
    json["static_gateway"] = static_gateway;
    json["static_subnet"] = static_subnet;

    json["get_data_delay"] = get_data_delay;
    json["reboot_delay"] = reboot_delay;

    json["narodmon_toogle"] = narodmon_toogle;
    json["ts_toogle"] = ts_toogle;
    json["thing_speak_api_key"] = thing_speak_api_key;

    json["sensor_bmp180_on"] = sensor_bmp180_on;
    json["sensor_dht22_on"] = sensor_dht22_on;
    json["sensor_ds18b20_on"] = sensor_ds18b20_on;
    json["sensor_analog_on"] = sensor_analog_on;

    json.printTo(configFile);
    configFile.close();

    Serial.println("Config: saved");

    return true;
}

