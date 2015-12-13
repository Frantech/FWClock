//
//    FILE: FWClock.h
//  AUTHOR: Frantech
// VERSION: 0.1.00
// PURPOSE: Interface methods for DS3231 Real Time Clock
//     URL: https://github.com/Frantech/FWClock
//REQUIRES: Wire library by Nicholas Zambetti <http://www.zambetti.com>
//          as provided with the Arduino IDE.
//
/*
    FWClock – Extended Interface for DS3231 Real Time Clock.
                  Copyright © 2015 Frank Williams

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
   This code reads and updates a RTC (Real Time Clock) connected to an Arduino.

   It provides an interface to:
    - Set, read, and adjust the time.
    - Display the current time or time until a set time.
    - Notify a part of the time. e.g. What month is it? What day of the week is it?
    - Indicate if a time given has passed.
    - Provide a time difference, within a day, to or from a given time. 
*/


#include <arduino.h>

// Inclusion guard. 
#ifndef FWClock_h
#define FWClock_h

#define DS3231_I2C_ADDRESS 0x68


class FWClock{

//-------------------------------------------------------------------------------------------------------------------
public:

// Set the time. On the first install.
void setTime(byte year, byte month, byte dayOfMonth, byte hour, byte minute, byte second, byte dayOfWeek);

// Show the time.
void displayTimeToConsole();

// Indicate if the trigger time specified has passed (format yymmddhhmm or hhmm). Returns N or Y. 
char isItTime(unsigned long trigger);

// Add a minute - for adjusting the time.
void adjustClockUpByOneMinute();

// Set seconds to zero - for adjusting the time.
void adjustClockSecondsToZero();

// Read a selected field into two chars. timeType selection values: y,m,d,h,n,s,w. ('n'=minutes). 
void readADateTimeField(char* pv, char* pw, char timeType);

// Write time fields to a String. Format: "yymmddhhmmssww"
String getTime();

// Calculate the time difference from now until the time given. Returns string like "hh-mm-ss".
String timeDiff(float hhmm[]);
 
// Same as above but overloaded for integer array input.
String timeDiff(int hhmm[]); 



//--------------------------------------------------------------------------------------------------------------------
private:
byte year, month, dayOfMonth, hour, minute, second, dayOfWeek;


// Read the DS3231 device. This is a prerequisite for most other methods in this class.
void readDS3231Time();

// Convert decimal numbers to binary coded decimal.
byte decToBcd(byte val);

// Convert binary coded decimal to decimal.
byte bcdToDec(byte val);

// Sub-method to allow easy overloading of timeDiff().
String timeDiffSub(long int targetSec);

};  // End class FWClock 
#endif