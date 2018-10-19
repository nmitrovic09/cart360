/*
 * file DFRobot_BME680_I2C.ino
 *
 * @ https://github.com/DFRobot/DFRobot_BME680
 *
 * connect bme680 I2C interface with your board (please reference board compatibility)
 *
 * Temprature, Humidity, pressure, altitude, calibrate altitude and gas resistance data will print on serial window.
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2017-12-7
 */

 //sample and average the photocell data
 //get a larger number for random number
 //constumize it to a range
 //assign it to a value that represents the vibration motor

//defining the libraries
#include "DFRobot_BME680_I2C.h"
#include "Wire.h"

/*use an accurate altitude to calibrate sea level air pressure*/
#define CALIBRATE_PRESSURE

//create an instance of the sensor
DFRobot_BME680_I2C bme(0x77);  //0x77 I2C address

//create a variables for our sensor data
//current seaLevel for Montreal: 225.0
float temp;
float pressure;
float humidity;
float gasResistance;
float altitude;
float calibratedAltitude;
float seaLevel; 

//first and second vibration motor pin
int pinVibA = 3;
int pinVibB = 5;

//photocell variables;
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDpin = 11;          // connect Red LED to pin 11 (PWM pin)

//photocell average calculation variables
int photocellReadTotal = 0;
int photocellReadAve = 0;
int photocellCount = 0;

//on/off logic
boolean on = false;

unsigned long startTime;
unsigned long timePassed;

unsigned int onTime = 5000;
int hitPeak = 0;
int here = 1;

//select vib A or B
// VibA = 1, VibB = rest
int randNumber = 0;
int count = 0;

class Reader
{
  unsigned long startTime;
  unsigned long timePassed;

  unsigned int onTime;

  int hitPeak;

  public:
    Reader(int target, int peak)
    {
      onTime = target;
      hitPeak = peak;
    }

    void setupReader()
    {
      startTime = millis();
      //Serial.println(onTime)
    }

    void updateReader()
    {
      timePassed = millis() - startTime;
      //Serial.println(timePassed);
    }

    boolean passedInterval()
    {
      if(timePassed > onTime)
      {
        startTime = millis();
        timePassed = 0;
        //Serial.println(onTime);
 
        return true;
      }
      return false;
    }
};

//Reader readerForSensor(1000, 0);


void setup()
{
  uint8_t rslt = 1;
  Serial.begin(9600);
  while(!Serial);
  delay(1000);
  Serial.println();

  //check if the sensor works and has connection
  while(rslt != 0) {
    rslt = bme.begin();
    if(rslt != 0) {
      Serial.println("bme begin failure");
      delay(2000);
    }
  }

  //start updating the sensor
  Serial.println("bme begin successful");
  #ifdef CALIBRATE_PRESSURE
  bme.startConvert();
  delay(1000);
  bme.update();
  /*You can use an accurate altitude to calibrate sea level air pressure. 
   *And then use this calibrated sea level pressure as a reference to obtain the calibrated altitude.
   *In this case,525.0m is chendu accurate altitude.
   */
  //Montreal's current seaLevel
  seaLevel = bme.readSeaLevel(225.0);
  Serial.print("seaLevel :");
  Serial.println(seaLevel);
  #endif

  //set up the sensor data reader
  setupReader();
}

