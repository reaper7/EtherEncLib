/*
  HelloWorld.ino - Sample Code consuming Library for Ethernet ENC29J60 Module.
                   Prints Html page with one button for activating Arduino ports
  Created by Renato Aloi, August, 2013.
  Released into the public domain.
  -- May 2015
  Modified by Renato Aloi according to changes at library level, 
  made by Suchkov (SKA)

*/

#include <SPI.h>
#include <EtherEncLib.h>
#if (!ESP8266) && (!ENERGIA)
#include <avr/pgmspace.h>
#endif

const PROGMEM char resp200Txt[] = {"HTTP/1.0 200 OK\n\rContent-Type: text/html\n\rPragma: no-cache\n\r\n\r"};

#if (ENERGIA)
const int RelayPin = 2;
#elif (ESP8266)
int RelayPin = 16;
#else
int RelayPin = 7;
#endif

EtherEncLib lib(80);

static unsigned char ipaddr[] = { 192, 168, 0, 125 };
static unsigned char macaddr[] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x25 };

void setup()
{
    Serial.begin(115200);

#if (ESP8266)
    pinMode(5,OUTPUT);	//--- ? -- SS pin must be output # by Renato Aloi
#else
    pinMode(10,OUTPUT);	//--- ? -- SS pin must be output # by Renato Aloi
#endif
    
    //
    // Starting the lib
    //
    lib.begin(ipaddr, macaddr);
    
    //
    // Configuring relay output
    //
    pinMode(RelayPin, OUTPUT);
    digitalWrite(RelayPin, LOW);
    
    Serial.println("EtherEncLib.h started!");
}

void loop()
{
    //
    // Check if request has arrived
    //
    if (lib.available())
    {
        //
        // EtherEncLib deals with GET and POST methods
        // Obs: POST method not yet implemented
        //
        
        // GET parameters
        char *params = lib.getParams();
        
        Serial.print("Params: ");
        Serial.println(params);
        
        //
        // Printing HTML header
        //
        lib.print("<HTML>");
        lib.print("<HEAD><TITLE>Arduino EtherEncLib.h</TITLE></HEAD>");
        lib.print("<BODY>");
        
        //
        // Printing welcome message
        //
        lib.print("<h3>Welcome to EtherEncLib.h library!</h3>");
        
        //
        // Checking parameters
        //
        if (strncmp(&params[1], "button", 5) == 0 )
        {
            //
            // If there is a button parameter
            //
            
            //
            // Checking for button value
            //
            if (strncmp(&params[8], "1", 1) == 0)
            {
                //
                // If received button state 1
                //
                
                //
                // Turn on relay
                //
                digitalWrite(RelayPin, HIGH);
                
                //
                // Printing the link modified for deactivation
                //
                lib.print("<p><a href=/?button=0>Click here to deactivate!</a></p>");
            }
            else
            {
                //
                // If received button state 0
                //
                
                //
                // Turn off relay
                //
                digitalWrite(RelayPin, LOW);
                
                //
                // Printing the link modified for activation
                //
                lib.print("<p><a href=/?button=1>Click here to activate!</a></p>");
            }
        }
        else
        {
            //
            // If there is NOT a button parameter
            // Normally this is the first call to the page
            //
            lib.print("<p><a href=/?button=1>Click here to activate!</a></p>");
        }
        
        // Printing HTML footer
        lib.print("</BODY>");
        lib.print("</HTML>");
        
        //
        // Closing connection
        // Put the EtherEncLib in listen state again
        //
        lib.close();
    }
}
