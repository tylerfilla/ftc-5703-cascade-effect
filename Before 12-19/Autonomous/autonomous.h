
// The Doppler Effect
// FTC #5703

// Autonomous Program

/* Definitions */

// Encoder calibration values (encoder counts per revolution divided by circumference of wheel plus tweak)
#define ENCODER_1_COUNTS_PER_CM 60
#define ENCODER_2_COUNTS_PER_CM 60
#define ENCODER_3_COUNTS_PER_CM 60
#define ENCODER_4_COUNTS_PER_CM 60

// Holonomic drive constant as per our holonomic drive algorithm
#define HOLONOMIC_DRIVE_CONSTANT_TRANSLATION 0.02222222

// Tolerances
#define TOLERANCE_DRIVE_ROTATE_ANGLE 9 // +/- 9 = 5% of 360

// Rotation directions
#define ROTATE_DIRECTION_CCW 1
#define ROTATE_DIRECTION_CW  -1

// Start positions
#define START_POSITION_FLOOR     0
#define START_POSITION_RAMP      1
#define START_POSITION_UNDEFINED 2

// Timing
#define TIME_DRIVE_RAMP 0
#define TIME_DRIVE_CENTER 0
#define TIME_DRIVE_CENTER_SECOND 0

/* Prototyping */

// Configuration GUI functions
void showConfigGui();
task configGuiEvent();

// Autonomous routine functions
task main();
void autonomousRoutine();

// Drive functions
void driveStop();
void driveTranslateStart(int angle, int power);
void driveRotateStart(int direction, int power);
void driveTranslateDuration(int angle, int power, int duration);
void driveTranslateDistance(int angle, int power, int distance);
void driveRotateDuration(int direction, int power, int duration);
void driveRotateAngleAbsolute(int direction, int power, int angle);
void driveRotateAngleRelative(int direction, int power, int angle);

// Utility functions
int wrapAngleDegrees(int angle);
