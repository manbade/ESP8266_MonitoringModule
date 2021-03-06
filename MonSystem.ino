#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <DallasTemperature.h>

#include "JsonConfig.h"
#include "WebCommon.h"
#include "Common.h"

extern "C" {
#include <user_interface.h>
}

ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;

const int maxConnectAttempts = 20;

JsonConfig config;
#define MAX_WIFI_COUNT 50
WiFiData wiFiDatas[MAX_WIFI_COUNT];

#define DHTPIN 4
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);
SensorData data1;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0
#define TEMPERATURE_PRECISION 12 // resolution
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire); 
SensorData data2;
// arrays to hold device address
DeviceAddress insideThermometer;

SFE_BMP180 bmp180;
#define ALTITUDE 20 //meters
SensorData data3;

SensorData data4;

bool bmp180initialized = false;
bool dhtinitialized = false;
bool ds18b20initialized = false;

unsigned long previousMillis = 0;
bool isRebooting = false;
int currentSensorCycle = 0;
int currentRebootCycle = 0;
const int ONE_SECOND = 1000;

//declaration
void renderWiFiStatus(String status, int r, int g, int b);
void renderServerStatus(String status, int r, int g, int b);
void renderAPStatus(String status, int r, int g, int b);

const char* host = "api.thingspeak.com";

void handleTimerWhileRebooting()
{
    int periods = atoi(config.reboot_delay);
    if (currentRebootCycle == periods)
    {
        ESP.restart();
        return;
    }

    Serial.printf("Reboot ESP in %d sec.\r\n", (periods - currentRebootCycle));
}

void rebootESP()
{

    handleTimerWhileRebooting();

    //ignore all messages to display while rebooting
    isRebooting = true;
}

void webRoot()
{
    Serial.println("\r\nServer: request ROOT");
    
    String ssid_h_name, uptime_str, ds_alert;
    if (WiFi.status() == WL_CONNECTED)
    { 
      ssid_h_name = "<p>Ви підключені до мережі <b id='ssid_name'>" + String(config.sta_ssid) + "</b></p>";
    }
    else { ssid_h_name = ""; ds_alert = "";}
    if(atoi(config.deepsleep_toogle)) {
//      unsigned long uptime = atoi(config.deepsleep_timer);
//      uptime_str = renderParameterRow("Час роботи", "", getUptimeData(uptime), true);
      ds_alert = "<p>Активовано режим <b>DeepSleep</b></p>";
    }
    else {
//      uptime_str = renderParameterRow("Час роботи", "", getUptimeData(), true);
      ds_alert = "";
    }
    String data = 
        renderTitle(config.module_name, "Головна") + FPSTR(stylesInclude) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + renderMenu() +
        String(F("<h2>")) + config.module_name + String(F("</h2>")) +
        ssid_h_name +
        ds_alert +
        String(F("<div class='container'>")) +
        renderParameterRow("Назва системи", "", config.module_name, true) + 
        renderParameterRow("IP системи", "", getIpString(WiFi.localIP()), true) + 
        renderParameterRow("MAC системи", "", getMacString(), true) + 
        "<hr/>" +
        renderParameterRow("Час роботи", "", getUptimeData(), true) + 
        "<hr/>" +
        renderParameterRow("Темп. DHT, C", "", data1.tempStr, true) + 
        renderParameterRow("Вологість DHT, %", "", data1.humidityStr, true) + 
        renderParameterRow("Темп. DS18B20, C", "", data2.tempStr, true) + 
        renderParameterRow("Темп. BMP, C", "", data3.tempStr, true) + 
        renderParameterRow("Атм.тиск BMP, мм.рт.ст", "", data3.pressureStr, true) + 
        renderParameterRow("ADC", "", data4.adcStr, true) + 
        "<hr/>" +
        renderParameterRow("Вільно пам'яті, байт", "", getFreeMemory(), true) + 
        String(F("</div>")) +
        FPSTR(bodyEnd);

    WebServer.send(200, "text/html", data);    
    Serial.println("Server: request ROOT sent");
}

