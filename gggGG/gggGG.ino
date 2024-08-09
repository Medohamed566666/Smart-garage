int NO_slots = 3;
int counter = 3;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 3;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
};
byte rowPins[ROWS] = {0, 1, 2}; 
byte colPins[COLS] = {3, 4, 5}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Servo setup
Servo garageDoor;
const int servoPin = 10; 
Servo slot1door;
Servo slot2door;
Servo slot3door;

// ULTRASONIC DEFINE
#define NUM_SENSORS 3
const int trigPins[NUM_SENSORS] = {15, 17, 19};
const int echoPins[NUM_SENSORS] = {14,16, 18};

// LEDS DEFINE
int leds[NUM_SENSORS] = {22, 23, 24};

// Password setup
String correctPassword = "123"; 

void setup() {
  // IR pins
  pinMode(6, INPUT);
  pinMode(12, INPUT);
  
  // LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("smart garage....");

  // Servo motor pins
  garageDoor.attach(servoPin);
  slot1door.attach(8);
  slot2door.attach(9);
  slot3door.attach(11);
  garageDoor.write(0); 

  // ULTRASONIC
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  // LEDS 
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW); 
  }
}

void loop() {



//distance and leds state for slots...

     int dis[NUM_SENSORS] = {};
  for (int i = 0; i < NUM_SENSORS; i++) {
    dis[i] = getDistance(trigPins[i], echoPins[i]);

    if (dis[i] < 50) {
      digitalWrite(leds[i], HIGH);
    }
     
     else {
      digitalWrite(leds[i], LOW);
    }

  }



//when IR sense a movement
  int IR = digitalRead(6);
  if (IR == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("welcome.....:");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("enter id :");
    
    String enteredPassword = getPassword();
    
    if (enteredPassword == correctPassword) {
       counter = 3;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Opening Door...");

      openGarageDoor();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door is Open");
      delay(250);

      closeGarageDoor();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AVAIL SLOTS: ");
      lcd.print(NO_slots);
      delay(500);
      lcd.clear();
      
      if (NO_slots <= 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sorry !");
        lcd.setCursor(0, 1);
        lcd.print("All slots are booked");
        delay(500);
      } else {
        lcd.setCursor(0, 0);
        lcd.print("enter 1");
        lcd.setCursor(0, 1);
        lcd.print("to book a slot");

        while (true) {
          char key = keypad.getKey();
          if (key == '1') {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Process success");
            NO_slots--;
            delay(500);

            if (NO_slots == 2) {
              slot1Open();
            } else if (NO_slots == 1) {
              slot2Open();
            } else if (NO_slots == 0) {
              slot3Open();
            }
            break;
          }
        }
      }
    } else {
      counter--;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ACCESS DENIED");
      lcd.setCursor(0, 1);
      lcd.print("Attempts left: ");
      lcd.print(counter);

      if (counter == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HELP ?");
        lcd.setCursor(0, 1);
        lcd.print("CALL: 16591");
        delay(500);
        counter = 3;  
      }
      delay(500);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smart garage");
    delay(500);
  }

  if (digitalRead(12) == HIGH) {
    NO_slots++;
     if (NO_slots == 1) {
      slot3Close();
    } else if (NO_slots == 2) {
      slot2Close();
    } else if (NO_slots == 3) {
      slot1Close();
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("pay");
    openGarageDoor();
    
   
  }



}

// Function to get password 
String getPassword() {

  String password = "";
  char key;
  lcd.setCursor(0, 1);

  while (password.length() < 3) {  
    key = keypad.getKey();
    
    if (key) {
      password += key;
      lcd.print('*'); 
    }

  }
  return password;
  
}

// Function to open the garage door
void openGarageDoor() {

  for (int i = 0; i <= 180; i++) {
    garageDoor.write(i); 
    delay(10); 
  }

}

// Function to close the garage door
void closeGarageDoor() {

  for (int i = 180; i >= 0; i--) {
    garageDoor.write(i); 
    delay(5);
  }

}

//functions to open slots
void slot1Open() {

  for (int i = 0; i <= 180; i++) {
    slot1door.write(i);
    delay(10);
  }

}

void slot2Open() {

  for (int i = 0; i <= 180; i++) {
    slot2door.write(i);
    delay(10);
  }

}

void slot3Open() {

  for (int i = 0; i <= 180; i++) {
    slot3door.write(i);
    delay(10);
  }

}

//functions to close slots
void slot1Close() {

  for (int i = 180; i >= 0; i--) {
    slot1door.write(i);
    delay(10);
  }

}

void slot2Close() {

  for (int i = 180; i >= 0; i--) {
    slot2door.write(i);
    delay(10);
  }

}

void slot3Close() {

  for (int i = 180; i >= 0; i--) {
    slot3door.write(i);
    delay(10);
  }

}


// function to calculate distance for ultrasonic
int getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;

}