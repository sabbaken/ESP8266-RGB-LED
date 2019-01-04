//             [ToDO List]
// [ ] Добавить сохронение яркости
// [ ] Считывание яркости из MQTT

/*===================[Libraries]==================*/
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <PubSubClient.h>
  #include "FastLED.h"

/*===================[Settings]===================*/
//[WiFi settings]
  const char* ssid = "***";
  const char* password = "***";

//[MQTT settings]
  const char* mqtt_server = "m21.cloudmqtt.com";
  const int mqtt_port = 10083;
  const char* mqtt_user = "***";
  const char* mqtt_pass = "***";
//[LED Settings]
  #define HighBR 200
  #define MidleBR 150
  #define LowBR 120
  int BR = 100;
  #define LED_PIN 4
  #define NUM_LEDS 22
  #define LED_TYPE WS2811
  #define COLOR_ORDER GRB
  #define RefreshRate 60
  CRGB leds[NUM_LEDS];
  #define UPDATES_PER_SECOND 100

  CRGBPalette16 currentPalette;
  TBlendType currentBlending;
  extern CRGBPalette16 myRedWhiteBluePalette;
  extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
  static uint8_t startIndex = 0;
  int selector = 0;
  int doFile = 0;
  boolean doMoveStartIndex = false;
  long HEXcolor;

  WiFiClient wclient;
  PubSubClient client(wclient, mqtt_server, mqtt_port);

/*===================[WiFi Start]==================*/
void StartWiFi() {
  LEDS.setBrightness(MidleBR);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    // one_color_all(39, 255, 90, true);
    // delay(500);
    // one_color_all(0, 0, 0, true);

  } else {
    // one_color_all(255, 57, 43, true);
    delay(1000);
    // one_color_all(0, 0, 0, true);
    ESP.restart();
  }
}

/*====================[CallBack]====================*/
void callback(const MQTT::Publish& pub) {
  String payload = pub.payload_string();
  String topic = pub.topic();

  if (topic == "LED/SelectTopic") {
    selector = payload.toInt();
  }

  if (topic == "LED/Color") {
    // HEXcolor =
  }
}

/*====================[Connecting to MQTT]====================*/
void StartMQTT() {
  if (client.connect(
          MQTT::Connect("ESP8266_LED").set_auth(mqtt_user, mqtt_pass))) {
    client.set_callback(callback);
    client.subscribe("watch/");
  }
}

/*====================[LED UPD]====================*/
void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] =
        ColorFromPalette(currentPalette, colorIndex, MidleBR, currentBlending);
    colorIndex += 3;
  }
}

/*===========[LED GenerateRandomPalette]===========*/
void GenerateRandomPalette() {
  for (int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV(random8(), 255, random8());
  }
}

/*=============[LED Modes]================*/
//  <===<===<===<===<===
void one_color_all(int cred, int cgrn, int cblu, boolean upd) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(cred, cgrn, cblu);
  }

  LEDS.setBrightness(MidleBR);

  if (upd == true) {
    LEDS.show();
  }
}

//  <===<===<===<===<===
void one_HEX_color_all(long rgb, boolean upd) {
  byte red, green, blue;
  int R, G, B;

  red = rgb >> 16;
  R = red;
  green = (rgb & 0x00ff00) >> 8;
  G = green;
  blue = (rgb & 0x0000ff);
  B = blue;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(R, G, B);
  }
  if (upd == true) {
    LEDS.show();
  }

  doMoveStartIndex = false;
  LEDS.setBrightness(MidleBR);
  doFile = 0;
}

//  <===<===<===<===<===
void Off() {
  one_color_all(0, 0, 0, true);
  doMoveStartIndex = false;
  doFile = 0;
  LEDS.setBrightness(MidleBR);
}

//  <===<===<===<===<===
void Rainbow() {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  doMoveStartIndex = true;
  doFile = 2;
  LEDS.setBrightness(MidleBR);
}

//  <===<===<===<===<===
void Nightlight() {
  CRGB Gray = CHSV(127, 140, 141);
  currentPalette =
      CRGBPalette16(Gray, Gray, Gray, Gray, Gray, Gray, Gray, Gray, Gray, Gray,
                    Gray, Gray, Gray, Gray, Gray, Gray);

  currentBlending = NOBLEND;
  doMoveStartIndex = false;
  doFile = 1;
  LEDS.setBrightness(100);
}

void Dots() {
  // CRGB Gray  = CHSV(127, 140, 141);
  CRGB Black = CHSV(0, 0, 0);
  CRGB White = CHSV(236, 240, 241);
  currentPalette =
      CRGBPalette16(White, Black, Black, Black, Black, Black, Black, Black,
                    Black, Black, Black, Black, Black, Black, Black, Black);
  currentBlending = NOBLEND;
  doMoveStartIndex = true;
  doFile = 2;
  LEDS.setBrightness(100);
}

/*====================[Setup]====================*/
void setup() {
  LEDS.setBrightness(MidleBR);
  LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.show();

  StartWiFi();
  delay(500);
  StartMQTT();

  ArduinoOTA.setHostname("ESP8266-00001");
  ArduinoOTA.begin();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip);
}

/*====================[Loop]====================*/
void loop() {
  ArduinoOTA.handle();

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      StartMQTT();
    }
  } else {
    StartWiFi();
  }

  if (client.connected()) {
    client.loop();
    // refreshData();
  }

  switch (selector) {
    case 0:
      Off();
      break;
    case 1:
      Nightlight();
      break;
    case 2:
      Rainbow();
      break;
    case 3:
      one_HEX_color_all(0x6f56a3, true);
      break;
    case 4:
      Dots();
      break;
  }

  if (doMoveStartIndex == true) {
    startIndex = startIndex + 1;
  }

  if (doFile == 2) {
    FillLEDsFromPaletteColors(startIndex);
  } else {
    if (doFile == 1) {
      FillLEDsFromPaletteColors(startIndex);
      doFile == 0;
    }
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}