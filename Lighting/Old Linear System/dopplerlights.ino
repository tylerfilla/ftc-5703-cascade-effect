
#include <Adafruit_NeoPixel.h>
#include "dopplerlights.h"

Adafruit_NeoPixel led = Adafruit_NeoPixel(LED_COUNT, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

int controlDebounce = millis();
int controlLast     = 0;
int controlState    = 0;

int mode = MODE_SOLID_RED;

/* Arduino control functions */

void setup() {
    Serial.begin(115200);
    led.begin();
    Serial.println("BEGIN");
    
    pinMode(PIN_CONTROL, INPUT_PULLUP);
    pinMode(PIN_STATUS,  OUTPUT);
    
    modeControl(MODE_CONTROL_SETUP);
}

void loop() {
    int control = digitalRead(PIN_CONTROL);
    
    if (Serial.read() == 'c') {
        Serial.println("control via serial");
        control = 0;
        delay(500);
    }
    
    if (control != controlLast) {
        controlDebounce = millis();
        controlState    = 0;
    }
    
    if (millis() - controlDebounce > 50 && !controlState) {
        controlLast  = control;
        controlState = 1;
        
        if (!control) {
            Serial.println("MODE_CONTROL TRIGGER");
            
            if (++mode > MODE_COUNT) {
                mode = 1;
            }
            modeControl(MODE_CONTROL_SETUP);
            
            digitalWrite(PIN_STATUS, HIGH);
            delay(300);
            digitalWrite(PIN_STATUS, LOW);
            delay(200);
            
            for (int i = 0; i < mode; i++) {
                digitalWrite(PIN_STATUS, HIGH);
                delay(100);
                digitalWrite(PIN_STATUS, LOW);
                delay(100);
            }
            
            delay(200);
            
            if (mode == MODE_COUNT) {
                digitalWrite(PIN_STATUS, HIGH);
                delay(300);
                digitalWrite(PIN_STATUS, LOW);
            }
        }
    }
    
    modeControl(MODE_CONTROL_UPDATE);
}

/* Lighting mode functions */

int           modeFlashSlowState = 0;
unsigned long modeFlashSlowTime  = 0ul;

int           modeFlashFastState = 0;
unsigned long modeFlashFastTime  = 0ul;

int           modeCrossfadeDir      = 1;
int           modeCrossfadePauseFwd = 0;
int           modeCrossfadePauseRev = 0;
int           modeCrossfadeState    = 0;
unsigned long modeCrossfadeTime     = 0ul;

int           modeWarpState[LED_COUNT][2];
unsigned long modeWarpTime  = 0ul;

unsigned long modeDopplerTime             = 0ul;
int           modeDopplerAccelRawAvgX[MODE_DOPPLER_ACCEL_RAW_AVG_SIZE];
int           modeDopplerAccelRawAvgY[MODE_DOPPLER_ACCEL_RAW_AVG_SIZE];
int           modeDopplerAccelRawAvgZ[MODE_DOPPLER_ACCEL_RAW_AVG_SIZE];
unsigned long modeDopplerCalibrationLast  = 0ul;
int           modeDopplerCalibrationTimer = 0;
float         modeDopplerVelocityX        = 0.0f;
float         modeDopplerVelocityY        = 0.0f;
float         modeDopplerVelocityZ        = 0.0f;

void modeControl(int control) {
    Serial.print("MODE_CONTROL MODE ");
    Serial.print(mode);
    Serial.print(" CONTROL ");
    Serial.print(control);
    Serial.println();
    
    switch (mode) {
    case MODE_SOLID_RED:
        modeSolidRed(control);
        break;
    case MODE_SOLID_BLUE:
        modeSolidBlue(control);
        break;
    case MODE_FLASH_SLOW:
        modeFlashSlow(control);
        break;
    case MODE_FLASH_FAST:
        modeFlashFast(control);
        break;
    case MODE_CROSSFADE:
        modeCrossfade(control);
        break;
    case MODE_WARP:
        modeWarp(control);
        break;
    case MODE_DOPPLER:
        modeDoppler(control);
        break;
    }
}

void modeSolidRed(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        for (int i = 0; i < LED_COUNT; i++) {
            led.setPixelColor(i, 0xFF0000);
        }
        led.show();
        break;
    case MODE_CONTROL_UPDATE:
        break;
    }
}

void modeSolidBlue(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        for (int i = 0; i < LED_COUNT; i++) {
            led.setPixelColor(i, 0x0000FF);
        }
        led.show();
        break;
    case MODE_CONTROL_UPDATE:
        break;
    }
}

