// DS1302:  CE pin    -> Arduino Digital 2
//          I/O pin   -> Arduino Digital 3
//          SCLK pin  -> Arduino Digital 4

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <DS1302.h>
int Hour;
int Min;
int bSettings = 8; // pushbutton for setting alarm
int phour = 9; // pushbutton for hour
int pmin = 10; // pushbutton for minutes
int pexit = 11; // pushbutton for exit of set alarm
int buzzer = 6;
int hOn1; //assign hour to switch alarm1 on
int hOff1; //assign hour to switch alarm1 off
int mOn1; //assign minutes to switch alarm on
int mOff2; //assign minutes to switch alarm off
int buttonforset = 0; // pushbutton state for setting alarm
int buttonforhour = 0; // pushbutton state for hour
int buttonformin = 0;// pushbutton state for minutes
int buttonforexit = 0; // pushbutton state for exit of set alarm
int SMode1 = 0;
Time t;

// Init the DS1302
DS1302 rtc(2, 3, 4);

void setup()
 { 
// Set the clock to run-mode, and disable the write protection
   rtc.halt(false);
   rtc.writeProtect(false);

// Setup LCD to 16x2 characters
  lcd.begin();
  
// Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);
  
// Setup Serial connection
  Serial.begin(9600);

  // The following lines can be commented out to use the values already stored in the DS1302
//  rtc.setDOW(MONDAY);        // Set Day-of-Week to FRIDAY
//  rtc.setTime(7, 25, 0);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(10, 2, 2020);   // Set the date to August 6th, 2010
}

void loop()
{
// Define time-showing mode
  if (SMode1 == 0) {
    
// Display time on the left conrner first line
   lcd.setCursor(5, 0);
//   lcd.print("Time: ");
//   lcd.setCursor(6, 0);
   lcd.print(rtc.getTimeStr());

// Display abbreviated Day-of-Week
//   lcd.setCursor(0, 10);
//   lcd.print(rtc.getDOWStr(FORMAT_SHORT));

// Display date in the lower left corner
   lcd.setCursor(0, 1);
//   lcd.print("Date: ");
//   lcd.setCursor(6, 1);
   lcd.print(rtc.getDateStr());

// We will need those further
   t = rtc.getTime();
   Hour = t.hour;
   Min = t.min;

//Here we read the signal from button bSettings and assign it to buttonforset
   buttonforset = digitalRead(bSettings);
 }

//If we read a signal we switch to setting mode 1 and clear the display
  if (buttonforset == HIGH) {
   SMode1 = 1;
   lcd.clear();  
  }

//While we still in the setting mode 1 we set the time
  while(SMode1 == 1){

//Writing signs
   lcd.setCursor(0,0);
   lcd.print("Set Alarm 1");
   lcd.setCursor(0,1);
   lcd.print("Hour: ");
   lcd.setCursor(9,1);
   lcd.print("Min: ");

//Reading a signal from phour button and if it's high incrementing the h and print it
   buttonforhour = digitalRead(phour);
   if (buttonforhour == HIGH){
     h++;
     lcd.setCursor(5,1);
     lcd.print(h);

//And since we are looping throuh the numbers, we need to get back to 00 hour after 23
   if (h>23){ 
     h = 0;
     lcd.clear(); //not sure why
   }
   delay(100); //not sure why
 }

//Now we can set minutes
    buttonformin = digitalRead(pmin);
   if (buttonformin == HIGH){
     m++;
     lcd.setCursor(13,1);
     lcd.print(m);
//Same looping shit
     if (m > 59){
       m = 0; 
       lcd.clear();
     }
     delay(100); 
   }

//And now we printing the results of our input     
   lcd.setCursor(5,1);
   lcd.print(h);
   lcd.setCursor(13,1);
   lcd.print(m);

//Exit from set alarm mode
   buttonforexit = digitalRead(pexit);
   if (buttonforexit == HIGH){
     SMode1 = 0;
     lcd.clear();
   }
  }

//And finally the alarm
  if (Hour == h && Min == m) {
    Serial.println(Hour);
//   tone(buzzer,400,300);
  }
  delay (500);

//  Serial.println(Hour);
//  Serial.println(h);
//  Serial.println(Min);
//  Serial.println(m);
  
}