void cloudSetup()
{
    Serial.println("\r\nServer: request SETUP");

    bool config_changed = false;
    String payload = WebServer.arg("narodmon_toogle");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.narodmon_toogle, sizeof(config.narodmon_toogle));
        config_changed = true;
    }
    payload = WebServer.arg("ts_toogle");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.ts_toogle, sizeof(config.ts_toogle));
        config_changed = true;
    }
    payload = WebServer.arg("thing_speak_api_key");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.thing_speak_api_key, sizeof(config.thing_speak_api_key));
        config_changed = true;
    }
    payload = WebServer.arg("get_data_delay");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.get_data_delay, sizeof(config.get_data_delay));
        config_changed = true;
    }
    String ssid_h_name;
    if (WiFi.status() == WL_CONNECTED)
    { 
      ssid_h_name = "<p>Ви підключені до мережі <b id='ssid_name'>" + String(config.sta_ssid) + "</b></p>";
    }
    else { ssid_h_name = "";}
    String data = 
        renderTitle(config.module_name, "Eкспорт даних") + FPSTR(stylesInclude) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + renderMenu() +
        "<h2>Eкспорт даних в хмарні сховища</h2>" +
        ssid_h_name +
        "<div class='container'>" +
        renderParameterRow("NarodMon.ru", "narodmon_toogle", config.narodmon_toogle) +
        renderParameterRow("ThingSpeak.com", "ts_toogle", config.ts_toogle) +
        renderParameterRow("API ключ ThingSpeak", "thing_speak_api_key", config.thing_speak_api_key) +
        "<div class='input-group'><label class='input_label' for='get_data_delay'>Інтервал експорту даних</label><select id='get_data_delay' class='form-control'>" +
        delay_list() +
        "<select></div>" +
        "<hr/>" +
        "<span id='data_delay' style='display:none;'>" + String(config.get_data_delay) + "</span>" +
        "<a class='btn btn-default marginTop0' role='button' onclick='saveFormData(\"/cloudSetup\");'>Зберегти</a>" +
        "</div>" +
        FPSTR(bodyEnd);

    WebServer.send(200, "text/html", data);

    if (config_changed)
    {
        config.saveConfig();
    }

    Serial.println("Server: request SETUP sent");
}

void wifiSetup()
{
    Serial.println("\r\nServer: request SETUP");

    bool config_changed = false;
    String payload = WebServer.arg("module_name");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.module_name, sizeof(config.module_name));
        config_changed = true;
    }
    payload = WebServer.arg("module_pwd");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.module_pwd, sizeof(config.module_pwd));
        config_changed = true;
    }
    payload = WebServer.arg("sta_ssid");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sta_ssid, sizeof(config.sta_ssid));
        config_changed = true;
    }
    payload = WebServer.arg("sta_pwd");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sta_pwd, sizeof(config.sta_pwd));
        config_changed = true;
    }
    payload = WebServer.arg("hidden_toogle");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.hidden_toogle, sizeof(config.hidden_toogle));
        config_changed = true;
    }
    payload = WebServer.arg("deepsleep_toogle");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.deepsleep_toogle, sizeof(config.deepsleep_toogle));
        config_changed = true;
    }
    payload = WebServer.arg("static_ip_toogle");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_ip_toogle, sizeof(config.static_ip_toogle));
        config_changed = true;
    }
    payload = WebServer.arg("static_ip");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_ip, sizeof(config.static_ip));
        config_changed = true;
    }
    payload = WebServer.arg("static_gateway");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_gateway, sizeof(config.static_gateway));
        config_changed = true;
    }
    payload = WebServer.arg("static_subnet");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_subnet, sizeof(config.static_subnet));
        config_changed = true;
    }
    String ssid_list, ssid_h_name;
    int numSsid = WiFi.scanNetworks();
          // print the name for each network found:
          for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            String ssid_str = WiFi.SSID(thisNet) + " Signal: " + WiFi.RSSI(thisNet) + " dBm",
                   ssid_name =  WiFi.SSID(thisNet),
                   rssi = String(WiFi.RSSI(thisNet));
            ssid_list += renderParameterList(ssid_name, ssid_str, rssi); 
          }
    if (WiFi.status() == WL_CONNECTED)
    { 
      ssid_h_name = "<p>Ви підключені до мережі <b id='ssid_name'>" + String(config.sta_ssid) + "</b></p>";
    }
    else { ssid_h_name = "";}
    String data = 
        renderTitle(config.module_name, "Налаштування WiFi") + FPSTR(stylesInclude) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + renderMenu() +
        "<h2>Налаштування WiFi</h2>" +
        ssid_h_name +
        "<div class='container'>" +
        renderParameterRow("Назва системи", "module_name", config.module_name) + 
        renderParameterRow("Пароль доступу до системи", "module_pwd", config.module_pwd, false, true) + 
        renderParameterRow("Прихований SSID", "hidden_toogle", config.hidden_toogle) +
        renderParameterRow("Режим DeepSleep", "deepsleep_toogle", config.deepsleep_toogle) +
        "<hr/>" +
        "<div class='input-group'><label class='input_label' for='sta_ssid'>SSID</label><select id='sta_ssid' class='form-control'>" +
        ssid_list +
        "<select></div>" +
        renderParameterRow("Пароль", "sta_pwd", config.sta_pwd, false, true) +  
        "<p>Для збереження внесених змін, необхідно перезавантажити систему.</p><hr/>" +
        renderParameterRow("Режим статичного IP ", "static_ip_toogle", config.static_ip_toogle) + 
        renderParameterRow("Статичний IP", "static_ip", config.static_ip) + 
        renderParameterRow("Основний шлюз", "static_gateway", config.static_gateway) + 
        renderParameterRow("Маска мережі", "static_subnet", config.static_subnet) + 
        "<hr/>" +
        "<a class='btn btn-default marginTop0' role='button' onclick='saveFormData(\"/wifiSetup\");'>Зберегти</a>" +
        "</div>" +
        FPSTR(bodyEnd);

    WebServer.send(200, "text/html", data);

    if (config_changed)
    {
        config.saveConfig();
    }

    Serial.println("Server: request SETUP sent");
}