void modeFlashSlow(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        modeFlashSlowState = 0;
        modeFlashSlowTime  = 0;
        
        for (int i = 0; i < LED_COUNT; i++) {
            led.setPixelColor(i, 0xFF0000);
        }
        led.show();
        break;
    case MODE_CONTROL_UPDATE:
        if (millis() - modeFlashSlowTime > MODE_FLASH_SLOW_DELAY) {
            modeFlashSlowState = !modeFlashSlowState;
            modeFlashSlowTime  = millis();
            
            for (int i = 0; i < LED_COUNT; i++) {
                if (modeFlashSlowState) {
                    led.setPixelColor(i, 0x0000FF);
                } else {
                    led.setPixelColor(i, 0xFF0000);
                }
            }
            led.show();
        }
        break;
    }
}

void modeFlashFast(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        modeFlashFastState = 0;
        modeFlashFastTime  = 0;
        
        for (int i = 0; i < LED_COUNT; i++) {
            led.setPixelColor(i, 0xFF0000);
        }
        led.show();
        break;
    case MODE_CONTROL_UPDATE:
        if (millis() - modeFlashFastTime > MODE_FLASH_FAST_DELAY) {
            modeFlashFastState = !modeFlashFastState;
            modeFlashFastTime  = millis();
            
            for (int i = 0; i < LED_COUNT; i++) {
                if (modeFlashFastState) {
                    led.setPixelColor(i, 0x0000FF);
                } else {
                    led.setPixelColor(i, 0xFF0000);
                }
            }
            led.show();
        }
        break;
    }
}

void modeCrossfade(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        break;
    case MODE_CONTROL_UPDATE:
        if (modeCrossfadePauseFwd <= 0 && modeCrossfadePauseRev <= 0) {
            if (millis() - modeCrossfadeTime > MODE_CROSSFADE_DELAY) {
                modeCrossfadeTime = millis();
                
                if (modeCrossfadeDir == 1) {
                    if (++modeCrossfadeState > 255) {
                        modeCrossfadeDir   = -1;
                        modeCrossfadeState = 255;
                    }
                } else if (modeCrossfadeDir == -1) {
                    if (--modeCrossfadeState <= 0) {
                        modeCrossfadeDir   = 1;
                        modeCrossfadeState = 0;
                    }
                }
                
                for (int i = 0; i < LED_COUNT; i++) {
                    led.setPixelColor(i, modeCrossfadeState, 0, 255 - modeCrossfadeState);
                }
                led.show();
                
                if (modeCrossfadeState == 0) {
                    modeCrossfadePauseRev = 200;
                }
                if (modeCrossfadeState == 255) {
                    modeCrossfadePauseFwd = 200;
                }
            }
        } else {
            if (--modeCrossfadePauseRev > 0) {
                delay(1);
            } else {
                modeCrossfadePauseRev = 0;
            }
            
            if (--modeCrossfadePauseFwd > 0) {
                delay(1);
            } else {
                modeCrossfadePauseFwd = 0;
            }
        }
        break;
    }
}

void modeWarp(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP: {
        int lambda = LED_COUNT/MODE_WARP_WAVELENGTHS;
        
        for (int w = 0; w < MODE_WARP_WAVELENGTHS; w++) {
            int ledStart = w*lambda;
            int ledStop  = ledStart + lambda - 1;
            
            int alt   = 1;
            int delta = 512/(ledStop - ledStart + 1);
            
            int vR = 0;
            int vB = 255;
            
            for (int l = ledStart; l <= ledStop; l++) {
                if (alt == 1) {
                    if ((vR += delta) > 255 || (vB -= delta) < 0) {
                        vR = 255;
                        vB = 0;
                        
                        alt = -1;
                    }
                } else if (alt == -1) {
                    if ((vR -= delta) < 0 || (vB += delta) > 255) {
                        vR = 0;
                        vB = 255;
                        
                        alt = 1;
                    }
                }
                
                modeWarpState[l][0] = vR;
                modeWarpState[l][1] = vB;
            }
        }
        
        for (int i = 0; i < LED_COUNT; i++) {
            led.setPixelColor(i, modeWarpState[i][0], 0, modeWarpState[i][1]);
        }
        led.show();
        break;
    }
    case MODE_CONTROL_UPDATE:
        if (millis() - modeWarpTime > MODE_WARP_DELAY) {
            modeWarpTime = millis();
            
            int oR = modeWarpState[0][0];
            int oB = modeWarpState[0][1];
            
            for (int i = 0; i < LED_COUNT - 1; i++) {
                modeWarpState[i][0] = modeWarpState[i + 1][0];
                modeWarpState[i][1] = modeWarpState[i + 1][1];
            }
            
            modeWarpState[LED_COUNT - 1][0] = oR;
            modeWarpState[LED_COUNT - 1][1] = oB;
            
            for (int i = 0; i < LED_COUNT; i++) {
                led.setPixelColor(i, modeWarpState[i][0], 0, modeWarpState[i][1]);
            }
            led.show();
        }
        break;
    }
}

