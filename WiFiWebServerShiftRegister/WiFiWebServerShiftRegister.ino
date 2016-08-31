#include <ESP8266WiFi.h>

const char* ssid = "ariffinastute";
const char* password = "se7enWIFI";

// Pin definitions:
// The 74HC595 uses a type of serial connection called SPI
// (Serial Peripheral Interface) that requires three pins:

int datapin = 3; 
int clockpin = 2;
int latchpin = 0;

// We'll also declare a global variable for the data we're
// sending to the shift register:

byte data = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set the three SPI pins to be outputs:
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  // Turn all the LEDs off  
  for(int index = 0; index <= 7; index++)
  {
    shiftWrite(index, LOW);
  }
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\n');
  Serial.println(req);
  
  // Match the request
  int number;
  int val;
  // Request for QA
  if (req.indexOf("/?pin=0&value=LOW") != -1){
    number = 0;
    val = 0;
  }
  else if (req.indexOf("/?pin=0&value=HIGH") != -1){
    number = 0;
    val = 1;
  }

  // Request QB
  else if (req.indexOf("/?pin=1&value=LOW") != -1){
    number = 1;
    val = 0;
  }
  else if (req.indexOf("/?pin=1&value=HIGH") != -1){
    number = 1;
    val = 1;
  }

  // Request for QC
  else if (req.indexOf("/?pin=2&value=LOW") != -1){
    number = 2;
    val = 0;
  }
  else if (req.indexOf("/?pin=2&value=HIGH") != -1){
    number = 2;
    val = 1;
  }

  // Request for QD
  else if (req.indexOf("/?pin=3&value=LOW") != -1){
    number = 3;
    val = 0;
  }
  else if (req.indexOf("/?pin=3&value=HIGH") != -1){
    number = 3;
    val = 1;
  }
  // Request for QE
  else if (req.indexOf("/?pin=4&value=LOW") != -1){
    number = 4;
    val = 0;
  }
  else if (req.indexOf("/?pin=4&value=HIGH") != -1){
    number = 4;
    val = 1;
  }

  // Request QF
  else if (req.indexOf("/?pin=5&value=LOW") != -1){
    number = 5;
    val = 0;
  }
  else if (req.indexOf("/?pin=5&value=HIGH") != -1){
    number = 5;
    val = 1;
  }

  // Request for QG
  else if (req.indexOf("/?pin=6&value=LOW") != -1){
    number = 6;
    val = 0;
  }
  else if (req.indexOf("/?pin=6&value=HIGH") != -1){
    number = 6;
    val = 1;
  }

  // Request for QH
  else if (req.indexOf("/?pin=7&value=LOW") != -1){
    number = 7;
    val = 0;
  }
  else if (req.indexOf("/?pin=7&value=HIGH") != -1){
    number = 7;
    val = 1;
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set IO according to the request
  shiftWrite(number, val);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO";
  s += number;
  s += " is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void shiftWrite(int desiredPin, boolean desiredState)

// This function lets you make the shift register outputs
// HIGH or LOW in exactly the same way that you use digitalWrite().

// Like digitalWrite(), this function takes two parameters:

//    "desiredPin" is the shift register output pin
//    you want to affect (0-7)

//    "desiredState" is whether you want that output
//    to be HIGH or LOW

// Inside the Arduino, numbers are stored as arrays of "bits",
// each of which is a single 1 or 0 value. Because a "byte" type
// is also eight bits, we'll use a byte (which we named "data"
// at the top of this sketch) to send data to the shift register.
// If a bit in the byte is "1", the output will be HIGH. If the bit
// is "0", the output will be LOW.

// To turn the individual bits in "data" on and off, we'll use
// a new Arduino commands called bitWrite(), which can make
// individual bits in a number 1 or 0.
{
  // First we'll alter the global variable "data", changing the
  // desired bit to 1 or 0:

  bitWrite(data,desiredPin,desiredState);

  // Now we'll actually send that data to the shift register.
  // The shiftOut() function does all the hard work of
  // manipulating the data and clock pins to move the data
  // into the shift register:

  shiftOut(datapin, clockpin, MSBFIRST, data);

  // Once the data is in the shift register, we still need to
  // make it appear at the outputs. We'll toggle the state of
  // the latchPin, which will signal the shift register to "latch"
  // the data to the outputs. (Latch activates on the low-to
  // -high transition).

  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);
}


/*
oneAfterAnother()

This function will light one LED, delay for delayTime, then light
the next LED, and repeat until all the LEDs are on. It will then
turn them off in the reverse order.
*/

