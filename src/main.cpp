/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Jun 12 2023                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                             
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robot-config.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/



//Starting autonomous
void autonomous(void) {

}




/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

//Set to 2 decimal places
template <typename T> T round(T var){
  float value = (int)(var * 100 + .5);
  return (T)(value / 100);
}


struct status{
  //Incase malloc doesn't work
  uint32_t battery;
  double temp;
  status(uint32_t battery, double temp){
    this->battery = battery;
    this->temp = temp;
  }
};

void usercontrol(void) {
  // User control code here, inside the loop
  //prints battery and temp to screen
  //Uses Dynamically Allocated Memory to return arrays(frees if called)
  auto printToScreen = [&](void* mem, bool end = false) -> double*{
    if(end){
      //Free memory and finish
      mem = NULL;
      free(mem);
      return nullptr;
    }
  
    //Prints battery and temp
    gameController.Screen.clearScreen();
    gameController.Screen.setCursor(0, 0);
    gameController.Screen.print("B: ");
    uint32_t batteryPct = Brain.Battery.capacity(percentUnits::pct);
    gameController.Screen.print(batteryPct);
    gameController.Screen.print("%");
    gameController.Screen.setCursor(0, 1);
    gameController.Screen.print("T: ");
    double temp = round(Brain.Battery.temperature(temperatureUnits::fahrenheit));
    gameController.Screen.print(temp);
    gameController.Screen.print(" F");

    //stores both in array
    double res[2] = {(double)batteryPct, temp};
    if(mem == NULL){
      //First time run, memory allocated to allow for the arr
      double *addr = (double*)malloc(sizeof(res));
      addr = res;
      return addr;
    } else {
      //Already ran, now stored in original allocation of memory
      double *addr = (double*)mem;
      addr = res;
      return addr;
    }
  };
  //addr points to an array with {battery, temp(in Fareinheit)}
  //Since the array is stored as pointers, do *addr for first element(battery) and *(addr + 1) for second element(temp)
  double* addr = printToScreen(NULL);
  double battery = *addr;
  double temp = *(addr + 1);
  if(battery < .05 || temp > 60){
    gameController.Screen.setCursor(0, 2);
    gameController.Screen.print("Warning!");
  }
  while (1) {
    wait(20, msec);
  }

}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    
    wait(100, msec);
  }
}
