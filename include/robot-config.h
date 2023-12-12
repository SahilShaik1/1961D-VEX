using namespace vex;

extern brain Brain;

// VEXcode devices
extern drivetrain Drivetrain;
extern controller Controller1;
extern motor leftMotorA;
extern motor rightMotorA; 
extern motor intake;
extern motor lcata;
extern motor rcata;
extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;
extern inertial imu1;
/**
 
Used to initialize code/tasks/devices added using tools in VEXcode Pro.
This should be called at the start of your int main function.
*/
void  vexcodeInit( void );