void webSensors()
{
    Serial.println("\r\nServer: request SENSORS");

    bool config_changed = false;
    String payload = WebServer.arg("sensor_bmp180_on");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sensor_bmp180_on, sizeof(config.sensor_bmp180_on));
        config_changed = true;
    }
    payload = WebServer.arg("sensor_dht_on");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sensor_dht_on, sizeof(config.sensor_dht_on));
        config_changed = true;
    }
    payload = WebServer.arg("sensor_ds18b20_on");
   if (payload.length() > 0)
   {
       payload.toCharArray(config.sensor_ds18b20_on, sizeof(config.sensor_ds18b20_on));
       config_changed = true;
   }
   payload = WebServer.arg("sensor_analog_on");
   if (payload.length() > 0)
   {
       payload.toCharArray(config.sensor_analog_on, sizeof(config.sensor_analog_on));
       config_changed = true;
   }

    String data = 
        renderTitle(config.module_name, "Давачі даних") + FPSTR(stylesInclude) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + renderMenu() +
        "<h2 style='display: block;'>Давачі даних</h2>" +
        "<div class='container'>" +
        renderParameterRow("BMP Val", "sensor_bmp180_on", config.sensor_bmp180_on) + 
        renderParameterRow("DHT Val", "sensor_dht_on", config.sensor_dht_on) + 
        renderParameterRow("DS18B20 Val", "sensor_ds18b20_on", config.sensor_ds18b20_on) + 
        renderParameterRow("ADC Pin Val", "sensor_analog_on", config.sensor_analog_on) + 
        "<hr/>" +
        "<a class='btn btn-default marginTop0' role='button' onclick='saveFormData(\"/sensors\");'>Зберегти</a>" +
        "</div>" +
        FPSTR(bodyEnd);

    WebServer.send(200, "text/html", data);

    if (config_changed)
    {
        config.saveConfig();
    }

    Serial.println("Server: request SENSORS sent");
}

void webReboot()
{
    Serial.println("\r\nServer: request REBOOT");

    String data =
        renderTitle(config.module_name, "Перезавантажити") + FPSTR(stylesInclude) +
        FPSTR(rebootScripts) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + renderMenu() +
        renderAlert("info", String("<strong id='info'>Систему буде перезавантажено через  ") + config.reboot_delay + " секунди</strong>") +
        FPSTR(bodyEnd);

    WebServer.send(200, "text/html", data);

    Serial.println("Server: request REBOOT sent");

    rebootESP();
}

void webStyles()
{
    Serial.println("\r\nServer: request STYLES");

    String stylesText = String("") + FPSTR(styles);
    WebServer.send(200, "text/css", stylesText);

    Serial.println("Server: request STYLES sent");
}

