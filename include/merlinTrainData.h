#pragma once

#define SECURE 1

#include <tinyxml2.h>
#include <merlinNetwork.h>
#include <WiFiClientSecure.h>

//WiFiClientSecure _espWiFiClient;

using namespace tinyxml2;
#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTDEC(x, DEC) Serial.print(x, DEC)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTLNDEC(x, DEC) Serial.println(x, DEC)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTDEC(x, DEC)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLNDEC(x, DEC)
#endif

#define STATIONCODE "NBY"
String _stationCode = STATIONCODE;
#define HOST "lite.realtime.nationalrail.co.uk" //"52.48.160.4"
#define HTTPPORT 443
#define ACCESSTOKEN "ab8ff9fb-f3bd-492a-91d7-5d47f1c94ba1"
#define URL "/OpenLDBWS/ldb9.asmx"
//https://lite.realtime.nationalrail.co.uk/OpenLDBWS/ldb9.asmx


//openssl s_client -showcerts -connect lite.realtime.nationalrail.co.uk:443 <-- NOT ROOT CA (Rather use amazon.com)
const char *root_ca =
"-----BEGIN CERTIFICATE-----\n"
"MIIEkjCCA3qgAwIBAgITBn+USionzfP6wq4rAfkI7rnExjANBgkqhkiG9w0BAQsF\n" \
"ADCBmDELMAkGA1UEBhMCVVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNj\n" \
"b3R0c2RhbGUxJTAjBgNVBAoTHFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4x\n" \
"OzA5BgNVBAMTMlN0YXJmaWVsZCBTZXJ2aWNlcyBSb290IENlcnRpZmljYXRlIEF1\n" \
"dGhvcml0eSAtIEcyMB4XDTE1MDUyNTEyMDAwMFoXDTM3MTIzMTAxMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaOCATEwggEtMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/\n" \
"BAQDAgGGMB0GA1UdDgQWBBSEGMyFNOy8DJSULghZnMeyEE4KCDAfBgNVHSMEGDAW\n" \
"gBScXwDfqgHXMCs4iKK4bUqc8hGRgzB4BggrBgEFBQcBAQRsMGowLgYIKwYBBQUH\n" \
"MAGGImh0dHA6Ly9vY3NwLnJvb3RnMi5hbWF6b250cnVzdC5jb20wOAYIKwYBBQUH\n" \
"MAKGLGh0dHA6Ly9jcnQucm9vdGcyLmFtYXpvbnRydXN0LmNvbS9yb290ZzIuY2Vy\n" \
"MD0GA1UdHwQ2MDQwMqAwoC6GLGh0dHA6Ly9jcmwucm9vdGcyLmFtYXpvbnRydXN0\n" \
"LmNvbS9yb290ZzIuY3JsMBEGA1UdIAQKMAgwBgYEVR0gADANBgkqhkiG9w0BAQsF\n" \
"AAOCAQEAYjdCXLwQtT6LLOkMm2xF4gcAevnFWAu5CIw+7bMlPLVvUOTNNWqnkzSW\n" \
"MiGpSESrnO09tKpzbeR/FoCJbM8oAxiDR3mjEH4wW6w7sGDgd9QIpuEdfF7Au/ma\n" \
"eyKdpwAJfqxGF4PcnCZXmTA5YpaP7dreqsXMGz7KQ2hsVxa81Q4gLv7/wmpdLqBK\n" \
"bRRYh5TmOTFffHPLkIhqhBGWJ6bt2YFGpn6jcgAKUj6DiAdjd4lpFw85hdKrCEVN\n" \
"0FE6/V1dN2RMfjCyVSRCnTawXZwXgWHxyvkQAiSr6w10kY17RSlQOYiypok1JR4U\n" \
"akcjMS9cmvqtmg5iUaQqqcT5NJ0hGA==\n" \
"-----END CERTIFICATE-----\n";


#define HTTPREQUESTTIMEOUT 15000
#define HTTPRETRIES 5
#define MAXENTRIES 5
#define MAXSTOPS 20

