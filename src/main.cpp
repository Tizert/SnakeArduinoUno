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

struct elem {
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

uint32_t endGameCondition = 3;

enum Direction { None, Up, Down, Left, Right };
Direction requestedDirection = Direction::None;
Direction currentDirection = Direction::None;

const int MIDDLE_XY = 512;
const int OFFSET_XY = 200;

bool isFoodAt(const elem *el){//проверка что элемент на еде
    if (el == NULL) {
        return false;
    }
    return el->x == food_x && el->y == food_y;
}

bool isPositionOnSnake(uint8_t tempX, uint8_t tempY){//проверка что координаты находятся на теле змейки
    for (elem *curr = head; curr != NULL;){
        if (curr->x == tempX && curr->y == tempY){
            return true;
        }
        curr = curr->nextElement;
    }
    return false;
}

bool collidesWithSnake(const elem *el){//проверка что элемент совпадает с телом змейки
    if (el == NULL) {
        return false;
    }
    return isPositionOnSnake(el->x, el->y);
}

void insertHead(elem *newHead){//добавление тела змейки, в голову
    if (newHead == NULL){
        return;
    }
    if (head == NULL){
        newHead->nextElement = NULL;
        newHead->prevElement = NULL;
        head = newHead;
        tail = newHead;
        size++;
        return;
    }
    newHead->prevElement = NULL;
    newHead->nextElement = head;
    head->prevElement = newHead;
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

elem *createNextHead(Direction direction){//создание элемента-претендента на новую голову, присваивание им (x.y)
    if (head == NULL || direction == Direction::None){
        return NULL;
    }
    uint8_t newX = 0;
    uint8_t newY = 0;
    switch (direction)
    {
    case Direction::Right:
        newX = (head->x + 1) % COLUMNS;
        newY = head->y;
        break;
    case Direction::Down:
        newX = head->x;
        newY = (head->y + 1) % ROWS;
        break;
    case Direction::Left:
        newX = (head->x == 0 ? COLUMNS - 1 : head->x - 1);
        newY = head->y;
        break;
    case Direction::Up:
        newX = head->x;
        newY = (head->y == 0 ? ROWS - 1 : head->y -1);
        break;
    default:
        return NULL;
    }
    elem *newHead = new elem;
    newHead->x = newX;
    newHead->y = newY;
    return newHead;
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
    uint8_t candidateX = random(0,COLUMNS);
    uint8_t candidateY = random(0,ROWS);
    while (isPositionOnSnake(candidateX,candidateY)){
        candidateX = random(0,COLUMNS);
        candidateY = random(0,ROWS);
    }
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

void printSnake(){
    uint8_t count = 0;
        if (head != NULL) {
            Serial.print("HEAD = ");
            Serial.print(head->x);
            Serial.print(",");
            Serial.println(head->y);
        } else {
            Serial.println("HEAD = NULL");
        }

        if (tail != NULL) {
            Serial.print("TAIL = ");
            Serial.print(tail->x);
            Serial.print(",");
            Serial.println(tail->y);
        } else {
            Serial.println("TAIL = NULL");
        }
    for (elem *curr = head; curr != NULL && count < 20;){
        if (curr) {
            Serial.print("CURR = ");
            Serial.print(curr->x);
            Serial.print(",");
            Serial.println(curr->y);
        } else {
            Serial.println("CURR = NULL");
        }

        curr= curr->nextElement;
        count++;
    }
    my_printf("\r\n");
    if (count == 20){
        Serial.print("printSnake stopped: possible cycle\r\n");
    }
    return;
};

void moveSnake(Direction requested){

    Serial.println("=== moveSnake ===");
    Serial.print("size before = ");
    Serial.println(size);
    printSnake();

    Direction resolvedDirection = resolveDirection(currentDirection, requested);
    if (resolvedDirection == Direction::None) {
        return;
    }

    elem *newHead = createNextHead(resolvedDirection);

    Serial.print("newHead = ");
    Serial.print(newHead->x);
    Serial.print(",");
    Serial.println(newHead->y);

    if (newHead == NULL){
        Serial.print("Invalid head state");
        endGameCondition = 2;
        return;
    }

    if (!isValidBoardPosition(newHead->x,newHead->y)){
        Serial.print("Incorrect head position");
        endGameCondition = 2;
        delete newHead;
        newHead = NULL;
        return;
    }
    
    currentDirection = resolvedDirection;

    if (collidesWithSnake(newHead)){
        Serial.print("Game over");
        endGameCondition = 1;
        delete newHead;
        newHead = NULL;
        return;
    }
    bool ateFood = isFoodAt(newHead);

    Serial.print("ateFood = ");
    Serial.println(ateFood);

    insertHead(newHead);
    if (ateFood) {
        if (size == COLUMNS * ROWS){
            endGameCondition = 0;
            return;
        }
        placeFood();
    } else {
        removeTail();
    }

    Serial.print("size after = ");
    Serial.println(size);
    printSnake();
}

void placeSnake(void) {
my_printf("Start placeSnake\r\n");
    elem *snakeEl3 = new elem;
    elem *snakeEl2 = new elem;
    elem *snakeEl1 = new elem;
    snakeEl3->x    = 0; // хвост первоначальной змейки
    snakeEl3->y    = 0;    //
    snakeEl2->x    = 1; // тело первоначальной змейки
    snakeEl2->y    = 0;    //
    snakeEl1->x    = 2; // голова первоначальной змейки
    snakeEl1->y    = 0;    //
    insertHead(snakeEl3);
    insertHead(snakeEl2);
    insertHead(snakeEl1);
    currentDirection = Direction::Right;
    my_printf("printSnake in placeSnake");
    printSnake();
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

void gameover(){
    my_printf("Start gameover\r\n");
    elem *crossHead = new elem {0, 0};
    elem *cross1 = new elem {1, 1};
    elem *cross2 = new elem {2, 2};
    elem *cross3 = new elem {3, 3};
    elem *cross4 = new elem {4, 4};
    elem *cross5 = new elem {4, 0};
    elem *cross6 = new elem {3, 1};
    elem *cross7 = new elem {1, 3};
    elem *crossTail = new elem {0, 4};
    crossHead->nextElement = cross1;
    cross1->nextElement = cross2;
    cross2->nextElement = cross3;
    cross3->nextElement = cross4;
    cross4->nextElement = cross5;
    cross5->nextElement = cross6;
    cross6->nextElement = cross7;
    cross7->nextElement = crossTail;
    crossTail->nextElement = NULL;
    for (elem *curr = crossHead; curr !=NULL;) {
        led_matrix(curr->x, curr->y);
        curr = curr->nextElement;
    }
    my_printf("LOL, you died!\r\n");
}

void winned(){
    my_printf("Start winned\r\n");
    elem *successMarkHead = new elem {0, 3};
    elem *successMark1 = new elem {1, 4};
    elem *successMark2 = new elem {2, 3};
    elem *successMark3 = new elem {3, 2};
    elem *successMarkTail = new elem {4, 1};
    successMarkHead->nextElement = successMark1;
    successMark1->nextElement = successMark2;
    successMark2->nextElement = successMark3;
    successMark3->nextElement = successMarkTail;
    successMarkTail->nextElement = NULL;
    for (elem *curr = successMarkHead; curr !=NULL;) {
        led_matrix(curr->x, curr->y);
        curr = curr->nextElement;
    }
    //добавить счетчик очков и вывод
    my_printf("Winner! Congratulations!");
}
void wtf(){
    my_printf("Start wtf\r\n");
    elem *fuckedupHead = new elem {2, 0};
    elem *fuckedup1 = new elem {3, 1};
    elem *fuckedup2 = new elem {4, 2};
    elem *fuckedup3 = new elem {3, 3};
    elem *fuckedup4 = new elem {2, 4};
    elem *fuckedup5 = new elem {1, 3};
    elem *fuckedup6 = new elem {0, 2};
    elem *fuckedupTail = new elem {1, 1};
    fuckedupHead->nextElement = fuckedup1;
    fuckedup1->nextElement = fuckedup2;
    fuckedup2->nextElement = fuckedup3;
    fuckedup3->nextElement = fuckedup4;
    fuckedup4->nextElement = fuckedup5;
    fuckedup5->nextElement = fuckedup6;
    fuckedup6->nextElement = fuckedupTail;
    fuckedupTail->nextElement = NULL;
    for (elem *curr = fuckedupHead; curr !=NULL;) {
        led_matrix(curr->x, curr->y);
        curr = curr->nextElement;
    }
    my_printf("End wtf\r\n");
}

void restartGame(){
    my_printf("Start restartGame\r\n");
    //добавить перезапуск игрв по нажатию стика
    my_printf("End restartGame\r\n");
}

uint32_t start_a       = 0;
uint32_t start_b       = 0;
uint32_t start_end_game= 0;
uint8_t last_direction = 0;

uint8_t currRow = 0;
uint8_t currCol = 0;

void lightSnakeAndFood(void) {
    for (elem *curr = head; curr!=NULL;){
        led_matrix(curr->x, curr->y);
        curr = curr->nextElement;
    }
    led_matrix(food_x, food_y);
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
    if (endGameCondition == 3) {
        if (millis() - start_a >= 250) {
            requestedDirection = readJoystickDirection();
            start_a = millis();
        }

        if (millis() - start_b >= 500) {
            moveSnake(requestedDirection);
            start_b = millis();
        }
        
        lightSnakeAndFood();
    } else if (millis() - start_end_game >= 500) {
        if (size > 0) {
            removeTail();
            start_end_game = millis();
        } else if (millis()/500%2) {
            if(endGameCondition == 0) {
                winned();//надо придумать как реализовать каждые полсекунды убывание змейки, а потом нарисовывание галочки победной из свтодиодов
            } else if (endGameCondition == 1) {
                gameover();//надо придумать как реализовать каждые полсекунды убывание змейки а потом нарисовывание проигрышного крестик
            } else {
                wtf();
            }
            start_end_game = millis();    
        } else {
            offLed();
            start_end_game = millis();
        }
    }
}