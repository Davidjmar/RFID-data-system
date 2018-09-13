/*
  RFID Eval 13.56MHz Shield example sketch v10

  Aaron Weiss, aaron at sparkfun dot com
  OSHW license: http://freedomdefined.org/OSHW

  works with 13.56MHz MiFare 1k tags

  Based on hardware v13:
  D7 -> RFID RX
  D8 -> RFID TX
  D9 -> XBee TX
  D10 -> XBee RX

  Note: RFID Reset attached to D13 (aka status LED)

  Note: be sure include the SoftwareSerial lib, http://arduiniana.org/libraries/newsoftserial/

  Usage: Sketch prints 'Start' and waits for a tag. When a tag is in range, the shield reads the tag,
  blinks the 'Found' LED and prints the serial number of the tag to the serial port
  and the XBee port.
  
  06/04/2013 - Modified for compatibility with Arudino 1.0. Seb Madgwick.

*/
#include <SoftwareSerial.h>

SoftwareSerial rfid(7, 8);
SoftwareSerial xbee(10, 9);

//Prototypes
void check_for_notag(void);
void halt(void);
void parse(void);
void print_serial(void);
void read_serial(void);
void seek(void);
void set_flag(void);
void assign_Identity(void);

//Global var
int flag = 0;
bool checkedInJD = false;
bool checkedInMJ = false;
bool checkedInMoJ = false;
bool checkedInJB = false;
bool checkedInAS = false;
int counter = 0;
int Str1[11];

//INIT
void setup()
{
  Serial.begin(9600);
  Serial.println("Start");

  // set the data rate for the SoftwareSerial ports
  xbee.begin(115200);//115200
  rfid.begin(19200);
  delay(10);
  halt();
}

//MAIN
void loop()
{
  read_serial();
  assign_Identity();
}

void check_for_notag()
{
  seek();
  delay(10);
  parse();
  set_flag();

  if(flag = 1){
    seek();
    delay(10);
    parse();
  }
}

void halt()
{
 //Halt tag
  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)147);
  rfid.write((uint8_t)148);
}

void parse()
{
  while(rfid.available()){
    if(rfid.read() == 255){
      for(int i=1;i<11;i++){
        Str1[i]= rfid.read();
      }
    }
  }
}

void print_serial()
{
  if(flag == 1){
    //print to serial port
    Serial.print(Str1[8], HEX);
    Serial.print(Str1[7], HEX);
    Serial.print(Str1[6], HEX);
    Serial.print(Str1[5], HEX);
    Serial.println();
    //print to XBee module
    xbee.print(Str1[8], HEX);
    xbee.print(Str1[7], HEX);
    xbee.print(Str1[6], HEX);
    xbee.print(Str1[5], HEX);
    xbee.println();
    delay(100);
    //check_for_notag();
  }
}

void read_serial()
{
  seek();
  delay(10);
  parse();
  set_flag();
  print_serial();
  delay(100);
}

void seek()
{
  //search for RFID tag
  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)130);
  rfid.write((uint8_t)131);
  delay(5);
}

void set_flag()
{
  //in here set more checked in or not checked in booleans for each associated RFID tag
  //initialize all of them as false and then change them here in the set flag program
  if(Str1[2] == 6){
    flag++;
  }
  if(Str1[2] == 2){
    flag = 0;
  }
}

