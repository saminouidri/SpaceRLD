/* Name : Space Reloaded
 * Author : Sami N. / Thibert B.
 * Description : Space invaders style game on 8x8 matrix
 * 
 */
#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include "FctAntirebond.h"
#include <LedControl.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#define NOTE_C4  262   //Defining note frequency
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988

#define SCL A5
#define SDA A4

#define MILLIS 10

#define OFF 0x0000 // Black

const int buzzer = 5;

int intronotes[] = {       //Note of the song, 0 is a rest/pulse
   NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_B4, 
   0, NOTE_C5, NOTE_D5, 0, NOTE_G4, NOTE_A4, 
   NOTE_G4, 0, NOTE_B4, NOTE_B4, 0, NOTE_C5, NOTE_E5};

int introduration[19] = {0};

int mainduration[25] = {0};

int mainnotes[] = {       //Note of the song, 0 is a rest/pulse
   NOTE_C5, 0, NOTE_D5, NOTE_E5, NOTE_E5, 0, NOTE_C5,
   0, NOTE_E5, NOTE_C5, NOTE_B4, 0, NOTE_C5, 0, NOTE_D5,
   NOTE_E5, NOTE_E5, 0, NOTE_C5, 0, NOTE_D5, NOTE_C5,
   NOTE_B4, NOTE_E4, NOTE_B4};


 
byte test[8] = {0b10000000};

 

byte intro1[]={0b00000000,
0b00001000,
0b00011000,
0b00101000,
0b00001000,
0b00001000,
0b00000000,
0b00000000};

byte intro2[]={0b00000000,
0b00111100,
0b00000100,
0b00111100,
0b00100000,
0b00111100,
0b00000000,
0b00000000};

byte intro3[]={0b00000000,
0b00111100,
0b00000100,
0b00111100,
0b00000100,
0b00111100,
0b00000000,
0b00000000};

byte bossintro[]={0b10100101,
0b01000010,
0b10100101,
0b00000000,
0b10000001,
0b10000001,
0b01000010,
0b00111100};

byte bossframe1[]={0b00011000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000};

byte bossframe2[]={0b00100100,
0b00011000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000};

byte bossframe3[]={0b01011010,
0b00100100,
0b00011000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000};

byte bossframe4[]={0b10011001,
0b01011010,
0b00100100,
0b00011000,
0b00000000,
0b00000000,
0b00000000,
0b00000000};

byte boss[]={0b11111111,
0b10011001,
0b01011010,
0b00100100,
0b00011000,
0b00000000,
0b00000000,
0b00000000};

byte level1[]={0b10000000,
0b00100000,
0b00001000,
0b00000010,
0b00000001,
0b00000000,
0b00000000,
0b00000000};

byte level2[]={0b01000000,
0b10000000,
0b00100000,
0b00001000,
0b00000010,
0b00010000,
0b00000000,
0b00000000};

byte win[]={0b00000000,
0b01100110,
0b01100110,
0b00000000,
0b00000000,
0b10000001,
0b01000010,
0b00111100};


int VRx = A1;
int VRy = A0;
int SW = 7;

int xPosition = 0;
int xTr=0;
int yPosition = 0;
int yTr=0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
int HP=10;

int CurrentLevel = 1;

unsigned long lMillis;  // Compteur de millisecondes
Adafruit_BicolorMatrix SCR = Adafruit_BicolorMatrix();

static int iCpt = 0;

static int iCpt2 = 0;
        

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Matrix");
  SCR.clear();
  for(int dura=0;dura<19;dura++)
  {
    introduration[dura] = 125;//duration of each note (in ms) Quarter Note is set to 250 ms
  }
  for(int dura=0;dura<25;dura++)
  {
   mainduration[dura] = 125;//duration of each note (in ms) Quarter Note is set to 250 ms
  }
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);
  lMillis = millis() + MILLIS;
  SCR.begin(0x70);  // pass in the address
  SCR.setTextSize(1);
  SCR.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  SCR.setTextColor(LED_ON);
  IntroMusic();
  for (int8_t x=0; x>=-78; x--) {
    SCR.clear();
    SCR.setCursor(x,0);
    SCR.print("Space RLD");
    SCR.writeDisplay();  
    delay(100);
  }
  SCR.drawBitmap(0, 0, &intro1[0], 8, 8, LED_GREEN);
  SCR.writeDisplay();
  delay(300);
  SCR.drawBitmap(0, 0, &intro1[0], 8, 8, OFF);
  SCR.writeDisplay();
  delay(300);
  SCR.drawBitmap(0, 0, &intro1[0], 8, 8, LED_GREEN);
  SCR.writeDisplay();
  delay(300);
  SCR.drawBitmap(0, 0, &intro1[0], 8, 8, OFF);
  SCR.writeDisplay();
  delay(300);
  SCR.drawBitmap(0, 0, &intro1[0], 8, 8, LED_GREEN);
  SCR.writeDisplay();
  delay(300);
  SCR.drawBitmap(0, 0, &intro1[0], 8, 8, OFF);
  SCR.writeDisplay();
  xTr = xTr + 5;
  yTr = yTr + 4;
  SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
  SCR.writeDisplay();
  //SCR.setCursor(4, 4);
  
}

