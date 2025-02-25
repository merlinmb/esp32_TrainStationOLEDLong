#pragma once

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
#define trainHost "transportapi.com"
#define httpSSLPort 443

const char *root_ca =
	"-----BEGIN CERTIFICATE-----\n"
	"MIIEkjCCA3qgAwIBAgITBn+USionzfP6wq4rAfkI7rnExjANBgkqhkiG9w0BAQsF\n"
	"ADCBmDELMAkGA1UEBhMCVVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNj\n"
	"b3R0c2RhbGUxJTAjBgNVBAoTHFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4x\n"
	"OzA5BgNVBAMTMlN0YXJmaWVsZCBTZXJ2aWNlcyBSb290IENlcnRpZmljYXRlIEF1\n"
	"dGhvcml0eSAtIEcyMB4XDTE1MDUyNTEyMDAwMFoXDTM3MTIzMTAxMDAwMFowOTEL\n"
	"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
	"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
	"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
	"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
	"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
	"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
	"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
	"jgSubJrIqg0CAwEAAaOCATEwggEtMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/\n"
	"BAQDAgGGMB0GA1UdDgQWBBSEGMyFNOy8DJSULghZnMeyEE4KCDAfBgNVHSMEGDAW\n"
	"gBScXwDfqgHXMCs4iKK4bUqc8hGRgzB4BggrBgEFBQcBAQRsMGowLgYIKwYBBQUH\n"
	"MAGGImh0dHA6Ly9vY3NwLnJvb3RnMi5hbWF6b250cnVzdC5jb20wOAYIKwYBBQUH\n"
	"MAKGLGh0dHA6Ly9jcnQucm9vdGcyLmFtYXpvbnRydXN0LmNvbS9yb290ZzIuY2Vy\n"
	"MD0GA1UdHwQ2MDQwMqAwoC6GLGh0dHA6Ly9jcmwucm9vdGcyLmFtYXpvbnRydXN0\n"
	"LmNvbS9yb290ZzIuY3JsMBEGA1UdIAQKMAgwBgYEVR0gADANBgkqhkiG9w0BAQsF\n"
	"AAOCAQEAYjdCXLwQtT6LLOkMm2xF4gcAevnFWAu5CIw+7bMlPLVvUOTNNWqnkzSW\n"
	"MiGpSESrnO09tKpzbeR/FoCJbM8oAxiDR3mjEH4wW6w7sGDgd9QIpuEdfF7Au/ma\n"
	"eyKdpwAJfqxGF4PcnCZXmTA5YpaP7dreqsXMGz7KQ2hsVxa81Q4gLv7/wmpdLqBK\n"
	"bRRYh5TmOTFffHPLkIhqhBGWJ6bt2YFGpn6jcgAKUj6DiAdjd4lpFw85hdKrCEVN\n"
	"0FE6/V1dN2RMfjCyVSRCnTawXZwXgWHxyvkQAiSr6w10kY17RSlQOYiypok1JR4U\n"
	"akcjMS9cmvqtmg5iUaQqqcT5NJ0hGA==\n"
	"-----END CERTIFICATE-----\n";

#define HTTPREQUESTTIMEOUT 15000
#define HTTPRETRIES 5
#define MAXENTRIES 3
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

	String departures_all_item_status; 					 // "LATE", "STARTS HERE", "ON ...
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