void modeDoppler(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        modeDopplerTime = millis();
        modeDopplerCalibrationTimer = MODE_DOPPLER_CALIBRATION_PERIOD;
        break;
    case MODE_CONTROL_UPDATE: {
        int accelRawX = analogRead(A2);
        int accelRawY = analogRead(A1);
        int accelRawZ = analogRead(A0);
        
        Serial.print("accelRawX=");
        Serial.println(accelRawX);
        Serial.print("accelRawY=");
        Serial.println(accelRawY);
        Serial.print("accelRawZ=");
        Serial.println(accelRawZ);
        
        float accelRawAvgX = (float) accelRawX;
        float accelRawAvgY = (float) accelRawY;
        float accelRawAvgZ = (float) accelRawZ;
        
        for (int i = 0; i < MODE_DOPPLER_ACCEL_RAW_AVG_SIZE - 1; i++) {
            modeDopplerAccelRawAvgX[i]  = modeDopplerAccelRawAvgX[i + 1];
            accelRawAvgX               += modeDopplerAccelRawAvgX[i + 1];
        }
        for (int i = 0; i < MODE_DOPPLER_ACCEL_RAW_AVG_SIZE - 1; i++) {
            modeDopplerAccelRawAvgY[i]  = modeDopplerAccelRawAvgY[i + 1];
            accelRawAvgY               += modeDopplerAccelRawAvgY[i + 1];
        }
        for (int i = 0; i < MODE_DOPPLER_ACCEL_RAW_AVG_SIZE - 1; i++) {
            modeDopplerAccelRawAvgZ[i]  = modeDopplerAccelRawAvgZ[i + 1];
            accelRawAvgZ               += modeDopplerAccelRawAvgZ[i + 1];
        }
        
        modeDopplerAccelRawAvgX[MODE_DOPPLER_ACCEL_RAW_AVG_SIZE - 1] = accelRawX;
        modeDopplerAccelRawAvgY[MODE_DOPPLER_ACCEL_RAW_AVG_SIZE - 1] = accelRawY;
        modeDopplerAccelRawAvgZ[MODE_DOPPLER_ACCEL_RAW_AVG_SIZE - 1] = accelRawZ;
        
        accelRawAvgX /= MODE_DOPPLER_ACCEL_RAW_AVG_SIZE;
        accelRawAvgY /= MODE_DOPPLER_ACCEL_RAW_AVG_SIZE;
        accelRawAvgZ /= MODE_DOPPLER_ACCEL_RAW_AVG_SIZE;
        
        Serial.print("accelRawAvgX=");
        Serial.println(accelRawAvgX);
        Serial.print("accelRawAvgY=");
        Serial.println(accelRawAvgY);
        Serial.print("accelRawAvgZ=");
        Serial.println(accelRawAvgZ);
        
        float accelX = ((float) accelRawAvgX/1023.0f*500.0f - 167.0f)/33.3f*-9.81f;
        float accelY = ((float) accelRawAvgY/1023.0f*500.0f - 161.0f)/34.0f*-9.81f;
        float accelZ = ((float) accelRawAvgZ/1023.0f*500.0f - 164.0f)/33.0f*-9.81f;
        
        Serial.print("accelX=");
        Serial.println(accelX);
        Serial.print("accelY=");
        Serial.println(accelY);
        Serial.print("accelZ=");
        Serial.println(accelZ);
        
        unsigned long elapsed = millis() - modeDopplerTime;
        
        Serial.print("millis=");
        Serial.println(millis());
        Serial.print("modeDopplerTime=");
        Serial.println(modeDopplerTime);
        Serial.print("elapsed=");
        Serial.println(elapsed);
        
        if (modeDopplerCalibrationTimer == 0) {
            modeDopplerVelocityX += accelX*((float) accelX/1000.0f);
            modeDopplerVelocityY += accelY*((float) accelX/1000.0f);
            modeDopplerVelocityZ += accelZ*((float) accelX/1000.0f);
            
            Serial.print("modeDopplerCalibrationLast=");
            Serial.println(modeDopplerCalibrationLast);
            
            Serial.print("modeDopplerVelocityX=");
            Serial.println(modeDopplerVelocityX);
            Serial.print("modeDopplerVelocityY=");
            Serial.println(modeDopplerVelocityY);
            Serial.print("modeDopplerVelocityZ=");
            Serial.println(modeDopplerVelocityZ);
            
            analogWrite(13, (int) accelRawAvgY - 150);
        } else {
            delay(1);
            modeDopplerCalibrationTimer--;
            
            Serial.print("modeDopplerCalibrationTimer=");
            Serial.println(modeDopplerCalibrationTimer);
            
            if (modeDopplerCalibrationTimer == 0) {
                Serial.println("MODE_DOPPLER CALIBRATING");
                
                modeDopplerVelocityX = 0.0f;
                modeDopplerVelocityY = 0.0f;
                modeDopplerVelocityZ = 0.0f;
                
                modeDopplerCalibrationLast = millis();
            }
        }
        
        modeDopplerTime = millis();
        break;
    }
    }
}
