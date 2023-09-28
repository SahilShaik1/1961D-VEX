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

//.585
//.0005
//.005

bool pid = true;
double desiredVal = 10;
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

double waitTime = 0.5;
//Kpc - .5 or .4 or .3
//Kdc - 0.05

int drivepid(){
  leftMotorA.setPosition(0, rev);
  rightMotorA.setPosition(0, rev);
  traveledDist = 0;
  first = true;
  while(pid){
    if(first){
      error = desiredVal - traveledDist;
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
      Controller1.Screen.setCursor(0,0);
      Controller1.Screen.print(revolutions);
      traveledDist = ((4*pi) * revolutions);
      if(traveledDist >= desiredVal){
        LeftDriveSmart.spin(fwd, 0, volt);
        RightDriveSmart.spin(fwd, 0, volt);
      } else {
        error = desiredVal - traveledDist;
        prop = error * Kpc;
        Kdv = (error - prevErr) / waitTime;
        deriv = Kdv * Kdc;
        pwr = prop + deriv;
        if(pwr > 11){
          pwr = 11;
        }
        prevErr = error;
        Controller1.Screen.setCursor(0,1);
        Controller1.Screen.print(" : Pwr: ");
        Controller1.Screen.print(pwr);
        LeftDriveSmart.spin(fwd, pwr, volt);
        RightDriveSmart.spin(fwd, pwr, volt);
      }
    }
    //get the position of both motors
    wait(waitTime, msec);
  }
  return 1;
}

double desiredDegrees = 90;
double degreesTraveled;
double TErr;
double TKpc = 0.1;
double TKdc = 0.05;
double TKdv;
bool firstTurn = true;
double TProp;
double TprevErr;
double TDeriv;
double TPwr;
bool Tpid = true;
double Tdps;
int turnpid(){
  leftMotorA.setPosition(0, rev);
  rightMotorA.setPosition(0, rev);
  degreesTraveled = 0;
  while(Tpid){
    if(firstTurn){
      TErr = desiredDegrees - degreesTraveled;
      TProp = TErr * TKpc;
      TprevErr = TErr;
      if(TProp > 11){
        TProp = 11;
      }
      LeftDriveSmart.spin(fwd, TProp * .5, volt);
      RightDriveSmart.spin(reverse, TProp * .5, volt);
      firstTurn = false;
    }
    else{
      while(degreesTraveled < desiredDegrees){
        double revolutions = leftMotorA.position(rev);  
        degreesTraveled = (revolutions * 360) / 2;
        Controller1.Screen.setCursor(0,0);
        Controller1.Screen.print(degreesTraveled);
        if(degreesTraveled >= desiredDegrees){
          LeftDriveSmart.stop();
          RightDriveSmart.stop();
        } else {
          TErr = desiredDegrees - degreesTraveled;
          TProp = TKpc * TErr;
          TKdv = (TErr - TprevErr) / waitTime;
          TDeriv = TKdv * TKdc;
          TPwr = TDeriv + TProp;
          if(TPwr > 11){
            TPwr = 11;
          }
          LeftDriveSmart.spin(fwd, TPwr * 0.5, volt);
          RightDriveSmart.spin(reverse, TPwr * 0.5, volt);
        }
      }
    }
  }
  wait(waitTime, msec);
  return 1;
}


bool coiling = false;
bool single = false;
//Starting autonomous



void autonomous(void) {
  drivepid();
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




  //normal run
  while (1) {
    if(counter % 50 == 0){
      addr = printToScreen(addr);
    }

    

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
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
