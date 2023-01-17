#include <TFT_eSPI.h>
#include "icons.h"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite icon = TFT_eSprite(&tft);
int x= 40;
void setSprite(int x,int y ,const unsigned short i[]){
  icon.pushImage(0,0,64,64,i);
icon.pushSprite(x-32,y-32,TFT_BLACK);
}
void setup() {
  // put your setup code here, to run once:
tft.init();
tft.setRotation(0);
tft.setSwapBytes(1);
tft.fillScreen(TFT_PURPLE);
icon.createSprite(64,64);
icon.setSwapBytes(true);
setSprite(67,44,game[0]);
pinMode(0,OUTPUT);
pinMode(35,OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
//if(digitalRead(0)==0){
//  x=x+1;
//icon.pushSprite(x,40,x+64,40+64,game);
//}
//if(digitalRead(35)==0){
//  x=x-1;
//  icon.pushSprite(x,40,x+64,40+64,game);
//}
}