struct Stops
{
	String stop_station_code;		  // "NBY", "KIT", "HGD", "BDW"
	String stop_tiploc_code;		  // "NEWBURY", "KNTBRY", "HUNGRFD", "BEDYN"
	String stop_station_name;		  // "Newbury", "Kintbury", "Hungerford", "Bedwyn"
	String stop_stop_type;			  // "LO", "LI", "LI", "LT"
	String stop_platform;			  // "1", "1", nullptr, "1"
	String stop_aimed_departure_date; // "2022-07-28", "2022-07-28", ...
	String stop_aimed_departure_time; // "17:24", "17:31", "17:36", ...
	String stop_aimed_arrival_date;	  // nullptr, "2022-07-28", ...
	String stop_aimed_arrival_time;	  // nullptr, "17:31", "17:36", "17:44"
									  // stop["aimed_pass_date"] is null
									  // stop["aimed_pass_time"] is null
};

struct Timetable
{
	String service;	  // "25519007"
	String train_uid; // "C38806"
	String headcode;  // nullptr

	String toc_atoc_code; // "GW"

	String train_status;	 // "P"
	String origin_name;		 // "Newbury"
	String destination_name; // "Bedwyn"
	// doc["stop_of_interest"] is null
	String date; // "2022-07-28"
	// doc["time_of_day"] is null
	String mode;		  // "train"
	String request_time;  // "2022-08-01T19:38:47+01:00"
	String category;	  // "OO"
	String operatorA;	  // "GW"
	String operator_name; // "Great Western Railway"
	Stops trainStops[MAXSTOPS];
};

struct Departures
{
	String departures_all_item_mode;		  // "train", "train", "train", ...
	String departures_all_item_service;		  // "25519007", "25519007", ...
	String departures_all_item_train_uid;	  // "C38517", "W44644", ...
	String departures_all_item_platform;	  // "2", "3", "1", "3"
	String departures_all_item_operator;	  // "GW", "GW", "GW", "GW"
	String departures_all_item_operator_name; // "Great Western ...
	String departures_all_item_aimed_departure_time;
	// departures_all_item["aimed_arrival_time"] is null
	// departures_all_item["aimed_pass_time"] is null
	String departures_all_item_origin_name;		 // "Newbury", ...
	String departures_all_item_destination_name; // "Bedwyn", ...
	String departures_all_item_source;			 // "ATOC", "ATOC", "ATOC", ...
	String departures_all_item_category;		 // "OO", "OO", "OO", "OO"
	String departures_all_item_service_timetable_id;

	String departures_all_item_status; // "LATE", "STARTS HERE", "ON ...
	String departures_all_item_expected_arrival_time;
	String departures_all_item_expected_departure_time;
	int departures_all_item_best_arrival_estimate_mins;
	int departures_all_item_best_departure_estimate_mins;

	Timetable timeTable;
	int timeTableLength = 0;
};

struct Station
{
	String date;		 // "2022-07-28"
	String time_of_day;	 // "16:14"
	String request_time; // "2022-08-01T17:46:19+01:00"
	String station_name; // "Newbury"
	String station_code; // "NBY"
	Departures departures[MAXENTRIES];
	int departuresLength = 0;
};

Station _mainStation = {};

