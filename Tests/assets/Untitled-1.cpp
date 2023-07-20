/* 
    Работаем с каналами при помощи массивов! 
    Программный ШИМ, любое разрешение до 8 бит, 5 каналов 
    Выводим ШИМ на пины А0,A1,A2,А3,A4,A5 
    Значение заполнения берем из генератора случайных чисел 
*/ 
 
#include <GyverTimers.h>  // Либа прерываний по таймеру 
#include "FastIO.h"       // Либа быстрого ввода/вывода 

static volatile uint8_t duty1 = 0;
static volatile uint8_t duty2 = 0;
static volatile uint8_t duty3 = 0;
static volatile uint8_t duty4 = 0;
static volatile uint8_t duty5 = 0;
static volatile uint8_t duty6 = 0;

static volatile float currentTimeSec1 = 0.f;
static volatile float currentTimeSec2 = 0.f;
static volatile float currentTimeSec3 = 0.f;
static volatile float currentTimeSec4 = 0.f;
static volatile float currentTimeSec5 = 0.f;
static volatile float currentTimeSec6 = 0.f;

constexpr float PERIOD = 3.f;

constexpr byte output1 = 5;
constexpr byte output2 = 6;
constexpr byte output3 = 7;
constexpr byte output4 = 8;
constexpr byte output5 = 9;
constexpr byte output6 = 10;

constexpr byte input1 = A3;
constexpr byte input2 = A4;
constexpr byte input3 = A5;
constexpr byte input4 = A0;
constexpr byte input5 = A1;
constexpr byte input6 = A2;

float createRandomOffset() {

    constexpr auto prescale = 10.f;
    constexpr auto maxStartOffset = PERIOD * prescale;

    const auto randomRaw = random(maxStartOffset);

    return static_cast<float>(randomRaw) / prescale;

}

void setup() { 

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
    pinMode(pin5, OUTPUT);
    pinMode(pin6, OUTPUT);

    Timer2.setFrequency(40000);
    Timer2.enableISR();        

    currentTimeSec1 = random(createRandomOffset());
    currentTimeSec2 = random(createRandomOffset());
    currentTimeSec3 = random(createRandomOffset());
    currentTimeSec4 = random(createRandomOffset());
    currentTimeSec5 = random(createRandomOffset());
    currentTimeSec6 = random(createRandomOffset());

} 
 
uint8_t updateLED(uint8_t inDuty, float currentTimeSec) {

    const auto yOffset = 100.f;
    const auto period = 3.f;
    const auto time = currentTimeSec * 3.1416f / period;
    const auto rawSine = sinf(time);

    const auto result = absf(rawSine) * 255.f + yOffset;
    const auto resultClamped = min(255.f, result);

    return static_cast(resultClamped);

}

constexpr uint8_t PWM_MAX = 255;

bool getInMailInABox(byte pinId) {

    const auto value = analogRead(pinId);

    if (value > 700) 
        return true;

    return false;

}

void updateDuty() {

    constexpr float deltaTime = 0.001f;

    currentTimeSec1 += deltaTime;
    currentTimeSec2 += deltaTime;
    currentTimeSec3 += deltaTime;
    currentTimeSec4 += deltaTime;
    currentTimeSec5 += deltaTime;
    currentTimeSec6 += deltaTime;

    duty1 = updateLED(duty1, currentTimeSec1);
    duty2 = updateLED(duty2, currentTimeSec2);
    duty3 = updateLED(duty3, currentTimeSec3);
    duty4 = updateLED(duty4, currentTimeSec4);
    duty5 = updateLED(duty5, currentTimeSec5);
    duty6 = updateLED(duty6, currentTimeSec6);

    if (getInMailInABox(input1)) 
        duty1 = PWM_MAX;
    if (getInMailInABox(input2)) 
        duty2 = PWM_MAX;
    if (getInMailInABox(input3)) 
        duty3 = PWM_MAX;
    if (getInMailInABox(input4)) 
        duty4 = PWM_MAX;
    if (getInMailInABox(input5)) 
        duty5 = PWM_MAX;
    if (getInMailInABox(input6)) 
        duty6 = PWM_MAX;

}

void loop() { // Получаем заполнение ШИМ с генератора случайных чисел 
    updateDuty();
} 
 
void pwmTick() { 

  static volatile uint8_t counter = 0;
 
  if (conuter == 0) { 
    digitalWrite(output1, LOW);
    digitalWrite(output2, LOW);
    digitalWrite(output3, LOW);
    digitalWrite(output4, LOW);
    digitalWrite(output5, LOW);
    digitalWrite(output6, LOW);
  }

  if (counter == duty1) {
    digitalWrite(output1, HIGH);
  }

  if (counter == duty2) {
    digitalWrite(output2, HIGH);
  }

  if (counter == duty3) {
    digitalWrite(output3, HIGH);
  }

  if (counter == duty4) {
    digitalWrite(output4, HIGH);
  }

  if (counter == duty5) {
    digitalWrite(output5, HIGH);
  }

  if (counter == duty6) {
    digitalWrite(output6, HIGH);
  }

  counter++;

} 
 
ISR(TIMER2_A) { 
  //pwmTick();    // Тикер ШИМ в прерывании таймера (можно перенести код тикера сюда) 
}
