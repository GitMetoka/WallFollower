#include <SoftwareSerial.h>
#include <Motor.h>
#include <NewPing.h>
#include <math.h>

// NewPing sonar(trigger_pin, echo_pin [, max_cm_distance]);
#define BAUDRATE 9600
#define TRIG 7
#define ECHO 6
#define MAX_DISTANCE 60

#define _DELAY 1000
#define LONG_DELAY 3000
#define ITERATION 2

#define POWER_PER_CM 2

motorPins pins = {4, 5, 2, 3};

int leftSpeed = 80; //150
int rightSpeed = 70; //133

Motor *motor;
NewPing *sonar;


const int setPoint = 13; //15cm

int currentDistance;

int difference(void){
	return (getDistance() - setPoint);
}

uint8_t absoluteDifference(void){
	return (abs(difference()));
}

enum STATE{
	SLIGHT_LEFT,
	SLIGHT_RIGHT,
	STRAIGHT
};

int getDirection(void){
	if (difference() > 0) return SLIGHT_RIGHT;
	else if (difference() < 0) return SLIGHT_LEFT;
	else if (difference() == 0) return STRAIGHT;
}

void setup()
{
	Serial.begin(BAUDRATE);
	motor = new Motor(pins);
	sonar = new NewPing(TRIG, ECHO, MAX_DISTANCE);
	delay(LONG_DELAY);
	currentDistance = getDistance();
}

int getDistance(void){
	return sonar->convert_cm(sonar->ping_median(ITERATION));
}

void loop()
{
	// Check Condition if necessary
	// if (getDirection() == SLIGHT_LEFT){
	// 	Serial.println("SLIGHT LEFT");
	// } else if (getDirection() == SLIGHT_RIGHT){
	// 	Serial.println("SLIGHT RIGHT");
	// } else {
	// 	Serial.println("STRAIGHT");
	// }

	// delay(_DELAY);

	if (getDirection() == STRAIGHT) {
		motor->go(leftSpeed, rightSpeed, FORWARD);
	} else if (getDirection() == SLIGHT_LEFT){

		int power = absoluteDifference() * POWER_PER_CM;
		motor->go(leftSpeed + power, rightSpeed - power, FORWARD);

	} else if (getDirection() == SLIGHT_RIGHT){


		int power = absoluteDifference() * POWER_PER_CM;
		motor->go(leftSpeed - power, rightSpeed + power, FORWARD);
	}

	//Speed test debugger
	// motor->go(leftSpeed, rightSpeed, FORWARD);
}
