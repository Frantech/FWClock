# FWClock - Extended Interface for DS3231 Real Time Clock.
This code was written for use with a DS3231 RTC connected to an Arduino.

The library provides a suite of methods to do the following:
    - Set, read, and adjust the time.
    - Display the current time or time until a set time.
    - Notify a part of the time. e.g. What month is it? What day of the week is it?
    - Indicate if a time given has passed.
    - Provide a time difference, within a day, to or from a given time. 

The code is provided as a C++ library with an example of it's use that demonstrates calls to each of it's methods. 
It has been written with the intention of a making it easy to use a Real Time Clock with an LED TM1638 seven segment display module.
Hence output is provided generally provided as char or String.

 
