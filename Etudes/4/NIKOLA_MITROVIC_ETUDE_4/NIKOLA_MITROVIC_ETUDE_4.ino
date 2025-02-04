/**********************ETUDE 4 CART 360 2018*******************************
 * WELCOME! IN THIS ETUDE WE WILL USE TIMERS & COMPUTE A RUNNING AVERAGE FOR PHOTOCELL
 * THE PURPOSE OF THIS EXERCISE IS EXTEND ETUDE_3 WITH THE FOLLOWING FUNCTIONALITY:
 * 1/ WE WILL CHANGE THE RECORD MODE SUCH THAT IT NOT ONLY RECORDS THE NOTE BUT THE DURATION THAT NOTE WAS PRESSED AS WELL
 * WE WILL ALLOW A USER TO HOLD DOWN A BUTTON FOR A MAXIMUM OF 5 SECONDS
 * 2/ WE WILL ADD A PHOTOCELL TO INCORPORATE FREQUENCY MODULATION - WHEN YOU PRESS A BUTTON AND AFFECT THE PHOTOCELL,
 * THE FREQUENCY WILL CHANGE
 * 3/ IN RECORD MODE -YOU WILL PLAY THE NOTE LIVE WITH THE LIVE VALUE OF THE PHOTOCELL, IN PLAYBACK YOU WILL USE THE AVERAGE
 * PHOTOCELL VALUE AS AN OFFSET TO THE FREQUENCY
 * 4/ WE WILL HAVE A NEW PLAYMODE WHICH WILL PLAYBACK THE FREQUENCY+AVERAGE PHOTO VAL AND THE DURATION ASSOCIATED WITH THE NOTE
 * 5/ ADDITIONALLY, IN PLAYBACK, THE LED BRIGHTNESS & COLOUR WILL CHANGE ACCORDING TO THE NOTE VALUE
 * 6/ RE-IMPLEMENT THE LIVE MODE TO INCORPORATE THE PHOTOCELL VALUE
 * PLEASE NOTE:: THE OTHER NEW FUNCTIONS(BESIDES 6/) TO IMPLEMENT ARE AT THE BOTTOM OF THE CODE...


 * PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
 * DO NOT ADD ANY MORE FUNCTION DEFINITIONS 
 * ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
 * THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
 * PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
 * GOOD LUCK!
 */
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 5 // R PIN
#define LED_PIN_G 9 // G PIN
#define LED_PIN_B 6 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration =200;

// constant for pin to output for buzzer 
#define BUZZER_PIN 3 // PWM

// constant for input from photo cell
#define PHOTO_PIN A1

// constant for size of running samples array (for averging)
#define RUNNING_SAMPLES 16

// max pressing time
const int MAX_PRESS_TIME = 5000;

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes =0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES]; 
int mode = 0; // start at off

/******** VARIABLES FOR ETUDE 4 *****************************************************/
// NEW FOR ETUDE 4: array to hold durations
// will be a parallel array to the notes array -
// meaning that at index 0 of notes[] is stored the 1st frequency, and at index 0 of
// the durations array  is stored the 1st duration for THAT note....
int durations [MAX_NOTES];

// boolean to determine if a note button was being pressed:
boolean activeNoteButton = false;

// keep track of the active frequency
int activeFrequency = 0;

// keep track of offset Freq generated by the photocell
int offsetFrequency = 0;

// keep track of average Freq generated by the photocell
int averageOffsetFreq = 0;

// timer vars
unsigned long timePassed;
unsigned long startTime;

// var to hold the note pressed
int testNote;

// vars for averageing
int runningAverageBuffer[RUNNING_SAMPLES];
int nextCount = 0;

/*************************************************************************/


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_MODE_PIN, INPUT);
}