String getTrainDataFromURL(String HOST, int PORT, String URL)
{

	WiFiClientSecure __wifiClientSecure; // wifi client object
	String resultString = "";
	DEBUG_PRINTLN("getTrainDataFromURL Connecting to " + HOST); // start a new connection

#ifdef ESP8266
	__wifiClientSecure.setInsecure();
#endif // ESP8266

	__wifiClientSecure.setTimeout(HTTPREQUESTTIMEOUT);
	__wifiClientSecure.setCACert(root_ca);

	int retries = 6;
	for (int i = 0; i < HTTPRETRIES; i++) // retry up to 5 times
	{
		if (__wifiClientSecure.connect(HOST.c_str(), PORT))
			break;
		DEBUG_PRINTLN("Connection failed: " + String(i) + "/" + String(HTTPRETRIES));
		delay(1500);
	}

	if (!__wifiClientSecure.connected())
		return "";

	DEBUG_PRINTLN("Requesting URL: " + String(URL));

	__wifiClientSecure.println("GET " + URL + " HTTP/1.1\r\n" + "Host: " + HOST + "\r\nConnection: close\r\n\r\n");
	// DEBUG_PRINTLN("GET " + URL + " HTTP/1.1\r\n" + "Host: " + HOST + "\r\nConnection: close\r\n\r\n");

	while (__wifiClientSecure.connected())
	{
		String line = __wifiClientSecure.readStringUntil('\n');
		if (line == "\r")
		{
			DEBUG_PRINTLN("headers received");
			break;
		}
	}

	DEBUG_PRINTLN("Receiving API train data");

	__wifiClientSecure.read();
	__wifiClientSecure.read();
	__wifiClientSecure.read();
	__wifiClientSecure.read();

	resultString = "";
	while (__wifiClientSecure.available())
	{
		resultString += __wifiClientSecure.readStringUntil('\n');
		// char c = __wifiClientSecure.read();
		// resultString += String(c);// readStringUntil('\r');
	}

	DEBUG_PRINTLN("\r\nClosing connection");
	__wifiClientSecure.stop();
	resultString.trim();

	// DEBUG_PRINTLN("Read the string:");

	// DEBUG_PRINTLN(resultString);
	return resultString;
}

bool parseStop(int _stationID, String _url)
{
	bool __ret = true;
	DEBUG_PRINTLN("parseStop");
	String input = getTrainDataFromURL(trainHost, httpPort, _url);

	DynamicJsonDocument doc(9000);
	DeserializationError error = deserializeJson(doc, input);

	if (error)
	{
		DEBUG_PRINT("deserializeJson() failed: ");
		DEBUG_PRINTLN(error.c_str());
		DEBUG_PRINTLN(input);
		return false;
	}

	//_mainStation.departures[_stationID].timeTable.service = doc["service"].as<String>();				   // "25519007"
	//_mainStation.departures[_stationID].timeTable.train_uid = doc["train_uid"].as<String>();			   // "C38806"
	//_mainStation.departures[_stationID].timeTable.headcode = doc["headcode"].as<String>();				   // nullptr
	//_mainStation.departures[_stationID].timeTable.toc_atoc_code = doc["toc"]["atoc_code"].as<String>();	   // "GW"
	//_mainStation.departures[_stationID].timeTable.train_status = doc["train_status"].as<String>();		   // "P"
	_mainStation.departures[_stationID].timeTable.origin_name = doc["origin_name"].as<String>();		   // "Newbury"
	_mainStation.departures[_stationID].timeTable.destination_name = doc["destination_name"].as<String>(); // "Bedwyn"
	// doc["stop_of_interest"] is null
	_mainStation.departures[_stationID].timeTable.date = doc["date"].as<String>(); // "2022-07-28"
	// doc["time_of_day"] is null
	//_mainStation.departures[_stationID].timeTable.mode = doc["mode"].as<String>();					 // "train"
	//_mainStation.departures[_stationID].timeTable.request_time = doc["request_time"].as<String>();	 // "2022-08-01T19:38:47+01:00"
	//_mainStation.departures[_stationID].timeTable.category = doc["category"].as<String>();			 // "OO"
	//_mainStation.departures[_stationID].timeTable.operatorA = doc["operator"].as<String>();			 // "GW"
	//_mainStation.departures[_stationID].timeTable.operator_name = doc["operator_name"].as<String>(); // "Great Western Railway"

	int __i = 0;
	int __realI = 0;
	bool _foundStationCode = false;
	for (JsonObject stop : doc["stops"].as<JsonArray>())
	{
		if (__i >= MAXSTOPS)
		{
			break;
		}
		if (__realI == 0)
		{
			DEBUG_PRINTLN("From:       " + _mainStation.departures[_stationID].timeTable.origin_name + " Departs: " + stop["aimed_departure_time"].as<String>());
			DEBUG_PRINTLN("Stopping at:");
		}

		_foundStationCode = _foundStationCode || (stop["station_code"].as<String>() == STATIONCODE);

		if (_foundStationCode)
		{
			//_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_station_code = stop["station_code"].as<String>();				 // "NBY", "KIT", "HGD", "BDW"
			//_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_tiploc_code = stop["tiploc_code"].as<String>();					 // "NEWBURY", "KNTBRY", "HUNGRFD", "BEDYN"
			_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_station_name = stop["station_name"].as<String>(); // "Newbury", "Kintbury", "Hungerford", "Bedwyn"
			//_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_stop_type = stop["stop_type"].as<String>();						 // "LO", "LI", "LI", "LT"
			_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_platform = stop["platform"].as<String>(); // "1", "1", nullptr, "1"
			//_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_departure_date = stop["aimed_departure_date"].as<String>(); // "2022-07-28", "2022-07-28", ...
			_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_departure_time = stop["aimed_departure_time"].as<String>(); // "17:24", "17:31", "17:36", ...
			//_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_arrival_date = stop["aimed_arrival_date"].as<String>();	 // nullptr, "2022-07-28", ...
			_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_arrival_time = stop["aimed_arrival_time"].as<String>(); // nullptr, "17:31", "17:36", "17:44"
			DEBUG_PRINTLN(" --> Arrives: "+_mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_arrival_time + " --> " + _mainStation.departures[_stationID].timeTable.trainStops[__i].stop_station_name + " & departs: " + _mainStation.departures[_stationID].timeTable.trainStops[__i].stop_aimed_departure_time);
			__i++;
		}
		__realI++;
	}

	DEBUG_PRINTLN("");
	_mainStation.departures[_stationID].timeTableLength = __i;
	DEBUG_PRINTLN("parseStop ended");
	return __ret;
}

