#include <Arduino.h>

// bool
// char

// int8_t
// int16_t
// int32_t
// int64_t

// uint8_t
// uint16_t
// uint32_t
// uint64_t

void my_printf(const char *format, ...) {
    const uint8_t MAX_STRING_SIZE = 64;
    char buf[MAX_STRING_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, MAX_STRING_SIZE, format, args);
    va_end(args);
    Serial.print(buf);
}

#define C1 8
#define C2 9
#define C3 2
#define C4 3

#define R1 4
#define R2 5
#define R3 6
#define R4 7

#define LED_C1 A2
#define LED_C2 A0
#define LED_C3 A1
#define LED_C4 12

#define LED_R1 11
#define LED_R2 A3
#define LED_R3 10
#define LED_R4 13

uint8_t columns[] = {C1, C2, C3, C4};
uint8_t rows[]    = {R1, R2, R3, R4};
uint8_t ass[16];

uint8_t d_columns[] = {LED_C1, LED_C2, LED_C3, LED_C4};
uint8_t d_rows[]    = {LED_R1, LED_R2, LED_R3, LED_R4};

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
}

void setup() {
    // C1 2
    // C2 3
    // C3 4
    // C4 5
    //
    // R1 6
    // R2 7
    // R3 8
    // R4 9
    // 3 A0
    // 4 A1
    // 8 10
    // 9 11
    // 10 12
    // 12 13
    // 13 A2
    // 14 A3

    pinMode(C1, INPUT);
    pinMode(C2, INPUT);
    pinMode(C3, INPUT);
    pinMode(C4, INPUT);

    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);

    pinMode(LED_C1, INPUT);
    pinMode(LED_C2, INPUT);
    pinMode(LED_C3, INPUT);
    pinMode(LED_C4, INPUT);

    pinMode(LED_R1, INPUT);
    pinMode(LED_R2, INPUT);
    pinMode(LED_R3, INPUT);
    pinMode(LED_R4, INPUT);

    Serial.begin(9600);
    Serial.println("Start code");
}

uint32_t start_b = 0;
uint32_t start_d = 0;

void loop() {
    // // put your main code here, to run repeatedly:
    // if (millis() - start_d >= 1000) {
    //     for (uint8_t col = 0; col < 4; col++) {
    //         pinMode(d_columns[col], OUTPUT);
    //         digitalWrite(d_columns[col], HIGH);
    //         for (uint8_t row = 0; row < 4; row++) {
    //             pinMode(d_rows[row], OUTPUT);
    //             digitalWrite(d_rows[row], LOW);
    //             delay(800);
    //             pinMode(d_rows[row], INPUT);
    //             ;
    //         }
    //         digitalWrite(d_columns[col], LOW);
    //         pinMode(d_columns[col], INPUT);
    //     }
    //     start_d = millis();
    // }

    // if (millis() - start_b >= 500) {
    //     my_printf("B");
    //     for (uint8_t row = 0; row < 4; row++) {
    //         pinMode(rows[row], INPUT_PULLUP);
    //         for (uint8_t col = 0; col < 4; col++) {
    //             pinMode(columns[col], OUTPUT);
    //             digitalWrite(columns[col], LOW);
    //             if (digitalRead(rows[row]) == 0) {
    //                 pinMode(d_columns[col], OUTPUT);
    //                 pinMode(d_rows[row], OUTPUT);
    //                 digitalWrite(d_rows[row], LOW);
    //                 digitalWrite(d_columns[col], HIGH);
    //             }
    //             pinMode(columns[col], INPUT);
    //             pinMode(d_columns[col], INPUT);
    //         }
    //         pinMode(rows[row], INPUT);
    //         pinMode(d_rows[row], INPUT);
    //     }
    //     start_b = millis();
    // }

    if (millis() - start_b >= 0) {
        for (uint8_t row = 0; row < 4; row++) {
            pinMode(rows[row], INPUT_PULLUP);
            for (uint8_t col = 0; col < 4; col++) {
                pinMode(columns[col], OUTPUT);
                digitalWrite(columns[col], LOW);
                if (digitalRead(rows[row]) == 0) {
                    // my_printf("Ass = S%d\r\n", row * 4 + col + 1);
                    led_matrix(row, col);
                }
                pinMode(columns[col], INPUT);
            }
            pinMode(rows[row], INPUT);
        }
        start_b = millis();
    }
}
