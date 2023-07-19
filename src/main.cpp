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
// rightUP              motor         1
// rightDN              motor         2
// rightSN              motor         3
// leftUP               motor         4
// leftDN               motor         5
// leftSN               motor         6
// intake               motor         7
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

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

void usercontrol(void) {
  // User control code here, inside the loop
  //prints battery and temp to screen
  //returns ptr to array
  auto printToScreen = [&](void* mem, bool end = false) -> double*{
    if(end){
      //Free memory and finish
      mem = NULL;
      free(mem);
      return nullptr; 
    }
    //Prints battery
    gameController.Screen.clearScreen();
    gameController.Screen.setCursor(0, 0);
    gameController.Screen.print("B: ");
    int batteryPct = Brain.Battery.capacity(percentUnits::pct);
    gameController.Screen.print(batteryPct);
    gameController.Screen.print("%");
    //stores both in array
    double res[1] = {(double)batteryPct};
    if(mem == NULL){
      //First time run
      double *addr = (double*)malloc(sizeof(res));
      //set the memory to the array
      addr = res;
      return addr;
    } else {
      //use old memory
      double *addr = (double*)mem;
      addr = res;
      return addr;
    }
  };


  while (1) {
    double* addr = printToScreen(NULL);
    double battery = *addr;
    //joystick
    int leftJoystickY = gameController.Axis3.position(pct);
    int rightJoystickX = gameController.Axis1.position(pct);
    //intake
    if (gameController.ButtonR1.pressing()){
      intake.spin(fwd, 1, pct);
    } else if(gameController.ButtonR2.pressing()){
      intake.spin(reverse, 1, pct);
    }

    //no input
    if (leftJoystickY > -5 && leftJoystickY < 5){
      leftJoystickY = 0;
    }
    if (rightJoystickX > -5 && rightJoystickX < 5){
      rightJoystickX = 0;
    }

    //driving
    if(leftJoystickY == 0 && rightJoystickX > 0){
      rightMotors.spin(fwd, 1, pct);
      rightMotors.spin(fwd, -1, pct);
    }
    else if(leftJoystickY == 0 && rightJoystickX < 0){
      rightMotors.spin(fwd, -1, pct);
      rightMotors.spin(fwd, 1, pct);
    }
    else {
      rightMotors.spin(fwd, leftJoystickY - rightJoystickX, pct);
      leftMotors.spin(fwd, leftJoystickY + rightJoystickX, pct);
    }

    if(battery < .05){
      gameController.Screen.setCursor(0, 1);
      gameController.Screen.print("Warning!");
    }

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