/**********************LOOP() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change the loop - it establishes the flow of the program
 * We call here 3 functions repeatedly:
 * 1: chooseMode(): this function will determine the mode that your program is in 
 * based on if the button (linked to the BUTTON_MODE_PIN) was pressed
 * 2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
 * 3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
     int val = digitalRead(BUTTON_MODE_PIN);
   Serial.print(val);
   Serial.print(" ");
   int val2 = analogRead(NOTE_IN_PIN);
   Serial.print(val2);
   Serial.println(" ");
   delay(100);
  chooseMode();
  setRGB();
  selectMode();
}
/******************CHOOSEMODE() *********************************
 * INSTRUCTIONS:
 * Read the value from the Button (linked to the BUTTON_MODE_PIN) and 
 * if is being pressed then change the mode variable.
 * REMEMBER:
 * mode == 0 is reset 
 * mode == 1 is live
 * mode == 2 is record
 * mode == 3 is play
 * mode == 4 is loopMode
 * Every time the user presses the button, the program will go to the next mode,
 * once it reaches 4, it should go back to mode == 0. 
 * (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode(){
  if (digitalRead(BUTTON_MODE_PIN) == HIGH)
  {
    delay(400);
    if (mode < 4) 
    {
      Serial.println(mode);
      mode++;
      //Serial.println(mode);
    } else {
      mode = 0;
    }
  }
}

/******************SETRGB()*********************************************
 * INSTRUCTIONS:
 * Depending on the value of the mode variable:
 * if the mode is 0 - RGB LED IS OFF
 * if the mode is 1 - RGB LED IS BLUE
 * if mode is 2 - RGB LED IS RED
 * if mode is 3 - RGB LED IS GREEN
 * if mode is 4 - RGB LED iS PURPLE
 * YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{ 
  switch (mode)
  {
    case 0:
      {
        //Serial.println(mode);
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 1:
      {
        //Serial.println(mode);
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
    case 2:
      {
        //Serial.println(mode);
        analogWrite(LED_PIN_R, 255);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 3:
      {
        //Serial.println(mode);
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 255);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }

    case 4:
      {
        //Serial.println(mode);
        analogWrite(LED_PIN_R, 125);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
    default:
      {
        // do nothing
        break;
      }
  }
}
/**********************SELECTMODE() *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
 * There are 4 possibilities
 * 1: reset(): this function will reset any arrays etc, and will do nothing else
 * 2: live(): this function will play the corresponding notes 
 * to the user pressing the respective buttons. 
 * NOTE:: the notes played are NOT stored
 * 3: record(): this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * 4: play(): this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * 5: loopMode(): this function will playback any notes stored in the array that were recorded, 
 * BUT unlike the previous mode, you can choose in which sequence the notes are played.
 * REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if(mode ==0) { 
    reset();
  }
  else if(mode ==1) {
    live();
  }
  else if(mode ==2) {
    //record();
    /******NEW*********/
    recordWithDuration();
  }
  
  else if(mode ==3) {
    //play();
    /******NEW*********/
    playWithDuration();
  }
   
   else if(mode ==4) {
    loopMode();
  }
}
/******************RESET() **************************************
 * INSTRUCTIONS:
 * this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
 countNotes = 0;
}
/******************LIVE()  - RE-IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * NOTE:: the notes played are NOT stored
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * ALSO - get the value of the photo cell and combine it with the note frequency
 * THEN - output the note to the buzzer using the tone() function, as 3rd argument 
 * make the duration value == to 100
**************************************************************************/
void live()
{
   // IMPLEMENT
//   Serial.print(mode);
//   Serial.print(" ");
      
   int val = analogRead(NOTE_IN_PIN);
//   Serial.print(val);
//   Serial.print(" ");
      
   //delay(100);

   //get the photocell offset
   offsetFrequency = getPhotoFrequency();
   
   int newduration = 100;
   
   // output sound with the offset
   if(getResistance(val, 245, 255)) {
     tone(BUZZER_PIN, val+offsetFrequency, newduration);
   } else if(getResistance(val, 325, 335)) {
     tone(BUZZER_PIN, val+offsetFrequency, newduration);
   } else if(getResistance(val, 485, 490)) {
     tone(BUZZER_PIN, val+offsetFrequency, newduration);
   } else if(getResistance(val, 930, 935)) {
     tone(BUZZER_PIN, val+offsetFrequency, newduration);
   } else if(getResistance(val, 1020, 1025)) {
     tone(BUZZER_PIN, val+offsetFrequency, newduration);
   } 
}

//get the button resistance
bool getResistance(int pinVal, int minimum, int maximum)
{
  if(pinVal > minimum && pinVal < maximum) {
    return true;
  } else {
    return false;
  }
}

/******************LOOPMODE(): DONE************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
  for (int i = 0; i < countNotes; i++)
  {
    /* https://www.arduino.cc/en/Reference/Tone */
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration); // wait for tone to finish
    //check if mode button is pressed
    if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
      break;
    }
  }
  for (int i = countNotes - 1; i >= 0; i--)
  {
    /* https://www.arduino.cc/en/Reference/Tone */
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration); // wait for tone to finish
    //check if mode button is pressed
    if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
      break;
    }
  }
}