String getTrainDataFromURL()
{

	WiFiClientSecure __wifiClient; // wifi client object
	String __resultString = "";
	DEBUG_PRINTLN("getTrainDataFromURL Connecting to " + String(HOST)); // start a new connection

#ifdef ESP8266
	__wifiClientSecure.setInsecure();
#endif // ESP8266

	__wifiClient.setTimeout(HTTPREQUESTTIMEOUT);
	__wifiClient.setCACert(root_ca);

	int retries = 6;
	for (int i = 0; i < HTTPRETRIES; i++) // retry up to 5 times
	{
		if (__wifiClient.connect(HOST, HTTPPORT))
			break;
		DEBUG_PRINTLN("Connection failed: " + String(i) + "/" + String(HTTPRETRIES));
		delay(1500);
	}

	if (!__wifiClient.connected())
		return "";

	DEBUG_PRINTLN("Requesting URL: " + String(URL));

	__wifiClient.print(F("POST "));
	__wifiClient.print(URL);
	__wifiClient.println(F(" HTTP/1.1"));

	__wifiClient.print(F("Host: "));
	__wifiClient.println(HOST);
	__wifiClient.println(F("Content-Type: text/xml; charset=utf-8"));
	__wifiClient.println(F("Content-Length: 648"));
	__wifiClient.println(F("Connection: keep-alive"));
//	__wifiClient.print(F("SOAPAction: \"https://"));
//	__wifiClient.print(HOST);
//	__wifiClient.print(URL);
//	__wifiClient.println(F("\""));
	__wifiClient.println();
	//__wifiClient.println(F("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
	__wifiClient.println(F("<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:typ=\"http://thalesgroup.com/RTTI/2013-11-28/Token/types\" xmlns:ldb=\"http://thalesgroup.com/RTTI/2016-02-16/ldb/\">"));
	__wifiClient.println(F("<soap:Header>"));
	__wifiClient.println(F("<typ:AccessToken>"));
	__wifiClient.print(F("<typ:TokenValue>"));
	__wifiClient.print(ACCESSTOKEN);
	__wifiClient.println(F("</typ:TokenValue>"));
	__wifiClient.println(F("</typ:AccessToken>"));
	__wifiClient.println(F("</soap:Header>"));
	__wifiClient.println(F("<soap:Body>"));
	__wifiClient.println(F("<ldb:GetDepBoardWithDetailsRequest>"));
	__wifiClient.println(F("<ldb:numRows>4</ldb:numRows>"));
	__wifiClient.print(F("<ldb:crs>"));
	__wifiClient.print(_stationCode);
	__wifiClient.println(F("</ldb:crs>"));
	__wifiClient.println(F("<ldb:filterCrs></ldb:filterCrs>"));
	__wifiClient.println(F("<ldb:filterType>to</ldb:filterType>"));
	__wifiClient.println(F("<ldb:timeOffset>0</ldb:timeOffset>"));
	__wifiClient.println(F("<ldb:timeWindow>180</ldb:timeWindow>"));
	__wifiClient.println(F("</ldb:GetDepBoardWithDetailsRequest>"));
	__wifiClient.println(F("</soap:Body>"));
	__wifiClient.println(F("</soap:Envelope>"));
	__wifiClient.println();
	/*while (__wifiClient.connected())
	{
		String line = __wifiClient.readStringUntil('\n');
		if (line == "\r")
		{
			DEBUG_PRINTLN("headers received");
			break;
		}
	}*/

 	while(__wifiClient.connected() && !__wifiClient.available()) 
		delay(1); //waits for data

	DEBUG_PRINTLN("Receiving API train data");

	__resultString = "";
	while (__wifiClient.available())
	{
		__resultString += __wifiClient.readStringUntil('\n');
		// char c = __wifiClient.read();
	    //__resultString += String(c);// readStringUntil('\r');
	}

	DEBUG_PRINTLN("\r\nClosing connection");
	__wifiClient.stop();
	//__resultString.trim();
	__resultString+="\0";
	__resultString.replace("<?xml version=\"1.0\" encoding=\"utf-8\"?>","");
	__resultString.replace(":","");

	DEBUG_PRINTLN("Read the string:");
	DEBUG_PRINTLN(__resultString);
	return __resultString;
}

String getServiceDetail(XMLElement * parentXMLE, const char * elementname)
{
	XMLElement * __elementValue = parentXMLE->FirstChildElement(elementname);
	if((__elementValue == NULL)) {	DEBUG_PRINTLN(String(elementname)+" is NULL"); return "";	}
	return __elementValue->GetText();
}

