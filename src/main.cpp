#include <Arduino.h>

void my_printf(const char *format, ...) {
    const uint8_t MAX_STRING_SIZE = 64;
    char buf[MAX_STRING_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, MAX_STRING_SIZE, format, args);
    va_end(args);
    Serial.print(buf);
}

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

uint8_t d_columns[] = {LED_C1, LED_C2, LED_C3, LED_C4, LED_C5};
uint8_t d_rows[]    = {LED_R1, LED_R2, LED_R3, LED_R4, LED_R5};

void led_matrix(uint8_t r, uint8_t c) {
    static uint8_t prev_row = 0;
    static uint8_t prev_col = 0;
    pinMode(d_rows[prev_row], INPUT);
    pinMode(d_columns[prev_col], INPUT);

    pinMode(d_rows[r], OUTPUT);
    pinMode(d_columns[c], OUTPUT);
    digitalWrite(d_columns[c], HIGH);
    digitalWrite(d_rows[r], LOW);
    prev_row = r;
    prev_col = c;
    pinMode(d_rows[r], INPUT);
    pinMode(d_columns[c], INPUT);
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

uint32_t start_b = 0;
uint32_t start_d = 0;

void loop() {
    if (millis() - start_b >= 0) {
        uint8_t xAxis = analogRead(VRX); 
        uint8_t yAxis = analogRead(VRY);
        xAxis = map(xAxis, 0, 169, 0, 255);
        yAxis = map(yAxis, 0, 169, 0, 255);
        my_printf("X axis is =%d Y axis is %d\r\n", xAxis, yAxis);
        delay(100);
        start_b = millis();
    }
    // if (millis() - start_b >= 0) {
    //     for (uint8_t row = 0; row < 4; row++) {
    //         pinMode(rows[row], INPUT_PULLUP);
    //         for (uint8_t col = 0; col < 4; col++) {
    //             pinMode(columns[col], OUTPUT);
    //             digitalWrite(columns[col], LOW);
    //             if (digitalRead(rows[row]) == 0) {
    //                 // my_printf("Ass = S%d\r\n", row * 4 + col + 1);
    //                 led_matrix(row, col);
    //             }
    //             pinMode(columns[col], INPUT);
    //         }
    //         pinMode(rows[row], INPUT);
    //     }
    //     start_b = millis();
    // }
}