void handleNotFound()
{
    Serial.println("\r\nServer: not found");

    String data =
        renderTitle(config.module_name, "Page not found") + FPSTR(stylesInclude) + FPSTR(headEnd) + FPSTR(bodyStart) + renderMenu() +
        renderAlert("danger", String("Page <strong>") + WebServer.uri() + "</strong> not found.") +
        FPSTR(bodyEnd);

    WebServer.send(404, "text/html", data);
}

void initWebServer()
{
    Serial.println("Server: starting");
    WebServer.on("/styles.css", webStyles);
    WebServer.on("/", webRoot);
    WebServer.on("/cloudSetup", cloudSetup);
    WebServer.on("/wifiSetup", wifiSetup);
    // WebServer.on("/time", webTime);
    WebServer.on("/reboot", webReboot);
    WebServer.on("/sensors", webSensors);
    WebServer.onNotFound(handleNotFound);
    
    Serial.println("Server: started");
}

int connectWiFi()
{
    Serial.println("Wifi: connecting");

    int connectAttempts = 0;

    while (connectAttempts < maxConnectAttempts)
    {
        Serial.printf("Wifi: connecting, attempt %d\r\n", connectAttempts);
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Wifi: connected");
            renderWiFiStatus("On", 255, 255, 255);
            return 1;
        }

        delay(500);
        connectAttempts++;

        yield();
    }

    Serial.println("Wifi: timeout");
    renderWiFiStatus("Timeout", 255, 0, 0);
    return 0;
}

void handleWiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
        case WIFI_EVENT_STAMODE_CONNECTED:
            Serial.println("Wifi event: WIFI_EVENT_STAMODE_CONNECTED");
            renderWiFiStatus("Linking", 255, 255, 0);
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            Serial.println("Wifi event: WIFI_EVENT_STAMODE_DISCONNECTED");
            renderWiFiStatus("Off", 255, 0, 0);
            break;
        case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
            Serial.println("Wifi event: WIFI_EVENT_STAMODE_AUTHMODE_CHANGE");
            break;
        case WIFI_EVENT_STAMODE_GOT_IP:
            Serial.println("Wifi event: WIFI_EVENT_STAMODE_GOT_IP");
            Serial.print("Wifi: connected, IP = ");
            Serial.print(WiFi.localIP());
            Serial.print(", MAC = ");
            Serial.print(getMacString());
            Serial.println();
            renderWiFiStatus("On", 255, 255, 255);
            break;
        case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
            Serial.println("Wifi event: WIFI_EVENT_STAMODE_DHCP_TIMEOUT");
            break;
        case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
            Serial.println("Wifi event: WIFI_EVENT_SOFTAPMODE_STACONNECTED");
            renderAPStatus("Connected", 255, 255, 255);
            break;
        case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
            Serial.println("Wifi event: WIFI_EVENT_SOFTAPMODE_STADISCONNECTED");
            renderAPStatus("Off", 255, 255, 255);
            break;
        case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
            break;
        case WIFI_EVENT_MAX:
            Serial.println("Wifi event: WIFI_EVENT_MAX");
            break;
    }
}

void initWiFi()
{
    Serial.println("Wifi: starting");

    renderWiFiStatus("Off", 255, 0, 0);
    renderServerStatus("-", 255, 255, 255);
    renderAPStatus("Off", 255, 255, 255);

    delay(1000);
    WiFi.mode(WIFI_AP_STA);
    WiFi.onEvent(handleWiFiEvent);
    WiFi.begin(config.sta_ssid, config.sta_pwd);
    if (atoi(config.static_ip_toogle) == 1)
    {
        Serial.println("Wifi: use static IP");
        IPAddress staticIP = stringToIp(config.static_ip);
        IPAddress staticGateway = stringToIp(config.static_gateway);
        IPAddress staticSubnet = stringToIp(config.static_subnet);
        WiFi.config(staticIP, staticGateway, staticSubnet);
    }
    else
    {
        Serial.println("Wifi: using DHCP");
    }

    Serial.println(String("Wifi: connect to '") + config.sta_ssid + "' with password '" + config.sta_pwd + "'");

    connectWiFi();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(String("Wifi: connected, creating AP ") + config.module_name);
        Serial.println(String("with password:  ") + config.module_pwd);
        if(atoi(config.hidden_toogle) == 1) {
        Serial.println("AP options: hidden ");
        WiFi.softAP(config.module_name, config.module_pwd, 1, 1);
        }
        else {
        WiFi.softAP(config.module_name, config.module_pwd);
        }
        Serial.print("Wifi: connected, IP = ");
        Serial.print(WiFi.localIP());
        Serial.println();
    }
    else
    {
        Serial.println(String("Wifi: not connected, creating AP ") + config.module_name);
        Serial.println(String("with password:  ") + config.module_pwd);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(config.module_name, config.module_pwd);
    }

    Serial.println("Wifi: started\r\n");

    initWebServer();

    if (!MDNS.begin(config.module_name))
    {
        while (1)
        {
            delay(1000);
            yield();
        }
    }
    httpUpdater.setup(&WebServer);
    WebServer.begin();
    MDNS.addService("http", "tcp", 80);
}

