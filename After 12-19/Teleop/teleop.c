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

// The Doppler Effect
// FTC #5703

// Teleop Program

#include "teleop.h"

// Standard FTC driver
#include "JoystickDriver.c"

// Momentary maximum drive power
int drivePowerMax = DRIVE_POWER_MAXIMUM_DEFAULT;

// Precision mode memory (0 = disabled, 1 = enabling, 2 = enabled, 3 = disabling)
int precisionModeState = 0;

bool launchDebounce = false;

// Main task
task main() {
    // Wait for FCS start signal
    waitForStart();
    
    // Teleop loop
    while (true) {
        // Read controller states
        getJoystickSettings(joystick);
        
        /* Joysticks */
        
        // Read and adjust joystick values to [-1, 1]
        float c1J1Y = (joystick.joy1_y1 + 0.5)/175.5;
        float c1J2Y = (joystick.joy1_y2 + 0.5)/175.5;
        
        // Zero controller 1 joystick deadbands
        if (abs(c1J1Y) < CONTROLLER_1_JOYSTICK_1_DEADBAND_RADIUS) {
            c1J1Y = 0.0;
        }
        if (abs(c1J2Y) < CONTROLLER_1_JOYSTICK_2_DEADBAND_RADIUS) {
            c1J2Y = 0.0;
        }
        
        // Set motor powers based on a tank drive scheme
        motor[motorDrive1] = c1J2Y*drivePowerMax;
        motor[motorDrive2] = c1J1Y*drivePowerMax;
        motor[motorDrive3] = c1J1Y*drivePowerMax;
        motor[motorDrive4] = c1J2Y*drivePowerMax;
        
        /* Buttons */
        
        // Handle buttons on controller 1
        if (joy1Btn(CONTROLLER_1_BUTTON_PRECISION_MODE)) {
            if (precisionModeState == PRECISION_MODE_STATE_DISABLED ||
                precisionModeState == PRECISION_MODE_STATE_ENABLED) {
                precisionModeState++;
            }
        } else {
            if (precisionModeState == PRECISION_MODE_STATE_ENABLING) {
                precisionModeState = PRECISION_MODE_STATE_ENABLED;
                drivePowerMax = DRIVE_POWER_MAXIMUM_PRECISION;
            } else if (precisionModeState == PRECISION_MODE_STATE_DISABLING) {
                precisionModeState = PRECISION_MODE_STATE_DISABLED;
                drivePowerMax = DRIVE_POWER_MAXIMUM_DEFAULT;
            }
        }
        
        // Handle buttons on controller 2
        if (joy2Btn(CONTROLLER_2_BUTTON_GOAL_CLAMP_RAISE)) {
            servo[servoGoalClamp] = SERVO_VALUE_GOAL_CLAMP_RAISE;
        }
        if (joy2Btn(CONTROLLER_2_BUTTON_GOAL_CLAMP_LOWER)) {
            servo[servoGoalClamp] = SERVO_VALUE_GOAL_CLAMP_LOWER;
        }
        if (joy2Btn(CONTROLLER_2_BUTTON_LAUNCHER_RELEASE)) {
            if (!launchDebounce) {
                launchDebounce = true;
                
                servo[servoLauncherRelease] = SERVO_VALUE_LAUNCHER_RELEASE;
                wait1Msec(DELAY_LAUNCHER_RELEASE);
                servo[servoLauncherRelease] = SERVO_VALUE_LAUNCHER_HOLD;
            }
        }
    }
}
