#include <TFT_eSPI.h>
#include <ESP32Time.h>
#include "icons.h"
#include "Main_Back_pic.h"
#include "Smooth_Fonts.h"
int app, foor, touch, oldMin, newMin, leftB, rightB, activeTime,done;
/*foor is in app
   app
   0 main
   1 menu
   2 settings

*/
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite icon = TFT_eSprite(&tft);
ESP32Time rtc(3600);
int x = 40;
void printSprite(int x, int y , const unsigned short i[]) {
  icon.pushImage(0, 0, 64, 64, i);
  icon.pushSprite(x - 32, y - 32, TFT_BLACK);
}
void setup() {
  Serial.begin(115200);

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
  tft.pushImage(0, 0,  135, 240, bootlogo);
  icon.createSprite(64, 64);
  icon.setSwapBytes(true);
//  printSprite(67, 44, gameIcon);
//  printSprite(67, 120, clockIcon);
//  printSprite(67, 196, settingsIcon);
  pinMode(0, INPUT);
  pinMode(35,INPUT);
  oldMin = rtc.getMinute();
  oldMin = newMin;
 Serial.print("ok1");

 tft.loadFont(smooth);
  Serial.print("okf");
  tft.pushImage(0, 0,  135, 240, bootlogo);
  Serial.print("ok2");
  tft.setCursor(20, 50);
  tft.setTextColor( TFT_WHITE);
  tft.setTextSize(100);
  //tft.println(String(rtc.getHour(true)));
  if (rtc.getHour(true) < 10) {

    tft.println("0" + String(rtc.getHour(true)));

  } else {
    tft.println(String(rtc.getHour(true)));
  }
  //delay(2000);
  tft.setCursor(20, 130);
  //tft.setTextSize(4);
  if (rtc.getMinute() < 10) {
    tft.println("0" + String(rtc.getMinute()));
  }
  else {
    tft.println(String(rtc.getMinute()));
  }
   Serial.print("ok2");

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
void refresh() {
  if (touch == 0) {
    switch (app) {
      case 0 :
        newMin = rtc.getMinute();
        if (oldMin != newMin||done==0) {
          tft.loadFont(smooth);
          tft.pushImage(0, 0,  135, 240, bootlogo);
          tft.setCursor(20, 50);
          tft.setTextColor( TFT_WHITE);
          tft.setTextSize(100);
          //tft.println(String(rtc.getHour(true)));
          if (rtc.getHour(true) < 10) {

            tft.println("0" + String(rtc.getHour(true)));

          } else {
            tft.println(String(rtc.getHour(true)));
          }
          //delay(2000);
          tft.setCursor(20, 130);
          //tft.setTextSize(4);
          if (rtc.getMinute() < 10) {
            tft.println("0" + String(rtc.getMinute()));
          }
          else {
            tft.println(String(rtc.getMinute()));
          }
          oldMin = newMin;
          done=1;
        }
        break;
      case 1 :
      //Serial.print(rightB);
      if(done==0){
        tft.pushImage(0, 0,  135, 240, bootlogo);
      }
        printSprite(67, 44, gameIcon);
        printSprite(67, 120, clockIcon);
        printSprite(67, 196, settingsIcon);
        break;
    }

  }
  else {
  }
}
void thread() {
  if (rightB ==0||leftB==0) {
    activeTime = millis();
  }
  switch (app) {
    case 0 :
      if (rightB == 0) {
        Serial.print(rightB);
        app = 1;
        done=0;
        
      }
      break;
    case 1 :
      if (leftB == 0) {
        app = 0;
         done=0;
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
