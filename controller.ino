#include <Wire.h>

int JoyConX = A0;
int JoyConY = A1;

#define BUTTON_UP 2
#define BUTTON_RIGHT 3
#define BUTTON_DOWN 4
#define BUTTON_LEFT 5
#define BUTTON_E 6
#define BUTTON_F  7
#define BUTTON_K 8

enum Mode {
    BUTTON_MODE,
    STICK_MODE
};
Mode mode = BUTTON_MODE;

bool lastButtonState = HIGH;


void drive_stick(){
  int joyX = analogRead(JoyConX);
  int joyY = analogRead(JoyConY);

  // Wertebereich liegt meist zwischen 0 und 1023 (Mitte ca. 512)
  if(joyY > 800) {
    Wire.beginTransmission(9);
    Wire.write(BUTTON_UP);
    Wire.endTransmission();  
    Serial.println("Stick -> Vorwärts");
  }
  else if (joyY < 200) {
    Wire.beginTransmission(9);
    Wire.write(BUTTON_DOWN);
    Wire.endTransmission(); 
    Serial.println("Stick -> Rückwärts");
  }
  else if (joyX < 200) {
    Wire.beginTransmission(9);
    Wire.write(BUTTON_LEFT);
    Wire.endTransmission(); 
    Serial.println("Stick -> Links");
  }
  else if (joyX > 800) {
    Wire.beginTransmission(9);
    Wire.write(BUTTON_RIGHT);
    Wire.endTransmission(); 
    Serial.println("Stick -> Rechts");
  }
  else if(digitalRead(BUTTON_E) == LOW) {
    Serial.println("Knopf E gedrückt");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_E);
    Wire.endTransmission();
  }
  else if(digitalRead(BUTTON_F) == LOW) {
    Serial.println("Knopf F gedrückt");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_F);
    Wire.endTransmission();
  }
  else {
    // Neutralstellung des Sticks -> Stopp-Signal (0) senden
    Wire.beginTransmission(9);
    Wire.write(0);
    Wire.endTransmission(); 
  }
}

void drive_buttons(){
  
  // Da INPUT_PULLUP genutzt wird, ist ein Tastendruck = LOW
  if(digitalRead(BUTTON_UP) == LOW) {
    Serial.println("Knopf -> Vorwärts");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_UP);
    Wire.endTransmission();
  }
  else if(digitalRead(BUTTON_RIGHT) == LOW) {
    Serial.println("Knopf -> Rechts");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_RIGHT);
    Wire.endTransmission();
  }  
  else if(digitalRead(BUTTON_DOWN) == LOW) {
    Serial.println("Knopf -> Rückwärts");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_DOWN);
    Wire.endTransmission();
  }  
  else if(digitalRead(BUTTON_LEFT) == LOW) {
    Serial.println("Knopf -> Links");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_LEFT);
    Wire.endTransmission();
  }
  else if(digitalRead(BUTTON_E) == LOW) {
    Serial.println("Knopf E gedrückt");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_E);
    Wire.endTransmission();
  }
  else if(digitalRead(BUTTON_F) == LOW) {
    Serial.println("Knopf F gedrückt");
    Wire.beginTransmission(9);
    Wire.write(BUTTON_F);
    Wire.endTransmission();
  }
  else {
    // Kein Knopf gedrückt -> Stopp-Signal (0) senden
    Wire.beginTransmission(9);
    Wire.write(0);
    Wire.endTransmission();
  }  
}

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Alle Button-Pins als Eingang mit internem Pull-Up-Widerstand definieren
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_E, INPUT_PULLUP);
  pinMode(BUTTON_F, INPUT_PULLUP);
  pinMode(BUTTON_K, INPUT_PULLUP);
}

void toggleMode() {
    bool currentButtonState = digitalRead(BUTTON_K);

    // Flankenerkennung (Wechsel von nicht gedrückt zu gedrückt)
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        if (mode == BUTTON_MODE) {
            mode = STICK_MODE;
            Serial.println("---Modus gewechselt zu: STICK---");
        } 
        else {
            mode = BUTTON_MODE;
            Serial.println("---Modus gewechselt zu: BUTTONS---");
        }
        delay(200); // Entprellen (Debounce) erhöht, damit es sauber umschaltet
    }
    lastButtonState = currentButtonState;
}

void loop() {
    toggleMode();

    if (mode == STICK_MODE) {
        drive_stick();
    } 
    else {
        drive_buttons();
    }
    
    delay(50); // Ganz wichtig, um den I2C-Bus und den seriellen Monitor nicht zu überlasten!
}
