#pragma config(Hubs,   S1,           HTMotor,              HTMotor,                    HTServo,    none)
#pragma config(Sensor, S1,           ,                     sensorI2CMuxController)
#pragma config(Sensor, S2,           SMUX,                 sensorI2CCustom)
#pragma config(Sensor, S3,           sensorUltrasonic,     sensorSONAR)
#pragma config(Sensor, S4,           sensorIR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C2_1,  motorDrive1,          tmotorTetrix,               PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,  motorDrive4,          tmotorTetrix,               PIDControl)
#pragma config(Motor,  mtr_S1_C1_1,  motorDrive2,          tmotorTetrix,               PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C1_2,  motorDrive3,          tmotorTetrix,               PIDControl, reversed)
#pragma config(Servo,  srvo_S1_C3_1, servoLauncherRelease, tServoContinuous)
#pragma config(Servo,  srvo_S1_C3_2, servoGoalClamp,       tServoStandard)

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

/* Inclusions */

#include "autonomous.h"
#include "gui.h"

// Standard FTC driver
#include "JoystickDriver.c"

// Multiplexer drivers
#include "drivers\hitechnic-sensormux.h"
#include "drivers\hitechnic-compass.h"
#include "drivers\lego-touch.h"

/* Multiplexer sensor definitions */

const tMUXSensor sensorRGDetectR = msensor_S2_1;
const tMUXSensor sensorTilt      = msensor_S2_2;
const tMUXSensor sensorCompass   = msensor_S2_3;
const tMUXSensor sensorRGDetectL = msensor_S2_4;

/* Configuration */

int configStartDelay;
int configStartPosition;

bool configGrabRollingGoal;
bool configShootBall;

/* Configuration GUI state */

// "Accept" button states
bool configAcceptStartDelay;
bool configAcceptStartPosition;
bool configAcceptActions;

// Start delay screen global elements
TGuiElement configSpinnerStartDelay;

// Start position screen global elements
TGuiElement configRadioButtonStartPositionOnRamp;
TGuiElement configRadioButtonStartPositionOnFloor;

// Actions screen global elements
TGuiElement configCheckboxActionsGrabRollingGoal;
TGuiElement configCheckboxActionsShootBall;

/* Configuration GUI functions */

