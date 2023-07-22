#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller gameController = controller(primary);
triport tp(PORT22);
limit limitSwitch = limit(tp.A);
motor rightUP = motor(PORT1, ratio18_1, false);
motor rightDN = motor(PORT2, ratio18_1, true);
motor rightSN = motor(PORT3, ratio18_1, false);
motor leftUP  = motor(PORT4, ratio18_1, false);
motor leftDN  = motor(PORT5, ratio18_1, true);
motor leftSN = motor(PORT6, ratio18_1, false);
motor intake = motor(PORT7, ratio18_1, false);
motor coilMotor = motor(PORT8, ratio18_1, false);
motor_group rightMotors = motor_group(rightUP, rightDN, rightSN);
motor_group leftMotors = motor_group(leftUP, leftDN, leftSN);
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}
