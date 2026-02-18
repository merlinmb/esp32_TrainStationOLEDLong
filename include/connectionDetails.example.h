#pragma once

const char * SSID = "myWiFiSSID";
const char* WIFIPASSWORD = "myWiFiPassword";

#define WIFI_ACCESSPOINT "xxx"
#define WIFI_ACCESSPOINT_PASSWORD "yyy"

#define WIFI_ACCESSPOINT1 "yyy"
#define WIFI_ACCESSPOINT_PASSWORD1 "xxx"

#define OTA_UPDATE_USERNAME "admin"
#define OTA_UPDATE_PASSWORD "xxx"




int MQTT_MAX_PACKET_SIZE = 256;
const char* MQTT_SERVERADDRESS = "192.168.1.55"; //IP address of the MQTT broker
#define MQTT_SERVER_PORT 1883
const char* MQTT_CLIENTNAME = "espTrainMonitor"; //name of the device in MQTT

const char* ARDUINO_OTA_URI_SUFFIX = "/firmware";
const char* ARDUINO_OTA_UPDATE_USERNAME = "admin";
const char* ARDUINO_OTA_UPDATE_PASSWORD = "my OTA password"; //password for OTA updates

#define NR_ACCESSTOKEN "ab8ff9fb-f3bd-492a-91d7-5d47f1c94ba1"