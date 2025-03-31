#include <Arduino.h>

#define FLOW_SENSOR_PIN 16 // Pin to send the signal to the sensor

volatile int pulseCount = 0;
float calibrationFactor = 450.0; // Pulses per liter 
unsigned long previousMillis = 0;
float flowRate;      // Liters per minute
float totalLiters = 0.0;

void IRAM_ATTR countPulses() {
  pulseCount++;
}

void setup() {
  Serial.begin(921600);
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), countPulses, RISING);

  Serial.println("Water Flow Meter Ready");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));

    // Calculate liters per minute (pulses / factor * 60)
    flowRate = ((float)pulseCount / calibrationFactor) * 60.0;

    // Calculate liters used in this second
    float litersThisSecond = (float)pulseCount / calibrationFactor;
    totalLiters += litersThisSecond;

    Serial.print("Flow Rate (L/min): ");
    Serial.print(flowRate);
    Serial.print(" | Total Water Used (L): ");
    Serial.println(totalLiters, 3); // Show with 3 decimals

    pulseCount = 0;
    previousMillis = currentMillis;

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), countPulses, RISING);
  }
}
