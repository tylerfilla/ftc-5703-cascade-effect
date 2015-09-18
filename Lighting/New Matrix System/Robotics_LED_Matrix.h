
/* Definitions */

#define LED_COUNT         60
#define LED_MATRIX_HEIGHT 5
#define LED_MATRIX_WIDTH  12

#define DISPLAY_BUFFER_HEIGHT LED_MATRIX_HEIGHT
#define DISPLAY_BUFFER_WIDTH  LED_MATRIX_WIDTH

#define MODE_CONTROL_SETUP  0
#define MODE_CONTROL_UPDATE 1

#define MODE_COUNT 1

#define MODE_TEAM_NUMBER 1

#define PIN_CONTROL_1    4
#define PIN_CONTROL_2    7
#define PIN_LED_STRIP    2
#define PIN_STATUS       13

/* Types */

typedef struct {
    // Internal buffer (allows off-screen drawing)
    unsigned char buffer[DISPLAY_BUFFER_WIDTH][DISPLAY_BUFFER_HEIGHT][3];
    
    // Window position (within buffer)
    int posX;
    int posY;
    
    // Dimensions
    int height;
    int width;
} display_t;

/* Prototyping */

// Arduino
void setup();
void loop();

// Display
void displayDrawLine(display_t* display, int x1, int y1, int x2, int y2, unsigned char cR, unsigned char cG, unsigned char cB);
void displayDrawRect(display_t* display, int x1, int y1, int x2, int y2, boolean fill, unsigned char cR, unsigned char cG, unsigned char cB);
void displayDrawText(display_t* display, int x, int y, char* text, unsigned char cR, unsigned char cG, unsigned char cB);
void displaySetPixel(display_t* display, int x, int y, unsigned char cR, unsigned char cG, unsigned char cB);
void displayUpdate(display_t* display, unsigned char exteriorR, unsigned char exteriorG, unsigned char exteriorB);
void displayUpdate(display_t* display);

// Mode
void modeControl(int control);
void modeTeamNumber(int control);
