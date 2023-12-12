#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;
triport tp(PORT22);
limit limitSwitch = limit(tp.A);
// VEXcode device constructors
motor leftMotorA = motor(PORT18, ratio6_1, true);
motor leftMotorB = motor(PORT19, ratio6_1, true);
motor leftMotorC = motor(PORT20, ratio6_1, true);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor rightMotorA = motor(PORT13, ratio6_1, false);
motor rightMotorB = motor(PORT12, ratio6_1, false);
motor rightMotorC = motor(PORT11, ratio6_1, false);
motor intake = motor(PORT1, ratio18_1, false);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);
controller Controller1 = controller(primary);
//motor lcata = motor(PORT11, ratio18_1, true);
//motor rcata = motor(PORT12, ratio18_1, false);
digital_out lwing = digital_out(Brain.ThreeWirePort.A);
digital_out rwing = digital_out(Brain.ThreeWirePort.B);
inertial imu1 (PORT10, turnType::left);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool pushed = false;
bool started = false;
bool stopped = false;
bool shotl = false;
bool shotr = false;
bool forever = true;
bool buttonL1State = false;
bool buttonR1State = false;
int c = 0;
int z = 0;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
//  lcata.setPosition(0, rev);
//  rcata.setPosition(0, rev);
  while(true) {
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }

      if(Controller1.ButtonX.pressing() && z == 0){
        forever = !forever;
        started = true;
      }

      if(started && z < 5){
        z++;
      }
      if(started && z==5){
        z = 0;
        started = false;
      }
      if(!Controller1.ButtonL1.pressing() && buttonL1State)
        {
            buttonL1State = false;
            shotl = !shotl;
        }
      if(Controller1.ButtonL1.pressing()) {
        buttonL1State = true;
      }
      if(shotl){
        lwing.set(true);
      } else {
        lwing.set(false);
      }

      if(!Controller1.ButtonR1.pressing() && buttonR1State)
        {
            buttonR1State = false;
            shotr = !shotr;
        }
      if(Controller1.ButtonR1.pressing()) {
        buttonR1State = true;
      }
      if(shotr){
        rwing.set(true);
      } else {
        rwing.set(false);
      }
      if(Controller1.ButtonR1.pressing()){
        intake.spin(fwd, 12, volt);
        printf("spin");
      } 
      if(Controller1.ButtonR2.pressing()){
        intake.spin(reverse, 12, volt);
      }
      if(!Controller1.ButtonR1.pressing() && !Controller1.ButtonR2.pressing()){
        intake.stop();
      }
    
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}
