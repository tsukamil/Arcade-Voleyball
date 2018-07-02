//ostantnia wersja
#include <Arduboy2.h>
#include <ArduboyPlaytune.h>

Arduboy2 arduboy;
ArduboyPlaytune tunes(arduboy.audio.enabled);

const unsigned char player[] PROGMEM  = {
 0x00, 0xc0, 0x20, 0x10, 0x8, 0x4, 0x4, 0x4, 0x4, 0xc4, 0x24, 0xa8, 0xd0, 0x20, 0xc0, 0x00, 0x00, 0x7, 0x8, 0x10, 0x20, 0x40, 0x40, 0x40, 0x48, 0x54, 0x65, 0x25, 0x14, 0x8, 0x7, 0x00, 
};

const unsigned char opon[] PROGMEM = {
 0x00, 0xc0, 0xe0, 0x30, 0x58, 0xdc, 0x3c, 0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x3, 0xb, 0xb, 0x2a, 0x6a, 0x6b, 0x77, 0x7f, 0x7f, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x00, 
};

const unsigned char ball[] PROGMEM  = {
0x18, 0x66, 0x42, 0x81, 0x85, 0x4a, 0x66, 0x18, 
};



int playerX=10;
float playerY=48;
bool playerJump = false;
float playerYvelocity=0;
float gravity=0.1;
int playerScore=0;
bool didPlayerScore=false;
int playerTouches=0;
bool freePlayerTouch=true;

int ballX = 56;
float ballY = 10;
int ballXvelocity = 0;
float ballYvelocity = 0;
float ballGravity = 0;

int oponX = 96;
float oponY = 48;
int oponScore=0;
int oponSpeed=2;
float oponGravity=0.1;
bool oponJump = false;
float oponYvelocity=0;
int oponTouches=0;
bool freeOponTouch=true;

bool stop=true;
int randomNumber=0;

int pointsToWin=10;
bool startGame=false;
bool playerWin=false;
bool oponWin=false;

int menuChoice=1;
bool threeTouches=true;

void setup() {
arduboy.begin();
arduboy.setFrameRate(45);
arduboy.initRandomSeed();
randomNumber = 1 + rand() % 5; // inne ustawienie do pilki

// initialize things here
  // audio setup
  tunes.initChannel(PIN_SPEAKER_1);
#ifndef AB_DEVKIT
  // if not a DevKit
  tunes.initChannel(PIN_SPEAKER_2);
#else
  // if it's a DevKit
  tunes.initChannel(PIN_SPEAKER_1); // use the same pin for both channels
  tunes.toneMutesScore(true);       // mute the score when a tone is sounding
#endif

arduboy.audio.on();
}

void loop() {
if (!(arduboy.nextFrame()))
 return;
arduboy.pollButtons();
arduboy.clear();

if (startGame==false) menu();
else game();

arduboy.display();

// !!! PILKA !!!
}

// **************** MENU ********************
void menu() {



   if (arduboy.justPressed(UP_BUTTON) == true) {
     if (menuChoice==1) menuChoice=3;
     else menuChoice--;
   }

    if (arduboy.justPressed(DOWN_BUTTON) == true) {
     if (menuChoice==3) menuChoice=1;
     else menuChoice++;
   }

   if (arduboy.justPressed(B_BUTTON) == true and menuChoice==1) {
    startGame=true;
    playerWin=false;
    oponWin=false;
    playerScore=0;
    oponScore=0;
   }

  if (menuChoice==2) {
    
   if (arduboy.justPressed(RIGHT_BUTTON) == true) {
     pointsToWin++;
   }

    if (arduboy.justPressed(LEFT_BUTTON) == true) {
     pointsToWin--;
   }
  }

  if (menuChoice==3) {
    if (arduboy.justPressed(LEFT_BUTTON) == true or arduboy.justPressed(RIGHT_BUTTON) == true) {
     threeTouches=!threeTouches;
   }
  }

  
  arduboy.setCursor(20,0);
  if (playerWin==true) arduboy.print("PLAYER WON");
  else if (oponWin==true) arduboy.print("COMPUTER WIN");
  else arduboy.print("ARCADE VOLEYBALL");
  arduboy.setCursor(20, 20);
  arduboy.print("Start Game");
  arduboy.setCursor(20, 30);
  arduboy.print("Points to win: ");
  arduboy.print(pointsToWin);
  arduboy.setCursor(20,40);
  arduboy.print("3 touches: ");
  arduboy.print(threeTouches);
  arduboy.drawBitmap(1, menuChoice*10+10, ball, 8, 8, WHITE);

}

