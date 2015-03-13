
#define PIN_CONTROL   4
#define PIN_LED_STRIP 2
#define PIN_STATUS    13

#define LED_COUNT 120

#define MODE_CONTROL_SETUP  0
#define MODE_CONTROL_UPDATE 1

#define MODE_COUNT 6

#define MODE_SOLID_RED  1
#define MODE_SOLID_BLUE 2
#define MODE_FLASH_SLOW 3
#define MODE_FLASH_FAST 4
#define MODE_CROSSFADE  5
#define MODE_WARP       6
#define MODE_DOPPLER    7

#define MODE_FLASH_SLOW_DELAY 1000
#define MODE_FLASH_FAST_DELAY 200

#define MODE_CROSSFADE_DELAY 2
#define MODE_CROSSFADE_PAUSE 1000

#define MODE_WARP_DELAY       5
#define MODE_WARP_WAVELENGTHS 2

#define MODE_DOPPLER_ACCEL_RAW_AVG_SIZE 64
#define MODE_DOPPLER_CALIBRATION_PERIOD 64
#define MODE_DOPPLER_MAX_VELOCITY       0.25f

void setup();
void loop();

void modeControl(int control);
void modeSolidRed(int control);
void modeSolidBlue(int control);
void modeFlashSlow(int control);
void modeFlashFast(int control);
void modeCrossfade(int control);
void modeWarp(int control);
void modeDoppler(int control);
