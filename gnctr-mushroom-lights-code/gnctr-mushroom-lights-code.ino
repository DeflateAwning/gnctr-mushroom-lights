// Arduino Pro or Pro Mini
// ATmega168 (5V, 16 MHz)

// 5 and 9 are switch low
// 7 and 11 are input pull up

// random(10,20) returns number from 10 to 19

#define PIN_TOP 4 // active low (inverted)
#define PIN_STEM 13 // active high (normal)

#define PIN_SW_1 7
#define PIN_SW_2 11


void setup() {
	// init 
	pinMode(PIN_TOP, OUTPUT);
	pinMode(PIN_STEM, OUTPUT);

	// init switch ground pins
	pinMode(5, OUTPUT);
	pinMode(9, OUTPUT);
	digitalWrite(5, LOW);
	digitalWrite(9, LOW);

	// init switch input
	pinMode(PIN_SW_1, INPUT_PULLUP);
	pinMode(PIN_SW_2, INPUT_PULLUP);
	
	randomSeed(analogRead(4));

	Serial.begin(115200);
}

int mode = 0;
unsigned long mode_start_millis = millis();
unsigned long time_per_mode_millis = 600000; // 10 minutes
const int mode_count = 5;

void loop() {
	//long mode = (millis() >> 14) % mode_count; // shift 19 = 10 minutes; shift 14 = 30 seconds

	// check mode and progress
	if ((millis() - mode_start_millis) > time_per_mode_millis) {
		mode_start_millis = millis();
		mode = (mode + 1) % mode_count;

		// pulse lights to indicate change
		for (int i = 0; i < 3; i++) {
			light(PIN_TOP, 1);
			light(PIN_STEM, 0);
			delay(200);
			light(PIN_TOP, 0);
			light(PIN_STEM, 1);
			delay(200);
		}
	}

	// check button read (either of them pulled low by press?)
	if ((! digitalRead(PIN_SW_1)) || (! digitalRead(PIN_SW_2))) {
		mode_start_millis = millis();
		mode = (mode + 1) % mode_count;
		
		// pulse lights to indicate change
		for (int i = 0; i < 3; i++) {
			light(PIN_TOP, 1);
			light(PIN_STEM, 1);
			delay(100);
			light(PIN_TOP, 0);
			light(PIN_STEM, 0);
			delay(100);
		}
		delay(1000);
	}

	//mode = 2; // force a mode for debug

	if (mode == 0) {
		light(PIN_STEM, 1);
		light(PIN_TOP, 1);
		delay(50); // don't waste resources
	}
	else if (mode == 1) {
		loop_b();
	}
	else if (mode == 2) {
		loop_lightning();
	}
	else if (mode == 3) {
		loop_a();
	}
	else if (mode == 4) {
		loop_a_flipped();
	}

	//loop_lightning();
	//loop_gentlepulse();
	//loop_a();
	//loop_b();
}

void loop_gentlepulse() {
	// do a single pulse up, and a single pulse down
	// super flashy, don't use
	long time_up_ms = random(500, 3000);
	long time_down_ms = random(500, 3000);

	long amount_on_top;
	long amount_on_stem;

	bool is_top_on = false;
	bool is_stem_on = false;

	light(PIN_TOP, 0);
	light(PIN_STEM, 0);

	while(1) {
		for (long us_num = 0; us_num < 1000; us_num++) {
			amount_on_top = (millis() >> 3) % 1000;
			
			//Serial.print("amount_on_top=");
			//Serial.println(amount_on_top);

			if (us_num < amount_on_top) {
				if (! is_top_on) {
					light(PIN_TOP, 1);
					is_top_on = true;
				}
			}
			else {
				if (is_top_on) {
					light(PIN_TOP, 0);
					is_top_on = false;
				}
			}

			//delayMicroseconds(1);
		}
	}
	
}

void loop_lightning() {
	light(PIN_STEM, 0);
	light(PIN_TOP, 0);
	
	int r1 = random(1,4);
	int r2 = random(1,4);

	for (int j = 0; j < r2; j++) {
		for (int i = 0; i < r1; i++) {
			light(PIN_TOP, 1);
			rdelay(10,100);
			light(PIN_TOP, 0);
			rdelay(10,200);
		}

		//delay(random(0,200)); // lightning travels to stem

		for (int i = 0; i < r1; i++) {
			light(PIN_STEM, 1);
			rdelay(10,100);
			light(PIN_STEM, 0);
			rdelay(10,200);
		}
	}

	rdelay(500,4000);

}

