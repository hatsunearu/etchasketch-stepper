//up/right = true

//stepper motor pin defines
int ENABLE_PIN = 13;
int STEP1_PIN = 11;
int STEP2_PIN = 10;
int DIR1_PIN = 9;
int DIR2_PIN = 8;

//stepper motor prototypes
void stepper_init();
void stepper_enable();
void stepper_disable();
void step_ch1();
void step_ch2();
void setdir_ch1(boolean d);
void setdir_ch2(boolean d);
void stepper_up();
void stepper_down();
void stepper_right();
void stepper_left();

//instructions (generate from matlab)
int instr[] = {2,1,1,4,1,1,4,7,8,8,8,8,8,8,8,8,8,8,9,8,9,6,6,6,6,6,3,3,3,3,3,3,3,2,1,2,2,1,2,2,2,2,2,2,2,2,2,3,2,2,2,3,2,2,3,2,3,2,3,2,3,3,3,6,3,6,9,8,8,8,8,7,8,8,8,8,7,8,8,8,7,8,8,8,7,8,7,8,8,7,8,7,8,7,8,7,8,8,8,9,9,6,9,6,6,6,6,6,3,6,6,2,2,2,2,2,2,2,2,2,2,2,3,3,2,3,3,3,6,6,6,6,8,6,9,8,8,8,8,7,8,7,8,7,7,7,7,7,4,7,4,8,8,6,8,9,6,6,9,3,6,6,6,6,3,6,3,6,6,6,2,2,2,2,2,2,2,2,2,2,2,2,3,2,3,2,3,3,3,3,6,6,6,6,9,9,9,9,8,8,7,8,7,7,7,7,8,7,4,7,7,7,4,7,7,8,9,9,9,6,6,6,6,6,6,6,6,3,6,3,6,3,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,3,3,3,6,3,6,6,6,6,9,9,9,8,8,7,8,8,7,8,7,8,7,4,8,4,7,7,7,7,4,4,8,9,9,6,9,6,6,6,6,6,6,3,6,6,6,6,6};
int instrlen = 295;

//step configuration
int stepdelay = 100;

//misc global variable
long lasttime = 0;
int instri = 0;

//when etchisketch is running
byte status = 0; //0: waiting for start, 1: running, 2: finished

void setup() {
  stepper_init();
  Serial.begin(9600);
  setdir_ch1(false);
  setdir_ch2(false);
  stepper_enable();
}

void loop() {
  switch(status){
		//listen for start
		case 0:
			/*BeagleBone to Arduino Protocol
			0: Run
			*/
			if(Serial.available() > '0'){
				byte in = Serial.read();
				switch(in){
					case 0://Run
						running = true;
						break;
				}
			}
			break;
		//run etchisketch
		case 1:
			if (running && lasttime + stepdelay < millis() && status == 0) {
				switch(instr[instri]) {
					
					/*
						1 2 3
						4 5 6
						7 8 9
					*/
					
					case 1:
						stepper_up();
						stepper_left();
						break;
					case 2:
						stepper_up();
						break;
					case 3:
						stepper_right();
						stepper_up();
						break;
					case 4:
						stepper_left();
						break;
					case 6:
						stepper_right();
						break;
					case 7:
						stepper_down();
						stepper_left();
						break;
					case 8:
						stepper_down();
						break;
					case 9:
						stepper_down();
						stepper_right();
						break;
					case 0:
						stepdelay = -1;
						stepper_disable();
						break;
					default:
						break;
					
				}
				instri++;
				//ending situation
				if (instri >= instrlen){
					status=2;
					Serial.write(0);//send finished signal to beagle board
				}
				
				lasttime = millis();
			}
			break;
		case 2:
			//finished
			break;
	}

void stepper_init() {
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(STEP1_PIN, OUTPUT);
  pinMode(STEP2_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  stepper_disable();
  setdir_ch1(true);
  setdir_ch2(true);
}


//begin stepper motor functions
void stepper_enable() {
  digitalWrite(ENABLE_PIN, LOW);
}

void stepper_disable() {
  digitalWrite(ENABLE_PIN, HIGH);
}

void step_ch1() {
  digitalWrite(STEP1_PIN, HIGH);
  digitalWrite(STEP1_PIN, LOW); 
}

void step_ch2() {
  digitalWrite(STEP2_PIN, HIGH);
  digitalWrite(STEP2_PIN, LOW);
}

void setdir_ch1(boolean d) {
  if (d) {
    digitalWrite(DIR1_PIN, HIGH);
    digitalWrite(DIR1_PIN, LOW);
  }
  else {
    digitalWrite(DIR1_PIN, LOW);
    digitalWrite(DIR1_PIN, HIGH);
  }
}

void setdir_ch2(boolean d) {
  if (d) {
    digitalWrite(DIR2_PIN, HIGH);
    digitalWrite(DIR2_PIN, LOW);
  }
  else {
    digitalWrite(DIR2_PIN, LOW);
    digitalWrite(DIR2_PIN, HIGH);
  }
}

void stepper_up() {
   setdir_ch1(true);
   step_ch1();
}
void stepper_down() {
   setdir_ch1(false);
   step_ch1();
}
void stepper_right() {
   setdir_ch2(true);
   step_ch2();
}
void stepper_left() {
   setdir_ch2(false);
   step_ch2();
}