void loop()
{
  //update the sensor
  bme.startConvert();
  delay(1000);
  bme.update();

  //update sensor data reader
  updateReader();

  //get photocell analog data
  photocellReading = analogRead(photocellPin);  
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading
  delay(50);

  //photcell averaging algorithm
  photocellReadTotal = photocellReadTotal + photocellReading;
  photocellCount++; 

  //Serial.println(photocellCount);
  
  //calculate photocell average after 5 times
  //to activate motors with boolean on/off
  if(photocellCount > 3)
  {
    //calculate average
    photocellReadAve = photocellReadTotal/photocellCount;
    
    //reset total and count
    photocellCount = 0;
    photocellReadTotal = 0;
    
    if(photocellReadAve > 500)
    {
      on = true;
    } else {
      on = false;
    }
  }

  
  
  //state on/off with boolean 
  //when average hits the right logic
  if(on == true)
  { 
    //turn on the motors algorithm
    Serial.println("light");

    //print all sensor data
    Serial.println();
    Serial.print("temperature(C) :");
    Serial.println(bme.readTemperature() / 100, 2);
    Serial.print("pressure(Pa) :");
    Serial.println(bme.readPressure());
    Serial.print("humidity(%rh) :");
    Serial.println(bme.readHumidity() / 1000, 2);
    Serial.print("gas resistance(ohm) :");
    Serial.println(bme.readGasResistance());
    Serial.print("altitude(m) :");
    Serial.println(bme.readAltitude());
    
    
    #ifdef CALIBRATE_PRESSURE
    Serial.print("calibrated altitude(m) :");
    Serial.println(bme.readCalibratedAltitude(seaLevel));
    #endif
    delay(10);

    
      //assign the sensor data to variable that can be used a threshold
      temp = bme.readTemperature() / 100;
      pressure = bme.readPressure();
      humidity = bme.readHumidity() / 1000;
      gasResistance = bme.readGasResistance();
      altitude = bme.readAltitude();
      calibratedAltitude = bme.readCalibratedAltitude(seaLevel);
      delay(10);

      //process data after certain time period
      if(passedInterval())
      {
        //check if there is any dangerous data incoming to the sensor
        if( (temp > 22 || temp < 18) || (pressure > 103000 || pressure < 96000) ||
            (humidity > 50 || humidity < 30) || (altitude < -5 || altitude > 500) )
            {
  
              int rValue = getRandomValue();
              Serial.println(rValue);

              int onTime = 1000;
              
                //activate the motors according to the randomly played generators
                if(rValue < 5)
                {
                  Serial.println(here);
                  analogWrite(pinVibA, 255);       // sets the digital pin 3 on
                  delay(1000);                  // waits for a second
                  analogWrite(pinVibA, 0);        // sets the digital pin 3 off
                  delay(100);                  // waits for a second
                }
    
                if(rValue > 5)
                {
                  Serial.println(here +1);
                  analogWrite(pinVibB, 255);       // sets the digital pin 5 on
                  delay(1000);                  // waits for a second
                  analogWrite(pinVibB, 0);        // sets the digital pin 5 off
                  delay(100);                  // waits for a second
                }
              
            }
      }

  } else {
    
    Serial.println("no light");
    Serial.println("no data reading");
    Serial.println("continue");
  }

  delay(10);
}

void setupReader()
{
  startTime = millis();
}

void updateReader()
{
  timePassed = millis() - startTime;
  //Serial.println(timePassed);
}

boolean passedInterval()
{
 if(timePassed > onTime)
 {
   Serial.println(timePassed);
   startTime = millis();
   timePassed = 0;
 
   return true;
 }
 return false;
}

//random value generator
int getRandomValue()
{
  int randomValue = 0;
  randomValue = random(0, 1000);
  randomValue = randomValue % 10;
  
  return randomValue;
}

//  } else if( (temp < 22 && temp > 18) && (pressure < 103000 && pressure > 96000) &&
//            (humidity < 50 && humidity > 30) && (altitude > -5 && altitude < 500) )
//  {
//    Serial.println("ideal air quality");
//    
//    analogWrite(pinVibB, 255);       // sets the digital pin 5 on
//    delay(1000);                  // waits for a second
//    analogWrite(pinVibB, 0);        // sets the digital pin 5 off
//    delay(100);                  // waits for a second
//
//    analogWrite(pinVibA, 255);       // sets the digital pin 3 on
//    delay(1000);                  // waits for a second
//    analogWrite(pinVibA, 0);        // sets the digital pin 3 off
//    delay(100);                  // waits for a second
//    