void loop_basictest() {
	// for determining active-high vs. active-low
	digitalWrite(PIN_STEM, HIGH);
	digitalWrite(PIN_TOP, HIGH);
	delay(1000);

	digitalWrite(PIN_STEM, LOW);
	digitalWrite(PIN_TOP, LOW);
	delay(1);

	digitalWrite(PIN_STEM, HIGH);
	digitalWrite(PIN_TOP, HIGH);
	delay(1000);

	digitalWrite(PIN_STEM, LOW);
	digitalWrite(PIN_TOP, LOW);
	delay(1000);

	digitalWrite(PIN_STEM, HIGH);
	digitalWrite(PIN_TOP, HIGH);
	delay(1);

	digitalWrite(PIN_STEM, LOW);
	digitalWrite(PIN_TOP, LOW);
	delay(1000);
}


const long period_us = 10000; // time for each frac

void loop_a() {
	// gentle pulses on stem and top, variable pulse, the odd glitch though, and the stem sorta sucks
	double onfrac;
	long on_num;
	double max_brightness = 2000.0;
	int shift_amt = 0;

	long last_regen_rands_millis = millis();

	on_num = ((millis() >> shift_amt) % 2000);
	if (on_num > 1000) {
		on_num = 2000 - on_num; // fade down
	}
	if (on_num < 150) {
		on_num = 150; // limit to minimum so it doesn't dip out completely
	}

	if (millis() - last_regen_rands_millis > 2000) {
		max_brightness = random(1000,3000);
		shift_amt = random(0,4);
		last_regen_rands_millis = millis();

	}
	onfrac = on_num / max_brightness; // num from 0.0 to 0.3-1.0 (range of reasonable onfrac)

	light(PIN_TOP, 1);
	light(PIN_STEM, 1);
	delayMicroseconds(onfrac * period_us);

	light(PIN_TOP, 0);
	light(PIN_STEM, 0);
	delayMicroseconds((1-onfrac) * period_us);
}

void loop_a_flipped() {
	// gentle pulses on stem and top, variable pulse, the odd glitch though, and the stem sorta sucks
	double onfrac;
	long on_num;
	double max_brightness = 1000.0; //2000.0;
	int shift_amt = 0;

	long last_regen_rands_millis = millis();

	on_num = ((millis() >> shift_amt) % 2000);
	if (on_num > 1000) {
		on_num = 2000 - on_num; // fade down
	}
	if (on_num < 150) {
		on_num = 150; // 10% brightness is as low as it can go
	}

	if (millis() - last_regen_rands_millis > 2000) {
		//max_brightness = random(1000,3000);
		shift_amt = random(0,4);
		last_regen_rands_millis = millis();

	}
	onfrac = on_num / max_brightness; // num from 0.0 to 0.3-1.0 (range of reasonable onfrac)

	light(PIN_TOP, 0);
	light(PIN_STEM, 1);
	delayMicroseconds(onfrac * period_us);

	light(PIN_TOP, 1);
	light(PIN_STEM, 0);
	delayMicroseconds((1-onfrac) * period_us);
}

void loop_b() {
	light(PIN_STEM, 1); // just set it on, it doesn't PWM very well

	double peak_bright = random(50,100) / 100.0;
	double change_rate_up = random(1,5) / 1000.0; // 0.001 to 0.01
	double change_rate_down = random(1,5) / 1000.0; // 0.001 to 0.01

	for (double onfrac = 0; onfrac < peak_bright; onfrac += change_rate_up) {
		light(PIN_TOP, 1);
		delayMicroseconds(onfrac * period_us);

		light(PIN_TOP, 0);
		delayMicroseconds((1-onfrac) * period_us);

	}

	for (double onfrac = peak_bright; onfrac > 0; onfrac -= change_rate_down) {
		light(PIN_TOP, 1);
		delayMicroseconds(onfrac * period_us);

		light(PIN_TOP, 0);
		delayMicroseconds((1-onfrac) * period_us);

	}
}


/*
void loop_a_orig() {
	for (double onfrac = 0; onfrac < 1; onfrac += 0.01) {
		digitalWrite(13, HIGH);
		digitalWrite(4, HIGH);
		delay(onfrac * period);

		digitalWrite(13, LOW);
		digitalWrite(4, LOW);
		delay((1-onfrac) * period);

	}

	for (double onfrac = 1; onfrac > 0; onfrac -= 0.01) {
		digitalWrite(13, HIGH);
		digitalWrite(4, HIGH);
		delay(onfrac * period);

		digitalWrite(13, LOW);
		digitalWrite(4, LOW);
		delay((1-onfrac) * period);

	}
}*/

void light(int pin, int on_or_off) {
	if (pin == PIN_STEM) {
		digitalWrite(pin, on_or_off);
	}
	else if (pin == PIN_TOP) {
		digitalWrite(pin, ! on_or_off); // inverted
	}
}

void rdelay(long min_time_ms, long max_time_ms) {
	delay(random(min_time_ms, max_time_ms));
}