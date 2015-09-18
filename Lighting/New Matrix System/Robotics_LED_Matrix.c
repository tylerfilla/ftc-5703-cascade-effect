
#include <Adafruit_NeoPixel.h>

#include "font.c"
#include "Robotics_LED_Matrix.h"

Adafruit_NeoPixel led = Adafruit_NeoPixel(LED_COUNT, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

int controlDebounce = millis();
int controlLast     = 0;
int controlState    = 0;

display_t display1;
display_t display2;

int mode = MODE_TEAM_NUMBER;

void setup() {
    Serial.begin(115200);
    led.begin();
    
    pinMode(PIN_CONTROL_1, INPUT_PULLUP);
    pinMode(PIN_CONTROL_2, INPUT_PULLUP);
    pinMode(PIN_STATUS,    OUTPUT);
    
    memset(&display1, 0, sizeof(display_t));
    display1.height = LED_MATRIX_HEIGHT;
    display1.width  = LED_MATRIX_WIDTH;
    
    memset(&display2, 0, sizeof(display_t));
    display2.height = LED_MATRIX_HEIGHT;
    display2.width  = LED_MATRIX_WIDTH;
}

void loop() {
    int control = digitalRead(PIN_CONTROL_1);
    
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

void displayDrawLine(display_t* display, int x1, int y1, int x2, int y2, unsigned char cR, unsigned char cG, unsigned char cB) {
    int cX = min(x1, x2);
    int cY = min(y1, y2);
    
    int dX = abs(x2 - x1);
    int dY = abs(y2 - y1);
    
    int error = 0;
    
    if (dX != 0) {
        for (cX = min(x1, x2); cX <= max(x1, x2); cX++) {
            displaySetPixel(display, cX, cY, cR, cG, cB);
            
            error += dY;
            if (error << 1 >= dX) {
                cY++;
                error -= dX;
            }
        }
    } else {
        for (int y = min(y1, y2); y <= max(y1, y2); y++) {
            displaySetPixel(display, x1, y, cR, cG, cB);
        }
    }
}

void displayDrawRect(display_t* display, int x1, int y1, int x2, int y2, boolean fill, unsigned char cR, unsigned char cG, unsigned char cB) {
    if (fill) {
        for (int x = x1; x <= x2; x++) {
            for (int y = y1; y <= y2; y++) {
                displaySetPixel(display, x, y, cR, cG, cB);
            }
        }
    } else {
        displayDrawLine(display, x1, y1, x2, y1, cR, cG, cB);
        displayDrawLine(display, x2, y1, x2, y2, cR, cG, cB);
        displayDrawLine(display, x2, y2, x1, y2, cR, cG, cB);
        displayDrawLine(display, x1, y2, x1, y1, cR, cG, cB);
    }
}

void displayDrawText(display_t* display, int x, int y, char* text, unsigned char cR, unsigned char cG, unsigned char cB) {
    for (int ic = 0; ic < strlen(text); ic++) {
        unsigned char bitmap[3];
        memcpy(bitmap, displayFont[text[ic]], 3);
        
        for (int column = 0; column < 3; column++) {
            char columnData = bitmap[column];
            
            for (int row = 0; row < 5; row++) {
                if ((columnData >> row) & 0x1) {
                    displaySetPixel(display, x + column + 4*ic, y + row, cR, cG, cB);
                }
            }
        }
    }
}

void displaySetPixel(display_t* display, int x, int y, unsigned char cR, unsigned char cG, unsigned char cB) {
    display->buffer[x][y][0] = cR;
    display->buffer[x][y][1] = cG;
    display->buffer[x][y][2] = cB;
}

void displayUpdate(display_t* display, unsigned char exteriorR, unsigned char exteriorG, unsigned char exteriorB) {
    for (int i = 0; i < LED_COUNT; i++) {
        led.setPixelColor(i, 0);
    }
    
    for (int x = display->posX; x < display->posX + display->width; x++) {
        for (int y = display->posY; y < display->posY + display->height; y++) {
            int ix = x - display->posX;
            int iy = y - display->posY;
            
            if (y%2) {
                ix = LED_MATRIX_WIDTH - x - 1 + display->posX;
            }
            
            if (x >= 0 && x < DISPLAY_BUFFER_WIDTH && y >= 0 && y < DISPLAY_BUFFER_HEIGHT) {
                led.setPixelColor(ix + iy*LED_MATRIX_WIDTH, display->buffer[x][y][0], display->buffer[x][y][1], display->buffer[x][y][2]);
            } else {
                led.setPixelColor(ix + iy*LED_MATRIX_WIDTH, exteriorR, exteriorG, exteriorB);
            }
        }
    }
    
    led.show();
}

void displayUpdate(display_t* display) {
    displayUpdate(display, 0, 0, 0);
}

void modeControl(int control) {
    Serial.print("MODE_CONTROL MODE ");
    Serial.print(mode);
    Serial.print(" CONTROL ");
    Serial.print(control);
    Serial.println();
    
    switch (mode) {
    case MODE_TEAM_NUMBER:
        modeTeamNumber(control);
        break;
    }
}

void modeTeamNumber(int control) {
    switch (control) {
    case MODE_CONTROL_SETUP:
        displayDrawText(&display1, 0, 0, "5 0", 0, 0, 255);
        displayDrawText(&display1, 0, 0, " 7 3", 255, 0, 0);
        displayUpdate(&display1);
        
        displayDrawText(&display2, 0, 0, "5 0", 0, 0, 255);
        displayDrawText(&display2, 0, 0, " 7 3", 255, 0, 0);
        displayUpdate(&display2);
        break;
    case MODE_CONTROL_UPDATE:
        break;
    }
}
