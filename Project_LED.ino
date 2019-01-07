//             [ToDO List]
// [ ] Добавить сохронение яркости
// [ ] Считывание яркости из MQTT

/*===================[Libraries]==================*/
  #include <ArduinoOTA.h>
  #include "FastLED.h"
  #include "EspMQTTClient.h"
/*===================[Settings]===================*/
//[WiFi/MQTT settings]
void onConnectionEstablished();
EspMQTTClient client(
  "Xiaomi_11C1",              // Wifi ssid
  "GPDcauldron15",            // Wifi password
  "m21.cloudmqtt.com",        // MQTT broker ip
  10083,                      // MQTT broker port
  "gcccfdqc",                 // MQTT username
  "IbnfGT5WbRn9",             // MQTT password
  "LED_Watch_2",                // Client name
  onConnectionEstablished,    // Connection established callback
  true,                       // Enable web updater
  true                        // Enable debug messages
);
//[LED Settings]
  #define HighBR      200
  #define MidleBR     150
  #define LowBR       120
  int BR =            100;
  #define LED_PIN     4
  #define NUM_LEDS    22
  #define LED_TYPE    WS2811
  #define COLOR_ORDER GRB
  #define RefreshRate 120
  CRGB leds[NUM_LEDS];
  #define UPDATES_PER_SECOND 120
  CRGBPalette16 currentPalette;
  TBlendType currentBlending;
  extern CRGBPalette16 myRedWhiteBluePalette;
  extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
  static uint8_t startIndex = 0;
  int selector        = 0;
  int doFile          = 0;
  boolean doMoveStartIndex = false;
  int r, g, b;

/*====================[Connecting to MQTT]====================*/
void onConnectionEstablished() {
  client.subscribe("Light/LED-Strip-01/LightMode", [] (const String &payload)
  {
    Serial.println(payload);
    selector = payload.toInt();
  });

  client.subscribe("Light/LED-Strip-01/LightColorHEX", [] (const String &payload)
  {
    String hexstring = payload;
    hexstring.remove(0,1); 
    int number = (int) strtol( &hexstring[1], NULL, 16);
    r = number >> 16;
    g = number >> 8 & 0xFF;
    b = number & 0xFF;
  });
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
  CRGB Color = CHSV(cred, cgrn, cblu);
  currentPalette =
      CRGBPalette16(Color, Color, Color, Color, Color, Color, Color, Color, Color, Color,
                    Color, Color, Color, Color, Color, Color);
  LEDS.setBrightness(MidleBR);

  if (upd == true) {
    LEDS.show();
  }

  doMoveStartIndex = false;
  doFile = 0;
  LEDS.setBrightness(MidleBR);
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

  currentBlending   = NOBLEND;
  doMoveStartIndex  = false;
  doFile            = 1;
  LEDS.setBrightness(100);
}

void Dots() {
  // CRGB Gray  = CHSV(127, 140, 141);
  CRGB Black = CHSV(0, 0, 0);
  CRGB White = CHSV(236, 240, 241);
  currentPalette =
      CRGBPalette16(White, Black, Black, Black, Black, Black, Black, Black,
                    Black, Black, Black, Black, Black, Black, Black, Black);
  currentBlending   = NOBLEND;
  doMoveStartIndex  = true;
  doFile            = 2;
  LEDS.setBrightness(100);
}

/*====================[Setup]====================*/
void setup() {
  LEDS.setBrightness(MidleBR);
  LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.show();

  ArduinoOTA.setHostname("[RGB_Strip]");
  ArduinoOTA.begin();

  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
}

/*====================[Loop]====================*/
void loop() {
  ArduinoOTA.handle();
  client.loop();

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
      one_color_all(r, g, b, true);
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