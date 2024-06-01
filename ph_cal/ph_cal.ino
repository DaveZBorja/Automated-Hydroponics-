/*
 * Dear programmer:
 * 
 * When I wrote this code, only god and
 * I knew how it worked.
 * Now, only god knows it!
 * 
 * Therefore, if u are trying to optimize
 * this routine and it fails (most surely),
 * please increase this counter as a
 * Warning for the next person:
 * 
 * total_hours_wasted_here = 96
 * 
 * By: Dave Borja
 */

#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int chipSelect = 10; // SD card chipSelect pin

const int SOIL_MOISTURE_PIN = A1;
const int RELAY_PH_UP = 3;
const int RELAY_PH_DOWN = 4;
const int RELAY_SOIL_MOISTURE = 2;

#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection

// Define the target pH value
const float TARGET_PH = 6.0;

// Define the pH offset values for the pH up and pH down solutions
const float PH_UP_OFFSET = 0.5;
const float PH_DOWN_OFFSET = -0.5;

int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

// Function to read the soil moisture value from the sensor
int getSoilMoisture() {
  int sensorValue = analogRead(SOIL_MOISTURE_PIN);
  int moisture = map(sensorValue, 0, 1023, 0, 100);
  return moisture;
}

void setup()
{
  Serial.begin(9600);

    // Set the relay pins as outputs
  pinMode(RELAY_PH_UP, OUTPUT);
  pinMode(RELAY_PH_DOWN, OUTPUT);
  pinMode(RELAY_SOIL_MOISTURE, OUTPUT);

  // Turn off the relay modules at the start
  digitalWrite(RELAY_PH_UP, HIGH);
  digitalWrite(RELAY_PH_DOWN, HIGH);
  digitalWrite(RELAY_SOIL_MOISTURE, HIGH);
  
  lcd.init();
  lcd.backlight();
  
  Serial.println("pH Test");    //Test the serial monitor
  Serial.print("Initializing SD card...");
    
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    lcd.setCursor(2,0);
    lcd.print("SD Card Error!!");
    while (1);
  }
  Serial.println("card initialized.");


  digitalWrite(RELAY_SOIL_MOISTURE, HIGH);
  digitalWrite(RELAY_PH_UP, HIGH);
  digitalWrite(RELAY_PH_DOWN, HIGH);
}

void loop()
{
//-----------------------------------------------------------------//
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
//-----------------------------------------------------------------//
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
      // Send Day-of-Week
    Serial.print(rtc.getDOWStr());
    Serial.print(" ");
  
  // Send date
    Serial.print(rtc.getDateStr());
    Serial.print(" ");

  // Send time
    Serial.print(rtc.getTimeStr());
    Serial.print(" -- ");
  
     Serial.print("Voltage:");
     Serial.print(voltage,2);
     
     lcd.setCursor(4,0);
     lcd.print("Ph: ");
     lcd.print(pHValue, 2);
    
     Serial.print("    pH value: ");
     Serial.println(pHValue,2);
     
     //digitalWrite(LED,digitalRead(LED)^1);
     printTime=millis();
  }
//-----------------------------------------------------------------//
9
float moisture = getSoilMoisture();

Serial.println(moisture);