void showConfigGui() {
    /* Reset config globals */
    
    configStartDelay    = -1;
    configStartPosition = START_POSITION_UNKNOWN;
    
    configAcceptStartDelay    = false;
    configAcceptStartPosition = false;
    
    // Start event task
    StartTask(configGuiEvent);
    
    // Disable JoystickDriver.c diagnostics screen to allow GUI rendering
    bDisplayDiagnostics = false;
    
    // Erase display
    eraseDisplay();
    
    /* Start delay screen */
    
    gui_begin();
    
    // Screen title (local element)
    TGuiElement textTitleStartDelay = gui_element_create(GUI_ELEMENT_TEXT);
    gui_property_set(    textTitleStartDelay, GUI_PROPERTY_TEXT_ALIGN, GUI_PROPERTY_TEXT_ALIGN_CENTER);
    gui_property_set(    textTitleStartDelay, GUI_PROPERTY_TEXT_LINE,  1);
    gui_property_set(    textTitleStartDelay, GUI_PROPERTY_TEXT_SIZE,  GUI_PROPERTY_TEXT_SIZE_NORMAL);
    gui_property_set_str(textTitleStartDelay, GUI_PROPERTY_TEXT_TEXT,  "Start Delay");
    
    // Start delay time spinner (global element)
    configSpinnerStartDelay = gui_element_create(GUI_ELEMENT_SPINNER);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_HEIGHT,  16);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_MAXIMUM, 30);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_MINIMUM, 0);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_POS_X,   25);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_POS_Y,   20);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_STEP,    1);
    gui_property_set_str(configSpinnerStartDelay, GUI_PROPERTY_SPINNER_SUFFIX,  " s");
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_VALUE,   0);
    gui_property_set(    configSpinnerStartDelay, GUI_PROPERTY_SPINNER_WIDTH,   50);
    
    // "Accept" button (local element)
    TGuiElement buttonAcceptStartDelay = gui_element_create(GUI_ELEMENT_BUTTON);
    gui_property_set(    buttonAcceptStartDelay, GUI_PROPERTY_BUTTON_HEIGHT, 10);
    gui_property_set(    buttonAcceptStartDelay, GUI_PROPERTY_BUTTON_POS_X,  25);
    gui_property_set(    buttonAcceptStartDelay, GUI_PROPERTY_BUTTON_POS_Y,  6);
    gui_property_set(    buttonAcceptStartDelay, GUI_PROPERTY_BUTTON_SHAPE,  GUI_PROPERTY_BUTTON_SHAPE_RECTANGLE);
    gui_property_set_str(buttonAcceptStartDelay, GUI_PROPERTY_BUTTON_TEXT,   "Accept");
    gui_property_set(    buttonAcceptStartDelay, GUI_PROPERTY_BUTTON_WIDTH,  50);
    
    // Give "Accept" button focus for convenience
    gui_element_focus(buttonAcceptStartDelay);
    
    // Wait for "Accept" button press
    gui_event_register(buttonAcceptStartDelay, GUI_EVENT_BUTTON_PRESS, "start_delay");
    while (!configAcceptStartDelay) {}
    
    gui_end();
    
    /* Start position screen */
    
    gui_begin();
    
    // Screen title (local element)
    TGuiElement textTitleStartPosition = gui_element_create(GUI_ELEMENT_TEXT);
    gui_property_set(    textTitleStartPosition, GUI_PROPERTY_TEXT_ALIGN, GUI_PROPERTY_TEXT_ALIGN_CENTER);
    gui_property_set(    textTitleStartPosition, GUI_PROPERTY_TEXT_LINE,  1);
    gui_property_set(    textTitleStartPosition, GUI_PROPERTY_TEXT_SIZE,  GUI_PROPERTY_TEXT_SIZE_NORMAL);
    gui_property_set_str(textTitleStartPosition, GUI_PROPERTY_TEXT_TEXT,  "Start Position");
    
    // "On ramp" radio button (global element)
    configRadioButtonStartPositionOnRamp = gui_element_create(GUI_ELEMENT_BUTTON_RADIO);
    gui_property_set(configRadioButtonStartPositionOnRamp, GUI_PROPERTY_BUTTON_RADIO_CHANNEL, 0);
    gui_property_set(configRadioButtonStartPositionOnRamp, GUI_PROPERTY_BUTTON_RADIO_CHECKED, true);
    gui_property_set(configRadioButtonStartPositionOnRamp, GUI_PROPERTY_BUTTON_RADIO_POS_X,   5);
    gui_property_set(configRadioButtonStartPositionOnRamp, GUI_PROPERTY_BUTTON_RADIO_POS_Y,   34);
    
    // "On ramp" radio button label (local element)
    TGuiElement labelConfigRadioButtonStartPositionOnRamp = gui_element_create(GUI_ELEMENT_STRING);
    gui_property_set(    labelConfigRadioButtonStartPositionOnRamp, GUI_PROPERTY_STRING_POS_X, 20);
    gui_property_set(    labelConfigRadioButtonStartPositionOnRamp, GUI_PROPERTY_STRING_POS_Y, 41);
    gui_property_set(    labelConfigRadioButtonStartPositionOnRamp, GUI_PROPERTY_STRING_SIZE,  GUI_PROPERTY_STRING_SIZE_NORMAL);
    gui_property_set_str(labelConfigRadioButtonStartPositionOnRamp, GUI_PROPERTY_STRING_TEXT,  "On ramp");
    
    // "On floor" radio button (global element)
    configRadioButtonStartPositionOnFloor = gui_element_create(GUI_ELEMENT_BUTTON_RADIO);
    gui_property_set(configRadioButtonStartPositionOnFloor, GUI_PROPERTY_BUTTON_RADIO_CHANNEL, 0);
    gui_property_set(configRadioButtonStartPositionOnFloor, GUI_PROPERTY_BUTTON_RADIO_CHECKED, false);
    gui_property_set(configRadioButtonStartPositionOnFloor, GUI_PROPERTY_BUTTON_RADIO_POS_X,   5);
    gui_property_set(configRadioButtonStartPositionOnFloor, GUI_PROPERTY_BUTTON_RADIO_POS_Y,   21);
    
    // "On floor" radio button label (local element)
    TGuiElement labelConfigRadioButtonStartPositionOnFloor = gui_element_create(GUI_ELEMENT_STRING);
    gui_property_set(    labelConfigRadioButtonStartPositionOnFloor, GUI_PROPERTY_STRING_POS_X, 20);
    gui_property_set(    labelConfigRadioButtonStartPositionOnFloor, GUI_PROPERTY_STRING_POS_Y, 28);
    gui_property_set(    labelConfigRadioButtonStartPositionOnFloor, GUI_PROPERTY_STRING_SIZE,  GUI_PROPERTY_STRING_SIZE_NORMAL);
    gui_property_set_str(labelConfigRadioButtonStartPositionOnFloor, GUI_PROPERTY_STRING_TEXT,  "On floor");
    
    // "Accept" button (local element)
    TGuiElement buttonAcceptStartPosition = gui_element_create(GUI_ELEMENT_BUTTON);
    gui_property_set(    buttonAcceptStartPosition, GUI_PROPERTY_BUTTON_HEIGHT, 10);
    gui_property_set(    buttonAcceptStartPosition, GUI_PROPERTY_BUTTON_POS_X,  25);
    gui_property_set(    buttonAcceptStartPosition, GUI_PROPERTY_BUTTON_POS_Y,  6);
    gui_property_set(    buttonAcceptStartPosition, GUI_PROPERTY_BUTTON_SHAPE,  GUI_PROPERTY_BUTTON_SHAPE_RECTANGLE);
    gui_property_set_str(buttonAcceptStartPosition, GUI_PROPERTY_BUTTON_TEXT,   "Accept");
    gui_property_set(    buttonAcceptStartPosition, GUI_PROPERTY_BUTTON_WIDTH,  50);
    
    // Give "Accept" button focus for convenience
    gui_element_focus(buttonAcceptStartPosition);
    
    // Wait for "Accept" button press
    gui_event_register(buttonAcceptStartPosition, GUI_EVENT_BUTTON_PRESS, "start_position");
    while (!configAcceptStartPosition) {}
    
    gui_end();
    
    /* Actions screen */
    
    gui_begin();
    
    // Screen title (local element)
    TGuiElement textTitleActions = gui_element_create(GUI_ELEMENT_TEXT);
    gui_property_set(    textTitleActions, GUI_PROPERTY_TEXT_ALIGN, GUI_PROPERTY_TEXT_ALIGN_CENTER);
    gui_property_set(    textTitleActions, GUI_PROPERTY_TEXT_LINE,  1);
    gui_property_set(    textTitleActions, GUI_PROPERTY_TEXT_SIZE,  GUI_PROPERTY_TEXT_SIZE_NORMAL);
    gui_property_set_str(textTitleActions, GUI_PROPERTY_TEXT_TEXT,  "Actions");
    
    // Display "Grab RG" checkbox if starting on ramp
    if (configStartPosition == START_POSITION_RAMP) {
        // "Grab RG" checkbox (global element)
        configCheckboxActionsGrabRollingGoal = gui_element_create(GUI_ELEMENT_CHECKBOX);
        gui_property_set(configCheckboxActionsGrabRollingGoal, GUI_PROPERTY_CHECKBOX_POS_X, 5);
        gui_property_set(configCheckboxActionsGrabRollingGoal, GUI_PROPERTY_CHECKBOX_POS_Y, 34);
        
        // "Grab RG" checkbox label (local element)
        TGuiElement labelConfigCheckboxActionsGrabRollingGoal = gui_element_create(GUI_ELEMENT_STRING);
        gui_property_set(    labelConfigCheckboxActionsGrabRollingGoal, GUI_PROPERTY_STRING_POS_X, 20);
        gui_property_set(    labelConfigCheckboxActionsGrabRollingGoal, GUI_PROPERTY_STRING_POS_Y, 41);
        gui_property_set(    labelConfigCheckboxActionsGrabRollingGoal, GUI_PROPERTY_STRING_SIZE,  GUI_PROPERTY_STRING_SIZE_NORMAL);
        gui_property_set_str(labelConfigCheckboxActionsGrabRollingGoal, GUI_PROPERTY_STRING_TEXT,  "Grab RG");
    }
    
    // "Shoot ball" checkbox (global element)
    configCheckboxActionsShootBall = gui_element_create(GUI_ELEMENT_CHECKBOX);
    gui_property_set(configCheckboxActionsShootBall, GUI_PROPERTY_CHECKBOX_POS_X, 5);
    gui_property_set(configCheckboxActionsShootBall, GUI_PROPERTY_CHECKBOX_POS_Y, 21);
    
    // "Shoot ball" checkbox label (local element)
    TGuiElement labelConfigCheckboxActionsShootBall = gui_element_create(GUI_ELEMENT_STRING);
    gui_property_set(    labelConfigCheckboxActionsShootBall, GUI_PROPERTY_STRING_POS_X, 20);
    gui_property_set(    labelConfigCheckboxActionsShootBall, GUI_PROPERTY_STRING_POS_Y, 28);
    gui_property_set(    labelConfigCheckboxActionsShootBall, GUI_PROPERTY_STRING_SIZE,  GUI_PROPERTY_STRING_SIZE_NORMAL);
    gui_property_set_str(labelConfigCheckboxActionsShootBall, GUI_PROPERTY_STRING_TEXT,  "Shoot ball");
    
    // "Accept" button (local element)
    TGuiElement buttonAcceptActions = gui_element_create(GUI_ELEMENT_BUTTON);
    gui_property_set(    buttonAcceptActions, GUI_PROPERTY_BUTTON_HEIGHT, 10);
    gui_property_set(    buttonAcceptActions, GUI_PROPERTY_BUTTON_POS_X,  25);
    gui_property_set(    buttonAcceptActions, GUI_PROPERTY_BUTTON_POS_Y,  6);
    gui_property_set(    buttonAcceptActions, GUI_PROPERTY_BUTTON_SHAPE,  GUI_PROPERTY_BUTTON_SHAPE_RECTANGLE);
    gui_property_set_str(buttonAcceptActions, GUI_PROPERTY_BUTTON_TEXT,  "Accept");
    gui_property_set(    buttonAcceptActions, GUI_PROPERTY_BUTTON_WIDTH,  50);
    
    // Give "Accept" button focus for convenience
    gui_element_focus(buttonAcceptActions);
    
    // Wait for "Accept" button press
    gui_event_register(buttonAcceptActions, GUI_EVENT_BUTTON_PRESS, "actions");
    while (!configAcceptActions) {}
    
    gui_end();
    
    // Stop event task
    StopTask(configGuiEvent);
    
    // Enable JoystickDriver.c diagnostics screen for visual diagnostics
    bDisplayDiagnostics = true;
}