/******************RECORDWITHDURATION(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * It will ALSO use a timer to keep track of how long the current note button has been pressed 
 * It will ALSO sample the photocell value to be used live as well as calling the function to calculate
 * the running average of the photocell value 
 * SO:I have implemented a basic framework for you to follow - the basic state machine.
 * YOUR JOB is to implement the NEW functions that get called within the framework...
**************************************************************************/
void recordWithDuration()
{
  Serial.print(mode);
  Serial.print(" ");
   
  // to handle a little bit of noise
  int theAdjuster = 8;
  // check we have not stored more than 16 notes
  if (countNotes < MAX_NOTES)
  {
    // read in the value from button press
    testNote = analogRead(NOTE_IN_PIN);
//    Serial.print(testNote);
//    Serial.print(" ");
    
    /*** STATE A::: *******************************************
     *  IF the testNote is > min AND the timer has not reached 5 secs:
     *  This means we are pressing on a note button
     */
    if (testNote > theAdjuster && (timePassed < MAX_PRESS_TIME)) 
    {
      /*** STATE AA::: *******************************************
       *  IF the boolean is false it means we have just started to press the button 
       *  YOU now need to implement the function below which will set up all things
       *  required to have a new timer
      */
      if (activeNoteButton == false)
      {
      /*** FUNCTION TO IMPLEMENT ***/
        startUpTimer();
      }
     /*** STATE AA::: *******************************************
       *  IF the boolean is true it means we are continuing to press the button
       *  YOU now need to implement the function below which will allow for
       *  1/ timer to be updated
       *  2/ get the photoCell Value
       *  3/ get the current running average
      */
      else {  // update the timer
         /*** FUNCTION TO IMPLEMENT ***/
        updateTimer();
         /*** FUNCTION TO IMPLEMENT ***/
        offsetFrequency = getPhotoFrequency();
         /*** FUNCTION TO IMPLEMENT ***/
        playCurrentNote(testNote, offsetFrequency);
         /*** FUNCTION TO IMPLEMENT ***/
        averageOffsetFreq = getRunningAverage(offsetFrequency);
       }
    }
     /*** STATE B::: *******************************************
     *  IF the testNote is > min AND the timer HAS reached 5 secs:
     *  then WRITE the code to turn OFF the buzzer.
     */
    else if (testNote > theAdjuster && (timePassed > MAX_PRESS_TIME)) 
    {
      // WRITE the code to turn OFF the buzzer
      noTone(BUZZER_PIN);
    }
     /*** STATE C::: *******************************************
     *  IF the testNote is < min AND the boolean is true
     *  this means we have STOPPED pressing the button
     *  YOU: need to implement the function to update the arrays etc
     */
    else if ((testNote <= theAdjuster && activeNoteButton == true) ) {
       /*** FUNCTION TO IMPLEMENT ***/
      updateArraysWithNoteAndTimings(testNote, averageOffsetFreq);
    }

  }

}

/******************STARTUPTIMER(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will set all required variables to reset the timer
**************************************************************************/
void startUpTimer()
{
  //IMPLEMENT
  startTime = millis();
  activeNoteButton = true;
//  Serial.print(startTime);
//  Serial.print(" ");
  
}
/******************UPDATETIMER(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will update the current timer 
**************************************************************************/
void updateTimer()
{
 //IMPLEMENT
 timePassed = millis() - startTime;
// Serial.print(timePassed);
// Serial.print(" ");
}
/******************PLAYCURRENTNOTE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will play the current note being pressed in RecordDuration mode
 * and the frequency should be note value + offset from photocell
 * add as the 3rd parameter to the tone() a duration of 100 
**************************************************************************/
void playCurrentNote(int note, int offset)
{
    //IMPLEMENT
    int newDuration = 100;
    tone(BUZZER_PIN, note+offset, newDuration);
}
/******************UPDATEARRAYSWITHNOTEANDTIMINGS(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will handle the case where the note is no longer being pressed:
 * you need to update the arrays
 * NOTE: please for the note value  - combine with the running average val from the photocell
 * reset the necessary booleans
 * reset any counters
**************************************************************************/
void updateArraysWithNoteAndTimings(int sensorVal, int averageOffsetVal)
{
 //IMPLEMENT
 //keep the count of the notes within the array
 if(countNotes >= MAX_NOTES)
 {
   countNotes = 0;
 }
 //keep the time limit to press the button
 if(timePassed >= MAX_PRESS_TIME)
 {
   timePassed = MAX_PRESS_TIME;
 }
 
 // array to hold the notes played (for record/play mode)
 notes[countNotes] = sensorVal + averageOffsetVal; 
 durations[countNotes] = timePassed;
 countNotes++;

 activeNoteButton = false;  //turn off the button's state
 timePassed = 0;            //reset the time for the next button
}