bool parseTrainData(String input)
{
	bool __ret = true;
	DEBUG_PRINTLN("parseTrainData");
	DynamicJsonDocument doc(9000);
	DeserializationError error = deserializeJson(doc, input);

	if (error)
	{
		DEBUG_PRINT("deserializeJson() failed: ");
		DEBUG_PRINTLN(error.c_str());
		DEBUG_PRINTLN(input);
		return false;
	}

	_mainStation.date = doc["date"].as<String>();				  // "2022-07-28"
	_mainStation.time_of_day = doc["time_of_day"].as<String>();	  // "16:14"
	_mainStation.request_time = doc["request_time"].as<String>(); // "2022-08-01T17:46:19+01:00"
	_mainStation.station_name = doc["station_name"].as<String>(); // "Newbury"
	//_mainStation.station_code = doc["station_code"].as<String>(); // "NBY"

	DEBUG_PRINTLN("Time of day: " + _mainStation.time_of_day);

	int __departuresID = 0;
	for (JsonObject departures_all_item : doc["departures"]["all"].as<JsonArray>())
	{
		if (__departuresID >= MAXENTRIES)
		{
			break;
		}

		//_mainStation.departures[__departuresID].departures_all_item_mode = departures_all_item["mode"].as<String>();				   // "train", "train", "train", ...
		//_mainStation.departures[__departuresID].departures_all_item_service = departures_all_item["service"].as<String>();			   // "25519007", "25519007", ...
		//_mainStation.departures[__departuresID].departures_all_item_train_uid = departures_all_item["train_uid"].as<String>();		   // "C38517", "W44644", ...
		_mainStation.departures[__departuresID].departures_all_item_platform = departures_all_item["platform"].as<String>(); // "2", "3", "1", "3"
		//_mainStation.departures[__departuresID].departures_all_item_operator = departures_all_item["operator"].as<String>();		   // "GW", "GW", "GW", "GW"
		//_mainStation.departures[__departuresID].departures_all_item_operator_name = departures_all_item["operator_name"].as<String>(); // "Great Western ...
		_mainStation.departures[__departuresID].departures_all_item_aimed_departure_time = departures_all_item["aimed_departure_time"].as<String>();

		// departures_all_item["aimed_arrival_time"] is null
		// departures_all_item["aimed_pass_time"] is null
		_mainStation.departures[__departuresID].departures_all_item_origin_name = departures_all_item["origin_name"].as<String>();			 // "Newbury", ...
		_mainStation.departures[__departuresID].departures_all_item_destination_name = departures_all_item["destination_name"].as<String>(); // "Bedwyn", ...
		//_mainStation.departures[__departuresID].departures_all_item_source = departures_all_item["source"].as<String>();					 // "ATOC", "ATOC", "ATOC", ...
		//_mainStation.departures[__departuresID].departures_all_item_category = departures_all_item["category"].as<String>();				 // "OO", "OO", "OO", "OO"

		_mainStation.departures[__departuresID].departures_all_item_service_timetable_id = departures_all_item["service_timetable"]["id"].as<String>();

		_mainStation.departures[__departuresID].departures_all_item_status = departures_all_item["status"].as<String>(); // "LATE", "STARTS HERE", "ON ...
		_mainStation.departures[__departuresID].departures_all_item_expected_arrival_time = departures_all_item["expected_arrival_time"].as<String>();
		_mainStation.departures[__departuresID].departures_all_item_expected_departure_time = departures_all_item["expected_departure_time"].as<String>();
		_mainStation.departures[__departuresID].departures_all_item_best_arrival_estimate_mins = departures_all_item["best_arrival_estimate_mins"];
		_mainStation.departures[__departuresID].departures_all_item_best_departure_estimate_mins = departures_all_item["best_departure_estimate_mins"];

		DEBUG_PRINT("Fetching stop #" + String(__departuresID) + " - ");

		// DEBUG_PRINTLN("departures_all_item_destination_name:     "+_mainStation.departures[__departuresID].departures_all_item_destination_name);
		// DEBUG_PRINTLN("departures_all_item_service_timetable_id: "+_mainStation.departures[__departuresID].departures_all_item_service_timetable_id);
		DEBUG_PRINTLN("Aimed Departure Time: " + _mainStation.departures[__departuresID].departures_all_item_aimed_departure_time);

		__ret &= parseStop(__departuresID, _mainStation.departures[__departuresID].departures_all_item_service_timetable_id);
		__departuresID++;
	}
	_mainStation.departuresLength = __departuresID;

	return __ret;
}

bool GetTrainConditionsAndForecast(String time, String date)
{
	DEBUG_PRINTLN("------------------------------------------------");
	DEBUG_PRINTLN("GetTrainConditionsAndForecast");

	//												 /v3/uk/train/station/NBY/2022-08-03/18:11/timetable.json?app_id=4744c1ef&amp;app_key=9094b940701879dd648a76b95991a787&amp;train_status=passenger
	//String __url = "https://" + String(trainHost) + "/v3/uk/train/station/" + STATIONCODE + "/" + date + "/" + time + "/timetable.json?app_id=4744c1ef&app_key=9094b940701879dd648a76b95991a787&train_status=passenger";
	String __url = "https://" + String(trainHost) + "/v3/uk/train/station/" + STATIONCODE + "/live.json?app_id=4744c1ef&app_key=9094b940701879dd648a76b95991a787";
	String __trainData = getTrainDataFromURL(trainHost, httpPort, __url);
	bool __ret = parseTrainData(__trainData);
	DEBUG_PRINTLN("------------------------------------------------");
	return __ret;
}
