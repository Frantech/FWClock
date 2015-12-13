/*
    FWClock – Extended Interface for DS3231 Real Time Clock.
                  Copyright © 2015 Frank Williams.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
    The author can be contacted at frank.w2@paradise.net.nz or by writing to:
      Frank Williams
      89 Miro St
      Trentham
      Upper Hutt
      New Zealand 5018
*/


/* Summary
   =======
 FWClock - Library for interacting with a DS3231 Real Time Clock connected to an Arduino.
 
 It provides high level functions to:
    - Set, read, and adjust the time.
    - Display the current time.
    - Notify a part of the time. e.g. What month is it? What day of the week is it?
    - Indicate if a time given has passed.
    - Provide a time difference, within a day, to or from a given time. 
 
 Created by Frank Williams, December 13, 2015.
 Released into the public domain.
*/


#include "Arduino.h"


#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <FWClock.h>


byte FWClock::decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal.
  return((val/10*16) + (val%10));
}

byte FWClock::bcdToDec(byte val){
// Convert binary coded decimal to normal decimal numbers.
  return((val/16*10) + (val%16));
}
  
void FWClock::setTime(byte year, byte month,byte dayOfMonth,
                    byte hour, byte minute, byte second, 
					byte dayOfWeek){
// Set the clock. e.g. When a new battery has been fitted.
// Sets time and date data to DS3231.
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register.
  Wire.write(decToBcd(second));     // set seconds.
  Wire.write(decToBcd(minute));     // set minutes.
  Wire.write(decToBcd(hour));       // set hours.
  Wire.write(decToBcd(dayOfWeek));  // set day of week (1=Sunday, 7=Saturday).
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31).
  Wire.write(decToBcd(month));      // set month.
  Wire.write(decToBcd(year));       // set year (0 to 99).
  Wire.endTransmission();
}

void FWClock::readDS3231Time() {
// btime is held in the private fields in this class.
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // Request seven bytes of data from DS3231 starting from register 00h.
  second = bcdToDec(Wire.read() & 0x7f);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3f);
  dayOfWeek = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
};

void FWClock::displayTimeToConsole(){
// Report the time.

  readDS3231Time();      // Set the private fields needed.

  // Send time to the serial monitor.
  if (dayOfMonth<10)
  {    
    Serial.print("0");
  }
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" ");

  Serial.print(hour, DEC);
  // Convert the byte variable to a decimal number when displayed.
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.println(second, DEC);
}

char FWClock::isItTime(unsigned long trigger){ 
// Tell whether a set time has passed.	

  readDS3231Time();      // Set the private fields hour, minute etc
  unsigned long timeNow;

  // We compare at yymmddhhmm or hhmm mm level. ( i.e. day of year or same day ).
  if (trigger/10000 == 0) {
      timeNow = hour; 
      timeNow *= 100;  
      timeNow += minute;  
  }
  else
  {
      timeNow = year;
      timeNow *= 100;
      timeNow += month;
      timeNow *= 100; 
      timeNow += dayOfMonth;
      timeNow *= 100;  
      timeNow += hour; 
      timeNow *= 100;  
      timeNow += minute;  
  }

  if (trigger <= timeNow) 
    return 'Y';
  else
    return 'N';
}

void FWClock::adjustClockUpByOneMinute(){
// For fine adjustments to the clock time.

  readDS3231Time();      // Set the private fields needed.
  
  if (minute<58){
    minute++;
  }
  setTime(year,month,dayOfMonth,hour,minute,second,dayOfWeek);
}

void FWClock::adjustClockSecondsToZero(){
// For fine adjustments to the clock time.

  readDS3231Time();      // Set the private fields needed
  
  second = 0;
  setTime(year,month,dayOfMonth,hour,minute,second,dayOfWeek);
}

void FWClock::readADateTimeField(char* pv, char* pw, char timeType){
// Purpose: Provide the contents of a designated field in char format. e.g. Year as "16" in common storage.
// How:     Convert binary to BCD then to two characters at the addresses of the first two parms.

  readDS3231Time();      // Set the private fields needed

  char A[17] = "0123456789ABCDEF";
  byte b,g,h;
  switch(timeType){
    case 'y': b=year;
      break;
	case 'm': b=month;
      break;
	case 'd': b=dayOfMonth;
      break;
	case 'h': b=hour;
      break;
	case 'n': b=minute;
      break;
	case 's': b=second;
      break;
	case 'w': b=dayOfWeek;
      break;
	default:  b=99;
      break;
	}

   // Convert binary to BCD
    g = b/10;      
    h = b%10;      
  *pv = A[g];     // Write a char into the output char.     
  *pw = A[h];    
  return;
}


