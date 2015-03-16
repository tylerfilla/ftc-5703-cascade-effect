
/**
 * 
 * 
 * The Doppler Effect
 * FTC #5703
 * 
 * Autonomous Program
 * 
 * 
 */

/* Definitions */

// Start position definitions
#define START_POSITION_FLOOR    0
#define START_POSITION_RAMP     1
#define START_POSITION_UNKNOWN -1

// Rotation direction definitions
#define ROTATE_DIRECTION_CCW 1
#define ROTATE_DIRECTION_CW  -1

// Maneuver definitions
#define MANEUVER_BRIDGE_RAMP_PARKING_ZONE 0
#define MANEUVER_CENTER_GOAL_APPROACH     1
#define MANEUVER_CENTER_GOAL_ORIENT       2
#define MANEUVER_RAMP_EXIT                3
#define MANEUVER_ROLLING_GOAL_DEPOSIT     4
#define MANEUVER_ROLLING_GOAL_PICKUP      5
#define MANEUVER_LAUNCHER_RELEASE         6

// Maneuver data definitions
#define MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION 1
#define MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH       30
#define MANEUVER_DATA_INITIAL_COMPASS_READING_LOW        31

// Encoder calibration values (encoder counts per revolution divided by circumference of wheel)
#define ENCODER_1_COUNTS_PER_CM 60
#define ENCODER_2_COUNTS_PER_CM 60
#define ENCODER_3_COUNTS_PER_CM 60
#define ENCODER_4_COUNTS_PER_CM 60

// Delays
#define DELAY_LAUNCHER_RELEASE 500
#define DELAY_SERVO_ACTUATION  500

// Servo values
#define SERVO_VALUE_GOAL_CLAMP_LOWER 0
#define SERVO_VALUE_GOAL_CLAMP_RAISE 127
#define SERVO_VALUE_LAUNCHER_HOLD    127
#define SERVO_VALUE_LAUNCHER_RELEASE 0

// Tolerances
#define TOLERANCE_DRIVE_ROTATE_ANGLE 4

/* Function prototyping */

// Configuration GUI functions
void showConfigGui();
task configGuiEvent();

// Autonomous routine functions
task main();
void initialize();
void autonomousRoutine();
void autonomousManeuver(int maneuver, char* data);

// Drive functions
void resetEncoders();
void driveStop();
void driveStart(int power);
void driveRotateStart(int direction, int power);
void driveDuration(int power, int duration);
void driveDistance(int power, int distance);
void driveRotateDuration(int direction, int power, int duration);
void driveRotateAngleAbsolute(int direction, int power, int angle);
void driveRotateAngleRelative(int direction, int power, int angle);

// Utility functions
int angleWrap180(int angle);
int angleWrap360(int angle);