/******************GETPHOTOFREQUENCY(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will get input from the photocell
 * please scale and return the value accordingly
**************************************************************************/
int getPhotoFrequency()
{
  //IMPLEMENT
  int val = analogRead(PHOTO_PIN);  //read the photcell value
  val = map(val, 0, 800, 0, 300);    //scale the photocell resistance
  return val;
}

/******************GETRUNNINGAVERAGE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function should compute the running average of the photcell value (returned from the 
 * getPhotoFrequency() function).
**************************************************************************/
int getRunningAverage(int photocellVal)
{
  //IMPLEMENT
  //restart our array counting
  if(nextCount >= RUNNING_SAMPLES)
  {
    nextCount = 0;
  }
  
  //assign our sensor value to the array
  runningAverageBuffer[nextCount] = photocellVal;
  nextCount++;
  
  
  //calculate our sum for average
  int sum = 0;
  for(int i = 0; i < RUNNING_SAMPLES; i++)
  {
    sum += runningAverageBuffer[i];
  }

  int averageSensorVal = sum/RUNNING_SAMPLES; //sensor average
  return averageSensorVal;
  
}
/******************COLORLED(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will change the color of the led (in playback mode) to have the brightness
 * adjusted according to the frequency of the note being played (range : 0-255)
**************************************************************************/
void colorLED(int col)
{
  analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
  analogWrite(LED_PIN_G, col);   // Turn on the LED -G
  analogWrite(LED_PIN_B, 0);   // Turn on the LED -B

}
/******************PLAYWITHDURATION(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will got through the notes array and play back the note and 
 * its corresponding duration value.
 * NOTE: we have not implemented any case/code for pauses  - this can be a fixed value
 * ALSO: call the function to change the color of the LED using the correct argument.
 * ALSO: note that the mode button will not be super responsive UNLESS you press the button 
 * continuously - that is ok for this etude.
**************************************************************************/
void playWithDuration()
{
  //IMPLEMENT
  //loop through all the notes stored in the array
  int pause = 100;
  for(int i = 0; i < MAX_NOTES; i++)
  {
    //stop playing the notes when the button has been pressed
    if(digitalRead(BUZZER_PIN) == HIGH)
    {
      break;
    }
    
    tone(BUZZER_PIN, notes[i], durations[i]); //make a note and its duration
    int colorMap = map(notes[i], 0, 1023, 0, 255);  //map the note value to the appropriate color brightness
    colorLED(colorMap);    //make the led change

    //allow an interval between the notes
    delay(pause+durations[i]);
  }
}

/******************PLAY(): SOLUTION: ETUDE_2 ************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ each value IN ORDER
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{
 
  for(int i=0; i<countNotes;i++)
  {
    //stop playing the notes when the button has been pressed
    if(digitalRead(BUZZER_PIN) == HIGH)
    {
      break;
    }
    
    /* https://www.arduino.cc/en/Reference/Tone */
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration); // wait for tone to finish
    //check if mode button is pressed
    if(digitalRead(BUTTON_MODE_PIN) ==HIGH){break;}
  }
}
/******************RECORD(): SOLUTION_ETUDE_2 **********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * AND - output the note to the buzzer using the tone() function
 * THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
********************************************************************************/
void record()
{
  if(countNotes < MAX_NOTES)
  {
    // add a note to the array if have a value
    int testNote = analogRead(NOTE_IN_PIN);
    if(testNote>0)
    {
      notes[countNotes] = testNote; // possible vals?
      tone(BUZZER_PIN, notes[countNotes], duration);
      delay(duration); // wait for tone to finish
      countNotes++;
      delay(100);
    }
  }
}
/*********************************************************************************/
