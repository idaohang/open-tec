#include <SD.h>

// This code still produced data in NMEA.
// to make it able to produce data in SirfIII:
// Format: $PSRF100,<protocol>,<baud>,<DataBits>,<StopBits>,<Parity>*CKSUM<CR><LF> 
// <protocol> 0=SiRF Binary, 1=NMEA, 4=USER1
// <baud> 1200, 2400, 4800, 9600, 19200, 38400
// <DataBits> 8,7. Note that SiRF protocol is only valid in f8 data bits
// <StopBits> 0,1
// <Parity> 0 = None, 1 = Odd, 2 = Even
//
// Example : Switch to SiRF Binary Protocol at 9600,8,N,1
// $PSRF100,0,9600,8,1,0*0C,<CR><LF>
 

const int chipSelect = 10;
void setup()
{
  Serial.begin(9600);
  
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
   
    return;
  }
 
}

void loop()
{
  // make a string for assembling the data to log:
  char index = 0;
  char temp = 0;
  String dataString = "";
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  /*
    while(Serial.available())
    {
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      if(dataFile)
      {
        temp = Serial.read();
        dataString += String(temp);
        dataFile.print(dataString);
        dataString = "";
        dataFile.close();
      }
    }
  */  
File dataFile = SD.open("datalog.txt", FILE_WRITE);
 if(dataFile)
 {
   while(Serial.available())
   {
     temp = Serial.read();
     dataString += String(temp);
     index++;
     if(index>200)
       break;
   }
   dataFile.print(dataString);
   dataFile.close();
 }
} 