void loop() {
  
    
    //SCR.writeDisplay();
  
  
  //SCR.clear();
  
  while (lMillis >= millis()); // Attente de "MILLIS" ms
  lMillis = lMillis + MILLIS; // Actualise la variable pour le tour suivant
  
  Joystick(); 
  //MainMusic();
  
  //const static int xTir = xTr;
  if(SW_state == 0)
  {
      
   int xTir = xTr;
    
    int yTir =yTr-1;
    Serial.println("tir");
    
    /*for(;yTir <= -2;yTir--)
    {
      Serial.println("boucle");
      SCR.drawBitmap(xTr, yTir, &test[0], 8, 8, LED_RED);
      SCR.writeDisplay();
    }*/
    
    for(int x = yTir; x > -2; x--){
      
    Serial.println("boucle");
    Joystick();
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
    SCR.drawBitmap(xTir, x, &test[0], 8, 8, LED_RED);
    SCR.writeDisplay();
    delay(20);  // increase to make the lin move slower.  Decrease to move faster
    SCR.drawBitmap(xTir, x, &test[0], 8, 8, OFF);
    CurrentLevelCollisions(CurrentLevel, xTir, x);
    }  
    
  }

  if(CurrentLevel == 3 && iCpt == 5)
  {
    iCpt = 0;
    int posb = 2;
    for(int xb = 0; xb < 9 ; xb++)
    {
      
      for(int yb = posb; yb < 8; yb++){
      Joystick();
      
      
      SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
      SCR.drawBitmap(xb, yb, &test[0], 8, 8, LED_YELLOW);
      SCR.writeDisplay();
      delay(50);  // increase to make the lin move slower.  Decrease to move faster
      SCR.drawBitmap(xb, yb, &test[0], 8, 8, OFF);
      //CurrentLevelCollisions(CurrentLevel, xTir, x);
        if(xb == xTr && yb == yTr)
        {
              SCR.clear();
              SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, OFF);
              SCR.drawBitmap(0, 0, &bossintro[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &bossintro[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &bossintro[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              while(1);
        }
      }
      if(xb >= 4)
      {
        posb--;
      } 
      else if(posb != 5)
      {
         posb++;
      }
      
    }
    
  }
  
  
  delay(185);
   
  iCpt++;
  iCpt2++;
  if(iCpt2 == 26)
  {
    iCpt2 = 0;
  }
}

void Joystick()
{
  SCR.clear();
  DrawCurrentLevel(CurrentLevel);
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = digitalRead(SW);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, -512, 512);
  //SCR.drawPixel(mapX, mapY, LED_YELLOW);
  if(mapX < -50)
  {
   
      xTr--;
    if(xTr == -1)
    {
      xTr = 0;
    }
    else if(xTr == 8)
    {
      xTr = 7;
    }
    
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
    SCR.writeDisplay();
  }
  else if(mapX > 50)
  {
    
    xTr++;
    if(xTr == -1)
    {
      xTr = 0;
    }
    else if(xTr == 8)
    {
      xTr = 7;
    }
   
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
    SCR.writeDisplay();
  }
  else if(mapY < -50)
  {
    
   
    if(yTr == -1)
    {
      yTr = 0;
    }
    else if(yTr == 8)
    {
      yTr = 7;
    }
    else if(CurrentLevel == 3)
    {
      if(yTr != 5)
      {
        yTr--;
      }
       
    }
    else if(CurrentLevel != 3)
    {
      yTr--;
    }
    
    
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
    SCR.writeDisplay();
  }
  else if(mapY > 50)
  {
    
    yTr++;
    if(yTr == -1)
    {
      yTr = 0;
    }
    else if(yTr == 8)
    {
      yTr = 7;
    }
   
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
    SCR.writeDisplay();
  }
  else if(mapX < -50 && mapY < -50)
  {
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, OFF);
    SCR.writeDisplay();
  }
  else if(mapX > 50 && mapY > 50)
  {
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, OFF);
    SCR.writeDisplay();
  }
  else
  {
    SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
    SCR.writeDisplay();
  }
  
  
  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print(" | Button: ");
  Serial.println(SW_state);
  Serial.print(" | PosX: ");
  Serial.println(xTr);
  Serial.print(" | PosY: ");
  Serial.println(yTr);
  static int xTir = xTr;
}

