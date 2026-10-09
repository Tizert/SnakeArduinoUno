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

struct elem {//class
    uint8_t x;
    uint8_t y;
    elem *nextElement;
    elem *prevElement;
};

elem *head  = NULL;
elem *tail  = NULL;
size_t size = 0;

const uint8_t ROWS      = 5;
const uint8_t COLUMNS   = 5;

uint8_t food_x = random(0, COLUMNS);
uint8_t food_y = random(0, ROWS);

uint8_t d_columns[] = {LED_C1, LED_C2, LED_C3, LED_C4, LED_C5};
uint8_t d_rows[]    = {LED_R1, LED_R2, LED_R3, LED_R4, LED_R5};

enum class Direction : uint8_t { None, Up, Down, Left, Right };
Direction requestedDirection = Direction::None;
Direction currentDirection = Direction::None;

enum class GameState : uint8_t { WaitingStart, Running, Lose, Win, Error };
GameState gameState = GameState::WaitingStart;

uint8_t flashCount = 0;
boolean ledOn = false;

uint32_t timerGameState = 0; 
uint32_t timerInput= 0;
uint32_t timerMovement= 0;

const int MIDDLE_XY = 512;
const int OFFSET_XY = 200;

bool isFoodAt(uint8_t tempX, uint8_t tempY){//проверка что элемент на еде
    return tempX == food_x && tempY == food_y;
}
bool isTailAt(uint8_t tempX, uint8_t tempY){
    return tempX == tail->x && tempY == tail->y;
}

bool isPositionOnSnake(uint8_t tempX, uint8_t tempY){//проверка что координаты находятся на теле змейки
    for (elem *curr = head; curr != NULL;curr = curr->nextElement){
        if (curr->x == tempX && curr->y == tempY){
            return true;
        }
    }
    return false;
}

void replaceTailToHead(uint8_t tempX, uint8_t tempY){
    elem *oldTail = tail;
    tail = oldTail->prevElement;
    tail->nextElement = NULL;
    oldTail->prevElement = NULL;
    oldTail->nextElement = head;
    head->prevElement = oldTail;
    head = oldTail;
    head->x = tempX;
    head->y = tempY;
    return;
}

void insertHead(uint8_t tempX, uint8_t tempY){//добавление тела змейки, в голову
    if (head == NULL){
        head = new elem;
        head->nextElement = NULL;
        head->prevElement = NULL;
        head->x = tempX;
        head->y = tempY;
        tail = head;
        size++;
        return;
    }
    elem *newHead = new elem;
    newHead->nextElement = head;
    head->prevElement = newHead;
    newHead->prevElement = NULL;
    newHead->x = tempX;
    newHead->y = tempY;
    head = newHead;
    size++;
}

void removeTail(){//удалить последний элемент змейки, хвост
    if (tail == NULL){
        return;
    }
    if (size == 1){
        delete head;
        head = NULL;
        tail = NULL;
        size = 0;
        return;
    }
    elem *oldTail = tail;
    tail = tail->prevElement;
    tail->nextElement = NULL;
    size--;
    delete oldTail;
    oldTail = NULL;
}

Direction readJoystickDirection(){
    int xAxis = analogRead(VRX);
    int yAxis = analogRead(VRY);
    int16_t dX = abs(xAxis-MIDDLE_XY);
    int16_t dY = abs(yAxis-MIDDLE_XY);
    if (dX <= OFFSET_XY && dY <= OFFSET_XY){
        return Direction::None;
    }
    if (dX >= dY) {
        if ((xAxis > MIDDLE_XY + OFFSET_XY)){
            return Direction::Down;
        } else {
            return Direction::Up;
        }
    } else {
        if ((yAxis > MIDDLE_XY + OFFSET_XY)){
            return Direction::Right;
        } else {
            return Direction::Left;
        }
    }
    Serial.println("Error: Can't read Direction");
    return Direction::None;
}

    void createNextHeadCoordinates(Direction direction, uint8_t &tempX, uint8_t &tempY){
    //создание претендента на новую голову, присваивание (x.y)
    if (head == NULL || direction == Direction::None){
        return;
    }
    tempX = head->x;
    tempY = head->y;
    switch (direction)
    {
    case Direction::Right:
        tempX = (head->x + 1) % COLUMNS;
        break;
    case Direction::Down:
        tempY = (head->y + 1) % ROWS;
        break;
    case Direction::Left:
        tempX = (head->x == 0 ? COLUMNS - 1 : head->x - 1);
        break;
    case Direction::Up:
        tempY = (head->y == 0 ? ROWS - 1 : head->y -1);
        break;
    default:
        return;
    }
    return;
}