String FWClock::getTime(){
  char sTime[15];                     // Local work field.
  char v,w;                           

  readADateTimeField(&v, &w, 'y');    // Year.
  sTime[0] = v;
  sTime[1] = w;
  readADateTimeField(&v, &w, 'm');    // Month.
  sTime[2] = v;
  sTime[3] = w;
  readADateTimeField(&v, &w, 'd');    // Day of Month.
  sTime[4] = v;
  sTime[5] = w;
  readADateTimeField(&v, &w, 'h');    // Hour.
  sTime[6] = v;
  sTime[7] = w;
  readADateTimeField(&v, &w, 'n');    // Minute.
  sTime[8] = v;
  sTime[9] = w;
  readADateTimeField(&v, &w, 's');    // Second.
  sTime[10] = v;
  sTime[11] = w;
  readADateTimeField(&v, &w, 'w');    // Day of week.
  sTime[12] = v;
  sTime[13] = w;
  sTime[14] = '\0';

String s = String(sTime);
return s;
}

// Overloaded - Format: hh.x,mm.x  float.
String FWClock::timeDiff(float* hhmm){ 
    // Convert target time to seconds to pass into the sub method.
    int hhmm1 = *hhmm;
    *hhmm++;
    int hhmm2 = *hhmm;
    long int targetSec = hhmm1 * 3600L;   // HH 
    targetSec += hhmm2 * 60L;             // MM 

    //Call the sub method to do most of the work
     return timeDiffSub(targetSec); 
}


// Overloaded - Format: hh,mm  Integer. 
String FWClock::timeDiff(int* hhmm){ 
    // Convert target time to seconds to pass into the sub method.
    int hhmm1 = *hhmm;
    *hhmm++;
    int hhmm2 = *hhmm;
    long int targetSec = hhmm1 * 3600L;   // HH 
    targetSec += hhmm2 * 60L;             // MM 

 	//Call the sub method to do most of the work.
    return timeDiffSub(targetSec); 
}


// Used for overloading diffTime(). 
String FWClock::timeDiffSub(long int targetSec){ 
    // Get current time.
    String now = getTime();    // populates now="yymmddhhmmssww" 
    //                                            ----+----1---+ 
    // Convert String to c-string. toCharArray() populates strTime[].
    char strTime[15]; 
    now.toCharArray(strTime, 15); 
 
 
    // Convert current time to seconds. 
    byte t1 = strTime[6]-0x30; 
    byte t2 = strTime[7]-0x30; 
    long int hrsSec = ((t1 * 10L) + t2) * 3600;        // Current time hour in seconds. 

    t1 = strTime[8]-0x30; 
    t2 = strTime[9]-0x30; 
    long int minSec = ((t1 * 10L) + t2) * 60;          // Current time minutes in seconds. 

    t1 = strTime[10]-0x30; 
    t2 = strTime[11]-0x30; 
    long int secSec = ((t1 * 10L) + t2);               // Current time seconds in seconds. 

    long int nowSec = hrsSec + minSec + secSec; 

	
    // Calculate difference – i.e. How long until target time? 
	long int diff = targetSec - nowSec;
    char divider = '-';             

    // Negative numbers don't report correctly in the code further below so make diff +ve always.   
    if (diff<0) {
        diff *=-1;      // Make diff +ve. 
        divider = '=';  // Indicate negative difference. 
    }       

	
    // Convert difference into char(6).
    //    - "hh-mm-ss"       Counting down, hh-mm-ss to go (less than 24 hrs )  
    //    - "hh=mm=ss"       Counting up, time has passed by hh=mm=ss plus n times 24 hrs. 
    int Dh,Dm,Ds; 
    Dh = diff / 3600;         // Whole hours. 
    diff = diff % 3600; 
    Dm = diff / 60;           // Whole minutes. 
    Ds = diff % 60;           // Whole seconds.   
	
	
    // Now convert int values to char(2) 
	char tab[11] = "0123456789";   // To convert integer to char.
 	int ic;
    char t[9];
 
    ic = Dh/10;               // Number of 10 hours.  
	t[0] = tab[ic];

	ic = Dh%10;               // Number of 1 hours.  
	t[1] = tab[ic];
	
	t[2] = divider;
	
	ic = Dm/10;               // Number of 10 minutes.  
	t[3] = tab[ic];
	ic = Dm%10;               // Number of 1 minutes.  
	t[4] = tab[ic];
	
	t[5] = divider;
	
	ic = Ds/10;               // Number of 10 seconds.  
	t[6] = tab[ic];
	ic = Ds%10;               // Number of 1 seconds.  
	t[7] = tab[ic];
	
	t[8] = '\0';
	
	String tdr = String(t);
    return tdr; 
}





















