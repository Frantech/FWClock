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


/* Example of usage of all public methods in the FWClock class */
/* =========================================================== */
/* This code reads the RTC (Real Time Clock) on the Arduino and provides examples of calls to all of the public methods in the FWClock class. */
/* Written by Frank Williams. December 13, 2015.               */


#include <Wire.h>
#include <FWClock.h>

FWClock clock;  // Instantiate.


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Say(" ");     // Helps to align the next line of output.


  // Set the initial time.
  Say("-- 1.0 --");
  //            yy  mm  dd  hh  mm  ss  ww     For ww: 1=Sunday
  clock.setTime(15, 12, 02, 14, 35, 00, 04);
  Serial.println("The clock has been set to the time below '-- 2.0 --'");


  // If we just want the time on the console we can do this:
  Say("-- 2.0 --");
  clock.displayTimeToConsole();


  // Get the time now. Format: yymmddhhmmssww.
  Say("-- 3.0 --");
  String strTimeNow = clock.getTime();
  Serial.print("The time now is: ");
  Serial.println(strTimeNow);


  // To get a part of the time field. e.g. What day of the week is it? We do this: (Note 3rd argument).
  Say("-- 4.0 --");
  char v, w;
  clock.readADateTimeField(&v, &w, 'w');   //Select with: {y|m|d|h|n|s|w}
  Serial.print("Day of the week is: ");
  Serial.print(v);
  Serial.println(w);


  // To adjust the clock's time to get it spot on, use these functions.
  Say("-- 5.0 --");
  Say("Time now...");
  clock.displayTimeToConsole();
  Say("Add a minute...");
  clock.adjustClockUpByOneMinute();
  clock.displayTimeToConsole();
  Say("Zero the seconds...");
  clock.adjustClockSecondsToZero();
  clock.displayTimeToConsole();
}


void loop()
{
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.print("                                      ");    // Right shift the time
  clock.displayTimeToConsole();
  Serial.println();


  // How long until 14:37? Example showing the count down then count up after the time has passed.
  // Note the output format change after time (14:37) is passed. Also note that the countup/countdown will reverse at midnight.
  Say("-- 6.1 --");
  float fhhmm[2] = {14.0, 37.0};
  String tdf = clock.timeDiff(fhhmm);
  Serial.print("Time diff until 14:37 using float (hh-mm-ss) = ");
  Serial.println(tdf);

  Say("-- 6.2 --");
  // Same thing using integers.
  int ihhmm[2] = {14, 37};
  String tdi = clock.timeDiff(ihhmm);
  Serial.print("Time diff until 14:37 using int.  (hh-mm-ss) = ");
  Serial.println(tdi);
 

  // Indicate if a time has been passed. The method supports two input formats.
  Say("-- 7.1 --");
  // "Day of the year" format example.
  char answer = clock.isItTime(1512021437);
  Serial.print("Is it time yet (02 December 2015, 14:37)?      ");
  Serial.println(answer);

  Say("-- 7.2 --");
  // "Same day" format example.
  answer = clock.isItTime(1437);
  Serial.print("Is it time yet          (today at 14:37)?      ");
  Serial.println(answer);

  delay(10000);   // Every 10 seconds... 
}  // End loop()


// Just to save typing...
void Say(char x[]) {
  Serial.println(x);
}