bool isOppositeDirection(Direction current, Direction requested){//определяется разворот на 180
    if (current == Direction::Left){
        return requested == Direction::Right;
    }
    if (current == Direction::Right){
        return requested == Direction::Left;
    }
    if (current == Direction::Up){
        return requested == Direction::Down;
    }
    if (current == Direction::Down){
        return requested == Direction::Up;
    }
    return false;
}

Direction resolveDirection(Direction current, Direction requested){//обработка изменения направления
    if (requested == Direction::None || current == requested || isOppositeDirection(current, requested)){
        return current;
    }
    return requested;
}

bool isValidBoardPosition(uint8_t tX, uint8_t tY){
    return tX < COLUMNS && tY < ROWS;
}


void placeFood(){//определение новых координат для еды
    Serial.print("Start placeFood");
    if (size >= COLUMNS * ROWS) {
        return;
    }
    uint8_t candidateX;
    uint8_t candidateY;
    do{
        candidateX = random(0,COLUMNS);
        candidateY = random(0,ROWS);
    }while (isPositionOnSnake(candidateX,candidateY));

    food_x = candidateX;
    food_y = candidateY;

    Serial.print("Food placed: ");
    Serial.print(food_x);
    Serial.print(",");
    Serial.println(food_y);
    
    return;
}

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

void offLed(void) {
    my_printf("Start offLed\r\n");
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
    my_printf("End offLed\r\n");
}

void enterGameState(GameState state){
    flashCount = 0;
    ledOn = false;
    timerGameState = millis();
    gameState = state;
}

void moveSnake(Direction requested){

    Serial.println("=== moveSnake ===");
    Direction resolvedDirection = resolveDirection(currentDirection, requested);
    if (resolvedDirection == Direction::None) {
        return;
    }
    uint8_t newX = 0;
    uint8_t newY = 0;
    createNextHeadCoordinates(resolvedDirection, newX, newY);

    if (!isValidBoardPosition(newX,newY)){
        Serial.print("Incorrect head position");
        offLed();
        enterGameState(GameState::Error);
        return;
    }
    
    currentDirection = resolvedDirection;

    bool ateFood = isFoodAt(newX, newY);
    bool onTail = isTailAt(newX, newY);

    if (isPositionOnSnake(newX, newY) && (ateFood || !onTail)) {
        enterGameState(GameState::Lose);
        return;
    }

    if (ateFood) {
        if (size + 1 == COLUMNS * ROWS){
            enterGameState(GameState::Win);
            return;
        }
        insertHead(newX, newY);
        placeFood();
        return;
    } else {
        replaceTailToHead(newX, newY);
    }
}

void placeSnake(void)
{
    my_printf("Start placeSnake\r\n");
    for (int i = 0; i < 3; i++)
    {
        insertHead(i,0);
    }
    my_printf("End placeSnake\r\n");
}


void led_matrix(uint8_t c, uint8_t r) {
    pinMode(d_rows[r], OUTPUT);
    pinMode(d_columns[c], OUTPUT);
    digitalWrite(d_columns[c], HIGH);
    digitalWrite(d_rows[r], LOW);
    pinMode(d_rows[r], INPUT);
    pinMode(d_columns[c], INPUT);
}

bool isRestartPressed(){
    if (digitalRead(SWITCH) == LOW){
        return true;
    }
    return false;
}

