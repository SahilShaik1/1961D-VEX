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
// Controller1          controller                    
// frontleft            motor         18              
// frontRight           motor         15              
// backLeft             motor         17              
// backRight            motor         11              
// DigitalOutA          digital_out   A               
// Intake               motor         9               
// Motor16              motor         16              
// Motor19              motor         19              
// Motor10              motor         10              
// DigitalOutB          digital_out   B               
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
const float PI = 3.141592653;

int inches2delay(int inches){return inches/((11*PI)/1200);}

void stop1() {
  frontleft.stop(brake);
  frontRight.stop(brake);
  backLeft.stop(brake);
  backRight.stop(brake);
}
void forwardIn(float inches){
  frontleft.spin(fwd, 99.9, pct);
  frontRight.spin(fwd, 100, pct);
  backLeft.spin(fwd, 99.9, pct);
  backRight.spin(fwd, 100, pct);
  wait(inches2delay(inches), msec);
  stop1();
}

void rightIn(float inches) {
  frontleft.spin(fwd, 100, pct);
  frontRight.spin(reverse, 100, pct);
  backLeft.spin(reverse, 100, pct);
  backRight.spin(fwd, 100, pct); 
  wait(inches2delay(inches), msec);
  stop1();
}

void leftIn(float inches) {
  frontleft.spin(reverse, 100, pct);
  frontRight.spin(fwd, 100, pct);
  backLeft.spin(fwd, 100, pct);
  backRight.spin(reverse, 100, pct); 
  wait(inches2delay(inches), msec);
  stop1();

}
void backIn(float inches) {
  frontleft.spin(reverse, 100, pct);
  frontRight.spin(reverse, 100, pct);
  backLeft.spin(reverse, 100, pct);
  backRight.spin(reverse, 100, pct);
  wait(inches2delay(inches), msec);
  stop1();

}
void rotateRight() {
  frontleft.spin(fwd, 100, pct);
  frontRight.spin(reverse, 100, pct);
  backLeft.spin(fwd, 100, pct);
  backRight.spin(reverse, 100, pct);
  

}
void rotateLeft() {
  frontleft.spin(reverse, 100, pct);
  frontRight.spin(fwd, 100, pct);
  backLeft.spin(reverse, 100, pct);
  backRight.spin(fwd, 100, pct);

}

void Flywheel_move(int delay){
Motor19.setVelocity(100, pct);
Motor16.setVelocity(100, pct);
Motor10.setVelocity(100, pct);
Motor16.spin(reverse);
Motor19.spin(forward);
Motor10.spin(reverse);
wait(delay, msec);
}

void Flywheel_stop(){
Motor16.stop(coast);
Motor19.stop(coast);
Motor10.stop(coast);
}

void Flywheel_pct(int pctA){
Motor19.setVelocity(pctA, pct);
Motor16.setVelocity(pctA, pct);
Motor10.setVelocity(pctA, pct);
Motor16.spin(reverse);
Motor19.spin(forward);
Motor10.spin(reverse);
}

float indexer;
bool digitaloutOn;
int pneu() { return 0; }
void out() { DigitalOutA.set(true); }
void retract() { DigitalOutA.set(false); }

float expansion;
bool digitaloutO;
int pneumatic() { return 0; }
void out1() { DigitalOutB.set(true); }
void retract1() { DigitalOutB.set(false); }

void autonomous(void) {

//moves forward to touch rollers
forwardIn(3);
wait(500, msec);
//intake spins rollers
Intake.setVelocity(100,pct);
Intake.spinFor(fwd, 100, degrees);
wait(50, msec);
Intake.stop();
//Finished Roller 1
backIn(3);
rotateRight();
wait(500,msec);
stop1();
Motor19.setVelocity(70, pct);
Motor16.setVelocity(70, pct);
Motor10.setVelocity(70, pct);
Motor16.spin(reverse);
Motor19.spin(forward);
Motor10.spin(reverse);
wait(3.5, sec);

out();
wait(1000, msec);
retract();
wait(300, msec);
out();
wait(1000, msec);
retract();

wait(1200,msec);

Motor16.stop(coast);
Motor19.stop(coast);
Motor10.stop(coast);

/*
//Rotate towards goal
backIn(24);
//rotate 1 tile to the 
rotateRight();
wait(575,msec);
stop1();
Intake.spinFor(reverse,100,degrees);
forwardIn(24);
Intake.stop();
rotateLeft();
wait(575,msec);
*/
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

bool flywheelState = false;
int FlywheelSpeed = 0;
bool flywheelPressed = false;

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    
    int leftJoystickX = Controller1.Axis4.position(pct);
    int leftJoystickY = Controller1.Axis3.position(pct);
    int rightJoystickX = Controller1.Axis1.position(pct);
    if (leftJoystickX > -5 && leftJoystickX < 5) {
      leftJoystickX = 0;
    }
    if (leftJoystickY < 5 && leftJoystickY > -5) {
      leftJoystickY = 0;
    }

    frontleft.spin(fwd, leftJoystickX + leftJoystickY + rightJoystickX, pct);
    frontRight.spin(fwd, leftJoystickY - leftJoystickX - rightJoystickX, pct);
    backRight.spin(fwd, leftJoystickX + leftJoystickY - rightJoystickX, pct);
    backLeft.spin(fwd, leftJoystickY - leftJoystickX + rightJoystickX, pct);
   
    wait(20, msec);

    // indexer
    if (Controller1.ButtonL1.pressing() == true) {
      out();
    } else {
      retract();
    }

    // Expansion

    if (Controller1.ButtonDown.pressing() == true) {
      out1();
    } else {
      retract1();
    }

    // Intake

    if (Controller1.ButtonR1.pressing() == true) {
      Intake.setVelocity(100, pct);
      Intake.spin(forward);
    } else if (Controller1.ButtonR2.pressing() == true) {
      Intake.setVelocity(100, pct);
      Intake.spin(reverse);
    } else {
      Intake.stop();
    }

//Flywheel
    if (Controller1.ButtonL2.pressing()) {
      Motor19.setVelocity(100, pct);
      Motor16.setVelocity(100, pct);
      Motor10.setVelocity(100, pct);

      Motor16.spin(reverse);
      Motor19.spin(forward);
      Motor10.spin(reverse);
    }  else {
      Motor16.stop(coast);
      Motor19.stop(coast);
      Motor10.stop(coast);
    }


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