/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Aug 26 2023                                           */
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


void calibration(inertial& inert){
  inert.startCalibration();
  inert.calibrate();
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(0, 0);
  while(inert.isCalibrating()){
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Calibrating");
    Controller1.Screen.setCursor(0, 0);
    wait(20, msec);
  }
  Controller1.Screen.clearScreen();
  inert.setHeading(0, deg);
}









void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  calibration(imu1);
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

//.585
//.0005
//.005

bool pid = true;
double DV = 40;
double Kpc = .4;
double Kdc = 0.1;
double pi = 3.14159265;
double traveledDist;
int prevErr;
double error;
double prop;
double Kdv;
double deriv;
double pwr;
bool first;
double waitTime = 20;
//Kpc - .5 or .4 or .3
//Kdc - 0.05

int drivepid(double desiredDist){
  desiredDist *= 2;
  leftMotorA.resetPosition();
  rightMotorA.resetPosition();
  leftMotorA.setPosition(0, rev);
  rightMotorA.setPosition(0, rev);

  first = true;
    while(pid){
      if(first){
        error = desiredDist - traveledDist;
        prop = error * Kpc;
        prevErr = error;
        if(prop > 11){
          prop = 11;
        }
        RightDriveSmart.spin(fwd, prop, volt);
        LeftDriveSmart.spin(fwd, prop, volt);
        first = false;
      }
      else {
        double revolutions = leftMotorA.position(rev);
        if(revolutions < 0){
          revolutions *= -1;
        }
        Controller1.Screen.setCursor(0, 0);
        Controller1.Screen.print(revolutions);
  
        traveledDist = ((4*pi) * revolutions);
        if(traveledDist >= desiredDist || (prevErr < 0.9 && prevErr > -0.9) ){
          LeftDriveSmart.stop(brake);
          RightDriveSmart.stop(brake);
          break;
        } else {
          Controller1.Screen.setCursor(0,0);
          Controller1.Screen.print(traveledDist);
          error = desiredDist - traveledDist;
          prop = error * Kpc;
          Kdv = (error - prevErr) / waitTime;
          deriv = Kdv * Kdc;
          pwr = prop + deriv;
          pwr *= 2.6;
          if(pwr > 12){
            pwr = 12;
          }
          prevErr = error;
          LeftDriveSmart.spin(fwd, pwr, volt);
          RightDriveSmart.spin(fwd, pwr, volt);
        }
      //get the position of both motors
      wait(20, msec);
    }
    wait(waitTime, msec);
  }
  return 1;
}

double TErr;
double TKpc = 0.2; // 0.1
double TKdc = 0.015; // 0.05
double TKdv;
bool firstTurn = true;
double TProp;
double TprevErr;
double TDeriv;
double TPwr;
double PDT;
bool Tpid = true;
bool ended = false;
double Tdps;
void turnpid(double desiredDegrees, bool rev){
  imu1.setRotation(0, deg);
  double degreesTraveled = 0;
  while(Tpid){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    if(firstTurn){
      degreesTraveled = imu1.rotation();
      if(degreesTraveled < 0){
        degreesTraveled*=-1;
      }
      TErr = desiredDegrees - degreesTraveled;
      TProp = TErr * TKpc;
      TprevErr = TErr;
      if(TProp > 11){
        TProp = 11;
      }
      if(!rev){
        LeftDriveSmart.spin(fwd, TProp, volt);
        RightDriveSmart.spin(reverse, TProp, volt);
      } else {
        LeftDriveSmart.spin(reverse, TProp, volt);
        RightDriveSmart.spin(fwd, TProp, volt);
      }
      firstTurn = false;
    }
    else{
        Controller1.Screen.clearScreen();
        degreesTraveled = imu1.rotation();
        if(degreesTraveled < 0){
          degreesTraveled *= -1;
        }
        if(degreesTraveled >= desiredDegrees || (TprevErr < 0.9 && TprevErr > -0.9)){
          LeftDriveSmart.stop(brake);
          RightDriveSmart.stop(brake);
          break;
        } else {
          TErr = desiredDegrees - degreesTraveled;
          TProp = TKpc * TErr;
          TKdv = (TErr - TprevErr) / waitTime;
          TDeriv = TKdv * TKdc;
          TPwr = TDeriv + TProp;
          TPwr *= .35; // 0.7
          if(round(degreesTraveled) == round(PDT)){
            LeftDriveSmart.stop(brake);
            RightDriveSmart.stop(brake);
            break;
          }
          if(TPwr > 11){
            TPwr = 11;
          }
          PDT = degreesTraveled;
          if(!rev){
            LeftDriveSmart.spin(fwd, TPwr, volt);
            RightDriveSmart.spin(reverse, TPwr, volt);
          } else {
            LeftDriveSmart.spin(reverse, TPwr, volt);
            RightDriveSmart.spin(fwd, TPwr, volt);
          }
      }
      wait(20, msec);
    }
  }
}


bool coiling = false;
bool single = false;
//Starting autonomous

int commands[] = {20, 180, 20};

void autonomous(void) {
  Controller1.Screen.clearScreen();
  turnpid(180, false);
}


void coil(){
  Controller1.rumble(".");
}

void shoot(){
  //apply tension and shoot
  //coilMotor.spinFor(reverse, 100, msec);
  coiling = true;
  single = true;
  Controller1.rumble(".");
}

bool intaking = false;
bool revIntaking = false;

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
  calibration(imu1);

  Controller1.Screen.clearScreen();
  //normal run
  while (1) {
    double deg = imu1.rotation();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(deg);
    
    
    
    
    

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(10, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
    
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  // Run the pre-autonomous function.
  pre_auton();
  
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);



  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
