#include <Arduino.h>

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
#define VRX    A0
#define VRY    A1
typedef struct {
    uint32_t x;
    uint32_t y;
    elem *nextElement;
    elem *prevElement;
} elem;

elem *head  = NULL;
elem *tail  = NULL;
size_t size = 0;

uint8_t food_x = random(0, COLUMNS);
uint8_t food_y = random(0, ROWS);

void placeSnake(void) {
    elem *snakeEl3 = new elem;
    elem *snakeEl2 = new elem;
    elem *snakeEl1 = new elem;
    snakeEl3->x    = d_columns[0]; // хвост первоначальной змейки
    snakeEl3->y    = d_rows[0];    //
    snakeEl2->x    = d_columns[1]; // тело первоначальной змейки
    snakeEl2->y    = d_rows[0];    //
    snakeEl1->x    = d_columns[2]; // голова первоначальной змейки
    snakeEl1->y    = d_rows[0];    //
    insInTail(snakeEl1);
    insInTail(snakeEl2);
    insInTail(snakeEl3);
}

void placeFood(void) {
    for (elem *curr = head; curr != NULL;) {
        if ((curr->x == food_x) && (curr->y == food_y)) {
            food_x = random(0, COLUMNS);
            food_y = random(0, ROWS);
            curr   = head;
            continue;
        }
        curr = curr->nextElement;
    }
    return;
}

void insInTail(elem *el) {
    if (el == NULL) {
        return;
    }
    if (head == NULL) {
        el->nextElement = NULL;
        el->prevElement = NULL;
        head            = el;
        tail            = el;
        size            = 1;
    }
    else {
        el->nextElement   = NULL;
        el->prevElement   = tail;
        tail->nextElement = el;
        tail              = el;
        size++;
    }
}
void delTail(void) {
    if (tail == NULL) {
        return;
    }
    else {
        tail              = tail->prevElement;
        tail->nextElement = NULL;
        size--;
    }
}
struct doublyLinkedList {};

void appleInHead() {}

uint8_t d_columns[] = {LED_C1, LED_C2, LED_C3, LED_C4, LED_C5};
uint8_t d_rows[]    = {LED_R1, LED_R2, LED_R3, LED_R4, LED_R5};

const uint8_t MIDDLE_XY = 128;
const uint8_t OFFSET_XY = 48;
const uint8_t ROWS      = 5;
const uint8_t COLUMNS   = 5;
enum Direction { None, Up, Down, Left, Right };
uint8_t actualDirection = Direction::Right;

uint32_t start_a       = 0;
uint32_t start_b       = 0;
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
        yDir = Direction::Left;
    }
    if (y < MIDDLE_XY - OFFSET_XY) {
        yDir = Direction::Right;
    }

    if (yDir == Direction::None) {
        return xDir;
    }
    else if (xDir == Direction::None) {
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
    xAxis         = map(xAxis, 0, 169, 0, 255);
    yAxis         = map(yAxis, 0, 169, 0, 255);

    uint8_t nowDirection = determineDirection(xAxis, yAxis); // Direction get
    if (nowDirection == Direction::None || actualDirection == nowDirection) {
        return;
    }

    my_printf("Changed direction from %d, to %d\r\n", actualDirection, nowDirection);
    actualDirection = nowDirection;
}

void light_snake_field() {
    switch (actualDirection) {
    case Direction::Right:
        currCol = (currCol + 1) % COLUMNS;
        break;
    case Direction::Left:
        currCol = currCol == 0 ? (COLUMNS - 1) : (currCol - 1);
        break;
    case Direction::Up:
        currRow = (currRow + 1) % ROWS;
        break;
    case Direction::Down:
        currRow = currRow == 0 ? (ROWS - 1) : (currRow - 1);
        break;
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

    placeSnake();
    placeFood();
}

void loop() {
    if (millis() - start_a >= 100) {
        sosok_direction();
        start_a = millis();
    }

    if (millis() - start_b >= 200) {
        light_snake_field();
        start_b = millis();
    }

    led_matrix(currRow, currCol);
}