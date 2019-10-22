/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
 NOTES FOR JULIANHG's WEMOS D1 MINI WIFI/IOT CUPBOARD LOCK PROJECT:
 1)sign up to blynk
 2)download the app to your smartphone etc
 3)open app, sign in, 'make a new project' (save/ok)
 4)use the unique Auth token that will arive via email in the code below...
 5)also fill in your homes own WiFi settings in the code below (so the lock can automatically connect to the internet)
 6)open up your new project on your smartphone's app... we need to add a couple of widgets to it...
 7)add a button... using the settings: output V3 0-90, then make it a switch. change it's label/appearance to your liking.
 8)add an email widget... then edit it: to: should be blank (we will keep the email address in the lock/software below), contect type: text/plain
 9)when making your circuit/breadboard:
      D1 on the D1 mini goes to:  SERVO SIGNAL WIRE
      D2 on the D1 mini goes to:  OPTICAL SWITCH SIGNAL WIRE
      D3 on the D1 mini goes to:  the resistor before the BLUE LED
      D4 on the D1 mini goes to:  the resistor before the RED LED
      GND on the D1 mini goes to: all devices GND leads (eg servo, optical switch and both LEDs)
      +5V (or +3V depending on your devices) on the D1 mini goes to: some devices +ve leads (eg servo and optical switch) 
                                                                      (note: the +ve for the LED's is supplied by D3 and D4)
      
 
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "PUT YOUR BLYNK AUTH CODE HERE";    // ########## YOU PUT YOUR OWN AUTH TOKEN HERE - you get emailed it when you start a new project
                  
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PUT YOUR WIFI's SSID HERE";        // ##########
char pass[] = "PUT YOUR PASSWORD HERE";           // ##########
int LED_RED = 0; // IDE pin number for red led (actually PIN 3 on D1 Mini)
int LED_BLUE = 2; // IDE pin number for red led (actually PIN 4 on D1 Mini)

Servo servo;

BLYNK_WRITE(V3)
{
  servo.write(param.asInt());
  if (servo.read()<=70){ 
    Serial.println("<=70 (UNLOCKED!!!!)");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, LOW);
  }
  if (servo.read()>70){ 
    Serial.println(">70 (LOCKED)");
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, HIGH);
  }
}

ICACHE_RAM_ATTR void cupboardSwitch_doorOpened()   // the ICACHE_RAM_ATTR stops the interupt causing an error and reconnecting to blynk server every few secs....
{
  // *** WARNING: You are limited to send ONLY ONE E-MAIL PER 15 SECONDS! ***
  // Let's send an e-mail when you press the button
  // connected to digital pin 2 on your Arduino
  int isButtonPressed = !digitalRead(4); // Invert state, since button is "Active LOW"
  Serial.println("cupboard opened"); // This can be seen in the Serial Monitor
  if (isButtonPressed) // You can write any condition to trigger e-mail sending
  {
    sendCupboardOpenEmail();
  }
}

void sendCupboardOpenEmail()
{
  
    Blynk.email("YOUR_EMAIL_address@something.com", "### THE CUPBOARD WAS OPENED ###", "just an FYI... the cupboard was just opened!");
    // ####### PUT YOUR OWN EMAIL ADDRESS IN ABOVE LINE
    // Or, if you want to use the email specified in the App (like for App Export):
    //Blynk.email("Subject: Button Logger", "You just pushed the button...");
}

ICACHE_RAM_ATTR void cupboardSwitch_doorClosed()   // the ICACHE_RAM_ATTR stops the interupt causing an error and reconnecting to blynk server every few secs....
{
  // *** WARNING: You are limited to send ONLY ONE E-MAIL PER 15 SECONDS! ***
  // Let's send an e-mail when you press the button
  // connected to digital pin 2 on your Arduino
  int isButtonPressed = !digitalRead(4); // Invert state, since button is "Active LOW"
  Serial.println("cupboard closed"); // This can be seen in the Serial Monitor
  
  if (isButtonPressed) // You can write any condition to trigger e-mail sending
  {
    //sendCupboardClosedEmail()
  }
}

void sendCupboardClosedEmail()
{
  
    Blynk.email("YOUR_EMAIL_address@something.com", "The cupboard was closed", "just an FYI... the cupboard was just closed!");
    // ####### PUT YOUR OWN EMAIL ADDRESS IN ABOVE LINE
    // Or, if you want to use the email specified in the App (like for App Export):
    //Blynk.email("Subject: Button Logger", "You just pushed the button...");
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  // Send e-mail when your hardware gets connected to Blynk Server
  // Just put the recepient's "e-mail address", "Subject" and the "message body"
  // Blynk.email("your email address here", "Cupboard lock enabled", "cupboard lock successfully joined the blynk server");
  // Setting the button
  pinMode(4, INPUT_PULLUP);
  // Attach pin 2 interrupt to our handler (physical pin 2 on D1 mini board is "pin 4" in this arduino IDE)
  attachInterrupt(digitalPinToInterrupt(4), cupboardSwitch_doorOpened, FALLING);
  //attachInterrupt(digitalPinToInterrupt(4), cupboardSwitch_doorClosed, RISING); // i didnt use this 'cupboard closed' code in the end, it never worked properly for me.
  servo.attach(5); // (5 on this IDE is really D1 on the D1 mini board)
}

void loop()
{
  Blynk.run();
}