//            if(hitPeak > 3)
//            {
//              
//      
//              //Serial.println(hitPeak);
//      
//              if(randNumber < 2) 
//              {
//                analogWrite(pinVibA, 255);       // sets the digital pin 3 on
//                delay(1000);                  // waits for a second
//                analogWrite(pinVibA, 0);        // sets the digital pin 3 off
//                delay(100);                  // waits for a second
//  
//                count++;
//               } 
//  
//               if(randNumber > 3)
//               {      
//                 Serial.println(here);
//                 analogWrite(pinVibB, 255);       // sets the digital pin 3 on
//                 delay(1000);                  // waits for a second
//                 analogWrite(pinVibB, 0);        // sets the digital pin 3 off
//                 delay(100);                  // waits for a second
//            
//                 count++;
//               }
//            }
//          } else {
//            hitPeak--;
//            //Serial.println(hitPeak);
//          }

        //reset photcell average
        //photocellReadAve = 0;
//    } 
//    
//    } else {
//      Serial.println("no light");
        //reset photcell average
        //photocellReadAve = 0;
//    }
//  
//    delay(100);  

//  //Read the sensor data according to the interval
//  if(readerForSensor.passedInterval) 
//  {
//    temp = bme.readTemperature() / 100;
//    pressure = bme.readPressure();
//    humidity = bme.readHumidity() / 1000;
//    gasResistance = bme.readGasResistance();
//    altitude = bme.readAltitude();
//    calibratedAltitude = bme.readCalibratedAltitude(seaLevel); 
//
//    //check if there is any dangerous sensor data, update the peak data
//    if( (temp > 22 || temp < 18) && (pressure > 103000 || pressure < 96000) &&
//        (humidity > 50 || humidity < 30) && (altitude > -5 || altitude < 500) )
//        {
//          readerForSensor.hitPeak =+ 1;
//        } else {
//          readerForSensor.hitPeak =- 1;
//        }
//
////    //print all sensor data
////    Serial.println();
////    Serial.print("temperature(C) :");
////    Serial.println(bme.readTemperature() / 100, 2);
////    Serial.print("pressure(Pa) :");
////    Serial.println(bme.readPressure());
////    Serial.print("humidity(%rh) :");
////    Serial.println(bme.readHumidity() / 1000, 2);
////    Serial.print("gas resistance(ohm) :");
////    Serial.println(bme.readGasResistance());
////    Serial.print("altitude(m) :");
////    Serial.println(bme.readAltitude());
////    #ifdef CALIBRATE_PRESSURE
////    Serial.print("calibrated altitude(m) :");
////    Serial.println(bme.readCalibratedAltitude(seaLevel));
////    #endif
//  }
//
//  if(readerForSensor.hitPeak > 3)
//  {
//    analogWrite(pinVibA, 255);       // sets the digital pin 3 on
////  delay(1000);                  // waits for a second
////  analogWrite(pinVibA, 0);        // sets the digital pin 3 off
////  delay(100);                  // waits for a second
//
//    //update the time interval to read data from sensor
//    readerForSensor.onTime = 2000;
//  }
//
//  if(readerForSensor.hitPeak < 0)
//  {
//    analogWrite(pinVibA, 255);       // sets the digital pin 3 on
////  delay(1000);                  // waits for a second
////  analogWrite(pinVibA, 0);        // sets the digital pin 3 off
////  delay(100);                  // waits for a second
//
//    analogWrite(pinVibB, 255);       // sets the digital pin 3 on
////  delay(1000);                  // waits for a second
////  analogWrite(pinVibB, 0);        // sets the digital pin 3 off
////  delay(1000);                  // waits for a second
//
//    //update the time interval to read data from sensor less frequently
//    readerForSensor.onTime =+ 10000;
//  }





