/*
Author: Qijun Tang
Guler Lab @Uva
4 beam per cage beam break

modified on 2020.09.22


*/

#include <DS3231.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

//pins for LCD
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// pins for SD card
File myFile;
int pinCS = 53; // Pin 10 on Arduino Uno
int n_file = 1;                 // a counter to generate new files in SD if the old one is not renamed yet
int pin_ClockPower = 13;     // use the pin 13 as the power for the RTC module

// pins for beam break data
// 4 pins per cage, use 4x4 matrix to store info
int datapin[4][4] = {
  {23,25,27,29},
  {22,24,26,28},
  {35,37,39,41},
  {34,36,38,40}
};
int pinstat[4][4]={
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};
                 

//others
int interval = 10000;           // register data every 10,000 ms
int n_interval = 0;             // a counter for total intervals that are collected


String filename="Exp_"+String(n_file)+".txt";


 
void setup() { 
    
  for (int i=0;i<4;i++){
    for (int j=0;j<4;j++){
      pinMode(datapin[i][j],INPUT);
    }
  }

// Initialize the rtc object
  
  pinMode(pinCS, OUTPUT);  //SD card
  pinMode(pin_ClockPower, OUTPUT);
  digitalWrite(pin_ClockPower, LOW);     
 
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

   // SD Card Initialization
  if (SD.begin()){
    lcd.print("SD card ready");
    lcd.clear();
  } 
  else{
    lcd.print("SD card failed");
    return;
  }

  while (SD.exists(filename)){
    n_file++;
    filename="Exp_"+String(n_file)+".txt";
  }

  Serial.println(filename);
  myFile = SD.open(filename,FILE_WRITE);
  
  if (myFile){
    lcd.print(filename+" Ready");
    digitalWrite(pin_ClockPower, HIGH);
    rtc.begin();
    
      // The following lines can be uncommented to set the date and time
    //rtc.setDOW(TUESDAY);     // Set Day-of-Week to TUESDAY
    //rtc.setTime(13, 27, 30);     // Set the time to HH:MM:SS (24hr format)
    //rtc.setDate(22, 2, 2022);   // Set the date to DAY,MONTH,YEAR
    
    myFile.print("Start time:");
    myFile.print(rtc.getDateStr());
    myFile.print(",");
    myFile.print(rtc.getTimeStr());
    myFile.print(",");
    myFile.print(rtc.getDOWStr());
    myFile.println();
    digitalWrite(pin_ClockPower, LOW);
    myFile.println("Intervals,Cage1-total,Cage1-amb,Cage2-total,Cage2-amb,Cage3-total,Cage3-amb,Cage4-total,Cage4-amb");
  } 
  else{
    lcd.print("File error");
    return;
  }
}

unsigned long startTime=millis(); //Capture current time
int laststat[4][4]={
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
  };                                //for calculating if there is beambreak, and count total
int lastbeambroken[4][4]={
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
  };                                //for calculating ambulatory movement

void loop(){
//set the counter to 0
  int n_total[4]={0,0,0,0};
  int n_amb[4]={0,0,0,0};
  
  n_interval++;
  


  while(millis()-startTime < interval) //loop for a set interval
  {
    // read through all the beams
    for (int i=0;i<4;i++){
      for (int j=0;j<4;j++){
        pinstat[i][j]=!digitalRead(datapin[i][j]); //invert the read so when the beam is broken (digital reads LOW), stat becomes 1
      }
    }

    // then decide if there is beambreak, total or amb
    for (int i=0;i<4;i++){
      int yestotal=0;
      int yesamb=0;
      for (int j=0;j<4;j++){                                  
        if (pinstat[i][j] && !laststat[i][j]){            
          yestotal++;
        }
        laststat[i][j]=pinstat[i][j];              //update laststat matrix        
        if (pinstat[i][j] && !lastbeambroken[i][j]){       
          yesamb++;
        }  
        
      }
      //increment to counter after went through each cage
      if (yestotal>0){
        n_total[i]++;
        // when there is a new beambroken, update lastbeambroken matrix
        for (int i=0;i<4;i++){
          for (int j=0;j<4;j++){
            lastbeambroken[i][j]=pinstat[i][j];
          }
        }
      }
      if (yesamb>0){
        n_amb[i]++;
      }
    }

  //delay(10);  //a little delay in case it goes crazy
  }
  
  myFile.print(n_interval);
  
  for (int i=0;i<4;i++){
    myFile.print(",");
    myFile.print(n_total[i]);
    myFile.print(",");
    myFile.print(n_amb[i]);
  }
  myFile.println();

  lcd.setCursor(1,2);
  lcd.print("                ");
  lcd.setCursor(1,2);
  lcd.print(String(n_interval)+" "+String(n_amb[0])+" "+String(n_amb[1])+" "+String(n_amb[2])+" "+String(n_amb[3]));
  

  //flush data to SD card every 300s
  if (n_interval%(300000/interval)==0){
    myFile.flush();
    lcd.clear();
    lcd.print(String(n_interval)+"Saved"+filename);
  }
  startTime += interval;
}
