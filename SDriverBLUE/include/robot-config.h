using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor frontleft;
extern motor frontRight;
extern motor backLeft;
extern motor backRight;
extern controller Controller1;
extern digital_out DigitalOutA;
extern motor Intake;
extern motor Motor16;
extern motor Motor19;
extern motor Motor10;
extern digital_out DigitalOutB;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );