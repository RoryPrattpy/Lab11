/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/CTD_2023/Lab11/src/Lab11.ino"
#include "LIS3DH.h"
#include "oled-wing-adafruit.h"

void positionInterruptHandler();
void setup();
void loop();
void show_position(float x, float y, float z);
#line 4 "/Users/admin/CTD_2023/Lab11/src/Lab11.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;

LIS3DHSPI accel(SPI, D3, WKP);

const unsigned long PRINT_SAMPLE_PERIOD = 100;

void positionInterruptHandler() {};

unsigned long lastPrintSample = 0;

volatile bool positionInterrupt = true;
uint8_t lastPos = 0;

float divider;

float speedMultiplier;

int counter = 0;

int calibrateTimes = 20;

void setup() {
  display.setup();
  display.clearDisplay();
  display.display();

  Serial.begin(9600);
  //while (!Serial.isConnected()) {}

	attachInterrupt(WKP, positionInterruptHandler, RISING);

	// Initialize sensors
	LIS3DHConfig config;
	config.setPositionInterrupt(16);

	bool setupSuccess = accel.setup(config);
	Serial.printlnf("setupSuccess=%d", setupSuccess);
}

void loop() {
  display.loop();

  LIS3DHSample sample;

	if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD) {
		lastPrintSample = millis();
		if (accel.getSample(sample)) {
      if (counter < calibrateTimes) {
        divider += sample.z / calibrateTimes;
        counter += 1;
        speedMultiplier = 9.8/divider; // knowing the sensor is at rest i calculate that the z var should be 9.8 so I do this simple calculation to change the input to mps

        display.clearDisplay();
        display.setTextSize(1);
	      display.setTextColor(WHITE);
	      display.setCursor(0,0);
        display.println("Calibrating...");
        display.display();
      } else {
        show_position(sample.x, sample.y, sample.z);
      }
		}
	}
}

void show_position(float x, float y, float z) {
  display.clearDisplay();
  display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
  display.println("meters per second");
	display.print("x: ");
  display.print(x * speedMultiplier);
  display.println();
  display.print("y: ");
  display.print(y * speedMultiplier);
  display.println();
  display.print("z: ");
  display.print(z * speedMultiplier);
	display.display();
}