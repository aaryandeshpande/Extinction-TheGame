#include <TFT_eSPI.h>
#include "dinob.h"
#include "dinof.h"
#include "cloud.h"
#include "volcano1.h"
#include "volcano2.h"

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

//define colour hex-codes
#define SKY_BLUE 0xbeff
#define GRASS 0x458b

int x = 5;
int endgame_counter;
int pause_counter;
int score = 0;
int screen_score = 0;
int compare_score = 0;
int high_score = 0;
int dinob_y = 175;

//Dino jumping variables
int dino_Yloc = 175;
int fall_rate = 30;  //18

//Cloud variables
int cld1_x = 10;
int cld2_x = 180;
int cld3_x = 380;

//object Varianles
int volc_x1 = 480;
int volc_x2 = 480;

//Game initialize
void game_init(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.drawCentreString("From WAMA. Co",240,160,1);
  delay(2000);
}


void title_screen(){
  tft.fillScreen(SKY_BLUE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(3);
  tft.drawCentreString("Extinction!",240,140,1);

  tft.setCursor(200, 250);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("High Score: "), tft.print(high_score);

  while (x==5){
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawCentreString("Hold START to play",240,180,1);
    delay(500);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawCentreString("Hold START to play",240,180,1);
    delay(500);
    int start_button = digitalRead(9);
    if (start_button == HIGH){
      break;
    }
  }
}


//Grass blades
void grass(){
  tft.setCursor(50,285);
  tft.setTextColor(TFT_DARKGREEN);
  tft.setTextSize(2);
  tft.println("^        ^        ^        ^       ^        ^        ^        ^       ^");
}


//Moving clouds
void sky(){
  tft.pushImage(cld1_x, 15, cloud_width, cloud_height, cloud);
  cld1_x-=3;
  if (cld1_x<-80){
    cld1_x= 480;
  }
  tft.pushImage(cld2_x, 20, cloud_width, cloud_height, cloud);
  cld2_x-=3;
  if (cld2_x<-80){
    cld2_x= 480;
  }
  tft.pushImage(cld3_x, 25, cloud_width, cloud_height, cloud);
  cld3_x -=3;
  if (cld3_x<-80){
    cld3_x = 480;
  }
}


//Running dino animation
void blue_dinorun(){
  tft.pushImage(10, dinob_y, 99, 105, dinob); //dinob_y = 175
  sky();
  tft.pushImage(10, 255, 99, 25, dinof); 
}


//Jumping
void jump_gravity(){
  //going up
  while (x==5){
    tft.pushImage(10,dino_Yloc,99,105,dinob);
    sky();
    objects();
    dino_Yloc -= fall_rate;
    tft.fillRect(10,dino_Yloc+105,99,fall_rate,SKY_BLUE);
    fall_rate -=10; //3
    if (fall_rate==0){
      break;
    }
  }

  //going down
  while(x==5){
    tft.pushImage(10,dino_Yloc,99,105,dinob);
    sky();
    objects();
    tft.fillRect(10,dino_Yloc,99,fall_rate,SKY_BLUE);
    dino_Yloc += fall_rate;
    fall_rate +=10; //3
    if (fall_rate==40){
      fall_rate=30;
      dino_Yloc = 175;
      break;
    }
  }
  if ((dino_Yloc > 112) && (volc_x1 == 0)){
    endgame_counter = 1;
  }
}


//Obstacles
void objects(){
  tft.pushImage(volc_x1,253,60,37,volc1);
  tft.fillRect(volc_x1+60,253,30,27,SKY_BLUE);
  tft.fillRect(volc_x1+60,280,30,10,GRASS);
  grass();

  volc_x1 -=30;

  if (volc_x1<-60){.     //randomize 60 to determine when volc2 prints (but volc1 will always be constant)
    volc_x1 = 480;
    screen_score+=1;
    compare_score+=1;
    score+=1;
    tft.setCursor(360, 2);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.fillRect(440,0,40,20,SKY_BLUE);
    tft.print("Score: "), tft.print(screen_score);
  }
}

void gameplay(){
  tft.fillRect(0,280,480,100, GRASS);
  tft.fillRect(0,0,480,280,SKY_BLUE);

  tft.setCursor(50,285);
  tft.setTextColor(TFT_DARKGREEN);
  tft.setTextSize(2);
  tft.println("^        ^        ^        ^       ^        ^        ^        ^       ^");

  tft.setCursor(360, 2);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.print("Score: 0");

  while (x==5) {
    blue_dinorun();
    objects();

    int jump_button = digitalRead(8);
    if (jump_button==HIGH){
      jump_gravity();
    }

    int pause_button = digitalRead(11);
    if (pause_button == HIGH){
      delay(1000);
      pause();
    }

    if (volc_x1 < 81){
      screen_score = 0;
      endgame_counter = 1;
    }
    while (endgame_counter == 1){
      game_over();
    }

    int start_button = digitalRead(9);
    if (start_button == HIGH){
      break;
    }
  }
  volc_x1 = 480;
}


void pause(){
  while (x==5){
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.drawCentreString("PAUSED",240,160,1);
    
    int pause_button = digitalRead(11);
    if (pause_button == HIGH){
      tft.fillRect(110,160,255,25,SKY_BLUE);
      break;
    }
  }
}

void game_over(){
  if (compare_score > high_score){
    high_score = score;
    score = 0;
    compare_score = 0;
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.drawCentreString("NEW HIGH SCORE",240,160,1);
    delay(3000);
    tft.fillRect(110,160,255,25,SKY_BLUE);
  }
  else if (compare_score <= high_score){
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.drawCentreString("GAME OVER",240,160,1);
    delay(500);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.drawCentreString("GAME OVER",240,160,1);
    delay(500);
    tft.setTextSize(1);
    tft.drawCentreString("Hold START to return to the menu",240,210,1);
  }

  int start_button = digitalRead(9);
  if (start_button == HIGH){
    endgame_counter = 0;
  }
}


//MAIN CODE//
void setup(){
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  int jump_button = digitalRead(8);
  int start_button = digitalRead(9);
  int pause_button = digitalRead(11);

  tft.init();
  tft.setRotation(1);

  game_init();
  delay(5000);
}

void loop() {
  title_screen();
  gameplay();
}