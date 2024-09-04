#include <Arduino.h>
#include <cppQueue.h>

#define	IMPLEMENTATION	FIFO

#define LED_C1 2
#define LED_C2 4
#define LED_C3 6
#define LED_C4 8
#define LED_C5 10

#define LED_R1 3 
#define LED_R2 5
#define LED_R3 7
#define LED_R4 9 
#define LED_R5 11 

#define SWITCH 13
#define VRX A0
#define VRY A1

cppQueue	q_snake(25, 25, IMPLEMENTATION);

uint8_t d_columns[] = {LED_C1, LED_C2, LED_C3, LED_C4, LED_C5};
uint8_t d_rows[]    = {LED_R1, LED_R2, LED_R3, LED_R4, LED_R5};

const uint_fast8_t MIDDLE_XY = 128;
const uint_fast8_t OFFSET_XY = 48;
const uint8_t ROWS = 5;
const uint8_t COLUMNS = 5;
enum Direction {None, Up, Down, Left, Right};

uint32_t start_a = 0;
uint32_t start_b = 0;
uint8_t last_direction = 0;

uint8_t currRow = 0;
uint8_t currCol = 0;

void my_printf(const char *format, ...) {
    const uint8_t MAX_STRING_SIZE = 64;
    char buf[MAX_STRING_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, MAX_STRING_SIZE, format, args);
    va_end(args);
    Serial.print(buf);
    // my_printf("X axis is =%d Y axis is %d\r\n", xAxis, yAxis);
}

Direction determineDirection(int x, int y) {
    Direction xDir = Direction::None;
    Direction yDir = Direction::None;
    if (x > MIDDLE_XY + OFFSET_XY) {
        xDir = Direction::Up;
    }
    if (x < MIDDLE_XY - OFFSET_XY) {
        xDir = Direction::Down;
    }
    if (y > MIDDLE_XY + OFFSET_XY) {
        yDir = Direction::Right;
    }
    if (y < MIDDLE_XY - OFFSET_XY) {
        yDir = Direction::Left;
    }

    if (yDir == Direction::None) {
        return xDir;
    } else if (xDir == Direction::None) {
        return yDir;
    }

    return Direction::None;
}

void led_matrix(uint8_t r, uint8_t c) {
    pinMode(d_rows[r], OUTPUT);
    pinMode(d_columns[c], OUTPUT);
    digitalWrite(d_columns[c], HIGH);
    digitalWrite(d_rows[r], LOW);
    pinMode(d_rows[r], INPUT);
    pinMode(d_columns[c], INPUT);
}

void sosok_direction() {
    uint8_t xAxis = analogRead(VRX); 
    uint8_t yAxis = analogRead(VRY);
    xAxis = map(xAxis, 0, 169, 0, 255);
    yAxis = map(yAxis, 0, 169, 0, 255);

    uint8_t nowDirection = determineDirection(xAxis, yAxis); // Direction get

    if (last_direction != nowDirection){
        my_printf("X axis is =%d Y axis is %d\r\n", xAxis, yAxis);
        switch (nowDirection)
        {
        case Direction::Up :
            currRow = (currRow + 1) % ROWS;
            break;
        case Direction::Down :
            currRow = (currRow - 1) < 0 ? ROWS - 1 : currRow - 1;
            break;
        case Direction::Right :
            currCol = (currCol + 1) % COLUMNS;
            break;
        case Direction::Left :
            currCol = (currCol - 1) < 0 ? COLUMNS - 1 : currCol - 1;
            break;
        case Direction::None :
            break;
        }
        Serial.print(nowDirection);
        Serial.print("\n");
        last_direction = nowDirection;
    }   
}

void setup() {

    pinMode(LED_C1, INPUT);
    pinMode(LED_C2, INPUT);
    pinMode(LED_C3, INPUT);
    pinMode(LED_C4, INPUT);
    pinMode(LED_C5, INPUT);

    pinMode(LED_R1, INPUT);
    pinMode(LED_R2, INPUT);
    pinMode(LED_R3, INPUT);
    pinMode(LED_R4, INPUT);
    pinMode(LED_R5, INPUT);

    pinMode(SWITCH, INPUT_PULLUP);
    pinMode(VRX, INPUT);
    pinMode(VRY, INPUT);

    Serial.begin(9600);
    Serial.println("Start code block");
}

void loop() {
    if (millis() - start_a >= 99) {
        sosok_direction();
        start_a = millis();
    }

    led_matrix(currRow, currCol);

    if (millis() - start_b >= 1000) {
        start_b = millis();
    }
}