void handleRunner()
{
  Serial.println();
  Serial.println("Did Running LED Effect Job!");
  
  int index;
  int delayTime = 100; // Time (milliseconds) to pause between LEDs
                       // Make this smaller for faster switching

  // Turn all the LEDs on:

  // This for() loop will step index from 0 to 7
  // (putting "++" after a variable means add one to it)
  // and will then use digitalWrite() to turn that LED on.

  for(index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);
    delay(delayTime);                
  }

  // Turn all the LEDs off:

  // This for() loop will step index from 7 to 0
  // (putting "--" after a variable means subtract one from it)
  // and will then use digitalWrite() to turn that LED off.

  for(index = 7; index >= 0; index--)
  {
    shiftWrite(index, LOW);
    delay(delayTime);
  }
}


/*
oneOnAtATime()

This function will step through the LEDs, lighting one at at time.
*/

void handleScroll()
{
  Serial.println();
  Serial.println("Did Scrolling LED Effect Job!");
  
  int index;
  int delayTime = 100; // Time (milliseconds) to pause between LEDs
                       // Make this smaller for faster switching

  // step through the LEDs, from 0 to 7

  for(index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);    // turn LED on
    delay(delayTime);       // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
}


/*
pingPong()

This function will step through the LEDs, lighting one at at time,
in both directions.
*/

void handlePingPong()
{
  Serial.println();
  Serial.println("Did Ping Pong LED Effect Job!");
  
  int index;
  int delayTime = 100; // time (milliseconds) to pause between LEDs
                       // make this smaller for faster switching

  // step through the LEDs, from 0 to 7

  for(index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);    // turn LED on
    delay(delayTime);       // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }

  // step through the LEDs, from 7 to 0

  for(index = 7; index >= 0; index--)
  {
    shiftWrite(index, HIGH);    // turn LED on
    delay(delayTime);       // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
}


/*
randomLED()

This function will turn on random LEDs. Can you modify it so it
also lights them for random times?
*/

void handleRandom()
{
  Serial.println();
  Serial.println("Did Random LED Effect Job!");
   
  int index;
  int delayTime = 100; // time (milliseconds) to pause between LEDs
                       // make this smaller for faster switching

  // The random() function will return a semi-random number each
  // time it is called. See http://arduino.cc/en/Reference/Random
  // for tips on how to make random() more random.

  index = random(8);    // pick a random number between 0 and 7

  shiftWrite(index, HIGH);  // turn LED on
  delay(delayTime);     // pause to slow down the sequence
  shiftWrite(index, LOW);   // turn LED off
}


/*
marquee()

This function will mimic "chase lights" like those around signs.
*/

void handleMarquee()
{
  Serial.println();
  Serial.println("Did Marque LED Effect Job!");
  
  int index;
  int delayTime = 200; // Time (milliseconds) to pause between LEDs
                       // Make this smaller for faster switching

  // Step through the first four LEDs
  // (We'll light up one in the lower 4 and one in the upper 4)

  for(index = 0; index <= 3; index++)
  {
    shiftWrite(index, HIGH);    // Turn a LED on
    shiftWrite(index+4, HIGH);  // Skip four, and turn that LED on
    delay(delayTime);       // Pause to slow down the sequence
    shiftWrite(index, LOW); // Turn both LEDs off
    shiftWrite(index+4, LOW);
  }
}


/*
binaryCount()

Numbers are stored internally in the Arduino as arrays of "bits",
each of which is a 1 or 0. Just like the base-10 numbers we use
every day, The position of the bit affects the magnitude of its 
contribution to the total number:

Bit position   Contribution
0              1
1              2
2              4
3              8
4              16
5              32
6              64
7              128

To build any number from 0 to 255 from the above 8 bits, just
select the contributions you need to make. The bits will then be
1 if you use that contribution, and 0 if you don't.

This function will increment the "data" variable from 0 to 255
and repeat. When we send this value to the shift register and LEDs,
you can see the on-off pattern of the eight bits that make up the
byte. See http://www.arduino.cc/playground/Code/BitMath for more
information on binary numbers.
*/

void handleBinary()
{  
  Serial.println();
  Serial.println("Did Binary LED Effect Job!");
  
  int delayTime = 1000; // time (milliseconds) to pause between LEDs
                        // make this smaller for faster switching

  // Send the data byte to the shift register:

  shiftOut(datapin, clockpin, MSBFIRST, data);

  // Toggle the latch pin to make the data appear at the outputs:

  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);

  // Add one to data, and repeat!
  // (Because a byte type can only store numbers from 0 to 255,
  // if we add more than that, it will "roll around" back to 0
  // and start over).

  data++;

  // Delay so you can see what's going on:

  delay(delayTime);
}

