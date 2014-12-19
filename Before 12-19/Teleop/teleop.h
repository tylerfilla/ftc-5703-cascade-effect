
// The Doppler Effect
// FTC #5703

// Teleop Program

// Joystick 1 definitions
#define JOYSTICK_1_DEADBAND_RADIUS (8.0 / 128.0)
#define JOYSTICK_1_SNAP_SIZE 3

// Joystick 2 definitions
#define JOYSTICK_2_DEADBAND_RADIUS (8.0 / 128.0)
#define JOYSTICK_2_SNAP_SIZE 3

// Controller 1 button definitions
#define CONTROLLER_BUTTON_ANGLE_REFERENCE 4
#define CONTROLLER_1_BUTTON_PRECISION_MODE 6

// Controller 2 button definitions
#define CONTROLLER_2_BUTTON_PRECISION_MODE 4
#define CONTROLLER_2_BUTTON_ELEVATOR_UP 8
#define CONTROLLER_2_BUTTON_ELEVATOR_DOWN 7
#define CONTROLLER_2_BUTTON_PITCH_UP 6
#define CONTROLLER_2_BUTTON_PITCH_DOWN 5
#define CONTROLLER_2_BUTTON_CONVEYOR_FORWARD 2
#define CONTROLLER_2_BUTTON_CONVEYOR_REVERSE 3

// Holonomic drive constant as per our holonomic drive algorithm
#define HOLONOMIC_DRIVE_CONSTANT_TRANSLATION 0.02222222

// Default maximum drive power
#define DRIVE_POWER_MAXIMUM_DEFAULT 100

// Maximum drive power under precision mode
#define DRIVE_POWER_MAXIMUM_PRECISION 60

// Threshold for target angle
#define DRIVE_ANGLE_TARGET_THRESHOLD 3

// Minimum power of drive motors when approaching target angle
#define DRIVE_ANGLE_TARGET_CORRECTION_MINIMUM 10;

// Size of previous iteration duration queue
#define PREVIOUS_ITERATION_DURATION_QUEUE_SIZE 8

// Function prototypes
void initialize();
int fixAngleRange(int angle);
task main();
