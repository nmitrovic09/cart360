//--------------PRESSURE SENSORS-------------
//first pressure sensor
int pressureSensorPin = 1;
int pressureVal = 0;
int pressureSensorValAverage = 0;
int pressureSensorValTotal = 0;
int lastPressureVal = 0;

//second pressure sensor
int pressureSensorPin2 = 2;
int pressureVal2 = 0;
int pressureSensorValAverage2 = 0;
int pressureSensorValTotal2 = 0;

//third pressure sensor
int pressureSensorPin3 = 3;
int pressureVal3 = 0;
int pressureSensorValAverage3 = 0;
int pressureSensorValTotal3 = 0;

//fourth pressure sensor
int pressureSensorPin4 = 4;
int pressureVal4 = 0;
int pressureSensorValAverage4 = 0;
int pressureSensorValTotal4 = 0;

//------------MICROPHONE SENSOR-------------
//sensor averaging
int microphonePin = 0;
int microphoneVal = 0;
int lastMicrophoneVal = 0;
//microphone averaging
int microphoneValAverage = 0;
int microphoneValTotal = 0;
//sensor clarity
int threshold = 600;
int noise = 50;

//------------LEDS-----------------
//#define PRESSURE_LED_PIN 3
//#define PRESSURE2_LED_PIN 4
//#define PRESSURE_LED_PIN 5
//#define PRESSURE2_LED_PIN 6

//------------AVERAGING-------------
//array for storing averaging
#define RUNNING_SAMPLES 16
int countingBuffer = 0;
int runningBuffer[RUNNING_SAMPLES];
//pressure snesor averaging
int runningPressureBuffer[RUNNING_SAMPLES];
int runningPressureBuffer2[RUNNING_SAMPLES];
int runningPressureBuffer3[RUNNING_SAMPLES];
int runningPressureBuffer4[RUNNING_SAMPLES];

//------------PEAK-----------------
//peak values
int peakVal = 0;
int prevPeakVal = 0;
int peakValAverage = 0;
int peakValTotal = 0;
int finalPeakVal = 0;

//array for storing the peaks
#define PEAK_SAMPLES 10
int peakBuffer[PEAK_SAMPLES];
int countingPeak = 0;

//------------TIMER-------------
unsigned long startTime;
unsigned long timePassed;
int interval = 1000;

void setup()
{
  Serial.begin (9600);
  startTimer();
//  pinMode(PRESSURE_LED_PIN, OUTPUT);
//  pinMode(PRESSURE2_LED_PIN, OUTPUT);
////  pinMode(PRESSURE3_LED_PIN, OUTPUT);
////  pinMode(PRESSURE4_LED_PIN, OUTPUT);
}
  