void initSensors()
{
    if (atoi(config.sensor_dht_on) == 1)
    {
      dht.begin();
    }

    if (atoi(config.sensor_ds18b20_on) == 1)
    { 
      sensors.begin();
      sensors.getAddress(insideThermometer,0);
      sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
    }
    
    if (atoi(config.sensor_bmp180_on) == 1)
    {
      Wire.begin(2, 14);
      if (bmp180.begin())
      {
          bmp180initialized = true;
      }
    }
}


void setup()
{
    Serial.begin(115200);

    isRebooting = false;
    currentSensorCycle = 0;
    currentRebootCycle = 0;

    Serial.println("\r\nStarting module...");

    if (!SPIFFS.begin())
    {
        Serial.println("Config: failed to mount file system");
    }
    else
    {
        if (!config.loadConfig())
        {
            Serial.println("Config: failed to load");
        }
        else
        {
            Serial.println("Config: loaded");
        }

        config.printConfig();
    }

    String mac = getMacString(),
           mactoname = "MonSystem_" + String(mac[0]) + String(mac[1]);
    char module_name[32];
    strcpy(module_name, mactoname.c_str());
    String payload = module_name;
    payload.toCharArray(config.module_name, sizeof(config.module_name));
    parseServerResponse(payload);
    config.saveConfig();

    initWiFi();
    initSensors();

    Serial.println("\r\nStarting complete.");
    rst_info *rinfo;
    rinfo = ESP.getResetInfoPtr();
      
    if ((*rinfo).reason == 5) {
      Serial.println(String("ESP.getResetReason = ") + (*rinfo).reason + " > " + ESP.getResetReason());
      
      requestSensorValues();
      renderSensorValues();
      
        if (WiFi.status() == WL_CONNECTED)
        {
          if (atoi(config.ts_toogle) == 1) {
              sendSensorsData();
          }
          if (atoi(config.narodmon_toogle) == 1) {
              sendNarodmon();
          }
         }
      int ds_time = (atoi(config.get_data_delay)); 
      ESP.deepSleep(ds_time * 1000000);
    }
    else {
      Serial.println(String("ESP.getResetReason() = ") + (*rinfo).reason + " > " + ESP.getResetReason());
      if(atoi(config.deepsleep_toogle)) { 
        Serial.println("DeepSleep mode OFF");  
        char ds_toogle[32] = "0";
        String payload = ds_toogle;
        payload.toCharArray(config.deepsleep_toogle, sizeof(config.deepsleep_toogle));
        parseServerResponse(payload);
        config.saveConfig();
        }
    }
}

 SensorData getDS18B20Data()
 {
     sensors.requestTemperatures();
     float t = sensors.getTempCByIndex(0);
     SensorData data;
     data.humidity = 0;
     data.temp = t;
     data.pressure = 0;
     return data;
 }

SensorData getdhtData()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    SensorData data;
    data.humidity = h;
    data.temp = t;
    data.pressure = 0;
    return data;
}

SensorData getAnalogSensorData()
{
    float a = analogRead(A0);
    SensorData data;
    data.adc = a;
    return data;
}

