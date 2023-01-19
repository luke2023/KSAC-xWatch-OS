//#include<AudioTools.h>
/*xwatch os 3.6.1
 * Fixed watchdog bug and hyper thread bug
 *
 *
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Made by 林澈 (LC2716)
 */
#include "soc/rtc_wdt.h"
#include "Smooth_Fonts.h"
float intime;
#include <Wire.h>
//#include "MAX30105.h
#include <WiFi.h>
#include "time.h"
//240x135
#include <ESP32Time.h>
const char * ssid = "Buffalo-G-F560";
const char * wifipw = "";
//const char * ssid = "Lin-4F";
//const char * wifipw = "123456";
//MAX30105 p;
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
#include "Main_Back_pic.h"
#define TFT_GREY 0x5AEB
#define lightblue 0x2D18
#define orange 0xFB60
#define purple 0xFB9B
float ys = 1;
float newsettime = 0.0;
float oldsettime = 0.0;
float x = random(30, 100); //coordinates of ball
float y = 70;
int ny = y; //coordinates of previous position
int nx = x;

float px = 45; //67 je sredina    pozicija igrača
int   pxn = px;
int vrije[2] = {1, -1};
int enx[16] = {8, 33, 58, 83, 108, 8, 33, 58, 83, 108, 22, 47, 72, 97, 47, 72};
int eny[16] = {37, 37, 37, 37, 37, 45, 45, 45, 45, 45, 53, 53, 53, 53, 61, 61};
int enc[16] = {TFT_RED, TFT_RED, TFT_RED, TFT_RED, TFT_RED, TFT_GREEN, TFT_GREEN, TFT_GREEN, TFT_GREEN, TFT_GREEN, orange, orange, orange, orange, lightblue, lightblue};
int score = 0;
int level = 1;
bool failed = 0;
float amount[4] = {0.25, 0.50, 0.75, 1};
float xs = amount[random(4)] * vrije[random(2)];
int fase = 0;
ESP32Time rtc;  // offset in seconds GMT+1
RTC_DATA_ATTR int don = 0;
RTC_DATA_ATTR long lastepoc = 0;
float last = 0.0;
TaskHandle_t ntp;
unsigned long getTimeepoc() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void setTimezone(String timezone) {
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1); //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void initTime(String timezone) {
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
  if (!getLocalTime(&timeinfo)) {
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time 1");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}


void stopWifi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  struct tm tm;

  tm.tm_year = yr - 1900;   // Set date
  tm.tm_mon = month - 1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;      // Set time
  tm.tm_min = minute;
  tm.tm_sec = sec;
  tm.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&tm);
  Serial.printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}






/////////////////////////////////////////////////////////////
int bright = 255;
//tft.setTextFont(7);
void setup(void) {
  rtc_wdt_protect_off();
  rtc_wdt_disable();
  xTaskCreatePinnedToCore(
    toNtp,   /* Task function. */
    "ntp",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &ntp,      /* Task handle to keep track of created task */
    0);

  //  p.setup();
  //  delay(100);
  //  p.shutDown();
  ledcSetup(0, 5000, 8);
  ledcAttachPin(4, 0);
  Serial.begin(115200);
 Serial.print("loop running on core ");
    Serial.println(xPortGetCoreID());
  if (don == 0) {
    //rtc.setTime(50, 17, 0, 13, 7, 2022); // 17th Jan 2021 15:24:30
    setTimezone("CST-8");

    setTime(2021, 10, 31, 0, 59, 50, 0);
    //rtc.setTime( getTimeepoc() );

    don = 1;
  }
  else
  {
    setTimezone("CST-8");
  }
  
  Serial.println("rtctime");
  Serial.println(rtc.getTime("RTC0: %A, %B %d %Y %H:%M:%S"));
  Serial.println("locotime");
  printLocalTime();
  rtc.setTime(getTimeepoc()  );
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  esp_sleep_enable_ext1_wakeup(0x800000000, ESP_EXT1_WAKEUP_ALL_LOW);
  pinMode(0, INPUT);
  pinMode(35, INPUT);
  tft.init();
  tft.setRotation(0);
  // setCpuFrequencyMhz(240);
  //Serial.println(getCpuFrequencyMhz());
  //function takes the following frequencies as valid values:
  //  240, 160, 80    <<< For all XTAL types
  //  40, 20, 10      <<< For 40MHz XTAL
  //  26, 13          <<< For 26MHz XTAL
  //  24, 12          <<< For 24MHz XTAL
  tft.setSwapBytes(true);
  tft.pushImage(0, 0,  135, 240, bootlogo);
  //tft.fillScreen(TFT_BLACK);
  ledcWrite(0, bright);
  intime = millis();
  oldsettime = rtc.getMinute();
  newsettime = rtc.getMinute();
  //  tft.clear()
  //tft.init();
  //tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0,  135, 240, bootlogo);
  tft.loadFont(smooth);
  tft.setCursor(20, 50);
  tft.setTextColor( TFT_WHITE); //tft.setTextFont(4);
  //tft.setTextSize(100);

  if (rtc.getHour(true) < 10) {

    tft.println("0" + String(rtc.getHour(true)));

  } else {
    tft.println(String(rtc.getHour(true)));
  }

  tft.setCursor(20, 130);
  tft.setTextSize(4);

  if (rtc.getMinute() < 10) {

    tft.println("0" + String(rtc.getMinute()));
    oldsettime = newsettime;
    last = millis();
  }
  else {
    tft.println(String(rtc.getMinute()));
    oldsettime = newsettime;
  }
  tft.unloadFont();
  //tft.setTextFont(7);
  tft.setCursor(0, 10);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); tft.setTextFont(2);
  tft.setTextSize(1);
  tft.println((((float)(analogRead(34)) * 3600 / 4095000 * 2 - 3.6) / 0.67) * 100);

}

