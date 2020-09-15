/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/jaydlawrence/projects/sleep-timer/src/sleep-timer.ino"
/*
 * Project sleep-timer
 * Description: Device code for sleep timer project
 * Author: Jayd Lawrence
 * Date: 9th September 2020
 * OS version: 1.5.2+
 */

void resetTimer();
void turnAllOff();
int resetAll(String command);
int setTimerPeriod(String periodString);
int setWakeNow(String command);
int setTimerEndTimestamp(String timestampString);
int getCurrentPercentage();
int checkTimeUpdateLights();
int getTimeRemaining();
char *getState();
void getFullState();
void setup();
void loop();
#line 9 "/Users/jaydlawrence/projects/sleep-timer/src/sleep-timer.ino"
int moon = D0;  // 0% pin
int star1 = D1; // 0% pin
int star2 = D2; // 20% pin
int star3 = D3; // 40% pin
int star4 = D4; // 60% pin
int star5 = D5; // 80% pin
int sun = D6;   // done pin
int startTime = 0;
int endTime = 0;
char fullState[200];

// reset timer
void resetTimer()
{
  startTime = 0;
  endTime = 0;
}

// turn off all LEDs
void turnAllOff()
{
  digitalWrite(moon, LOW);
  digitalWrite(star1, LOW);
  digitalWrite(star2, LOW);
  digitalWrite(star3, LOW);
  digitalWrite(star4, LOW);
  digitalWrite(star5, LOW);
  digitalWrite(sun, LOW);
}

// reset state
int resetAll(String command)
{
  resetTimer();
  turnAllOff();
  return 1;
}

//set time end to now plus passed seconds
int setTimerPeriod(String periodString)
{
  int period = periodString.toInt();
  if (period > 0)
  {
    startTime = Time.now();
    endTime = startTime + period;
    turnAllOff();
    return checkTimeUpdateLights();
  }
  else
  {
    return -1;
  }
}

//set time end to now
int setWakeNow(String command)
{
  int end = Time.now();
  startTime = end - 2;
  endTime = end - 1;
  turnAllOff();
  return checkTimeUpdateLights();
}

//set time end to passed timestamp
int setTimerEndTimestamp(String timestampString)
{
  int timestamp = timestampString.toInt();
  int start = Time.now();
  if (timestamp > start)
  {
    startTime = start;
    endTime = timestamp;
    turnAllOff();
    return checkTimeUpdateLights();
  }
  else
  {
    return -1;
  }
}

// get the current time vs start and end time as a percentage of completion
int getCurrentPercentage()
{
  if (startTime == 0 && endTime == 0)
    return 0;
  int currentTime = Time.now();
  if (currentTime > endTime)
    return 100;
  int currentPercentage = (int)((((double)currentTime - (double)startTime) / ((double)endTime - (double)startTime)) * 100.00);
  return currentPercentage;
}

// check the current timer state and update lights accordingly
int checkTimeUpdateLights()
{
  int currentPercentage = getCurrentPercentage();
  turnAllOff();
  if (currentPercentage >= 100)
  {
    digitalWrite(sun, HIGH);
    // TODO publish event that we have completed
    return 1;
  }
  if (currentPercentage > 80)
  {
    digitalWrite(star5, HIGH);
  }
  if (currentPercentage > 60)
  {
    digitalWrite(star4, HIGH);
  }
  if (currentPercentage > 40)
  {
    digitalWrite(star3, HIGH);
  }
  if (currentPercentage > 20)
  {
    digitalWrite(star2, HIGH);
  }
  digitalWrite(star1, HIGH);
  digitalWrite(moon, HIGH);
  return 1;
}

int getTimeRemaining()
{
  if (endTime == 0)
    return -1;
  int currentTime = Time.now();
  return endTime - currentTime;
}

char *getState()
{
  // If no start or end time is set, then device is idle/ready
  if (endTime == 0 || startTime == 0)
    return "Ready";

  // if we are passed the endTime is passed, then device is Done
  if (Time.now() >= endTime)
    return "Done";

  // Otherwise we are busy timing
  return "Timing";
}

void getFullState()
{
  sprintf(
      fullState,
      "{\"state\": \"%s\", \"startTime\": %d, \"endTime\": %d, \"timeRemaining\": %d, \"percentageComplete\": %d}",
      getState(),
      startTime,
      endTime,
      getTimeRemaining(),
      getCurrentPercentage());
}

void setup()
{
  // pin modes
  pinMode(moon, OUTPUT);
  pinMode(star1, OUTPUT);
  pinMode(star2, OUTPUT);
  pinMode(star3, OUTPUT);
  pinMode(star4, OUTPUT);
  pinMode(star5, OUTPUT);
  pinMode(sun, OUTPUT);

  // cloud functions
  Particle.function("setEnd", setTimerEndTimestamp);
  Particle.function("setPeriod", setTimerPeriod);
  Particle.function("setWakeNow", setWakeNow);
  Particle.function("reset", resetAll);

  //variables wrapped into single json
  Particle.variable("state", fullState);
}

void loop()
{
  if (startTime > 0 && endTime > 0)
  {
    checkTimeUpdateLights();
  }
  getFullState();
  delay(100);
}