if (moisture > 50.0) {
    // If int moisture it is, turn on the soil moisture pump
    digitalWrite(RELAY_SOIL_MOISTURE, LOW);
  } else if (moisture < 55.0) {
    
  digitalWrite(RELAY_SOIL_MOISTURE, HIGH);
  }

    //pHValue < TARGET_PH - PH_UP_OFFSET
   //H is less than the target value minus the pH up offset
  if (pHValue <= 6.0) {
    // If it is, turn Print the pH value to the serial monitor on the pH up motor
    digitalWrite(RELAY_PH_UP, LOW);
    Serial.print("pH: up ");
    Serial.print("pH: ");
    Serial.println(pHValue);
    lcd.setCursor(0,1);
    lcd.print("pH UP: ON");

  //**************************************************************//
  
  File dataFile = SD.open("Phlog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDOWStr());
    dataFile.print(" ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(" ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" -- ");
    dataFile.print("pH value: ");
    dataFile.print(pHValue,2);
    dataFile.print(" -- ");
    dataFile.print("Ph UP ON   "); 
    dataFile.print(" -- ");
    dataFile.print("Water Level: ");
    dataFile.print(moisture);
    dataFile.print(" -- ");
    dataFile.print("Voltage ");
    dataFile.println(voltage,2);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


//**************************************************************//

  // Check if the pH is less than theRELAY_PH_UP, HIGH);
  } else {
    // target value minus the pH up offset
  //if (p If it is not, turn off the pH up motor
    digitalWrite(RELAY_PH_UP, HIGH);
    Serial.print("pH: UP OFF");
    Serial.println(pHValue);
    lcd.setCursor(0,1);
    lcd.print("pH UP: OFF");

    //**************************************************************//
  File dataFile = SD.open("Phlog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDOWStr());
    dataFile.print(" ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(" ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" -- ");
    dataFile.print("pH value: ");
    dataFile.print(pHValue,2);
    dataFile.print(" -- ");
    dataFile.print("Ph UP OFF  "); 
    dataFile.print(" -- ");
    dataFile.print("Water Level: ");
    dataFile.print(moisture);
    dataFile.print(" -- ");
    dataFile.print("Voltage ");
    dataFile.println(voltage,2);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


//**************************************************************//
  }
      
    // pHValue > TARGET_PH - PH_UP_OFFSET
   //H is less than the target value minus the pH up offset
  if (pHValue >= 6.59) {
    // If it is, turn Print the pH value to the serial monitor on the pH up motor
    digitalWrite(RELAY_PH_DOWN, LOW);
    Serial.print("pH: Down ");
    Serial.print("pH: ");
    Serial.println(pHValue);
    lcd.setCursor(9,1);
    lcd.print(" DN: ON");

    //**************************************************************//
  File dataFile = SD.open("Phlog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDOWStr());
    dataFile.print(" ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(" ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" -- ");
    dataFile.print("pH value: ");
    dataFile.print(pHValue,2);
    dataFile.print(" -- ");
    dataFile.print("Ph Down ON "); 
    dataFile.print(" -- ");
    dataFile.print("Water Level: ");
    dataFile.print(moisture);
    dataFile.print(" -- ");
    dataFile.print("Voltage ");
    dataFile.println(voltage,2);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


//**************************************************************//

  // Check if the pH is less than theRELAY_PH_UP, HIGH);
  } else {
    // target value minus the pH up offset
  //if (p If it is not, turn off the pH up motor
    digitalWrite(RELAY_PH_DOWN, HIGH);
    Serial.print("pH: down LOW");
    Serial.print("pH: ");
    Serial.println(pHValue);
        lcd.setCursor(9,1);
    lcd.print(" DN: OFF");

    //**************************************************************//
  File dataFile = SD.open("Phlog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDOWStr());
    dataFile.print(" ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(" ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" -- ");
    dataFile.print("pH value: ");
    dataFile.print(pHValue,2);
    dataFile.print(" -- ");
    dataFile.print("Ph Down OFF"); 
    dataFile.print(" -- ");
    dataFile.print("Water Level: ");
    dataFile.print(moisture);
    dataFile.print(" -- ");
    dataFile.print("Voltage ");
    dataFile.println(voltage,2);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


//**************************************************************//
  }
  
    //pHValue == TARGET_PH
  if (pHValue <= 6.0 && pHValue >= 6.59) {
    // If it is, turn Print the pH value to the serial monitor on the pH up motor
    digitalWrite(RELAY_PH_UP, HIGH);
    digitalWrite(RELAY_PH_DOWN, HIGH);
    Serial.print("pH:Off ");
    Serial.print("pH:Off");
    Serial.println(pHValue);
    lcd.setCursor(0,1);
    lcd.print("pH UP OFF DN OFF");

//**************************************************************//
  File dataFile = SD.open("Phlog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDOWStr());
    dataFile.print(" ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(" ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" -- ");
    dataFile.print("pH value: ");
    dataFile.print(pHValue,2);
    dataFile.print(" -- ");
    dataFile.print("Ph UP and Ph Down OFF"); 
    dataFile.print(" -- ");
    dataFile.print("Water Level: ");
    dataFile.print(moisture);
    dataFile.print(" -- ");
    dataFile.print("Voltage ");
    dataFile.println(voltage,2);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


//**************************************************************//
  }


  

  
  
//-----------------------------------------------------------------//
/*
 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("Phlog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDOWStr());
    dataFile.print(" ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(" ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" -- ");
    dataFile.print("pH value: ");
    dataFile.print(pHValue,2);
    dataFile.print(" -- ");
    dataFile.print("Water Level: ");
    dataFile.print(moisture);
    dataFile.print(" -- ");
    dataFile.print("Voltage ");
    dataFile.println(voltage,2);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  */
//-----------------------------------------------------------------//
}


double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