float xstep = 1;
int spe = 0;
int pom = 0;
int gameSpeed = 6100;
int con;
//
//int lonprele=0;
//int lonpreri=0;
//int shoprele=0;
//int shopreri=0;
//float pretimele=0;
//float pretimeri=0;
//
//int last
void loop() {
  // printLocalTime();Serial.print(WiFi.status());
  ledcWrite(0, bright);
  if (millis() - last > 30000 && fase != 1) {
    digitalWrite(4, LOW); // Should force backlight off

    tft.writecommand(ST7789_DISPOFF);// Switch off the display

    tft.writecommand(ST7789_SLPIN);// Sleep the display driver
    esp_deep_sleep_start();


  }

  newsettime = rtc.getMinute();
  float VBAT = (float)(analogRead(34)) * 3600 / 4095000 * 2;
  if ( VBAT > 4.7) {
    con = 1;
  }
  else {
    con = 0;
  }
  if (millis() - intime > 1000) {
    tft.unloadFont();
    if (con == 0) {

      tft.setCursor(0, 10);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      //tft.setTextColor(TFT_BLACK);
      //tft.setTextFont(7);
      tft.setTextSize(1);
      tft.println(((VBAT - 3.6) / 0.67) * 100);
      intime = millis();
    }
    else {
      tft.setCursor(0, 10);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK); tft.setTextFont(2);
      tft.setTextSize(1);
      tft.println("connected");
      tft.setCursor(0, 10);


    }

  }
  if (digitalRead(0) == 0 || digitalRead(35) == 0)
  {
    last = millis();
  }
  if (fase == 0)
  {
    if (oldsettime != newsettime) {
      ////      tft.clear() ;
      //tft.fillScreen(TFT_BLACK);
      tft.loadFont(smooth);
      tft.pushImage(0, 0,  135, 240, bootlogo);
      //delay(3000);
      tft.setCursor(20, 50);
      tft.setTextColor( TFT_WHITE); //tft.setTextFont(4);
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
        oldsettime = newsettime;
        last = millis();
      }
      else {
        tft.println(String(rtc.getMinute()));
        oldsettime = newsettime;
      }
      tft.unloadFont();
    }
    if (digitalRead(0) == 0 || digitalRead(35) == 0)
    {
      if (pom == 0) {
        tft.fillScreen(TFT_BLACK);
        tft.drawLine(0, 17, 0, 240, TFT_GREY);
        tft.drawLine(0, 17, 135, 17, TFT_GREY);
        tft.drawLine(134, 17, 134, 240, TFT_GREY);



        tft.setCursor(3, 3, 2);

        tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(1);

        tft.setCursor(0, 0, 2);
        tft.println("SCORE " + String(score));

        tft.setCursor(99, 0, 2);
        tft.println("LVL" + String(level));
        fase = fase + 1;
        pom = 1;
      }

    } else {
      pom = 0;
    }
  }

  if (fase == 1) {

    if (y != ny) {
      tft.fillEllipse(nx, ny, 2, 2, TFT_BLACK);   //brisanje loptice
      ny = y;
      nx = x;
    }
    if (int(px) != pxn) {
      tft.fillRect(pxn, 234, 24, 4, TFT_BLACK); //briasnje igraca
      pxn = px;
    }


    // spe=spe+1;

    if (px >= 2 && px <= 109) {
      if (digitalRead(0) == 0)
        px = px - 1;
      if (digitalRead(35) == 0)
        px = px + 1;
    }
    if (px <= 3)
      px = 4;

    if (px >= 108)
      px = 107;

    if (y > 232 && x > px && x < px + 24) { ///brisati kasnije
      ys = ys * -1;

      xs = amount[random(4)] * vrije[random(2)];


    }

    for (int j = 0; j < 16; j++)
    {
      if (x > enx[j] && x < enx[j] + 20 && y > eny[j] && y < eny[j] + 5)
      {
        tft.fillRect(enx[j], eny[j], 20, 4, TFT_BLACK);
        enx[j] = 400;
        ys = ys * -1;
        xs = amount[random(4)] * vrije[random(2)];

        score = score + 1;

        tft.setCursor(0, 0, 2);
        tft.println("SCORE " + String(score));




      }
    }

    if (y < 21)
      ys = ys * -1.00;

    if (y > 240)
      fase = fase + 1;



    if (x >= 130)
      xs = xs * -1.00;

    if (x <= 4)
      xs = xs * -1.00;


    for (int i = 0; i < 16; i++) //draw enemies
      tft.fillRect(enx[i], eny[i], 20, 4, enc[i]);


    tft.fillEllipse(int(x), y, 2, 2, TFT_WHITE); // draw ball

    //if(spe>10){  //change coordinates of ball
    y = y + ys;
    x = x + xs;
    //spe=0;
    //}

    tft.fillRect(px, 234, 24, 4, TFT_WHITE);

    if (score == 16 || score == 33 || score == 50 || score == 67 || score == 84 || score == 101 || score == 118 || score == 135 || score == 152 || score == 169)
      newLevel();

    //vTaskDelay(gameSpeed / 1 * portTICK_PERIOD_MS);

    delayMicroseconds(gameSpeed);
  }
  if (fase == 2)
  {

    tft.fillScreen(TFT_BLACK);




    tft.setCursor(13, 103, 2);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(1);

    tft.println("GAME OVER");
    tft.setCursor(13, 123, 4);
    tft.println("SCORE:" + String(score));

    tft.setCursor(13, 153, 4);
    tft.println("LEVEL:" + String(level));

    tft.setCursor(13, 123, 4);
    tft.println("SCORE:" + String(score));

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  if (fase == 2 && digitalRead(35) == 0) {
    esp_sleep_enable_timer_wakeup(1);
    esp_deep_sleep_start();
  }
  if (fase == 2 && digitalRead(0) == 0) {
    digitalWrite(4, 0);
    digitalWrite(4, LOW); // Should force backlight off

    tft.writecommand(ST7789_DISPOFF);// Switch off the display

    tft.writecommand(ST7789_SLPIN);// Sleep the display driver
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_deep_sleep_start();




  }
}