void Inc(int pos)
{
  if(pos > 0 && pos < 7)
  {
    pos++;
  }
}
void Dec(int pos)
{
  if(pos > 0 && pos < 7)
  {
    pos--;
  }
}

int DrawCurrentLevel(int lvl)
{
  switch(lvl)
  {
            case 1 :
            SCR.drawBitmap(0, 0, &level1[0], 8, 8, LED_GREEN);
            SCR.writeDisplay();
            return 1;            
            break;
            
            case 2 :
            SCR.drawBitmap(0, 0, &level2[0], 8, 8, LED_GREEN);
            SCR.writeDisplay();
            return 1;
            break;

            case 3 :
            SCR.drawBitmap(0, 0, &boss[0], 8, 8, LED_GREEN);
            SCR.writeDisplay();
            return 1;
            break;
  }
  
}
void CurrentLevelCollisions(int lvl, int xTir, int x)
{
  switch(lvl)
  {
            case 1 :
            if(xTir == 7 && x == 4)
            {
              level1[4] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 6 && x == 3)
            {
              level1[3] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 4 && x == 2)
            {
              level1[2] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 2 && x == 1)
            {
              level1[1] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 0 && x == 0)
            {
              level1[0] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(level1[0] == 0b00000000 && level1[1] == 0b00000000 && level1[2] == 0b00000000 && level1[3] == 0b00000000 && level1[4] == 0b00000000)
            {
              SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, OFF);
              SCR.drawBitmap(0, 0, &intro2[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro2[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro2[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro2[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro2[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro2[0], 8, 8, OFF);
              SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
              SCR.writeDisplay();
              CurrentLevel = 2;
            }
            break;
            case 2 :
            if(xTir == 3 && x == 5)
            {
              level2[5] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 6 && x == 4)
            {
              level2[4] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 4 && x == 3)
            {
              level2[3] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 2 && x == 2)
            {
              level2[2] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 0 && x == 1)
            {
              level2[1] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(xTir == 1 && x == 0)
            {
              level2[0] = 0b00000000;
              DrawCurrentLevel(lvl);
              x = -3;
            }
            if(level2[0] == 0b00000000 && level2[1] == 0b00000000 && level2[2] == 0b00000000 && level2[3] == 0b00000000 && level2[4] == 0b00000000 && level2[5] == 0b00000000)
            {
              SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, OFF);
              SCR.drawBitmap(0, 0, &intro3[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro3[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro3[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro3[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro3[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &intro3[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              iCpt = 0;
              SCR.drawBitmap(0, 0, &bossframe1[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &bossframe2[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &bossframe3[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(100);              SCR.drawBitmap(0, 0, &bossframe4[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, LED_RED);
              SCR.writeDisplay();
              CurrentLevel = 3;
            }
            break;
            case 3:
            if((xTir == 0 && x == 1) || (xTir == 1 && x == 2) || (xTir == 2 && x == 3) || (xTir == 3 && x == 4) || (xTir == 4 && x == 4) || (xTir == 5 && x == 3) || (xTir == 6 && x == 2) || (xTir == 7 && x == 1))
            {
              Joystick();
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(300);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              HP--;
            }
            if(HP == 0)
            {
              
              Serial.println("death");
              Joystick();
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, LED_GREEN);
              SCR.writeDisplay();
              delay(100);
              SCR.drawBitmap(0, 0, &boss[0], 8, 8, OFF);
              SCR.writeDisplay();
              delay(100);
              for(int i=0;i<8;i++)
              {
                boss[i] = {0};
              }
              SCR.drawBitmap(xTr, yTr, &test[0], 8, 8, OFF);
              SCR.drawBitmap(0, 0, &win[0], 8, 8, LED_RED);
              SCR.writeDisplay();
              while(1);
              
            }
            break;
  }
}

void IntroMusic()
{
  for (int id=0;id<19;id++){              
  int wait = introduration[id] * 1.5;
  tone(buzzer,intronotes[id],wait);          
  delay(wait);                        
  }
}
void MainMusic(){

  for (int id=0;id<25;id++){             
  int wait = mainduration[id] * 1.5;
  tone(buzzer,mainnotes[id],wait);          
  delay(wait);}                      
}


