/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>
#include<HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

uint8_t id;
int a=0;
int b[]={0,0,0,0};
int j;
int n;
int s=0;
#define r1   13
#define r2   12
#define r3   14
#define r4   27
#define green_bt  4
#define red_bt    15


void setup()
{
  EEPROM.begin(10);
  pinMode(green_bt, INPUT_PULLUP);
  pinMode(red_bt, INPUT_PULLUP);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
}
void loop(){  
  for(j=0;j<4;j++){
    Serial.println(EEPROM.read(j));}
int o=sum();
//getFingerprintEnroll();
  //Serial.println(getFingerprintIDez());
  int v=button1();
  if(v==0){
     Serial.println("Register:Red, Open:Green");
  lcd.setCursor(0, 0);
  lcd.print("Register:Red");
  lcd.setCursor(0, 1);
  lcd.print("Open:Green         ");
 
    }
  else if (v==2){
  int i=getFingerprintIDez();
  while(i==0){   
    i=getFingerprintIDez();
    Serial.println(i);
      lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("finger:");
    }if (i==1){
      deleteFingerprint(id);
      Serial.print("Open lock ");
      Serial.print(id);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Open lock ");
      lcd.print(id);
      delay(2000);
      openlock(id);
      int c=button2();
  long StartTime = millis();
    while(c==0){
      lcd.setCursor(0, 0);
      lcd.print("Press red      ");
      lcd.setCursor(0, 1);
      lcd.print("button to close     ");
      c=button2();
      long CurrentTime = millis();
      long ElapsedTime = CurrentTime - StartTime;
    if(c==1||ElapsedTime>10000){
      lcd.clear();
      break;
      }
      }
      closelock(id); 
    delay(2000);
    
      o=o-id;
      removing();
    }else{
      Serial.println("Not found");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not found");
      delay(1000);
    }
 }else if(v==1){
  Serial.println("o2");
  Serial.println(o);
  if(o>=10){
    Serial.println("No more lock available");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No more lock");
      lcd.setCursor(0, 1);
      lcd.print("available");
      delay(1000); 
  } 
  else{
    a=getFingerprintEnroll();
  Serial.println("a la");
  Serial.println(a);
  if(a==5){
  int c=button2();
  long StartTime = millis();
    while(c==0){
      lcd.setCursor(0, 0);
      lcd.print("Press red      ");
      lcd.setCursor(0, 1);
      lcd.print("button to close       ");
      c=button2();
      long CurrentTime = millis();
      long ElapsedTime = CurrentTime - StartTime;
    if(c==1||ElapsedTime>5000){
      lcd.clear();
      break;
      }
      }
      closelock(id); 
    delay(2000);
    }
    
 }
  }
}
uint8_t getFingerprintEnroll() {
int p = -1;
  Serial.println("Waiting for finger:");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("finger:");
  delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  Serial.println("Image taken");

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
  }
  Serial.println("Remove finger");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("Place same finger again");//can be removed
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place same");
  lcd.setCursor(0, 1);
  lcd.print("finger again");
  delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  Serial.println("Image taken");
  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return p;
  }

  // OK converted!

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  }
  id=assign1();
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Serial.print("Open lock");
    Serial.println(id);
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stored!");
      lcd.setCursor(0, 1);
      lcd.print("Open lock ");
      lcd.print(id);
      openlock(id);
      delay(1000);   
      return 5;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    return p;
  }
}
int getFingerprintIDez() {
int i=0;
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return i;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return i;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  id=finger.fingerID;
  return i=1;
}
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.print("Deleted!");
    Serial.println(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  }

  return p;
} 
int assign1(){
  for(j=0;j<4;j++){
    if(EEPROM.read(j)==0){
      Serial.println("tui la");
      EEPROM.write(j,j+1);
      EEPROM.commit();
      Serial.println(EEPROM.read(0));
      break;
    }
    }
    return EEPROM.read(j); 
}
int sum(){
int s=0;
  for(j=0;j<4;j++){
      s=s+EEPROM.read(j);
  }
    return s;
}
int removing(){
  for(j=0;j<4;j++){
    if(EEPROM.read(j)==id){
      EEPROM.write(j,0);
      EEPROM.commit();
      break;
    }
    }
    return EEPROM.read(j); 
}
uint8_t deleteFingerprint() {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
  }

  return p;
}
void openlock(int g){
  switch(g){
    case 1:
    digitalWrite(r1, 1); 
    break;
    case 2:
    digitalWrite(r2, 1);
    break;
    case 3:
    digitalWrite(r3, 1); 
    break;
    case 4:
    digitalWrite(r4, 1);
    break;
    }
}
void closelock(int g){
  switch(g){
    case 1:
    digitalWrite(r1, 0); 
    break;
    case 2:
    digitalWrite(r2, 0);
    break;
    case 3:
    digitalWrite(r3, 0); 
    break;
    case 4:
    digitalWrite(r4, 0);
    break;
    }
}

int button1(){
   if(digitalRead(green_bt)==1 && digitalRead(red_bt)==1)
  {
  return 0;
  }
  else if(digitalRead(green_bt)==0)
  {
    return 2;
  }
  else if(digitalRead(red_bt)==0)
  {
    return 1;
  }
  else if(digitalRead(red_bt)==0&&digitalRead(green_bt)==0){
    return 3;
    }
  
}

int button2(){
   if(digitalRead(green_bt)==1 && digitalRead(red_bt)==1)
  {
  return 0;
  }
  else if(digitalRead(green_bt)==0)
  {
    return 2;
  }
  else if(digitalRead(red_bt)==0)
  {
    return 1;
  }
  
}

  