// Event task (receives all registered GUI events)
task configGuiEvent() {
    char event[GUI_NUM_BYTES_PER_EVENT];
    
    while (true) {
        if (gui_event_get(event)) {
            if (!strcmp(event, "start_delay")) {
                configStartDelay = gui_property_get(configSpinnerStartDelay, GUI_PROPERTY_SPINNER_VALUE);
                
                // Mark completed
                configAcceptStartDelay = true;
            } else if (!strcmp(event, "start_position")) {
                if (gui_property_get(configRadioButtonStartPositionOnRamp, GUI_PROPERTY_BUTTON_RADIO_CHECKED)) {
                    configStartPosition = START_POSITION_RAMP;
                } else if (gui_property_get(configRadioButtonStartPositionOnFloor, GUI_PROPERTY_BUTTON_RADIO_CHECKED)) {
                    configStartPosition = START_POSITION_FLOOR;
                }
                
                // Mark completed
                configAcceptStartPosition = true;
            } else if (!strcmp(event, "actions")) {
                configGrabRollingGoal = (bool) gui_property_get(configCheckboxActionsGrabRollingGoal, GUI_PROPERTY_CHECKBOX_CHECKED);
                configShootBall       = (bool) gui_property_get(configCheckboxActionsShootBall, GUI_PROPERTY_CHECKBOX_CHECKED);
                
                // Mark completed
                configAcceptActions = true;
            }
        }
    }
}

