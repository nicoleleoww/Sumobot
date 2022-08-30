/**
 * Basic Sumo Bot
 * 
 * This is written assuming:
 *      - The ring is black tape on a white surface
 *      - Ultrasonic is on the front of the bot
 *      - IR sensor is on the back of the bot
 */

///////////////////////
//// Defining pins ////
///////////////////////

// Motors
#define LEFT_F  6
#define LEFT_R  5
#define RIGHT_F 10
#define RIGHT_R 9

#define ENA 7
#define ENB 8

// Sensors
#define US_TRIG 11
#define US_ECHO 12
#define IR      A0

#define MAX_SPEED 255
#define DETECTION_DISTANCE 80
#define WHITE 1
#define BLACK 0

////////////////////////
//// Main Functions ////
////////////////////////

void setup(){
    Serial.begin(9600);
    Serial.println("Setting pins for motors");
    pinMode(LEFT_F, OUTPUT);
    pinMode(LEFT_R, OUTPUT);
    pinMode(RIGHT_R, OUTPUT);
    pinMode(RIGHT_R, OUTPUT);

    Serial.println("Setting pins for sensors");
    pinMode(US_TRIG, OUTPUT);
    pinMode(US_ECHO, INPUT);
    pinMode(IR, INPUT);
}

void loop(){
    // First make sure we're not getting pushed out
    if (get_colour() == BLACK){
        Serial.println("Found the edge, going forward");
        forward(MAX_SPEED);
        return;
    }

    // Rotate until target is found
    if (get_distance() > DETECTION_DISTANCE) {
        Serial.println("Haven't found a target, spinning");
        c_rotate(MAX_SPEED);
    } else {
        Serial.println("Found a target, going forward");
        forward(MAX_SPEED);
    }
}

/////////////////////////////
//// Basic Bot Movements ////
/////////////////////////////

/** Function: forward
 * Sends equal power to both motors in the forward direction.
 * 
 * int speed: pwm value to specify the speed (0-255)
 */
void forward(int speed){
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    analogWrite(RIGHT_F, speed);
    analogWrite(RIGHT_R, 0);
    analogWrite(LEFT_F, speed);
    analogWrite(LEFT_R, 0);
}

/** Function: reverse
 * Sends equal power to both motors in the reverse direction.
 * 
 * int speed: pwm value to specify the speed (0-255)
 */
void reverse(int speed){
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    analogWrite(RIGHT_F, 0);
    analogWrite(RIGHT_R, speed);
    analogWrite(LEFT_F, 0);
    analogWrite(LEFT_R, speed);
}

/** Function: c_rotate
 * Sends equal power to both motors in opposite directions to rotate clockwise.
 * 
 * int speed: pwm value to specify the speed (0-255)
 */
void c_rotate(int speed){
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    analogWrite(RIGHT_F, 0);
    analogWrite(LEFT_F, speed);
    analogWrite(RIGHT_R, speed);
    analogWrite(LEFT_R, 0);
}

/** Function: cc_rotate
 * Sends equal power to both motors in opposite directions to rotate
 * counter-clockwise.
 * 
 * int speed: pwm value to specify the speed (0-255)
 */
void cc_rotate(int speed)
{
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    analogWrite(RIGHT_F, speed);
    analogWrite(LEFT_F, 0);
    analogWrite(RIGHT_R, 0);
    analogWrite(LEFT_R, speed);
}
//////////////////////////////////
//// Sensor Reading Functions ////
//////////////////////////////////

/** Function: get_distance
 * Returns the distance from the ultrasonic sensor in centimetres
 */
int get_distance(){
    digitalWrite(US_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(US_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_TRIG, LOW);

    long duration = pulseIn(US_ECHO, HIGH);
    int distance = (duration * 0.034) / 2;
    return distance;
}

/**Function: get_colour
 * Returns the colour in front of the infrared sensor, as either black or white
 */
int get_colour(){
    if (analogRead(IR) < 650)
        return WHITE;
    return BLACK;
}