SensorData getBmp180Data()
{
    char status;
    double T, P, p0, a;

    // If you want to measure altitude, and not pressure, you will instead need
    // to provide a known baseline pressure. This is shown at the end of the sketch.
    // You must first get a temperature measurement to perform a pressure reading.  
    // Start a temperature measurement:
    // If request is successful, the number of ms to wait is returned.
    // If request is unsuccessful, 0 is returned.
    status = bmp180.startTemperature();

    if (status != 0)
    {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed temperature measurement:
        // Note that the measurement is stored in the variable T.
        // Function returns 1 if successful, 0 if failure.
        status = bmp180.getTemperature(T);

        if (status != 0)
        {
            // Start a pressure measurement:
            // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
            // If request is successful, the number of ms to wait is returned.
            // If request is unsuccessful, 0 is returned.
            status = bmp180.startPressure(3);

            if (status != 0)
            {
                // Wait for the measurement to complete:
                delay(status);

                // Retrieve the completed pressure measurement:
                // Note that the measurement is stored in the variable P.
                // Note also that the function requires the previous temperature measurement (T).
                // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
                // Function returns 1 if successful, 0 if failure.
                status = bmp180.getPressure(P, T);

                if (status != 0)
                {
                    // The pressure sensor returns abolute pressure, which varies with altitude.
                    // To remove the effects of altitude, use the sealevel function and your current altitude.
                    // This number is commonly used in weather reports.
                    // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
                    // Result: p0 = sea-level compensated pressure in mb

                    p0 = bmp180.sealevel(P, ALTITUDE);

                    // On the other hand, if you want to determine your altitude from the pressure reading,
                    // use the altitude function along with a baseline pressure (sea-level or other).
                    // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
                    // Result: a = altitude in m.

                    a = bmp180.altitude(P, p0);
                }
            }
        }
    }

    SensorData data;
    if (bmp180initialized)
    {
        data.temp = T;
        data.pressure = p0 * 0.0295333727 * 25.4;
        data.humidity = 0;
    }    
    return data;
}

void renderRowValue(String value, int row, int r = 0, int g = 255, int b = 0)
{
    if (isRebooting)
    {
        //do nothing if rebooting
        return;
    }

    while (value.length() < 10)
    {
        value += " ";
    }
}

void renderWiFiStatus(String status, int r, int g, int b)
{
    renderRowValue(status, 0, r, g, b);
}

void renderServerStatus(String status, int r, int g, int b)
{
    renderRowValue(status, 1, r, g, b);
}

void renderAPStatus(String status, int r, int g, int b)
{
    renderRowValue(status, 2, r, g, b);
}

void requestSensorValues()
{
    if (atoi(config.sensor_dht_on) == 1)
    {
        data1 = getdhtData();
    }

    if (atoi(config.sensor_bmp180_on) == 1)
    {
        data3 = getBmp180Data();
    }

    if (atoi(config.sensor_ds18b20_on) == 1)
    {
        data2 = getDS18B20Data();
    }

    if (atoi(config.sensor_analog_on) == 1)
    {
        data4 = getAnalogSensorData();
    }
}

void renderSensorValues()
{
    if (atoi(config.sensor_dht_on) == 1)
    {
        data1.tempStr = floatToString(data1.temp, VALUE_TEMP);
        data1.humidityStr = floatToString(data1.humidity, VALUE_HUMIDITY);
        renderRowValue(data1.tempStr, 6);
        renderRowValue(data1.humidityStr, 4);
        Serial.println(String("dht temp : " + data1.tempStr));
        Serial.println(String("dht hum : " + data1.humidityStr));
    }

    if (atoi(config.sensor_ds18b20_on) == 1)
     {
         data2.tempStr = floatToString(data2.temp, VALUE_TEMP);
         renderRowValue(data2.tempStr, 6);
         Serial.println(String("ds18b20 temp : " + data2.tempStr));
     }

    if (atoi(config.sensor_bmp180_on) == 1)
    {
        data3.tempStr = floatToString(data3.temp, VALUE_TEMP);
        data3.pressureStr = floatToString(data3.pressure, VALUE_PRESSURE, 3, 2);
        renderRowValue(data3.pressureStr, 5);
        renderRowValue(data3.tempStr, 6);
        Serial.println(String("bmp180 temp : " + data3.tempStr));
        Serial.println(String("bmp180 pressure : " + data3.pressureStr));
    }

    if (atoi(config.sensor_analog_on) == 1)
    {
        data4.adcStr = String(data4.adc);
        renderRowValue(data4.adcStr, 5);
        Serial.println(String("analog pin value : " + data4.adcStr));
    }

}

void parseServerResponse(String payload)
{
    StaticJsonBuffer<2048> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload.c_str());
}

float getTempForJson(float value)
{
    return isnan(value) ? 0 : value;
}

float getPressureForJson(float value)
{
    return isnan(value) ? 0 : value;
}

float getADCForJson(float value)
{
    return isnan(value) ? 0 : value;
}