/* Autonomous routine functions */

// Main task
task main() {
    // Show GUI to configure autonomous session
    showConfigGui();
    
    // Wait for FCS start signal
    waitForStart();
    
    // Peripheral initialization
    initialize();
    
    // Wait for configured start delay
    wait1Msec(1000*configStartDelay);
    
    // Autonomous routine
    autonomousRoutine();
}

// Initialization
void initialize() {
    // Move goal clamp to initial position
    servo[servoGoalClamp] = SERVO_VALUE_GOAL_CLAMP_RAISE;
}

// Autonomous routine
void autonomousRoutine() {
    // An array of data shared across maneuvers
    unsigned char data[32];
    
    // Store initial compass reading to data array in little-endian format
    unsigned int angle = HTMCreadHeading(sensorCompass);
    data[MANEUVER_DATA_INITIAL_COMPASS_READING_LOW]  = (unsigned char) (angle & 0xff);
    data[MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH] = (unsigned char) (angle >> 8);
    
    // Suppress launcher release on servo initialization
    servo[servoLauncherRelease] = SERVO_VALUE_LAUNCHER_HOLD;
    
    if (configStartPosition == START_POSITION_FLOOR) {
        if (configShootBall) {
            
            /*
             *
             * Task: Starting on floor, drive to shooting position and shoot
             * ball
             *
             */
            
            autonomousManeuver(MANEUVER_CENTER_GOAL_ORIENT, data);
            
            // Only continue if orientation was successful
            if (data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] > 0) {
                autonomousManeuver(MANEUVER_CENTER_GOAL_APPROACH, data);
                autonomousManeuver(MANEUVER_LAUNCHER_RELEASE,     data);
            }
        }
    } else if (configStartPosition == START_POSITION_RAMP) {
        if (configGrabRollingGoal && configShootBall) {
            
            /*
             *
             * Task: Starting on ramp, grab frontmost rolling goal and deposit
             * it in parking zone; then drive to shooting position and shoot
             * ball
             *
             */
            
            autonomousManeuver(MANEUVER_RAMP_EXIT,            data);
            autonomousManeuver(MANEUVER_ROLLING_GOAL_PICKUP,  data);
            autonomousManeuver(MANEUVER_ROLLING_GOAL_DEPOSIT, data);
            autonomousManeuver(MANEUVER_CENTER_GOAL_ORIENT,   data);
            
            // Only continue if orientation was successful
            if (data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] > 0) {
                autonomousManeuver(MANEUVER_CENTER_GOAL_APPROACH, data);
                autonomousManeuver(MANEUVER_LAUNCHER_RELEASE,     data);
            }
        } else if (!configGrabRollingGoal && configShootBall) {
            
            /*
             *
             * Task: Starting on ramp, drive to shooting position and shoot ball
             *
             */
            
            autonomousManeuver(MANEUVER_RAMP_EXIT,                data);
            autonomousManeuver(MANEUVER_BRIDGE_RAMP_PARKING_ZONE, data);
            autonomousManeuver(MANEUVER_CENTER_GOAL_ORIENT,       data);
            
            // Only continue if orientation was successful
            if (data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] > 0) {
                autonomousManeuver(MANEUVER_CENTER_GOAL_APPROACH, data);
                autonomousManeuver(MANEUVER_LAUNCHER_RELEASE,     data);
            }
        } else if (configGrabRollingGoal && !configShootBall) {
            
            /*
             *
             * Task: Starting on ramp, grab frontmost rolling goal and deposit
             * it in parking zone
             *
             */
            
            autonomousManeuver(MANEUVER_RAMP_EXIT,            data);
            autonomousManeuver(MANEUVER_ROLLING_GOAL_PICKUP,  data);
            autonomousManeuver(MANEUVER_ROLLING_GOAL_DEPOSIT, data);
        }
    }
}