void clearSnake(){
    while (tail != NULL)
    {
        removeTail();
        if (tail)
            delay(300);
    }
}

void restartGame(){
    clearSnake();
    currentDirection = Direction::None;
    requestedDirection = Direction::None;
    placeSnake();
    placeFood();
    uint32_t now = millis();
    timerGameState = now;
    timerInput= now;
    timerMovement= now;
    gameState = GameState::WaitingStart;
}

void lightSnakeAndFood(void) {
    for (elem *curr = head; curr!=NULL;){
        led_matrix(curr->x, curr->y);
        curr = curr->nextElement;
    }
    led_matrix(food_x, food_y);
}

void drawLoseCross(void){
    for (uint8_t x = 0; x < COLUMNS; x++){
        for (uint8_t y = 0; y < ROWS; y++){
            if (x == y || x + y == COLUMNS - 1) {
                led_matrix(x, y);
            }
        }
    }
}

void drawWinCheckmark (void) {
    for (uint8_t i = 0; i < 4; i++){
        led_matrix(i,i);
    }
    led_matrix(2,4);
}

void drawErrorE (void){
    for (uint8_t i = 0; i < COLUMNS; i++){
        led_matrix(4, i);
    }
    led_matrix(3, 0);
    led_matrix(2, 0);
    led_matrix(3, 2);
    led_matrix(2, 2);
    led_matrix(3, 4);
    led_matrix(2, 4);
}

void handleWaitingStart(){
    lightSnakeAndFood();
    uint32_t now = millis();
    if (now - timerGameState < 250) {
        return;
    }
    requestedDirection = readJoystickDirection();
    if (requestedDirection == Direction::None) {
            return;
    } 
    currentDirection = requestedDirection;
    gameState = GameState::Running;
    timerInput= now;
    timerMovement= now;
    return;
}

void handleRunning(){
    lightSnakeAndFood();
    uint32_t now = millis();
    if (now - timerInput >= 100) {
        Direction tempRequestedDirection = readJoystickDirection();
        if (tempRequestedDirection != Direction::None) {
            requestedDirection = tempRequestedDirection;
        }
        timerInput = now;
    }
    if (now - timerMovement >= 500) {
        moveSnake(requestedDirection);
        timerMovement = now;

        if (gameState != GameState::Running){
            return;
        }
    }
}

void handleEndGame(void drawPicture()){
    if (size > 0){
        lightSnakeAndFood();
    } else {
        if (ledOn == true) {
            drawPicture();
        }
    }
    uint32_t now = millis();
    if (flashCount > 3) {
        return;
    }
    if (now - timerGameState >= 500) {
        if (size > 0) {
            removeTail();
            offLed();
        } else {
            if (ledOn == true){
                offLed();
                ledOn = false;
                flashCount++;
            } else {
                ledOn = true;
            }
        }
        timerGameState = now;
    }
}

void handleError(){
    if (ledOn == true) {
        drawErrorE();
    }
    uint32_t now = millis();
    if (flashCount > 3) {
        return;
    }
    if (now - timerGameState >= 500) {
        if (ledOn == true){
                offLed();
                ledOn = false;
                flashCount++;
            } else {
                ledOn = true;
            }
        timerGameState = now;
    }
}

void setup() {
    offLed();

    pinMode(SWITCH, INPUT_PULLUP);
    pinMode(VRX, INPUT);
    pinMode(VRY, INPUT);

    Serial.begin(9600);
    Serial.println("Start code block");

    placeSnake();
    placeFood();
}

void loop() {
    if (isRestartPressed()){
        restartGame();
    }
    switch (gameState)
    {
    case GameState::WaitingStart:
        handleWaitingStart();
        break;
    case GameState::Running:
        handleRunning();
        break;
    case GameState::Lose:
        handleEndGame(drawLoseCross);
        break;
    case GameState::Win:
        handleEndGame(drawWinCheckmark);
        break;
    case GameState::Error:
        handleError();
        break;
    default:
        break;
    }
}
