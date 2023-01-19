#include "TFT_eSPI.h"
#include "FreeSans50pt7b.h"
#define gray 0x6B6D
#define blue 0x0AAD
#define orange 0xC260
#define purple 0x604D
#define green 0x1AE9
TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&lcd);
TFT_eSprite bgm = TFT_eSprite(&lcd);
double fps = 0;
void setup() {
  // put your setup code here, to run once:

  lcd.init();
  lcd.setRotation(0);
  sprite.createSprite(240, 135);
  sprite.setSwapBytes(true);
  sprite.setTextColor(TFT_WHITE);
  sprite.setTextDatum(0);
  sprite.setFreeFont(&FreeSans50pt7b);
  bgm.createSprite(240, 135);
  bgm.setSwapBytes(true);
  bgm.fillSprite(blue);
}

void loop() {
  int startF = millis();
 
  sprite.fillSprite(TFT_BLACK);
  String strr = String(int(millis()));
  sprite.drawString(String(43),12, 30);
   //sprite.drawString(String(43),12, 77);
  sprite.drawString(String(int(fps)), 0, 0, 2);
  sprite.pushToSprite(&bgm, 0, 0, TFT_BLACK);
  bgm.pushSprite(0, 0);
 int endF=millis();
  fps = 1000 / (endF - startF);
}