// Directs the robot to perform the specified maneuver with the given data
void autonomousManeuver(int maneuver, char* data) {
    switch (maneuver) {
    case MANEUVER_BRIDGE_RAMP_PARKING_ZONE: {
            // Extract initial compass angle from data array
            unsigned int angleInitial = (((unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH] << 8) | (unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_LOW]);
            
            // Aim toward parking zone
            driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 50, 90);
            driveDistance(100, 50);
            driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 50, 45);
            
            // Drive to parking zone
            driveDistance(100, 215);
            driveRotateAngleAbsolute(ROTATE_DIRECTION_CCW, 50, angleWrap360(145 - angleInitial));
        } break;
    case MANEUVER_CENTER_GOAL_APPROACH: {
            // Extract initial compass angle from data array
            unsigned int angleInitial = (((unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH] << 8) | (unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_LOW]);
            
            // Extract center field structure orientation from data array
            unsigned int centerFieldStructureOrientation = (unsigned int) data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION];
            
            driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 25, 10);
            
            // Aim directly at center goal
            driveRotateStart(ROTATE_DIRECTION_CCW, 20);
            while (SensorValue[sensorIR] == 5) {}
            driveStop();
            driveRotateStart(ROTATE_DIRECTION_CW, 15);
            while (SensorValue[sensorIR] != 5) {}
            driveStop();
            //driveRotateStart(ROTATE_DIRECTION_CCW, 10);
            //while (SensorValue[sensorIR] == 5) {}
            //driveStop();
            //driveRotateAngleRelative(ROTATE_DIRECTION_CW, 25, 10 - 2);
            wait1Msec(500);
            
            driveRotateAngleRelative(ROTATE_DIRECTION_CW, 10, 10);
            
            // Approach center goal
            driveStart(-50);
            while (SensorValue[sensorUltrasonic] >= 25) {}
            driveStop();
            wait1Msec(500);
            
            // Final adjustment based on CFS orientation
            switch (centerFieldStructureOrientation) {
            case 1:
                driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 25, 5);
                break;
            case 2:
                driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 25, 10);
                break;
            case 3:
                driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 25, 8);
                break;
            }
            driveDistance(-20, 7);
            
            // Delay before launching
            wait1Msec(500);
        } break;
    case MANEUVER_CENTER_GOAL_ORIENT: {
            unsigned int angleInitial = (((unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH] << 8) | (unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_LOW]);
            
            // Drive forward
            driveDistance(-50, 10);
            driveStop();
            wait1Msec(500);
            
            driveRotateAngleRelative(ROTATE_DIRECTION_CW, 25, 15);
            int differentialIRValue = SensorValue[sensorIR];
            driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 25, 15);
            
            // If center field structure is in orientation 3
            if (differentialIRValue == 5) {
                // Remember orientation 3
                data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] = 3;
            } else {
                // Try orientation 2
                driveRotateAngleAbsolute(ROTATE_DIRECTION_CCW, 25, angleWrap360(angleInitial - 45));
                driveDistance(-75, 90);
                driveRotateAngleAbsolute(ROTATE_DIRECTION_CW, 25, angleWrap360(angleInitial + 45));
                
                driveRotateAngleRelative(ROTATE_DIRECTION_CW, 25, 10);
                differentialIRValue = SensorValue[sensorIR];
                driveRotateAngleRelative(ROTATE_DIRECTION_CCW, 25, 10);
                
                // If center field structure is in orientation 2
                if (differentialIRValue == 5) {
                    // Remember orientation 2
                    data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] = 2;
                } else {
                    // Try orientation 1
                    driveRotateAngleAbsolute(ROTATE_DIRECTION_CCW, 25, angleWrap360(angleInitial - 10));
                    driveDistance(-75, 75);
                    driveRotateAngleAbsolute(ROTATE_DIRECTION_CW, 25, angleWrap360(angleInitial + 78));
                    
                    if (SensorValue[sensorIR] == 5) {
                        // Remember orientation 1
                        data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] = 1;
                    } else {
                        // Orientation is unknown
                        data[MANEUVER_DATA_CENTER_FIELD_STRUCTURE_ORIENTATION] = 0;
                    }
                }
            }
        } break;
    case MANEUVER_RAMP_EXIT: {
            // Start driving forward
            driveStart(100);
            
            // Open rolling goal clamp to clear ramp
            servo[servoGoalClamp] = SERVO_VALUE_GOAL_CLAMP_RAISE;
            
            // Allow robot to drive onto ramp before polling tilt sensor
            wait1Msec(1500);
            
            // Wait until robot reaches floor
            while (!TSreadState(sensorTilt)) {}
            
            // Stop robot
            driveStop();
        } break;
    case MANEUVER_ROLLING_GOAL_DEPOSIT: {
            // Extract initial compass angle from data array
            unsigned int angleInitial = (((unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH] << 8) | (unsigned int) data[MANEUVER_DATA_INITIAL_COMPASS_READING_LOW]);
            
            // Rotate toward parking zone
            driveRotateAngleAbsolute(ROTATE_DIRECTION_CCW, 20, angleWrap360(angleInitial - 108));
            
            // Drive to parking zone
            driveDistance(80, 205);
            wait1Msec(500);
            driveRotateAngleAbsolute(ROTATE_DIRECTION_CCW, 50, angleWrap360(angleInitial - 150));
            
            // Open rolling goal clamp
            servo[servoGoalClamp] = SERVO_VALUE_GOAL_CLAMP_RAISE;
            wait1Msec(DELAY_SERVO_ACTUATION);
            
            unsigned int angle = angleWrap360(angleInitial + 180);
            data[MANEUVER_DATA_INITIAL_COMPASS_READING_LOW]  = (unsigned char) (angle & 0xff);
            data[MANEUVER_DATA_INITIAL_COMPASS_READING_HIGH] = (unsigned char) (angle >> 8);
            
            wait1Msec(1000);
        } break;
    case MANEUVER_ROLLING_GOAL_PICKUP: {
            // Drive forward
            driveStart(100);
            
            // Wait until robot touches rolling goal with either front sensor (inverted signals)
            while (TSreadState(sensorRGDetectL) && TSreadState(sensorRGDetectR)) {}
            driveStop();
            
            // Close rolling goal clamp
            servo[servoGoalClamp] = SERVO_VALUE_GOAL_CLAMP_LOWER;
            wait1Msec(DELAY_SERVO_ACTUATION);
        } break;
    case MANEUVER_LAUNCHER_RELEASE: {
            // Release launcher
            servo[servoLauncherRelease] = SERVO_VALUE_LAUNCHER_RELEASE;
            wait1Msec(DELAY_LAUNCHER_RELEASE);
            servo[servoLauncherRelease] = SERVO_VALUE_LAUNCHER_HOLD;
        } break;
    }
}

