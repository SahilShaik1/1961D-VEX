/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 2, 15, 16    
// Controller1          controller                    
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

bool coiling = false;
bool single = false;
//Starting autonomous
void autonomous(void) {

}


void coil(){
  Controller1.rumble(".");
  //coiling = !coiling;
}

void shoot(){
  //apply tension and shoot
  //coilMotor.spinFor(reverse, 100, msec);
  coiling = true;
  single = true;
  Controller1.rumble(".");
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
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    auto printToScreen = [&](void* mem, bool end = false) -> double*{
    if(end){
      //Free memory and finish
      mem = NULL;
      free(mem);
      return nullptr; 
    }
    //Prints battery
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print("B: ");
    int batteryPct = Brain.Battery.capacity(percentUnits::pct);
    Controller1.Screen.print(batteryPct);
    Controller1.Screen.print("%");
    double temp = leftMotorA.temperature(temperatureUnits::celsius);
    Controller1.Screen.print(" T: ");
    Controller1.Screen.print(temp);
    Controller1.Screen.print("C");
    //stores in array

    Controller1.Screen.setCursor(2, 2);
    Controller1.Screen.print(batteryPct);
  

    double res[2] = {(double)batteryPct, temp};
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

  double* addr = printToScreen(NULL);
  int counter = 0;
  while (1) {
    if(counter % 50 == 0){
      addr = printToScreen(addr);
    }
    double battery = *(addr);
    double temp = *(addr + 1);
    //coil controls
    void (*b1ptr)() = &coil;
    void (*b2ptr)() = &shoot;

    Controller1.ButtonL1.pressed(b1ptr);
    Controller1.ButtonL2.pressed(b2ptr);
/*    
    //single shot coiling
    if(single && coiling){
      //The limit switch hasn't been pressed
      if(!limitSwitch.pressing()){
        coilMotor.spin(reverse, 1, pct);
      } else {
      //limit switch reached, stop coiling
        single = false;
        coiling = false;
      }
    } 
    //coiling toggled on
    if(coiling && !single){
      coilMotor.spin(reverse, 1, pct);
    }
*/
    //intake
    if (Controller1.ButtonR1.pressing()){
      intake.spin(fwd, 1, pct);
    } else if(Controller1.ButtonR2.pressing()){
      intake.spin(reverse, 1, pct);
    }

    

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
    
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