void newLevel()
{
  score = score + 1;
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  if (gameSpeed - 500 > 0) {
    gameSpeed = gameSpeed - 500;
  }
  level = level + 1;
  tft.setCursor(99, 0, 2);
  tft.println("LVL" + String(level));
  y = 75;
  ys = 1;
  x = random(30, 100);

  int enx2[16] = {8, 33, 58, 83, 108, 8, 33, 58, 83, 108, 22, 47, 72, 97, 47, 72};
  for (int n = 0; n < 16; n++)
  {
  }
}
void toNtp( void * pvParameters ) {
   Serial.print("timer running on core ");
    Serial.println(xPortGetCoreID());
  while (1) {
   
    if (rtc.getEpoch() - lastepoc > 36000000 && failed == 0) {
      int contime = millis();
      WiFi.begin(ssid, wifipw);
      Serial.println("Connecting Wifi");
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (millis() - contime > 5000) {

          failed = 1;
          break;
        }
      }
      if (WiFi.status() == WL_CONNECTED) {


        Serial.print("Wifi RSSI=");
        Serial.println(WiFi.RSSI());

        Serial.println("wiok");
        initTime("CST-8");

        Serial.println("inok");
        printLocalTime();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        stopWifi();
        rtc.setTime(getTimeepoc()  );
        Serial.println("rtctime");
        Serial.println(rtc.getTime("RTC0: %A, %B %d %Y %H:%M:%S"));
        Serial.println(rtc.getEpoch());
        Serial.println(lastepoc);
        lastepoc = rtc.getEpoch();
      }
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