/* Drive functions */

// Resets all drive encoders
void resetEncoders() {
    // Set all drive encoder values to 0
    nMotorEncoder[motorDrive1] = 0;
    nMotorEncoder[motorDrive2] = 0;
    nMotorEncoder[motorDrive3] = 0;
    nMotorEncoder[motorDrive4] = 0;
}

// Stops all drive motors
void driveStop() {
    // Set all motor powers to 0
    motor[motorDrive1] = 0;
    motor[motorDrive2] = 0;
    motor[motorDrive3] = 0;
    motor[motorDrive4] = 0;
}

// Begins driving at the given power
void driveStart(int power) {
    // Reset encoders
    resetEncoders();
    
    // Set all motor powers
    motor[motorDrive1] += power;
    motor[motorDrive2] += power;
    motor[motorDrive3] += power;
    motor[motorDrive4] += power;
}

// Begins rotating the robot in the given direction at the given power
void driveRotateStart(int direction, int power) {
    // Reset encoders
    resetEncoders();
    
    // Set all motor powers
    motor[motorDrive1] += direction*power;
    motor[motorDrive2] -= direction*power;
    motor[motorDrive3] -= direction*power;
    motor[motorDrive4] += direction*power;
}

// Drives the robot at the given power for the given duration
void driveDuration(int power, int duration) {
    // Begin driving
    driveStop();
    driveStart(power);
    
    // Wait for duration
    wait1Msec(duration);
    
    // Stop driving
    driveStop();
}

