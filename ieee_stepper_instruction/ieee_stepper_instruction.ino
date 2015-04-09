//communication codes
#define STEPPER_START 0x61
#define STEPPER_ABORT 0x62
#define STEPPER_PAUSE 0x63
#define STEPPER_RESUME 0x64

//pin definitions
#define ENABLE_PIN 13
#define STEP1_PIN 7
#define STEP2_PIN 4
#define DIR1_PIN 9
#define DIR2_PIN 8
#define PWM1_PIN 3 //pin assignments to the on-board headers
#define PWM2_PIN 5
#define PWM3_PIN 6

//Defines stepper motor related methods
#define STEP_DELAY 100

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

boolean stepper_enabled = false;

//misc global variable
long lasttime = 0; //last time an operation has occured. May need to be changed to last time of stepper operation 
int instri = 0; //index of current instruction

void setup() {
  stepper_init();
  Serial.begin(9600); //TODO change Serial to SoftwareSerial on different pin
  setdir_ch1(false);
  setdir_ch2(false);
  stepper_disable();
}

void loop() {
  
  if (Serial.available() > 0) {
    char msg = Serial.read();
    
    switch (msg) {
      //Stepper motor control messages
      case STEPPER_START:
        stepper_enabled = true;
        stepper_enable();
        instri = 0;
        break;
      case STEPPER_ABORT:
        stepper_enabled = false;
        stepper_disable();
        instri = 0;
        break;
      case STEPPER_PAUSE:
        stepper_enabled = false;
        stepper_disable();
        break;
      case STEPPER_RESUME:
        instri = 0;
        stepper_enable();
        break;
      default:
        break;
      
    }
     
  }
  
  
  if (lasttime + STEP_DELAY < millis() && stepper_enabled) {
    
    switch(instr[instri]) {
      
      /*
		the codes are shown by the following:
        1 2 3
        4 5 6
        7 8 9
		8 is directly down, 2 is directly up, and so on
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
        stepper_disable();
        break;
      default:
        break;
      
    }
    instri++;
    if (instri >= instrlen) { //if out of bounds
      stepper_enabled = false;
      instri = 0;
      stepper_disable(); //stop everything
    }

    lasttime = millis(); //update last time the stepper motor has ran
  }
  
}

//begin stepper motor code (this should be in a seperate file)
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
