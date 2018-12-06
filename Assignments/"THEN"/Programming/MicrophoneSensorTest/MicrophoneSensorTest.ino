//sensor values
int sensorPin = 0;
int sensorVal = 0;
int lastSensorVal = 0;
//sensor averaging
int sensorValAverage = 0;
int sensorValTotal = 0;
//sensor clarity
int threshold = 600;
int noise = 50;

//array for storing averaging
#define RUNNING_SAMPLES 50
int runningBuffer[RUNNING_SAMPLES];
int countingBuffer = 0;

//peak values
int peakVal = 0;
int prevPeakVal = 0;
int peakValAverage = 0;
int peakValTotal = 0;

//array for storing the peaks
#define PEAK_SAMPLES 10
int peakBuffer[PEAK_SAMPLES];
int countingPeak = 0;

void setup()
{
  Serial.begin (9600);
}
  
void loop ()
{ 
  //run the average of the sampled values
  if(countingBuffer < RUNNING_SAMPLES)
  {
    //read the sensor data
    sensorVal = analogRead(sensorPin);
//    Serial.print("Current sensor Value: ");
    Serial.println(sensorVal);

    //collect the peak values
    if(countingPeak < PEAK_SAMPLES)
    {
      //find the peak value
//      if(sensorVal > peakVal)
//      {
//        peakVal = sensorVal;
//      }

      //find the peak values with the threshold and noise
      if(sensorVal >= threshold + noise)
      {
        //check if the current one is bigger than the last value
        if(sensorVal >= lastSensorVal + noise)
        {
          //capture the peakVal in the array
          peakVal = sensorVal;
          Serial.print("Current Peak: ");
          Serial.println(peakVal);
          delay(50);
          //store the peak in the array
          peakBuffer[PEAK_SAMPLES] = peakVal;
        }

        countingPeak++;
        Serial.println(countingPeak);
      }

    } else {
       //the final peak
       if(peakVal >= threshold)
       {
        Serial.print("Final Peak: ");
        Serial.println(peakVal);
        Serial.println();
       }

       //calculate the average of peak sampling values
       for(int i = 0; i < PEAK_SAMPLES; i++)
       {
         peakValTotal += peakBuffer[i];
         delay(50);
       }

       //calculate the peak average values
       peakValAverage = peakValTotal/PEAK_SAMPLES;
            
//       Serial.print("Current peak average: ");
//       Serial.println(peakValAverage);
//       Serial.println();

       //reset the peak value and the peak counter
       peakVal = 0;
       countingPeak = 0;
    }
    //set the current sensor value as the previous one
    lastSensorVal = sensorVal;
    
    //store the value to the buffer for overall average
    //Serial.print("Buffer count: ");
    //Serial.println(countingBuffer);
    //Serial.println();
    runningBuffer[countingBuffer] = sensorVal;

    countingBuffer++;
    delay(10);
   } else {
   // Serial.println();
    
    //calculate the total sampling values
    for(int i = 0; i < RUNNING_SAMPLES; i++)
    {
      sensorValTotal += runningBuffer[i];
      //Serial.println(sensorValTotal);
      //delay(50);
    }
    //    Serial.println(sensorValTotal);
    
    //calculate the average sensor value
    sensorValAverage = sensorValTotal/RUNNING_SAMPLES;
        
//    Serial.print("Current average: ");
//    Serial.println(sensorValAverage);
//    Serial.println();

    delay(100);
    
    sensorValTotal = 0; //reset total
    sensorValAverage = 0; //reset average
    countingBuffer = 0; //reset the count when it passes the limit of array
   }
}