bool parseStop(int _stationID, XMLElement * parentXMLE)
{
	bool __ret = true;
	DEBUG_PRINTLN("parseStop");

	XMLElement * __elementlt4callingPoint = parentXMLE->FirstChildElement("lt4callingPoint");
	if((__elementlt4callingPoint == NULL)) {	DEBUG_PRINTLN("lt4callingPoint is NULL"); return false;	}

	int __departuresID = 0;
	int __i = 0;

	bool _foundStationCode = false;
	while(__elementlt4callingPoint != NULL){

		if (__i >= MAXSTOPS)  {	break; }

		_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_station_name = getServiceDetail(__elementlt4callingPoint,"lt4locationName"); // "Newbury", "Kintbury", "Hungerford", "Bedwyn"
		_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_station_code = getServiceDetail(__elementlt4callingPoint,"lt4crs"); // "NBY"
		//_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_platform = stop["platform"].as<String>(); // "1", "1", nullptr, "1"
		_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_departure_time = getServiceDetail(__elementlt4callingPoint,"lt4st"); // "17:24", "17:31", "17:36", ...
		_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_arrival_time = getServiceDetail(__elementlt4callingPoint,"lt4st"); //  "17:31", "17:36", "17:44"
		DEBUG_PRINTLN(" --> Arrives: " + _mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_arrival_time + " --> " + _mainStation.departures[_stationID].timeTable.trainStops[__i].stop_station_name );
		__i++;
		__elementlt4callingPoint = __elementlt4callingPoint->NextSiblingElement();

	}

	DEBUG_PRINTLN("");
	_mainStation.departures[_stationID].timeTableLength = __i;
	DEBUG_PRINTLN("parseStop ended");
	return __ret;
}

