#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

const int Top = 0x01;
const int TopRight = 0x02;
const int BottomRight = 0x04;
const int Bottom = 0x08;
const int BottomLeft = 0x10;
const int TopLeft = 0x20;
const int Center = 0x40;
const int Decimal = 0x80;


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//Pin connected to ST_CP of 74HC595
int latchPin = 4;
//Pin connected to SH_CP of 74HC595
int clockPin = 5;
////Pin connected to DS of 74HC595
int dataPin = 3;

int D1 = 0;
int D2 = 2;
int D3 = 6;
int D4 = 7;

byte rotator[7];
byte pinRotator[4];
int currentIndex;
int rotatorIndex;
int diffTime;
unsigned long startTime;
String lastKey;
bool cardRead;
bool timeoutComplete;

void setup() {
  //Serial.begin(9600);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // put your setup code here, to run once:
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  rotator[0] = 3;
  rotator[1] = 6;
  rotator[2] = 12;
  rotator[3] = 24;
  rotator[4] = 48;
  rotator[5] = 33;

  currentIndex = 0;
  rotatorIndex = 0;

  startTime = 0;
  diffTime = 3;
  timeoutComplete = false;
}

void loop() {
  if (!timeoutComplete) {
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return;
    }

    Serial.print("UID tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      //Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    //Serial.println();
    //Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "24 2F 2D 2B") //change here the UID of the card/cards that you want to give access
    {
      if (startTime == 0) {
        startTime = millis();
      }
      lastKey = "24 2F 2D 2B";
      cardRead = true;
    } else if (content.substring(1) == "C1 99 DC 66") {
      if (startTime == 0) {
        startTime = millis();
      }
      lastKey = "C1 99 DC 66";
      cardRead = true;
    } else if (content.substring(1) == "81 74 D2 66") {
      if (startTime == 0) {
        startTime = millis();
      }
      lastKey ="81 74 D2 66";
      cardRead = true;
    } else if (content.substring(1) == "61 E7 BD 66") {
      if (startTime == 0) {
        startTime = millis();
      }
      lastKey = "61 E7 BD 66";
      cardRead = true;
    } else if (content.substring(1) == "81 E7 CD 66") {
      if (startTime == 0) {
        startTime = millis();
      }
      lastKey ="81 E7 CD 66";
      cardRead = true;
    } else {
      cardRead = false;
    }
    if (cardRead) {
      //Serial.println("Entered Code");
      changeNum(rotatorIndex);
      // shift out the bits:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, rotator[currentIndex]);
      digitalWrite(latchPin, HIGH);

      if (currentIndex != 5) {
        currentIndex++;
      } else {
        currentIndex = 0;
      }

      if (currentIndex == 0 ) {
        if (rotatorIndex != 3) {
          rotatorIndex++;
        } else {
          rotatorIndex = 0;
        }
      }
      delay(100);
      if ((millis() - startTime) / 1000 > diffTime ) {
        timeoutComplete = true;
      }
    }
  } else {
    if (lastKey == "24 2F 2D 2B") {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);

      digitalWrite(D1, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 48);
      digitalWrite(latchPin, HIGH);
      delay(12);
      digitalWrite(D1, HIGH);

      digitalWrite(latchPin, LOW);
      digitalWrite(D2, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 119);
      digitalWrite(latchPin, HIGH);
      delay(12);
    } else if (lastKey == "C1 99 DC 66") {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);

      writeLetter(D1, '1', false);
      writeLetter(D2, '4', false);
      writeLetter(D3, '6', true);
      writeLetter(D4, '2', false);
    
    } else if (lastKey == "81 74 D2 66") {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);

      writeLetter(D1, '3', false);
      writeLetter(D2, '5', false);
      writeLetter(D3, '7', true);
      writeLetter(D4, '9', false);
    
  }else if (lastKey == "61 E7 BD 66") {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);

      writeLetter(D1, '1', false);
      writeLetter(D2, '3', false);
      writeLetter(D3, '0', true);
      writeLetter(D4, '9', false);
    
  } else if (lastKey == "81 E7 CD 66") {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);

      writeLetter(D1, '2', false);
      writeLetter(D2, '6', false);
      writeLetter(D3, '3', true);
      writeLetter(D4, '8', false);
    
  }
  }
}

void displaydigits(int num) {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  switch (num) {
    case 1:
      digitalWrite(D1, LOW);
      
      delay(12);
      digitalWrite(D1, HIGH);
  }
}

void writeLetter(int segNum, char letter, bool decimalPt){
  digitalWrite(segNum, LOW);
  digitalWrite(latchPin, LOW);
  switch(letter){
    case '1':
      shiftOut(dataPin, clockPin, MSBFIRST, TopRight | BottomRight | ( decimalPt ? Decimal : 0 ));
      break;
    case '2':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopRight | Center | BottomLeft | Bottom | ( decimalPt ? Decimal : 0 ) );
      break;
    case '3':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopRight | Center | BottomRight | Bottom | ( decimalPt ? Decimal : 0 ));
      break;
    case '4':
      shiftOut(dataPin, clockPin, MSBFIRST, TopLeft | Center | TopRight | BottomRight | ( decimalPt ? Decimal : 0 ));
      break;
    case '5':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopLeft | Center | BottomRight | Bottom | ( decimalPt ? Decimal : 0 ));
      break;
    case '6':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopLeft | BottomLeft | Center | BottomRight | Bottom | ( decimalPt ? Decimal : 0 ));
      break;
    case '7':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopRight | BottomRight | ( decimalPt ? Decimal : 0 ));
      break;
    case '8':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopLeft | BottomLeft | Bottom | BottomRight | TopRight | Center | ( decimalPt ? Decimal : 0 ));
      break;
    case '9': 
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopLeft | Center | BottomRight | TopRight | ( decimalPt ? Decimal : 0 ));      
      break;
    case '0':
      shiftOut(dataPin, clockPin, MSBFIRST, Top | TopLeft | BottomLeft | Bottom | BottomRight | TopRight | ( decimalPt ? Decimal : 0 ));
      break;
  }
  digitalWrite(latchPin, HIGH);
  delay(5);
  digitalWrite(segNum, HIGH);
  
}
void changeNum(int num) {
  switch (num) {
    case 3:
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);
      Serial.println(num);
      break;
    case 2:
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);
      Serial.println(num);
      break;
    case 1:
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH);
      Serial.println(num);
      break;
    case 0:
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
      Serial.println(num);
      break;
  }
}
