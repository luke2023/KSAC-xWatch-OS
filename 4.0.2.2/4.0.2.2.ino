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

button:
1 single
2 double
3 triple
4 long
*/

#include <TFT_eSPI.h>
#include <SPI.h>
#include <ESP32Time.h>
#include "icons.h"
#include "Main_Back_pic.h"
#include "FreeSans50pt7b.h"
#include "Button2.h"
#define BUTTON_A_PIN 0
#define BUTTON_B_PIN 35

int app, oldCursor, cursor, foor, touch, oldMin, newMin, leftB, rightB, activeTime, oldApp, pressTimeA, pressTimeB;
int iconX = 67 - 32;
int iconY = 44;
int iconY1 = 44 + 76;
int iconY2 = 44 + 76 + 76;
Button2 buttonA, buttonB;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite icon = TFT_eSprite(&tft);
TFT_eSprite clockNum = TFT_eSprite(&tft);
TFT_eSprite bgp = TFT_eSprite(&tft);  //back ground picture
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
  buttonA.begin(BUTTON_A_PIN);
  buttonB.begin(BUTTON_B_PIN);
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
  bgp.createSprite(135, 240);
  icon.createSprite(64, 64);
  clockNum.createSprite(135, 240);
  bgp.setSwapBytes(true);
  icon.setSwapBytes(true);
  clockNum.setSwapBytes(true);
  bgp.pushImage(0, 0, 135, 240, bootlogo);


  //  printSprite(67, 44, gameIcon);
  //  printSprite(67, 120, clockIcon);
  //  printSprite(67, 196, settingsIcon);

  printClock();
  bgp.pushSprite(0, 0);
}

void loop() {
  buttonA.loop();
  buttonB.loop();
  thread();  /////sensor
  printClock();
  refresh();  ////gui and check old app

  Serial.print(leftB);
  Serial.print(rightB);
  Serial.print("app");
  Serial.println(app);
  
}
void refresh() {  //contral gui
  if (touch == 0) {
    switch (app) {
      case 0:
        newMin = rtc.getMinute();
        if (oldMin != newMin || oldApp != app) {
          printClock();
          bgp.pushSprite(0, 0);
          oldMin = newMin;
          oldApp = app;
        }
        break;
      case 1:
        //Serial.print(rightB);
        /////////set up
        if (oldApp != app || oldCursor != cursor) {
          bgp.pushImage(0, 0, 135, 240, bootlogo);
          printCursor();
          printApp();
          icon.pushToSprite(&bgp, 0, 0, TFT_BLACK);
          oldApp = app;
          bgp.pushSprite(0, 0);
        }
        //////loop
        break;
    }

  } else {
  }
}


void thread() {  ////control variables and sensors
  checkSleep();
  buttonRefresh();
  switch (app) {
    case 0:
      if (rightB == 1) {
        Serial.print(rightB);
        switchApp(1);
        
      }
      break;
    case 1:  ///app1 menu
      if (leftB != 0) {
        switch (leftB) {
          case 1:
            switchCursor(0);
            break;
          case 2:
            switchApp(0);
            break;
        }
       
      }
      if (rightB != 0) {
        switch (rightB) {
          case 1:
            switchCursor(1);
            
            break;
          case 2:
            switchApp(1);
            break;
        }
       
      }
  }
}
void printClock() {
  oldMin = rtc.getMinute();
  oldMin = newMin;
  clockNum.setFreeFont(&FreeSans50pt7b);
  //tft.setCursor(20, 50);

  clockNum.setTextColor(TFT_WHITE);
  //tft.println(String(rtc.getHour(true)));
  if (rtc.getHour(true) < 10) {
    clockNum.drawString("0" + String(rtc.getHour(true)), 20, 50);
  } else {
    clockNum.drawString(String(rtc.getHour(true)), 20, 50);
  }
  //tft.setCursor(20, 130);
  //tft.setTextSize(4);
  if (rtc.getMinute() < 10) {
    clockNum.drawString("0" + String(rtc.getMinute()), 20, 130);
  } else {
    clockNum.drawString(String(rtc.getMinute()), 20, 130);
  }
  clockNum.pushToSprite(&bgp, 0, 0, TFT_BLACK);
}
void printCursor() {  ///////app and cursor, may have many pages

  icon.fillSprite(TFT_BLACK);

  switch (cursor) {
    case 0:
      icon.pushImage(iconX - 4, iconY - 4, iconX - 4 + 72, iconY - 4 + 72, cursorIcon);
      break;
    case 1:
      icon.pushImage(iconX - 4, iconY1 - 4, iconX - 4 + 72, iconY1 - 4 + 72, cursorIcon);
      break;
    case 2:
      icon.pushImage(iconX - 4, iconY2 - 4, iconX - 4 + 72, iconY2 - 4 + 72, cursorIcon);
      break;
    case 3:
      icon.pushImage(iconX - 4, iconY2 - 4, iconX - 4 + 72, iconY2 - 4 + 72, cursorIcon);
      break;
  }
  icon.pushToSprite(&bgp, 0, 0, TFT_BLACK);
}
void printApp() {
  icon.fillSprite(TFT_BLACK);
  switch (cursor) {
    case 1:
      icon.pushImage(iconX, iconY, iconX + 64, iconY + 64, gameIcon);
      icon.pushImage(iconX, iconY1, iconX + 64, iconY1 + 64, clockIcon);
      icon.pushImage(iconX, iconY2, iconX + 64, iconY2 + 64, settingsIcon);
      break;
  }
  icon.pushToSprite(&bgp, 0, 0, TFT_BLACK);
}
void buttonRefresh() {
  if (buttonA.isPressed() == 1) {
    if (millis() - pressTimeA > 1000) {
      pressTimeA = millis();
      leftB = 4;
    }
  } else {
    pressTimeA = millis();
  }
  if (buttonB.isPressed() == 1) {
    if (millis() - pressTimeB > 1000) {
      pressTimeB = millis();
      rightB = 4;
    }
    // }
  } else {
    //first=1;
    pressTimeB = millis();
  }
  if (buttonA.wasPressed()) {
  switch (buttonA.read()) {
      case single_click:
        Serial.println("single");
        leftB = 1;
        break;
      case double_click:
        Serial.println("double");
        leftB = 2;
        break;
      case triple_click:
        Serial.println("triple");
        leftB = 3;
        break;
    }
  }
  if (buttonB.wasPressed()) {
    switch (buttonB.read()) {
      case single_click:
        Serial.println("single");
        rightB = 1;
          while(1){
    Serial.println("singleeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
  }
    
        break;
      case double_click:
        Serial.println("double");
        rightB = 2;
        break;
      case triple_click:
        Serial.println("triple");
        rightB = 3;
        break;
    }
  }
}
void switchApp(bool n) {
  if (n) {
    if (app < 4) {
      app++;
    }
  } else {
    if (app > 0) {
      app--;
    }
  }
  rightB=0;
  leftB=0;
}  //1=++
void switchCursor(bool n) {
  if (n) {
    if (cursor < 4) {
      cursor++;
    }
  } else {
    if (cursor > 0) {
      cursor--;
    }
  }
    rightB=0;
  leftB=0;
}
void checkSleep() {
  if (millis() - activeTime > 100000) {
    digitalWrite(4, LOW);              // Should force backlight off
    tft.writecommand(ST7789_DISPOFF);  // Switch off the display
    tft.writecommand(ST7789_SLPIN);    // Sleep the display driver
    esp_deep_sleep_start();
  }
  if (rightB > 0 || leftB > 0) {
    activeTime = millis();
  }
}
