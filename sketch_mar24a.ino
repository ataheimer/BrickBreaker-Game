#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int paletWidth = 30;
const int paletHeight = 3;
const int topSize = 3;

int paletX = (SCREEN_WIDTH - paletWidth) / 2;
int topX = SCREEN_WIDTH / 2;
int topY = SCREEN_HEIGHT - paletHeight - topSize - 1;
float topYonX = 2;
float topYonY = -2;

int can = 3;
int skor = 0;
int level = 1;
int konum;
int objeY = 5;
bool obje = false;
int ldr;

const int potPin = A0; // Potansiyometre bağlı olduğu pin
const int yukariPin = 11;
const int asagiPin = 10;
const int secimPin = 9;

byte rakamlar[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
  };

int kontrol = -1;

bool tugla[25];
int tuglaX[5] = {0, 27, 54, 81, 108};

void setup() {
  // 7 segment display çıkışları
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  // Giriş ekranı için buton çıkışları
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);

  // Can gösterimi için led çıkışları
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  // Karanlık mod için ışık sensörü çıkışı
  pinMode(12, INPUT);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 başlatma başarısız."));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 10);
  display.println("BASLAT");
  display.setCursor(20, 30);
  display.println("CIKIS");
  display.display();
}

void loop() {

  int yukari = digitalRead(yukariPin);
  int asagi = digitalRead(asagiPin);
  int secim = digitalRead(secimPin);

  ldr = digitalRead(12);

  if(yukari)
  {
    display.clearDisplay();
    display.setCursor(20, 10);
    display.println("BASLAT <-");
    display.setCursor(20, 30);
    display.println("CIKIS");
    display.display();
    kontrol = 0;
  }
  if(asagi)
  {
    display.clearDisplay();
    display.setCursor(20, 10);
    display.println("BASLAT");
    display.setCursor(20, 30);
    display.println("CIKIS <-");
    display.display();
    kontrol = 1;
  }
  if(secim)
  {
    if(kontrol == -1)
    {
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("ASAGI-YUKARI TUSLARI");
      display.println("KULLANARAK SECIM YAP!");
      display.display();
    }
    else if(kontrol == 0)
    {
      display.clearDisplay();
      display.setCursor(20, 20);
      display.print("LEVEL ");
      display.print(level);
      display.display();
      delay(3000);

      for(int i = 0; i < 25; i++)
        tugla[i] = true;
      
      while(can)
      {
        oyun();
      }

      display.clearDisplay();
      display.setCursor(20, 20);
      display.print("SKOR: ");
      display.print(skor);
      display.display();
      delay(3000);

      kontrol = -1;
      can = 3;
      skor = 0;
      level = 1;
      topYonX = 2;
      topYonY = -2;
      topX = SCREEN_WIDTH / 2;
      topY = SCREEN_HEIGHT - paletHeight - topSize - 1;

      display.clearDisplay();
      display.setCursor(20, 10);
      display.println("BASLAT");
      display.setCursor(20, 30);
      display.println("CIKIS");
      display.display();
    }
    else if(kontrol == 1)
    {
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("OYUNUMUZA");
      display.println("GOSTERDIGINIZ");
      display.println("ILGI ICIN");
      display.println("TESEKKURLER!");
      display.display();
    }
  }
}

void oyun(){
  display.clearDisplay();
  if(ldr)
    display.fillScreen(SSD1306_WHITE);

  tuglaciz();
  hareketTop();
  hareketPalet();
  skorgoster(skor);
  cangoster(can);
  if(obje)
    objedusur();
  
  display.display();
}

void tuglaciz(){
  if(level >= 1){
    for(int i = 0; i < 5; i++){
      if(tugla[i]){
        if(ldr)
          display.fillRect(tuglaX[i], 0, 20, 5, BLACK);
        else
          display.fillRect(tuglaX[i], 0, 20, 5, WHITE);
      }
    }
  }

  if(level >= 2){
    for(int i = 5; i < 10; i++){
      if(tugla[i]){
        if(ldr)
          display.fillRect(tuglaX[i-5], 6, 20, 5, BLACK);
        else
          display.fillRect(tuglaX[i-5], 6, 20, 5, WHITE);
      }
    }
  }

  if(level >= 3){
    for(int i = 10; i < 15; i++){
      if(tugla[i]){
        if(ldr)
          display.fillRect(tuglaX[i-10], 12, 20, 5, BLACK);
        else
          display.fillRect(tuglaX[i-10], 12, 20, 5, WHITE);
      }
    }
  }

  if(level >= 4){
    for(int i = 15; i < 20; i++){
      if(tugla[i]){
        if(ldr)
          display.fillRect(tuglaX[i-15], 18, 20, 5, BLACK);
        else
          display.fillRect(tuglaX[i-15], 18, 20, 5, WHITE);
      }
    }
  }

  if(level >= 5){
    for(int i = 20; i < 25; i++){
      if(tugla[i]){
        if(ldr)
          display.fillRect(tuglaX[i-20], 24, 20, 5, BLACK);
        else
          display.fillRect(tuglaX[i-20], 24, 20, 5, WHITE);
      }
    }
  }
}

