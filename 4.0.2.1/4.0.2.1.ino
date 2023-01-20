#include <TFT_eSPI.h>
#include <SPI.h>
#include <ESP32Time.h>
#include "icons.h"
#include "Main_Back_pic.h"
#include "FreeSans50pt7b.h"


int app,oldcursor,cursor, foor, touch, oldMin, newMin, leftB, rightB, activeTime, oldApp, aniOldXaniNewX;
/*foor is in app
   app
   0 main
   1 menu
   2 settings
  
  link for font
  https://rop.nl/truetype2gfx/
  now try to convert vlw to gfx
  taiwanese https://github.com/will127534/AdafruitGFX-ChineseFont-Addon

  great example:
  https://github.com/VolosR/nyanCatTTGO
 datum;
 TL_DATUM = 0 = Top left
TC_DATUM = 1 = Top centre
TR_DATUM = 2 = Top right
ML_DATUM = 3 = Middle left
MC_DATUM = 4 = Middle centre
MR_DATUM = 5 = Middle right
BL_DATUM = 6 = Bottom left
BC_DATUM = 7 = Bottom centre
BR_DATUM = 8 = Bottom right

point(pt)*3/4=pixel

*/
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite icon = TFT_eSprite(&tft);
TFT_eSprite clockNum = TFT_eSprite(&tft);
TFT_eSprite cursor= TFT_eSprite(&tft);
TFT_eSprite bgp = TFT_eSprite(&tft);//back ground picture
ESP32Time rtc(3600);
int x = 40;
//void printSprite(int x, int y , const unsigned short i[]) {
 // icon.pushImage(0, 0, 64, 64, i);
 // icon.pushSprite(x - 32, y - 32, TFT_BLACK);
//}
void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT);
  pinMode(35, INPUT);
  Serial.print(leftB);
  Serial.print(rightB);
  Serial.print("app");
  Serial.print(app);
  Serial.println(activeTime);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  esp_sleep_enable_ext1_wakeup(0x800000000, ESP_EXT1_WAKEUP_ALL_LOW);
  activeTime = millis();
  rtc.setTime(30, 24, 15, 17, 1, 2021);
  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(1);
  bgp.createSprite(135,240);
  icon.createSprite(64, 64);
  clockNum.createSprite(64, 64);
  bgp.setSwapBytes(true);
  icon.setSwapBytes(true);
  clockNum.setSwapBytes(true);
  bgp.pushImage(0, 0,  135, 240, bootlogo);
 
  
  //  printSprite(67, 44, gameIcon);
  //  printSprite(67, 120, clockIcon);
  //  printSprite(67, 196, settingsIcon);
  
  printClock();
   bgp.pushSprite(0,0);
}

void loop() {
  leftB = digitalRead(0);
  rightB = digitalRead(35);
  thread();
  refresh();

  // Serial.print(rightB);
  //  Serial.print(leftB);
  //  Serial.print(rightB);
  //   Serial.print("app");
  //    Serial.print(app);
  //    Serial.print("n");
  //    Serial.print(newMin);
  //        Serial.print("op");
  //    Serial.print(oldMin);
  //     Serial.println(activeTime);

}
void refresh() {//contral gui
  if (touch == 0) {
    switch (app) {
      case 0 :
        newMin = rtc.getMinute();
        if (oldMin != newMin || oldApp != app) {
          tprintClock();
          bgp.pushSprite(0,0);
          oldMin = newMin;
          oldApp = app;
        }
        break;
      case 1 :
        //Serial.print(rightB);
        /////////set up
        if (oldApp != app) {
          bgp.pushImage(0, 0,  135, 240, bootlogo);
          printCursor();
          printApp();
          oldApp = app;
        }
        //////loop 

        }
        break;
    }
   bgp.pushSprite(0,0); 
  }
  else {
  }
}
void thread() {////control variables and sensors
  if (rightB == 0 || leftB == 0) {
    activeTime = millis();
  }
  switch (app) {
    case 0 :
      if (rightB == 0) {
        Serial.print(rightB);
        app = 1;


      }
      break;
    case 1 :
      if (leftB == 0) {
        app = 0;
      }
      break;
  }
  if (millis() - activeTime > 5000) {
    digitalWrite(4, LOW); // Should force backlight off
    tft.writecommand(ST7789_DISPOFF);// Switch off the display
    tft.writecommand(ST7789_SLPIN);// Sleep the display driver
    esp_deep_sleep_start();
  }
}

void printClock(){
  oldMin = rtc.getMinute();
  oldMin = newMin;
  clockNum.setFreeFont(FreeSans50pt7b);
  //tft.setCursor(20, 50);
   
  clockNum.setTextColor( TFT_WHITE);
  //tft.println(String(rtc.getHour(true)));
  if (rtc.getHour(true) < 10) {
  clockNum.drawString("0"+String(rtc.getHour(true)),20,50);
  } else {
   clockNum.drawString(String(rtc.getHour(true)).20,50);
  }
  //tft.setCursor(20, 130);
  //tft.setTextSize(4);
   if (rtc.getMinute(true) < 10) {
  clockNum.drawString("0"+String(rtc.getHour(true)),20,130);
  } else {
   clockNum.drawString(String(rtc.getHour(true)).20,130);
  }
  clockNum.pushToSprite(&bgp, 0, 0, TFT_BLACK);
}
void printCursor(){///////app and cursor, may have many pages
          if (oldCursor!= cursor||oldApp!=app){
            switch (app){
              case 1:
              icon.pushImage(67 - 32, 44 - 32, gameIcon);
              icon.pushImage(67 - 32, 120 - 32, clockIcon);
              icon.pushImage(67 - 32, 196 - 32, settingsIcon);
            }
              break;
            }
            icon.pushSprite(&bgp,0,0, TFT_BLACK);
}
void printApp(){
  switch (cursor){
    case 0:
    icon.pushImage(67 - 32, 196 - 32, cursorIcon);
    break;
     case 1:
    icon.pushImage(67 - 32, 196 - 32, cursorIcon);
    break;
     case 2:
icon.pushImage(67 - 32, 196 - 32, cursorIcon);
    break;
     case 3:
icon.pushImage(67 - 32, 196 - 32, cursorIcon);
    break;

}