float getHumidityForJson(float value)
{
    return (isnan(value) || value > 100) ? 0 : value;
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void sendSensorsData()
{
    // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      String dht_temp = String(getTempForJson(data1.temp)),
             dht_humidity = String(getHumidityForJson(data1.humidity)),
             ds18b20_temp = String(getTempForJson(data2.temp)),
             bmp180_temp = String(getTempForJson(data3.temp)),
             bmp180_pressure = String(data3.pressure),
             adc_val = String(getADCForJson(data4.adc)),
             url = "/update?key=";
             
      url += config.thing_speak_api_key;

      if (atoi(config.sensor_dht_on) == 1) {
      url += "&field1=";
      url += dht_temp;
      url += "&field2=";
      url += dht_humidity;
      }
      
      if (atoi(config.sensor_ds18b20_on) == 1) {
      url += "&field3=";
      url += ds18b20_temp;
      }
      
      if (atoi(config.sensor_bmp180_on) == 1) {
      url += "&field4=";
      url += bmp180_temp;
      
      url += "&field5=";
      url += bmp180_pressure;
      }

      if (atoi(config.sensor_analog_on) == 1) {      
      url += "&field6=";
      url += adc_val;
      }
      
      Serial.print("Requesting URL: ");
      Serial.println(url);
      
      // This will send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n");
      delay(10);
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
}

void sendNarodmon()
{
    // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 8283;
      if (!client.connect("narodmon.ru", httpPort)) {
        Serial.println("connection failed");
        return;
      }
    
      String dht_temp = String(getTempForJson(data1.temp)),
             dht_humidity = String(getHumidityForJson(data1.humidity)),
             ds18b20_temp = String(getTempForJson(data2.temp)),
             bmp180_temp = String(getTempForJson(data3.temp)),
             bmp180_pressure = String(data3.pressure),
             adc_val = String(getADCForJson(data4.adc));
             
      client.print("#");
      client.print(WiFi.macAddress()); // МАС ESP8266       
      client.print("#");
      client.print("ESP8266"); // Назва пристрою
      client.println();
      
      if (atoi(config.sensor_dht_on) == 1) {
        client.print("#T1#");
        client.print(dht_temp);
        client.print("#dht Temp#");
        client.println();
        client.print("#H1#");
        client.print(dht_humidity);
        client.print("#dht Humidity#");
        client.println();
      }
      
      if (atoi(config.sensor_ds18b20_on) == 1) {
        client.print("#T2#");
        client.print(ds18b20_temp);
        client.print("#DS18B20 Temp#");
        client.println();
      }
      
      if (atoi(config.sensor_bmp180_on) == 1) {

        client.print("#T3#");
        client.print(bmp180_temp);
        client.print("#BMP180 Temp#");
        client.println();
        client.print("#P1#");
        client.print(bmp180_pressure);
        client.print("#BMP180 Pressure#");
        client.println();
      }

      if (atoi(config.sensor_analog_on) == 1) {  
        client.print("#ADC#");
        client.print(adc_val);
        client.print("#Analog pin data#");
        client.println();
      }
      
      client.println("##");
      Serial.println("\r Sensors data send to narodmod.ru \r");
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
}

void loop()
{
    WebServer.handleClient();
    unsigned long currentMillis = millis();
  
    if (currentMillis - previousMillis >= ONE_SECOND)
    {
        currentSensorCycle++;
        previousMillis = currentMillis;

        if (isRebooting)
        {
            currentRebootCycle++;
            handleTimerWhileRebooting();
        }
        else
        {
//            renderDateTime();  
            if (currentSensorCycle % atoi(config.get_data_delay) == 0)
            {
                requestSensorValues();
                renderSensorValues();
                
                  if (WiFi.status() == WL_CONNECTED)
                  {
                    if (atoi(config.ts_toogle) == 1) {
                        sendSensorsData();
                    }
                    if (atoi(config.narodmon_toogle) == 1) {
                        sendNarodmon();
                    }
                   }
                if(atoi(config.deepsleep_toogle) == 1) {
                
//                char ds_timer[32];
//                itoa(currentMillis, ds_timer, 10);
//                
//                String payload = ds_timer;
//                payload.toCharArray(config.deepsleep_timer, sizeof(config.deepsleep_timer));
//                parseServerResponse(payload);
//                config.saveConfig();
               
                int ds_time = (atoi(config.get_data_delay)); 
                ESP.deepSleep(ds_time * 1000000);
                }
            }
        }
    }
}
