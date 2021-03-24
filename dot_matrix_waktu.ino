#include <MD_Parola.h>

#include <MD_MAX72xx.h>

#include <SPI.h>

#include "NTPClient.h"

#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>

#include <WiFiUdp.h>

//coba millis()
unsigned long currentMillis = millis();
const long interval = 1000;
int ledState = LOW;
unsigned long previousMillis = 0;

const long utcOffsetInSeconds = 7 * 60 * 60 + 1;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.google.com", utcOffsetInSeconds);

/* Put your SSID & Password */
const char * ssid = "Gabriel Kheisa N"; // Enter SSID here
const char * password = "2147483648"; //Enter Password here

/*biar ga ngelag, update timeclient pakek millis */
// int jam_mtbs = 1000 * 3600 * 1; //per jam
int jam_mtbs = 1000 * 60 * 5;
long jam_lasttime;

//biar ga statis jamnya
int detik_mtbs = 1000;
long detik_lasttime;

String jamTeks = (String) timeClient.getMinutes();
int jamGerak = jamTeks.toInt();

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
//#define CLK_PIN   13
//#define DATA_PIN  11
#define CS_PIN D4

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Definisi hari
String Hari_teks[7] = {
  "Minggu",
  "Senin",
  "Selasa",
  "Rabu",
  "Kamis",
  "Jumat",
  "Sabtu"
};

//tebak random
bool tebak = false;

void loading_pre(){
    int i;
    for(i=0; i<=10; i++) {
    P.print(" -");
    delay(250);
    P.print(" --");
    delay(250);
    P.print(" ---");
    delay(250);
    P.print(" ----");
    delay(250);
    }
    P.print(" ");
}

void loading_post(){
    P.print("|");
    delay(10);
    P.print("||");
    delay(10);
    P.print("|||");
    delay(10);
    P.print("||||");
    delay(10);
    P.print("|||||");
    delay(10);
    P.print("||||||");
    delay(10);
    P.print("|||||||");
    delay(10);
    P.print("||||||||");
    delay(10);
    P.print("|||||||||");
    delay(10);
    P.print("||||||||||");
    delay(10);
    P.print("|||||||||||");
    delay(10);
    P.print("||||||||||||");
    delay(10);
    P.print("|||||||||||||");
    delay(10);
    P.print("||||||||||||||");
    delay(10);
    P.print("|||||||||||||||");
    delay(10);
    P.print("||||||||||||||||");
    delay(10);
}

void setup(void) {
  P.begin();
  //P.print("- UwU -");
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  loading_pre();
  while (!timeClient.update()) {
    loading_post();
    timeClient.update();
  }
}

void loop(void) {
  if ((int) timeClient.getSeconds() % 5 == 0) {

    if (!tebak) {
      P.print("" + Hari_teks[timeClient.getDay()]);
    } else if (tebak) {
      P.print("" + timeClient.getFormattedDate());
    } else {}

  } else {

    if ((int) timeClient.getSeconds() % 2 == 0) {
      P.print((String) timeClient.getFormattedTime());
      tebak = true;
    } else {
      P.print((String) timeClient.getFormattedTime() + ".");
      tebak = false;
    }
  }

  if (millis() > jam_lasttime + jam_mtbs) {
    timeClient.update();
    while (!timeClient.update()) {
      timeClient.update();
      P.print("- UwU -");
    }
  }
  jam_lasttime = millis();
}
