/*
  HelloWorld.ino - Sample Code consuming Library for Ethernet ENC29J60 Module.
                   Prints Html page with welcome message
  Created by Renato Aloi, August, 2013.
  Released into the public domain.

  -- May 2015
  Modified by Renato Aloi according to changes at library level, 
  made by Suchkov (SKA)

*/

#include <SPI.h>
#include <EtherEncLib.h>
#if (ESP8266)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

const PROGMEM char resp200Txt[] = {"HTTP/1.0 200 OK\n\rContent-Type: text/html\n\rPragma: no-cache\n\r\n\r"};

EtherEncLib lib(80);

static unsigned char ipaddr[] = { 192, 168, 0, 125 };
static unsigned char macaddr[] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x25 };

void setup()
{
#if (ESP8266)
    Serial.begin(115200);
    pinMode(5,OUTPUT);	//--- ? -- SS pin must be output # by Renato Aloi
#else
    Serial.begin(9600);
    pinMode(10,OUTPUT);	//--- ? -- SS pin must be output # by Renato Aloi
#endif
    
    //
    // Starting the lib
    //
    lib.begin(ipaddr, macaddr);
    
    Serial.println(F("EtherEncLib.h started!"));
}

void loop()
{
    //
    // Check if request has arrived
    //
    if (lib.available())
    {
        char *params = lib.getParams();
        
        Serial.print(F("Params: "));
        Serial.println(params);
        
	//
	// Printing HTTP 200 OK
	//
	lib.print((char *)&resp200Txt[0],strlen_P(&resp200Txt[0]));

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