// Drive the robot at the given power over the given distance
void driveDistance(int power, int distance) {
    // Begin driving
    driveStop();
    driveStart(power);
    
    // Calculate individual motor encoder targets
    int targetMotor1 = distance*ENCODER_1_COUNTS_PER_CM;
    int targetMotor2 = distance*ENCODER_2_COUNTS_PER_CM;
    int targetMotor3 = distance*ENCODER_3_COUNTS_PER_CM;
    int targetMotor4 = distance*ENCODER_4_COUNTS_PER_CM;
    
    // Whether or not each motor has met its encoder target
    bool targetMetMotor1 = false;
    bool targetMetMotor2 = false;
    bool targetMetMotor3 = false;
    bool targetMetMotor4 = false;
    
    // Stop each motor as respective encoder targets are met
    while (!targetMetMotor1 || !targetMetMotor2 || !targetMetMotor3 || !targetMetMotor4) {
        if (!targetMetMotor1 && abs(nMotorEncoder[motorDrive1]) >= targetMotor1) {
            motor[motorDrive1] = 0;
            targetMetMotor1 = true;
        }
        if (!targetMetMotor2 && abs(nMotorEncoder[motorDrive2]) >= targetMotor2) {
            motor[motorDrive2] = 0;
            targetMetMotor2 = true;
        }
        if (!targetMetMotor3 && abs(nMotorEncoder[motorDrive3]) >= targetMotor3) {
            motor[motorDrive3] = 0;
            targetMetMotor3 = true;
        }
        if (!targetMetMotor4 && abs(nMotorEncoder[motorDrive4]) >= targetMotor4) {
            motor[motorDrive4] = 0;
            targetMetMotor4 = true;
        }
    }
}

