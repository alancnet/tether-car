#include <Servo.h>
Servo throttle;

int led = 13;
int calibSwitch = 4;
int button = 5;
int signalPin = 3;
int buzzer = 2;
int full = 180;
int brake = 0;
int neutral = 90;
float currentThrottle = neutral / (full - brake);
float currentMin;
float currentMax;
long currentDur;


int dip1 = 7;
int dip2 = 8;
int dip3 = 9;
int dip4 = 10;
int dip5 = 11;
int dip6 = 12;

float minOptions[4] = {
	.1,
	.2,
	.3,
	.4
};
float maxOptions[4] = {
	.5,
	.75,
	.9,
	1
};
long durOptions[4] = {
	5000,
	10000,
	15000,
	20000
};

void setup()
{

	pinMode(calibSwitch, INPUT);
	pinMode(button, INPUT);
	pinMode(led, OUTPUT);
	pinMode(buzzer, OUTPUT);


	throttle.attach(signalPin);

}

void loop()
{


	if (digitalRead(calibSwitch) != LOW) {
		calibrationSound();
		calibrate();
	}
	else {
		readySound();
		waitForButton();
		config();
		startSound();
		race();
	}
	while (true) {
		// Halt
		delay(1000);
	}
	/* add main program code here */
	while (false) {
		//analogWrite(3, 0);
		//delay(500);
		//analogWrite(3, 255);
		//delay(500);
		for (int i = 0; i < 180; i += 1) {
			throttle.write(i);
			analogWrite(13, i);
			delay(10);
		}
		for (int i = 0; i < 180; i+= 1) {
			throttle.write(180 - i);
			analogWrite(13, 180 - i);
			delay(10);
		}
		
		
	}

}

void readySound() {
	int lowFreq = (1024 + 256) * 2;
	tone(buzzer, lowFreq, 125);
	delay(250);
	tone(buzzer, lowFreq, 125);
	delay(250);
}

void fullPowerSound() {
	int lowFreq = (1024 + 256) * 2;
	tone(buzzer, lowFreq, 2000);
	delay(2000);
}

void startSound() {
	int lowFreq = (1024 + 256) * 2;
	int lowDuration = 250;
	int interval = 1000;
	int highFreq = lowFreq / 2;

	for (int i = 0; i < 3; i++) {
		tone(buzzer, lowFreq, lowDuration);
		delay(lowDuration);
		delay(interval - lowDuration);
	}

	tone(buzzer, highFreq, interval);
}

void calibrationSound() {
	int lowFreq = 1024;
	int lowDuration = 125;
	int interval = 250;
	int highFreq = 2048;

	for (int i = 0; i < 4; i++) {
		tone(buzzer, lowFreq, lowDuration);
		delay(lowDuration);
		delay(interval - lowDuration);

		tone(buzzer, highFreq, lowDuration);
		delay(lowDuration);
		delay(interval - lowDuration);
	}
}


void blink(int count) {
	int low = 50;
	int high = 255;
	analogWrite(led, 0);
	delay(500);
	analogWrite(led, low);
	delay(500);
	for (int i = 0; i < count; i++) {
		analogWrite(led, low);
		delay(133);
		analogWrite(led, high);
		delay(133);
		analogWrite(led, low);
		delay(133);
	}
	delay(500);
	analogWrite(led, 0);
}

void waitForButton() {
	while (digitalRead(button) == LOW) {}
	delay(100);
}

void calibrate() {
	// Calibration
	throttle.write(full); // Wide open

	waitForButton();
	blink(1);

	throttle.write(brake); // Full brake

	waitForButton();
	blink(2);

	throttle.write(neutral);
		
	// Calibration complete
}

void race() {
	rampThrottle(currentMin, 0);
	rampThrottle(currentMax, currentDur);
	fullPowerSound();
	delay(120000);
	blink(10);
	rampThrottle(0, 1000);
	blink(1000000);
}

void rampThrottle(float target, long duration) {
	long start = millis();
	long now;
	float elapsed;
	float progress;
	float fDuration = duration;
	float newThrottle;
	float delta = target - currentThrottle;
	do {
		now = millis();
		elapsed = now - start;
		progress = elapsed / fDuration;
		newThrottle = currentThrottle + (delta * progress);
		setThrottle(newThrottle);
	} while (now < start + duration);
	setThrottle(target);
	currentThrottle = target;
}

void setThrottle(float newThrottle) {
	analogWrite(led, 255 * newThrottle);
	throttle.write(neutral + ((full - neutral) * newThrottle));
}

void config() {
	int iMin = digitalRead(dip1) * 2 + digitalRead(dip2);
	int iMax = digitalRead(dip3) * 2 + digitalRead(dip4);
	int iDur = digitalRead(dip5) * 2 + digitalRead(dip6);
	currentMin = minOptions[iMin];
	currentMax = maxOptions[iMax];
	currentDur = durOptions[iDur];

	//blink(iMin);
	//blink(iMax);
	//blink(iDur);
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    //TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}