// **************** GAME ********************
void game() {
ballX += ballXvelocity;
ballY += ballYvelocity;


if (ballY<0 and ballYvelocity<0) ballYvelocity=-ballYvelocity;
if (ballX<0 and ballXvelocity<0) ballXvelocity = -ballXvelocity;
if (ballX+8>128 and ballXvelocity>0) ballXvelocity =-ballXvelocity;
if (ballY+8>64 or (playerTouches>3 and threeTouches) or (oponTouches>3 and threeTouches)){
   if (ballX>60 or oponTouches>3) {
     playerScore+=1;
     didPlayerScore=true;
   }
   if (ballX<60 or playerTouches>3) {
     oponScore+=1;
     didPlayerScore=false;
   }
   
   tunes.tone(1300,300);
   ballX = 56;
   ballY = 10;  
   ballXvelocity = 0;
   ballYvelocity = 0;
   ballGravity = 0;
   oponTouches=0;
   playerTouches=0;
   stop=true;

   }

//WZNOWIENIE - SERWIS

   if (arduboy.pressed(A_BUTTON) == true and stop==true) {
      if (didPlayerScore==true) ballXvelocity = -1;
      else ballXvelocity = 1;

      ballYvelocity = 1;
      ballGravity = 0.05;
      stop=false;
   }

   if (stop==true) {
     oponX+=0;
     arduboy.setCursor(40,25);
    arduboy.print("PRESS A");
   }

//do ogarniecia!! grawitacja prostsza niz sie wydawalo :D

if (ballYvelocity < 3) ballYvelocity+=ballGravity;

//kolizje

if (ballY+8>41 and ballX>=62 and ballX<=63 and ballXvelocity<0) ballXvelocity=2;
if (ballY+8>41 and ballX+8<=60 and ballX+8>=59 and ballXvelocity>0) ballXvelocity=-2;
if (ballY+8>=41 and ballX+4>57 and ballX+4<65) ballYvelocity=-0.5;

//"uwalnianie pilki" do ponowengo naliczenia odbicia
if (ballY+8<playerY) freePlayerTouch=true;
if (ballY+8<oponY) freeOponTouch=true;

//srodek dol piki - odbija sie wgore
if (ballY+8>=playerY and ballY+8<playerY+8 and ballX+4>playerX+4 and ballX+4<playerX+12) {
  ballYvelocity=-2 + 0.05*randomNumber;
  randomNumber = 1 + rand() % 5;
  oponTouches=0;
  if (freePlayerTouch==true) {
    playerTouches++;
    freePlayerTouch=false;
  }
  
}


//odbicie z prawej strony gracza
if (ballY+8>=playerY and ballY+8<playerY+8 and ballX+4>=playerX+12 and ballX+4<=playerX+16) {
  ballYvelocity=-2 + 0.05*randomNumber;
  ballXvelocity=2;
  randomNumber = 1 + rand() % 5;
  oponTouches=0;
   if (freePlayerTouch==true) {
    playerTouches++;
    freePlayerTouch=false;
  }
}

//odbicie z lewej strony gracza
if (ballY+8>=playerY and ballY+8<playerY+8 and ballX+4>playerX and ballX+4<=playerX+4) {
  ballYvelocity=-2 + 0.05*randomNumber;
  ballXvelocity=-2;
  randomNumber = 1 + rand() % 5;
  oponTouches=0;
   if (freePlayerTouch==true) {
    playerTouches++;
    freePlayerTouch=false;
  }

}


//srodek dol piki - odbija sie w gore od przeciwnika
if (ballY+8>=oponY and ballY+8<oponY+8 and ballX+4>oponX+4 and ballX+4<oponX+12) 
{
  ballYvelocity=-2 + 0.05*randomNumber;
  randomNumber = 1 + rand() % 5;
  playerTouches=0;
  if (freeOponTouch==true) {
    oponTouches++; 
    freeOponTouch=false;
  }
}

//odbicie z prawej strony przeciwnika
if (ballY+8>=oponY and ballY+8<oponY+8 and ballX+4>=oponX+12 and ballX+4<=oponX+16) {
  ballYvelocity=-2 + 0.05*randomNumber;
  ballXvelocity=2;
  randomNumber = 1 + rand() % 5;
  playerTouches=0;
    if (freeOponTouch==true) {
    oponTouches++; 
    freeOponTouch=false;
  } 
}

//odbicie z lewej strony przeciwnika
if (ballY+8>=oponY and ballY+8<oponY+8 and ballX+4>oponX and ballX+4<=oponX+4) {
  ballYvelocity=-2 + 0.05*randomNumber;
  ballXvelocity=-2;
  randomNumber = 1 + rand() % 5;
  playerTouches=0;
    if (freeOponTouch==true) {
    oponTouches++; 
    freeOponTouch=false;
  }
}

// !!!PRZECIWNIK!!!

/*if (ballX>60) {
   if (ballX+4<oponX and oponX>64+randomNumber) oponX-=oponSpeed;  //dodanie randomize minimalnie
   if (ballX+4>oponX and oponX<104 and ballY>=15) oponX+=oponSpeed;
   if (ballX+4>oponX and oponX<104 and oponX>75 and ballY<15) oponX-=oponSpeed;
   randomNumber= 1 + rand() % 5;
 }*/

 if (ballX>50) {
  if (ballX+4<96 and ballY+4<15 and oponX<=110 and ballXvelocity>0) oponX+=oponSpeed;
  else oponX+=0;// <96
  if (ballX+4>96 and ballY+4<15 and oponX>=64) oponX-=oponSpeed; // >96
  else oponX+=0;
  if (ballY+4>=20 and ballX-2+randomNumber<=oponX and oponX>=64) oponX-=oponSpeed; // >30 +2 do edycji, im wiekszy z tym wiekszym opoznieniem
  else oponX+=0;
  if (ballY+4>=20 and ballX+9-randomNumber>=oponX+16 and oponX<=110) oponX+=oponSpeed; // >30 +6 im mniejszy...
  else oponX+=0;
 } else oponX+=0;

if (ballX==oponX-7 and oponJump == false and ballXvelocity>0) {
  oponJump=true;
  oponYvelocity=2;
  oponGravity=0.1;
}



// !!! STEROWANIE !!!
if( arduboy.pressed(B_BUTTON) == true and playerJump == false) {
 playerJump=true;
 playerYvelocity=2;
 gravity=0.1;

}

if( arduboy.pressed(LEFT_BUTTON) == true and playerX > 0 ) {
 playerX-=1;
}

if( arduboy.pressed(RIGHT_BUTTON) == true and playerX + 16 < 60 ) {
 playerX+=1;
}

// GRAWITACJA GRACZA i PRZECIWNIKA SKOK
if (playerJump) {
 if (playerYvelocity>0) {
   playerYvelocity-=gravity;
   playerY-=playerYvelocity;
 } else if (playerY < 48) {
   if (gravity<1.5) gravity+=gravity;
   playerY+=gravity;
 } else {
   playerJump=false;
 }
}

if (oponJump) {
 if (oponYvelocity>0) {
   oponYvelocity-=oponGravity;
   oponY-=oponYvelocity;
 } else if (oponY < 48) {
   if (oponGravity<1.5) oponGravity+=oponGravity;
   oponY+=oponGravity;
 } else {
   oponJump=false;
 }
}

//koniec gry
if (playerScore==pointsToWin) {
  playerWin=true;
  startGame=false;
}
if (oponScore==pointsToWin) {
  oponWin=true;
  startGame=false;
}


arduboy.drawRect(60,40,2,24,WHITE); //SIATKA!
arduboy.drawBitmap(playerX, playerY, player, 16, 16, WHITE);
arduboy.drawBitmap(oponX, oponY, opon, 16, 16, WHITE);
arduboy.drawBitmap(ballX, ballY, ball, 8, 8, WHITE);
arduboy.setCursor(30,0);
arduboy.print(playerScore);
arduboy.setCursor(90,0);
arduboy.print(oponScore);
/*arduboy.setCursor(0,0);
arduboy.print(playerTouches);
arduboy.setCursor(120,0);
arduboy.print(oponTouches);*/
}