void hareketTop() {
  // Topun hareketi ve çarpışmaları kontrol et
  topX += topYonX;
  topY += topYonY;

  if (topX <= 0 || topX >= SCREEN_WIDTH) {
    topYonX = -topYonX;
  }

  if (topY <= 0 || (topY >= SCREEN_HEIGHT - paletHeight - topSize && topX >= paletX && topX <= paletX + paletWidth)) {
    topYonY = -topYonY;
  }

  if(topY >= SCREEN_HEIGHT - 1){
    can--;
    topX = SCREEN_WIDTH / 2;
    topY = SCREEN_HEIGHT - paletHeight - topSize - 1;
    if(topYonY > 0)
      topYonY = -topYonY;
    display.setCursor(20, 20);
    display.print("KALAN CAN: ");
    display.print(can);
    display.display();
    delay(1000);
  }

  for(int i = 0; i < 5; i++){
    if(tugla[i] && topY <= 5 + topSize && topX >= tuglaX[i] && topX <= tuglaX[i] + 20){
      topYonY = -topYonY;
      tugla[i] = false;
      skor++;
      Serial.println(skor);
      levelkontrol();
      if(!obje){
        if(!random(0, 10)){
          obje = true;
          konum = i;
        }
      }
    }
  }

  if(level >= 2){
    for(int i = 5; i < 10; i++){
      if(tugla[i] && topY <= 11 + topSize && topX >= tuglaX[i-5] && topX <= tuglaX[i-5] + 20){
        topYonY = -topYonY;
        tugla[i] = false;
        skor++;
        Serial.println(skor);
        if(!obje){
          if(!random(0, 10)){
            obje = true;
            konum = i;
          }
        }
      }
    }
  }

    if(level >= 3){
    for(int i = 10; i < 15; i++){
      if(tugla[i] && topY <= 17 + topSize && topX >= tuglaX[i-10] && topX <= tuglaX[i-10] + 20){
        topYonY = -topYonY;
        tugla[i] = false;
        skor++;
        Serial.println(skor);
        if(!obje){
          if(!random(0, 10)){
            obje = true;
            konum = i;
          }
        }
      }
    }
  }

    if(level >= 4){
    for(int i = 15; i < 20; i++){
      if(tugla[i] && topY <= 23 + topSize && topX >= tuglaX[i-15] && topX <= tuglaX[i-15] + 20){
        topYonY = -topYonY;
        tugla[i] = false;
        skor++;
        Serial.println(skor);
        if(!obje){
          if(!random(0, 10)){
            obje = true;
            konum = i;
          }
        }
      }
    }
  }

    if(level >= 5){
    for(int i = 20; i < 25; i++){
      if(tugla[i] && topY <= 30 + topSize && topX >= tuglaX[i-20] && topX <= tuglaX[i-20] + 20){
        topYonY = -topYonY;
        tugla[i] = false;
        skor++;
        Serial.println(skor);
        if(!obje){
          if(!random(0, 10)){
            obje = true;
            konum = i;
          }
        }
      }
    }
  }

  // Topu çiz
  if(ldr)
    display.fillCircle(topX, topY, topSize, SSD1306_BLACK);
  else
    display.fillCircle(topX, topY, topSize, SSD1306_WHITE);
}

void hareketPalet() {
  // Potansiyometreden değeri oku
  int potValue = analogRead(potPin);

  // Çubuğun hareketini kontrol et
  paletX = map(potValue, 0, 1023, 0, SCREEN_WIDTH - paletWidth);

  // Çubuğu çiz
  if(ldr)
    display.fillRect(paletX, SCREEN_HEIGHT - paletHeight, paletWidth, paletHeight, SSD1306_BLACK);
  else
    display.fillRect(paletX, SCREEN_HEIGHT - paletHeight, paletWidth, paletHeight, SSD1306_WHITE);
}

