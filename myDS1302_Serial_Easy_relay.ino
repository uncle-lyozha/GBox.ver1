//RTC-based timer by uncle Lyozha. 
//Connections:
//DS1302:  CE -> D2, I/O -> D3, SCLK -> D4.
//Buttons on D8-11, grounded through 1 kOhm resistors. Buzzer on D6.
//LCD through I2C: SDA -> A4, SCL -> A5.

#include <DS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//We will keep current time here
int Hour;
int Min;

//Assign pins for buttons: set, hour, minute, exit
int pinSet = 8;
int pinHour = 9;
int pinMin = 10;
int pinExit = 11;

//Assign pin for buzzer
int pinBuzzer = 6;

//Variables to store time values when timer turns on and off
int hOn1;
int hOff1;
int mOn1;
int mOff1;

//Button variables to store the read value from button's pin
int btnSet = 0;
int btnHour = 0;
int btnMin = 0;
int btnExit = 0;

//Clock's mode, 0 shows time, other values for setting timers
int SMode1 = 0;

//Time variable
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

// Set the time in 24hr format d-m-y. Comment out if time is already set
//  rtc.setDOW(MONDAY);
//  rtc.setTime(7, 25, 0);     // h, m, s
//  rtc.setDate(10, 2, 2020);   // d, M, y
}

void loop()
{
// Define clock mode
	if (SMode1 == 0) {
    
// Display time starting from 5th position of 0 line
		lcd.setCursor(5, 0);
		lcd.print(rtc.getTimeStr());

// Display abbreviated Day-of-Week
//   lcd.setCursor(0, 10);
//   lcd.print(rtc.getDOWStr(FORMAT_SHORT));

// Display date on the 1 line
		lcd.setCursor(0, 1);
		lcd.print(rtc.getDateStr());

// Assign current time to the variables
		t = rtc.getTime();
		Hour = t.hour;
		Min = t.min;

//Here we read the signal from set button and assign it to btnSet variable
		btnSet = digitalRead(pinSet);
	}

//When the signal is high we switch to setting mode 1 and clear the display
	if (btnSet == HIGH) {
		SMode1 = 1;
		lcd.clear();  
	}

//While we still in the setting mode 1 we set the timer 1
	while(SMode1 == 1){

//Marking signs
		lcd.setCursor(12,0);
		lcd.print("Timer");
		lcd.setCursor(16,1);
		lcd.print("1");
		lcd.setCursor(3,0);
		lcd.print(":");
		lcd.setCursor(3,1);
		lcd.print(":");
		lcd.setCursor(7,0);
		lcd.print("ON");
		lcd.setCursor(7,1);
		lcd.print("OFF");

//Reading a signal from hour button and if it's high incrementing the hOn1 and print it
		btnHour = digitalRead(pinHour);
		if (btnHour == HIGH){
			hOn1++;
			lcd.setCursor(0,0);
			lcd.print(hOn1);

//And since we are looping throuh the numbers, we need to get back to 00 hour after 23
			if (hOn1 > 23){ 
				hOn1 = 0;
				lcd.clear(); //clear lcd so we can show the next input
			}
			delay(100); //blink fast to update the reading
		}

//Now we can set minutes
		btnMin = digitalRead(pinMin);
		if (btnMin == HIGH){
			mOn1++;
			lcd.setCursor(4,0);
			lcd.print(mOn1);
//Same looping shit
			if (mOn1 > 59){
				mOn1 = 0; 
				lcd.clear();
			}
			delay(100); 
		}

//And now we printing the results of our input     
		lcd.setCursor(0,0);
		lcd.print(hOn1);
		lcd.setCursor(4,0);
		lcd.print(mOn1);

//Exit from set alarm mode
		btnExit = digitalRead(pinExit);
		if (btnExit == HIGH){
			SMode1 = 0;
			lcd.clear();
		}
	}

//And finally the alarm
  if (Hour == hOn1 && Min == mOn1) {
    Serial.println(Hour);
//   tone(pinBuzzer, 400, 300);
  }
  delay (500);

//  Serial.println(Hour);
//  Serial.println(h);
//  Serial.println(Min);
//  Serial.println(m);
  
}