// Rotates the robot in the given direction at the given power for the given duration
void driveRotateDuration(int direction, int power, int duration) {
    // Begin rotating
    driveStop();
    driveRotateStart(direction, power);
    
    // Wait for duration
    wait1Msec(duration);
    
    // Stop rotating
    driveStop();
}

// Rotates the robot in the given direction at the given power through the given absolute angle
void driveRotateAngleAbsolute(int direction, int power, int angle) {
    // Begin rotating
    driveStop();
    driveRotateStart(direction, power);
    
    // Calculate an appropriate tolerance to maximize precision while minimizing overshooting
    // This formula is the result of a quadratic regression over (10, 2); (50, 5); and (100, 8)
    int tolerance = (int) ((float) (-5 * power * power + 2550 * power + 35000) / 30000.0);
    
    // Wait until target angle is within +/- tolerance
    while (abs(angleWrap180(HTMCreadHeading(sensorCompass) - angle)) > tolerance) {}
    
    // Stop rotating
    driveStop();
}

// Rotates the robot in the given direction at the given power through the given relative angle
void driveRotateAngleRelative(int direction, int power, int angle) {
    // Calculate absolute target angle based on direction and current compass reading
    int angleAbsolute = angleWrap360(HTMCreadHeading(sensorCompass) + angle * (direction == ROTATE_DIRECTION_CCW ? -1 : 1));
    
    // Act on this absolute angle like any other
    driveRotateAngleAbsolute(direction, power, angleAbsolute);
}

/* Utility functions */

// Wraps the given angle within [-180, 180)
int angleWrap180(int angle) {
    while (angle < -180) {
        angle += 360;
    }
    while (angle >= 180) {
        angle -= 360;
    }
    
    return angle;
}

// Wraps the given angle within [0, 360)
int angleWrap360(int angle) {
    while (angle < 0) {
        angle += 360;
    }
    while (angle >= 360) {
        angle -= 360;
    }
    
    return angle;
}
