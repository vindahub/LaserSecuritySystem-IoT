#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int ldrpin=0;
int buzzer=8;

short a=0;
short code[4]={'0','0','0','0'};  //pass code stored in a array you can make it longer or shorter
                                  //by changing '4' and add/remove values

const byte numRows= 4;            //Rows and columns of the keypad
const byte numCols= 4;
 
char keymap[numRows][numCols]=    //Keypad map
          {
          {'1', '2', '3', 'A'}, 
          {'4', '5', '6', 'B'}, 
          {'7', '8', '9', 'C'},
          {'*', '0', '#', 'D'}
          };

byte rowPins[numRows] = {7,6,5,4}; //Keypad 8 pins
byte colPins[numCols]= {3,2,1,0}; 

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(115200);
  pinMode(ldrpin, INPUT);
  pinMode(buzzer, OUTPUT);
  const unsigned char myBitmap [] PROGMEM = { /* bmp */ };
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { 
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
}

void loop() {
  display.clearDisplay();
  displayScreen(1,5,20,"Security is active");  
  int ldrStatus = analogRead(ldrpin);
  if (ldrStatus > 200) {
      digitalWrite(buzzer, HIGH);
      display.clearDisplay();
      displayScreen(1,10,20,"Alert");
      delay(1000);
      digitalWrite(buzzer, LOW); 
      display.clearDisplay();
    }
  char keypressed = myKeypad.getKey();
  if (keypressed == '*') {    //if '*' is pressed go to Getpass function
    displayScreen(1,5,20,"Enter the Key");
    Getpass();
  }
  
}

void displayScreen(int textSize,int setX, int setY, char str[20]) {
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(setX, setY);
  display.print(str);
  display.display();  
 } 

void Getpass(){               //Getting passcode function
  tone(buzzer, 200);
  for(short i=0 ; i<4 ; i++) {
        char keypressed = myKeypad.waitForKey();
        if (keypressed==code[i])   //If the sequence is correct the a increases (4) means all values are right
        a++;
            }
  if(a==4) {
    noTone(buzzer);
    displayScreen(1,5,20,"Lock Disabled");
    delay(5000);
    displayScreen(1,5,20,"Locking");  delay(200);
    displayScreen(1,5,20,"Locking."); delay(200);
    displayScreen(1,5,20,"Locking..");  delay(400);
    a=0;
    } else {
    for(int i=100;i<=5000;i+=200) {
      tone(buzzer,i);
      delay(50);  
    }
    noTone(buzzer);  
    }
}