void objedusur(){
  objeY++;
  if(ldr)
    display.fillCircle(tuglaX[konum] + 10, objeY, 1, SSD1306_BLACK);
  else
    display.fillCircle(tuglaX[konum] + 10, objeY, 1, SSD1306_WHITE);

  if(objeY >= SCREEN_HEIGHT){
    objeY = 5;
    obje = false;
  }

  if(objeY >= SCREEN_HEIGHT - paletHeight && tuglaX[konum] + 10 >= paletX && tuglaX[konum] + 10 <= paletX + paletWidth){
    if(can < 3)
      can++;
    objeY = 5;
    obje = false;
  }
}

void skorgoster(int skor) {
  int rakam = skor % 10;
  digitalWrite(2, rakamlar[rakam][0]);
  digitalWrite(3, rakamlar[rakam][1]);
  digitalWrite(4, rakamlar[rakam][2]);
  digitalWrite(5, rakamlar[rakam][3]);
  digitalWrite(6, rakamlar[rakam][4]);
  digitalWrite(7, rakamlar[rakam][5]);
  digitalWrite(8, rakamlar[rakam][6]);
}

void cangoster(int can){
  if(can == 3){
    digitalWrite(A1, 1);
    digitalWrite(A2, 1);
    digitalWrite(A3, 1);
  }
  else if(can == 2){
    digitalWrite(A1, 1);
    digitalWrite(A2, 1);
    digitalWrite(A3, 0);
  }
  else if(can == 1){
    digitalWrite(A1, 1);
    digitalWrite(A2, 0);
    digitalWrite(A3, 0);    
  }
  else if(can == 0){
    digitalWrite(A1, 0);
    digitalWrite(A2, 0);
    digitalWrite(A3, 0);
  }
}

void levelkontrol() {
  if (skor == 5) {
    level = 2;
    display.clearDisplay();
    display.setCursor(20, 20);
    display.print("LEVEL ");
    display.print(level);
    display.display();
    delay(5000);
    topX = SCREEN_WIDTH / 2;
    topY = SCREEN_HEIGHT - paletHeight - topSize - 1;
    // topu %20 hızlandır
    topYonX += topYonX * 0.2;
    topYonY += topYonY * 0.2;
    if(topYonY > 0)
      topYonY = -topYonY;
    for (int i = 0; i < 25; i++)
      tugla[i] = true;
  }

  if (skor == 15) {
    level = 3;
    display.clearDisplay();
    display.setCursor(20, 20);
    display.print("LEVEL ");
    display.print(level);
    display.display();
    delay(5000);
    topX = SCREEN_WIDTH / 2;
    topY = SCREEN_HEIGHT - paletHeight - topSize - 1;
    // topu %20 hızlandır
    topYonX += topYonX * 0.2;
    topYonY += topYonY * 0.2;
    if(topYonY > 0)
      topYonY = -topYonY;
    for (int i = 0; i < 25; i++)
      tugla[i] = true;
  }

  if (skor == 30) {
    level = 4;
    display.clearDisplay();
    display.setCursor(20, 20);
    display.print("LEVEL ");
    display.print(level);
    display.display();
    delay(5000);
    topX = SCREEN_WIDTH / 2;
    topY = SCREEN_HEIGHT - paletHeight - topSize - 1;
    // topu %20 hızlandır
    topYonX += topYonX * 0.2;
    topYonY += topYonY * 0.2;
    if(topYonY > 0)
      topYonY = -topYonY;
    for (int i = 0; i < 25; i++)
      tugla[i] = true;
  }

  if (skor == 50) {
    level = 5;
    display.clearDisplay();
    display.setCursor(20, 20);
    display.print("LEVEL ");
    display.print(level);
    display.display();
    delay(5000);
    topX = SCREEN_WIDTH / 2;
    topY = SCREEN_HEIGHT - paletHeight - topSize - 1;
    // topu %20 hızlandır
    topYonX += topYonX * 0.2;
    topYonY += topYonY * 0.2;
    if(topYonY > 0)
      topYonY = -topYonY;
    for (int i = 0; i < 25; i++)
      tugla[i] = true;
  }

  if (skor == 75) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.print("TEBRIKLER, KAZANDINIZ");
    display.display();
    delay(5000);
    can = 0;
  }
}