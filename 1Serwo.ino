#include <Servo.h>

Servo myservo1;  // Serwo 1


int currentAngle = 0, targetAngle = 0;


int timeValue = 15; //czas ruchu
const int fullMoveTime = timeValue*1000; // Czas na ruch do 90° w ms
unsigned long lastMoveTime = 0;

// Piny przycisków
const int pAwaryjny1 = D6, pZasilania1 = D7, pSterowania1 = D5;
const int mOtworz = D2, mZamknij = D1;


int awaria1 = 0, zasilanie1 = 0, sterowanie1 = 0;


// Ustawienia kątów
const int st_awaria = 90;
const int st_zasilanie = 0;
const int st_sterowanie = 180;

void setup() {
    myservo1.attach(D8);

    pinMode(pAwaryjny1, INPUT_PULLUP);
    pinMode(pZasilania1, INPUT_PULLUP);
    pinMode(pSterowania1, INPUT_PULLUP);
  
  	pinMode(mOtworz, OUTPUT);
  	pinMode(mZamknij, OUTPUT);

    myservo1.write(currentAngle);

    Serial.begin(9600);
}

void buttonState() {
    awaria1 = digitalRead(pAwaryjny1) == LOW;
    zasilanie1 = digitalRead(pZasilania1) == LOW;
    sterowanie1 = digitalRead(pSterowania1) == LOW;
}

void getTargetAngle() {
    targetAngle = (!zasilanie1) ? st_sterowanie : (awaria1 ? st_awaria : (sterowanie1 ? st_zasilanie : st_sterowanie));
}

void updateServo() {
    
          
    int stepTime = fullMoveTime / st_sterowanie; 

    if (millis() - lastMoveTime >= stepTime && currentAngle != targetAngle) {
        currentAngle += (currentAngle < targetAngle) ? 1 : -1;

      //Serial.println(currentAngle);
        
       myservo1.write(currentAngle);
       
        lastMoveTime = millis();
    }
}
void loop() {
    buttonState();
    getTargetAngle();
    updateServo();
  if (currentAngle == st_sterowanie) {
    pinMode(mZamknij, OUTPUT);
    digitalWrite(mZamknij, LOW); // Zwieramy D1 do masy
  } else {
    pinMode(mZamknij, INPUT); // Odłączamy D1
  }

  if (currentAngle == 0) {
    pinMode(mOtworz, OUTPUT);
    digitalWrite(mOtworz, LOW); // Zwieramy D2 do masy
  } else {
    pinMode(mOtworz, INPUT); // Odłączamy D2
  }
}


