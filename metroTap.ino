// simple sketch to try averaging a tapped BPM using arduino

#include "Bounce.h"

const int pinTap = 2;
const int pinBlink = 13;

int numTaps = 0;
int lastTapTime = 0;
bool firstTap = true;

const int avgSize = 10;
float avgStore[avgSize] = {0};
int avgPos = 0;
bool avgReady = false;

Bounce tapIn = Bounce(pinTap, 5); // 5ms debounce

void setup() {
	Serial.begin(115200);
	pinMode(pinTap, INPUT);
	digitalWrite(pinTap, HIGH);
	pinMode(pinBlink, OUTPUT);
	Serial.println("Ready to go!");
}

void loop() {
	tapIn.update();
	if (tapIn.fallingEdge()) {
		numTaps++;
		Serial.print("Taps: ");
		Serial.println(numTaps);
		updateTempo();
		if (avgReady) {
			Serial.print("\n\n");
			Serial.print("Average BPM: ");
			Serial.println(getAvg());
		}
	}
}

void updateTempo() {
	if (firstTap) {
		lastTapTime = millis();
		firstTap = false;
	} else {
		int tapMillisDelta = millis() - lastTapTime;
		if (tapMillisDelta < 100) {
			Serial.println("Invalid tap!\n");
		} else {
			Serial.print("Time since last tap: ");
			Serial.println(tapMillisDelta);
			lastTapTime = millis();
			float bpm = 60000 / tapMillisDelta;
			Serial.print("BPM: ");
			Serial.println(bpm);
			storeAvg(bpm);
		}
	}
}

void storeAvg(float bpm) {
	avgStore[avgPos] = bpm;
	avgPos++;
	if (avgPos == avgSize) {
		avgPos = 0;
		avgReady = true;
	}
}

float getAvg() {
	float avgTotal = 0;
	for (int i = 0; i < avgSize; i++) {
		avgTotal += avgStore[i];
	}
	return avgTotal / avgSize;
}