void loop ()
{ 
  //run the average of the sampled values
  if(countingBuffer < RUNNING_SAMPLES)
  {
    //read the sensors data
    microphoneVal = analogRead(microphonePin);
    delay(5);
    pressureVal = analogRead(pressureSensorPin);
    delay(5);
    pressureVal2 = analogRead(pressureSensorPin2);
    delay(5);
    pressureVal3 = analogRead(pressureSensorPin3);
    delay(5);
    pressureVal4 = analogRead(pressureSensorPin4);
    delay(5);
    
     //when should I output the data to serial? ADDING A TIMER
     //output the sensor data
     Serial.println();
     Serial.print(microphoneVal);
     Serial.print(" ");
     Serial.print(peakVal);
     Serial.print(" ");
     Serial.print(finalPeakVal);
     Serial.print(" ");
     Serial.print(pressureVal);
     Serial.print(" ");
     Serial.print(pressureVal2);
     Serial.print(" ");
     Serial.print(pressureVal3);
     Serial.print(" ");
     Serial.print(pressureVal4);
     Serial.print(" ");

    //collect the peak values
    if(countingPeak < PEAK_SAMPLES)
    {
      //find the peak values with the threshold and noise
      if(microphoneVal >= threshold + noise)
      {
        //check if the current one is bigger than the last value
        if(microphoneVal >= lastMicrophoneVal + noise)
        {
          //capture the peakVal in the array
          peakVal = microphoneVal;
          //store the peak in the array
          peakBuffer[PEAK_SAMPLES] = peakVal;
        }
        countingPeak++;
      }
    } else {
       //the final peak
       if(peakVal >= threshold)
       {
          //final peak value
          finalPeakVal = peakVal;
       }

       //calculate the average of peak sampling values
       for(int i = 0; i < PEAK_SAMPLES; i++)
       {
         peakValTotal += peakBuffer[i];
       }

       //calculate the peak average values
       peakValAverage = peakValTotal/PEAK_SAMPLES;     

       //reset the peak value and the peak counter
       peakVal = 0;
       countingPeak = 0;
    }
    
   //set the current sensor value as the previous one
   lastMicrophoneVal = microphoneVal;
   lastPressureVal = pressureVal;
   
   runningBuffer[countingBuffer] = microphoneVal; //store the microphone value to the array
   runningPressureBuffer[countingBuffer] = pressureVal; //store the first sensor value to the array
   runningPressureBuffer2[countingBuffer] = pressureVal2; //store the second sensor value to the array
   runningPressureBuffer3[countingBuffer] = pressureVal3; //store the third sensor value to the array
   runningPressureBuffer4[countingBuffer] = pressureVal4; //store the fourth sensor value to the array

   countingBuffer++;
    
   } else {
    
      //calculate the total sampling values
      for(int i = 0; i < RUNNING_SAMPLES; i++)
      {
        microphoneValTotal += runningBuffer[i];
        pressureSensorValTotal += runningPressureBuffer[i];
        pressureSensorValTotal2 += runningPressureBuffer2[i];
        pressureSensorValTotal3 += runningPressureBuffer3[i];
        pressureSensorValTotal4 += runningPressureBuffer4[i];
      }
      
      //calculate the average sensor value
      microphoneValAverage = microphoneValTotal/RUNNING_SAMPLES;
//      Serial.print(microphoneValAverage);
//      Serial.print(" ");
      
      //calculate the average of each pressure sensor
      pressureSensorValAverage = pressureSensorValTotal/RUNNING_SAMPLES;
      pressureSensorValAverage2 = pressureSensorValTotal2/RUNNING_SAMPLES;
      pressureSensorValAverage3 = pressureSensorValTotal3/RUNNING_SAMPLES;
      pressureSensorValAverage4 = pressureSensorValTotal4/RUNNING_SAMPLES;
      
      Serial.print(pressureSensorValAverage);
      Serial.print(" ");
      Serial.print(pressureSensorValAverage2);
      Serial.print(" ");
      Serial.print(pressureSensorValAverage3);
      Serial.print(" ");
      Serial.print(pressureSensorValAverage4);
      Serial.print(" ");

      //reset all the values
     reset();
   }
   
   delay(100);
}

//--------------RESET FUNCTION--------------
void reset()
{
  //reset microphone
  microphoneValTotal = 0; 
  microphoneValAverage = 0; 

  //reset the pressure sensors
  pressureSensorValTotal = 0; //reset total
  pressureSensorValAverage = 0; //reset average
  pressureSensorValTotal2 = 0; //reset total
  pressureSensorValAverage2 = 0; //reset average
  pressureSensorValTotal3 = 0; //reset total
  pressureSensorValAverage3 = 0; //reset average
  pressureSensorValTotal4 = 0; //reset total
  pressureSensorValAverage4 = 0; //reset average
  
  //reset the count
  countingBuffer = 0; 
}

//--------------TIMER FUNCTIONS-------------
void startTimer()
{
  startTime = millis();
}

void getTime()
{
  timePassed = millis() - startTime;
}

void resetTimer()
{
  startTime = millis();
  timePassed = 0;
}

