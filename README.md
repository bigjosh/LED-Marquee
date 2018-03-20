# LED-Marquee


This project uses 2 Arduino boards to retrieve a custom message that is saved on my personal website server and display it on the LED marquee sign. Here's the steps that are taken to achieve that:

data.txt - This is a plain text file and lives in a folder called /marquee/ on the root of my server and contains whatever custom message I want the sign to display. In future phases, this may contain multiple messages formatted in JSON, but I'm not there yet. For now, sticking with strictly plain text. Currently, each line of text is separated by a 'Z' character so we can grab each line one at a time.

Phase1_ESP - This is the sketch that is uploaded to the NodeMCU ESP8266 WIFI board. It is used strictly for communicating with my server via my wifi network to retrieve the custom message. It then holds onto that message in a variable until the Arduino UNO requests it.

Phase1_Uno - This is the sketch that is uploaded to the Arduino UNO board. The UNO is connected to 8 LED strips and also to the ESP. It scrolls/displays a message along the length of the sign, and when the scrolling/displaying completes, it requests the custom message from the ESP (by sending out a special character to the ESP; something like '?'). The ESP responds and sends it to the UNO, which does some parsing to ensure it displays properly, then begins scrolling/displaying again, with the new custom message. NOTE: The UNO is already utilizing pin 0 (RX) for one of the LED strips, so it is set up to use the SoftwareSerial library, which allows me to use pin 10 as RX and pin 11 as TX. These of course are virtual RX/TX pins, but they bahave the save way the hardware RX/TX pins do. RX/TX on UNO is connected to TX/RX on ESP.
