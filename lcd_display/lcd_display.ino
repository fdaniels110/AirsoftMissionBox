#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

uint8_t r0c1[8] = {0x0, 0x0, 0x0, 0x0, 0x0,0x4, 0xE, 0x1F };
uint8_t r0c2[8] = {0x0, 0x4, 0xE, 0x1F, 0x0, 0x1F, 0xE, 0x4 };
uint8_t r1c0[8] = {0x4, 0xA, 0x1F, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t r1c1[8] = {0x1F, 0xE, 0x4, 0x0, 0x4, 0xE, 0x1F, 0x0};
uint8_t r1c2[8] = {0x1F, 0xA, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t r2c1[8] = {0x1F, 0xE, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0 };

LiquidCrystal_I2C lcd(0x3F,20, 4);

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 11, 12, 13}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
const String freqs[5] = {"146.2", "357.9", "130.9", "263.8"};
const String stages[4] = { "first", "second", "third", "fourth" };

int stage; 
bool displayDone;
char currentString[5];
String currentOtherString = "";
int charCurrentIndex = 0;
void setup()
{
    Serial.begin(9600);

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  display_initial_screen(); 
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  stage = 1;
  displayDone = false;
}

void loop()
{
  if(stage < 5){
    if(!displayDone){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please enter the");      
      lcd.setCursor(0,1);      
      lcd.print(stages[stage - 1] + " frequency");      
      displayDone = true;
      lcd.setCursor(0,2);
    }else{
      char digit = keypad.waitForKey();
      if(digit == '*'){
        digit = '.';
      }
      lcd.print(digit);
      currentOtherString = currentOtherString + digit;
      
      Serial.println(currentOtherString);
      if(currentOtherString.length() == 5){
        bool stringCheck = true;
        if(freqs[stage - 1] != currentOtherString){
          Serial.println("Invalid");
          stringCheck = false;
        }else{
          Serial.println("Valid");
        }
                
        if(stringCheck){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Frequency found");
          delay(1000);
          stage++;
        }else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Frequency not found");
          delay(1000);
        }
        currentOtherString = "";
        displayDone = false;
      }else{
        charCurrentIndex++;
      }
    }
  }else if(stage == 5){
    Serial.println("Stage 5");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Frequency acquired");
    lcd.setCursor(0,1);
    lcd.print("Press # to arm");
    char digit = keypad.waitForKey();
    if(digit == '#'){
      stage = 6;
    }
  }else if(stage == 6){
    if(!displayDone){
      Serial.println("Stage 6");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Accessing");
      lcd.setCursor(0,1);
      lcd.print("Frequency");
      delay(500);
      lcd.print(".");
      delay(500);
      lcd.print(".");
      delay(500);
      lcd.print(".");
      delay(500);
      lcd.print(".");
      delay(500);
      stage++;
    }
  }else{
    if(!displayDone){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("JAMMING");
      displayDone = true;
    }
  }
  
}

void display_initial_screen()
{
  lcd.createChar(0, r0c1);
  lcd.createChar(1, r0c2);
  lcd.createChar(2, r1c0);
  lcd.createChar(3, r1c1);
  lcd.createChar(4, r1c2);
  lcd.createChar(5, r2c1);
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.write(0);
  lcd.setCursor(2,0);
  lcd.write(1);  
  lcd.setCursor(0,1);
  lcd.write(2);
  lcd.setCursor(1,1);
  lcd.write(3);
  lcd.setCursor(2,1);
  lcd.write(1);
  lcd.setCursor(3,1);
  lcd.write(4);  
  lcd.setCursor(1,2);
  lcd.write(5);   

  lcd.setCursor(5,0);
  lcd.print("Property of");
  lcd.setCursor(5, 1);
  lcd.print("ARC Syndicate");  
}