void assign_Identity()
{
  //58F1E01D == John Doe
    if( Str1[8] == 0x58 && Str1[7] == 0xF1 && Str1[6] == 0xE0 && Str1[5] == 0x1D)
    {
      if(checkedInJD == false)
      {
        xbee.print("Name: John Doe");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 34");
        xbee.println();
        xbee.print("Married to: Ryan Reynolds");
        xbee.println();
        xbee.print("Children: Jimmy Kimmel");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED IN");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter++;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInJD = true;
        delay(100);

      }
      else if(checkedInJD == true)
      {
        xbee.print("Name: John Doe");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 34");
        xbee.println();
        xbee.print("Married to: Ryan Reynolds");
        xbee.println();
        xbee.print("Children: Jimmy Kimmel");
        xbee.println();
         xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED OUT");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter = counter- 1;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInJD = false;
        xbee.println();
        delay(100);
     }
    }  

   //DCC3F9B4 == Madison James
     if( Str1[8] == 0xDC && Str1[7] == 0xC3 && Str1[6] == 0xF9 && Str1[5] == 0xB4)
    {
      if(checkedInMJ == false)
      {
        xbee.print("Name: Madison James");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 42");
        xbee.println();
        xbee.print("Married to: Dylan Clark");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED IN");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter++;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInMJ = true;
        delay(100);
      }
      else if(checkedInMJ == true)
      {
        xbee.print("Name: Madison James");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 42");
        xbee.println();
        xbee.print("Married to: Dylan Clark");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED OUT");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter = counter -1;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInMJ = false;
        delay(100);
      }
     }

     //DCC3E9B4 == Juan Brooks
     if( Str1[8] == 0xDC && Str1[7] == 0xC3 && Str1[6] == 0xE9 && Str1[5] == 0xB4)
    {
      if(checkedInJB == false)
      {
        xbee.print("Name: Juan Brooks");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 19");
        xbee.println();
        xbee.print("Married to: N/A");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED IN");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter++;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInJB = true;
        delay(100);
      }
      else if(checkedInJB == true)
      {
        xbee.print("Juan Brooks just signed out.");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 19");
        xbee.println();
        xbee.print("Married to: N/A");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED OUT");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter = counter -1;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInJB = false;
        delay(100);
      }
     }

     //DCC42144 == Aisha Smith
     if( Str1[8] == 0xDC && Str1[7] == 0xC4 && Str1[6] == 0x21 && Str1[5] == 0x44)
    {
      if(checkedInAS == false)
      {
        xbee.print("Name: Aisha Smith");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 22");
        xbee.println();
        xbee.print("Married to: N/A");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED IN");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter++;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInAS = true;
        delay(100);
      }
      else if(checkedInAS == true)
      {
        xbee.print("Name: Aisha Smith");
        xbee.println();
        xbee.print("Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 22");
        xbee.println();
        xbee.print("Married to: N/A");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED OUT");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter = counter -1;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInAS = false;
        delay(100);
      }
     }

     //DCC49E4 == Mohammed Jones
     if( Str1[8] == 0xDC && Str1[7] == 0xC4 && Str1[6] == 0x9E && Str1[5] == 0x4)
    {
      if(checkedInMoJ == false)
      {
        xbee.print("Name: Mohammed Jones.");
        xbee.println();
        xbee.print("Sign-In-Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 15");
        xbee.println();
        xbee.print("Married to: N/A");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED IN");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter++;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInMoJ = true;
        delay(100);
      }
      else if(checkedInMoJ == true)
      {
        xbee.print("Mohammed Jones");
        xbee.println();
        xbee.print("Sign-Out-Location: ITLL - 112S");
        xbee.println();
        xbee.print("Age: 15");
        xbee.println();
        xbee.print("Married to: N/A");
        xbee.println();
        xbee.print("Children: N/A");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        xbee.print("SIGNED OUT");
        xbee.println();
        xbee.print("----------");
        xbee.println();
        counter = counter -1;
        xbee.print("Number of Occupants: ");
        xbee.print(counter);
        xbee.println();
        xbee.print("---------------------------------");
        xbee.println();
        checkedInMoJ = false;
        delay(100);
      }
    }     
  }


// so I want to write a function that assign s teh identity of a give rfid tag. With that information it should then be able to t least pul up the name of the person and some kind of relevant information about them
// Then I want to create a body count function that will use flags to essentially see if the person is leaving the tent or entering it. It will then print the count to the app
// buy a ble bluetooth detector asap

