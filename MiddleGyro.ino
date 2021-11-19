// #define ARDUINOOSC_DEBUGLOG_ENABLE

// Please include ArduinoOSCWiFi.h to use ArduinoOSC on the platform
// which can use both WiFi and Ethernet

#include <ArduinoOSCWiFi.h>
// including libraries for the gyrosensor lsm303lhc
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
// this is also valid for other platforms which can use only WiFi
// #include <ArduinoOSC.h>


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// WiFi stuff
const char* ssid = "Bienvenue2";
const char* pwd = "olju1507";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

// for ArduinoOSC
const char* host = "192.168.1.94";
const int recv_port = 8000;
const int send_port = 9000;
// send / receive varibales
int i;
float f;
String s;

void onOscReceived(const OscMessage& m) {
    Serial.print(m.remoteIP());
    Serial.print(" ");
    Serial.print(m.remotePort());
    Serial.print(" ");
    Serial.print(m.size());
    Serial.print(" ");
    Serial.print(m.address());
    Serial.print(" ");
    Serial.print(m.arg<int>(0));
    Serial.print(" ");
    Serial.print(m.arg<float>(1));
    Serial.print(" ");
    Serial.print(m.arg<String>(2));
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    // WiFi stuff (no timeout setting for WiFi)
#ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());

#ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
    Serial.begin(115200);
    Serial.println("Accelerometer Test"); Serial.println("");

    /* Initialise the sensor */
    if(!accel.begin())
    {
      /* There was a problem detecting the ADXL345 ... check your connections */
      Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
      while(1);
    }

    // subscribe osc messages

    OscWiFi.send(host, send_port, "/accelX", 21);
    OscWiFi.update();  // should be called to receive + send osc

}

void loop() {
    sensors_event_t event;
    accel.getEvent(&event);

    
    OscWiFi.send(host, send_port, "/accelXMiddle", event.acceleration.x);
    OscWiFi.update();  // should be called to receive + send osc
    OscWiFi.send(host, send_port, "/accelYMiddle", event.acceleration.y);
    OscWiFi.update();  // should be called to receive + send osc
    OscWiFi.send(host, send_port, "/accelZMiddle", event.acceleration.z);
    OscWiFi.update();  // should be called to receive + send osc
    

    delay(500);
    // or do that separately
    // OscWiFi.parse(); // to receive osc
    // OscWiFi.post(); // to publish osc
}
