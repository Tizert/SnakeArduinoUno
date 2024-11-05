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

void placeFood(void) {//генерация еды на поле
    for (elem *curr = head; curr != NULL;) {//проверка что еда не внутри тела змейки
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

void insInTail(elem *el) {//добавление сегмента в хвост списка (змейки)
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

void delTail(void) {//удаление сегмента из хвоста
    if (tail == NULL) {
        return;
    }
    else {
        tail              = tail->prevElement;
        tail->nextElement = NULL;
        size--;
    }
}

bool appleNotInHead(elem *head) {//проверка события что змейка съела яблоко и прирост тела
    if (head->x == food_x && head->y == food_y) { //если координаты головы и координаты еды совпадают
        if (size == 25) {//проверка на победу
            endGameCondition = 0;
            return true;
        }
        placeFood();//размещаем новую еду
    }
    return false;
}

void turnSnake() {//поворот головы змейки
    if (actualDirection == None) {
        return;
    }
    switch (headDirection) {//смена направления движения в зависимости от начального направления и положения соска
    //надо оптимизировать 100%
        case Direction::Up:
            if (actualDirection == Direction::Right){
                headDirection = Direction::Right;
            }
            if (actualDirection == Direction::Left){
                headDirection = Direction::Left;
            }
            break;
        case Direction::Right:
            if (actualDirection == Direction::Right){
                headDirection = Direction::Down;
            }
            if (actualDirection == Direction::Left){
                headDirection = Direction::Up;
            }
            break;
        case Direction::Down:
            if (actualDirection == Direction::Right){
                headDirection = Direction::Left;
            }
            if (actualDirection == Direction::Left){
                headDirection = Direction::Right;
            }
            break;
        case Direction::Left:
            if (actualDirection == Direction::Right){
                headDirection = Direction::Up;
            }
            if (actualDirection == Direction::Left){
                headDirection = Direction::Down;
            }
            break;
    }    
}

uint32_t endGameCondition = NULL;

void gameover(){
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
}

void restartGame(){
    //добавить перезапуск игрв по нажатию стика
}

void  moveSnake (Direction headDirection) {//движение змейки на один тик
        elem *tempHead = new elem;
        head->nextElement = tempHead;
        switch (headDirection) {
            case Direction::Right:
                tempHead->x = (head->x + 1) % COLUMNS;
                tempHead->y = head->y;
                break;
            case Direction::Down:
                tempHead->x = head->x;
                tempHead->y = (head->y + 1) % ROWS;
                break;
            case Direction::Left:
                tempHead->x = (head->x == 0 ? COLUMNS - 1 : head->x - 1);
                tempHead->y = head->y;
                break;
            case Direction::Up:
                tempHead->x = head->x;
                tempHead->y = (head->y == 0 ? ROWS - 1 : head->y -1);
                break;
            default://если каким-то образм смогли вывести направление в None, выдаем в консоль сообщение и геймовер
            my_printf("OMG! The Snake is lost outside the space!\r\n");
            endGameCondition = 2;
        }
        tempHead->nextElement = NULL;
        tempHead->prevElement = head;
        if (appleNotInHead(tempHead)) {
            delTail();
        }
        for (elem *curr = head; curr != NULL;) {//проверка на проигрыш
            if (curr->x == tempHead->x && curr->y == tempHead->y) {
                endGameCondition = 1;
                return;
            } 
        }
        head = tempHead;
        size++;
        return;
}

uint8_t d_columns[] = {LED_C1, LED_C2, LED_C3, LED_C4, LED_C5};
uint8_t d_rows[]    = {LED_R1, LED_R2, LED_R3, LED_R4, LED_R5};

const uint8_t MIDDLE_XY = 128;
const uint8_t OFFSET_XY = 48;
const uint8_t ROWS      = 5;
const uint8_t COLUMNS   = 5;
enum Direction { None, Up, Down, Left, Right };
uint8_t actualDirection = Direction::Right;
uint8_t headDirection = Direction::Right;

uint32_t start_a       = 0;
uint32_t start_b       = 0;
uint32_t start_end_game= 0;
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
        //возможно неверно
        //xDir = Direction::Up; //змейка умеет только поворачивать влево-вправо
        xDir = Direction::None;
    }
    if (x < MIDDLE_XY - OFFSET_XY) {
        //возможно неверно
        //xDir = Direction::Down; //змейка умеет только поворачивать влево-вправо
        xDir = Direction::None;
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

void lightSnakeAndFood(void) {
    for (elem *curr = head; curr!=NULL;){
        led_matrix(curr->x, curr->y);
        curr = curr->nextElement;
    }
    led_matrix(food_x, food_y);
}

void offLed(void) {
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
    if (endGameCondition == NULL) {
        if (millis() - start_a >= 100) {
            sosok_direction();
            start_a = millis();
        }

        if (millis() - start_b >= 200) {
            turnSnake();
            moveSnake(headDirection);
            start_b = millis();
        }
        
        lightSnakeAndFood();
    } else if (millis() - start_end_game >= 500) {
        if (size > 0) {
            delTail();
            start_end_game = millis();
        } else if (millis()/500%2) {
            if(endGameCondition == 0) {
                winned();//надо придумать как реализовать каждые полсекунды убывание змейки, а потом нарисовывание галочки победной из свтодиодов
            } else if (endGameCondition == 1) {
                gameover();//надо придумать как реализовать каждые полсекунды убывание змейки а потом нарисовывание проигрышного крестик
            } else {
                wtf();
                my_printf("Oops! Something went wrong!\r\n");
            }
            start_end_game = millis();    
        } else {
            offLed();
            start_end_game = millis();
        }
    }
}            