bool parseTrainData(const char* input)
{
	bool __ret = true;
	DEBUG_PRINTLN("parseTrainData");

	XMLDocument __xmlDocument;
	XMLError __xmlError = __xmlDocument.Parse(input);
	if(__xmlError!= XML_SUCCESS){

		DEBUG_PRINT("__xmlDocument.Parse() failed, Line Number ");
		DEBUG_PRINTLN(__xmlDocument.ErrorLineNum());
		DEBUG_PRINTLN(__xmlDocument.ErrorStr());		
		return false;
	}
	else
	{
		DEBUG_PRINTLN("__xmlDocument.Parse() success");
	}

	XMLNode * __root = __xmlDocument.FirstChildElement("soapEnvelope");		//<soapEnvelope>
	if((__root == NULL)) {	DEBUG_PRINTLN("__root is NULL"); return false;	}

	XMLElement * __elementFirstChild = __root->FirstChildElement("soapBody"); //<soap:Body>
	if((__elementFirstChild == NULL)) {	DEBUG_PRINTLN("__elementFirstChild is NULL"); return false;	}

	XMLElement * __elementGetDepBoardWithDetailsResponse = __elementFirstChild->FirstChildElement();
	if((__elementGetDepBoardWithDetailsResponse == NULL)) {	DEBUG_PRINTLN("__elementGetDepBoardWithDetailsResponse is NULL"); return false;	}

	XMLElement * __elementGetStationBoardResult = __elementGetDepBoardWithDetailsResponse->FirstChildElement("GetStationBoardResult");	
	if((__elementGetStationBoardResult == NULL)) {	DEBUG_PRINTLN("__elementGetStationBoardResult is NULL"); return false;	}

	XMLElement * __elementlt4locationName = __elementGetStationBoardResult->FirstChildElement("lt4locationName");
	if((__elementlt4locationName == NULL)) {	DEBUG_PRINTLN("__elementlt4locationName is NULL"); return false;	}
	_mainStation.station_name = __elementlt4locationName->GetText(); // "Newbury"
	DEBUG_PRINTLN("Station Name: "  + _mainStation.station_name);

	XMLElement * __elementlt4lt4crs = __elementGetStationBoardResult->FirstChildElement("lt4crs");
	if((__elementlt4lt4crs == NULL)) {	DEBUG_PRINTLN("__elementlt4lt4crs is NULL"); return false;	}
	_mainStation.station_code = __elementlt4lt4crs->GetText(); // "NBY"
	DEBUG_PRINTLN("Station Code: "  + _mainStation.station_code);	

	XMLElement * __elementlt5trainServices = __elementGetStationBoardResult->FirstChildElement("lt5trainServices");
	if((__elementlt5trainServices == NULL)) {	DEBUG_PRINTLN("lt5trainServices is NULL"); return false;	}

	XMLElement * __elementlt5service = __elementlt5trainServices->FirstChildElement("lt5service");
	if((__elementlt5service == NULL)) {	DEBUG_PRINTLN("lt5service is NULL"); return false;	}

	int __departuresID = 0;
	while(__elementlt5service != NULL){

		if (__departuresID >= MAXENTRIES)	{	break;	}

		_mainStation.departures[__departuresID].departures_all_item_platform = getServiceDetail(__elementlt5service, "lt4platform"); // "2", "3", "1", "3"
		_mainStation.departures[__departuresID].departures_all_item_aimed_departure_time = getServiceDetail(__elementlt5service, "lt4std");
		_mainStation.departures[__departuresID].departures_all_item_status =  getServiceDetail(__elementlt5service, "lt4etd"); // "LATE", "STARTS HERE", "ON ...
		_mainStation.departures[__departuresID].departures_all_item_service_timetable_id = getServiceDetail(__elementlt5service,"lt4serviceID");
		//_mainStation.departures[__departuresID].departures_all_item_service_timetable_id = getServiceDetail(__elementlt5service,"lt4serviceID");

		XMLElement * __elementlt5origin = __elementlt5service->FirstChildElement("lt5origin");
		if((__elementlt5origin != NULL)) {	
			XMLElement * __elementlt4location = __elementlt5origin->FirstChildElement("lt4location");
			if((__elementlt4location != NULL)) {
				_mainStation.departures[__departuresID].departures_all_item_origin_name = getServiceDetail(__elementlt4location,"lt4locationName");		 // "Newbury", ...
			}
		}

		XMLElement * __elementlt5destination = __elementlt5service->FirstChildElement("lt5destination");
		if((__elementlt5destination != NULL)) {	
			XMLElement * __elementlt4location = __elementlt5destination->FirstChildElement("lt4location");
			if((__elementlt4location != NULL)) {
				_mainStation.departures[__departuresID].departures_all_item_destination_name = getServiceDetail(__elementlt4location,"lt4locationName");		 // "Newbury", ...
			}
		}
		
		//_mainStation.departures[__departuresID].departures_all_item_expected_arrival_time = departures_all_item["expected_arrival_time"].as<String>();
		//_mainStation.departures[__departuresID].departures_all_item_expected_departure_time = departures_all_item["expected_departure_time"].as<String>();
		//_mainStation.departures[__departuresID].departures_all_item_best_arrival_estimate_mins = departures_all_item["best_arrival_estimate_mins"];
		//_mainStation.departures[__departuresID].departures_all_item_best_departure_estimate_mins = departures_all_item["best_departure_estimate_mins"];
		//DEBUG_PRINTLN("Aimed Departure Time: " + _mainStation.departures[__departuresID].departures_all_item_aimed_departure_time);
		DEBUG_PRINTLN("Fetching stops for train #" + String(__departuresID) + ": ");
		//DEBUG_PRINTLN("From:       " + _mainStation.departures[_stationID].departures_all_item_origin_name + " Departs: " + getServiceDetail(__elementlt4callingPoint,"lt4st"));
		DEBUG_PRINTLN("Stopping at:");

		XMLElement * __elementlt5subsequentCallingPoints = __elementlt5service->FirstChildElement("lt5subsequentCallingPoints");
		if((__elementlt5subsequentCallingPoints != NULL)) {	
			XMLElement * __elementlt4callingPointList = __elementlt5subsequentCallingPoints->FirstChildElement("lt4callingPointList");
			if((__elementlt4callingPointList != NULL)) {
				__ret &= parseStop(__departuresID, __elementlt4callingPointList);
			}
		}
   		__elementlt5service = __elementlt5service->NextSiblingElement();
		__departuresID++;
	}
	
	_mainStation.departuresLength = __departuresID;
	return __ret;
}

bool GetTrainConditionsAndForecast(String time, String date)
{
	DEBUG_PRINTLN("------------------------------------------------");
	DEBUG_PRINTLN("GetTrainConditionsAndForecast");
	
	bool __ret = parseTrainData(getTrainDataFromURL().c_str());

	DEBUG_PRINTLN("------------------------------------------------");
	return __ret;
}
