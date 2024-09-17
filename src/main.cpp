#include <Arduino.h>
#include "state_machine.h"
#include "keypad.h"
#include "rotary_encoder.h"
#include "pin_defs.h"
#include "lcd.h"

void led_setup();
void button_setup();
void lcd_setup();
void check_buttons();
void check_states();
void check_pin();
void printEnteredPin();
void updateLCD();

void setup()
{
  led_setup();
  button_setup();
  //setup_encoder(RE_PIN_A, RE_PIN_B, RE_SW); // Wegen Pinmangel auskommentiert
  keypad_setup_pins();
  Serial.begin(115200); //115200
  lcd_setup();
}

int32_t last_encoder_state = 0;

void loop()
{
  check_buttons();
  check_states();
  /*int32_t encoder_state = get_encoder_state();
  if (encoder_state != last_encoder_state) {
    Serial.printf("Encoder state: %d \n", encoder_state);
    last_encoder_state = encoder_state;
  }*/
}

void led_setup()
{
  // Define all leds as output
  pinMode(LED_RED_LOCKED_PIN, OUTPUT);
  pinMode(LED_GREEN_LOCK_1_PIN, OUTPUT);
  pinMode(LED_GREEN_LOCK_2_PIN, OUTPUT);
  pinMode(LED_GREEN_LOCK_3_PIN, OUTPUT);
}

void button_setup()
{
  // Define all buttons as input. PULLUPs are used external
  pinMode(BTN_1_PIN, INPUT);
  pinMode(BTN_2_PIN, INPUT);
  pinMode(BTN_3_PIN, INPUT);
  pinMode(BTN_4_PIN, INPUT);
}

void lcd_setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Tresor bereit");
  delay(2000);
  lcd.clear();
}

void check_buttons()
{
  if ('A' == get_key())
  {
    state_machine(INPUT_1_ACCEPTED);
    Serial.println("Bitte PIN eingeben: ");
    lcd.clear();
    lcd.print("Bitte PIN eingeben: ");
  }
  if ('C' == get_key())
  {
    state_machine(INPUT_3_ACCEPTED);
    Serial.println("Tresor offen");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tresor offen");
    state_machine(OPEN_DOOR);
  }
  if ('D' == get_key())
  {
    state_machine(INPUT_REFUSED);
    state_machine(CLOSE_DOOR);
    Serial.println("Tresor gesperrt!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tresor gesperrt!");
  }
}

void check_states()
{
  switch (state)
  {
  case SAFE_LOCKED:
    digitalWrite(LED_RED_LOCKED_PIN, HIGH);
    digitalWrite(LED_GREEN_LOCK_1_PIN, LOW);
    digitalWrite(LED_GREEN_LOCK_2_PIN, LOW);
    digitalWrite(LED_GREEN_LOCK_3_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tresor gesperrt!");
    delay(500);
    break;
  case LEVEL_1_UNLOCKED:
    digitalWrite(LED_RED_LOCKED_PIN, LOW);
    digitalWrite(LED_GREEN_LOCK_1_PIN, HIGH);
    digitalWrite(LED_GREEN_LOCK_2_PIN, LOW);
    digitalWrite(LED_GREEN_LOCK_3_PIN, LOW);
    check_pin();
    /*if (char key = get_key())
      {
        Serial.printf("Keypad Key: %c \n", key);
      };*/
    break;
  case LEVEL_2_UNLOCKED:
    digitalWrite(LED_RED_LOCKED_PIN, LOW);
    digitalWrite(LED_GREEN_LOCK_1_PIN, HIGH);
    digitalWrite(LED_GREEN_LOCK_2_PIN, HIGH);
    digitalWrite(LED_GREEN_LOCK_3_PIN, LOW);
    break;
  case LEVEL_3_UNLOCKED:
    digitalWrite(LED_RED_LOCKED_PIN, LOW);
    digitalWrite(LED_GREEN_LOCK_1_PIN, HIGH);
    digitalWrite(LED_GREEN_LOCK_2_PIN, HIGH);
    digitalWrite(LED_GREEN_LOCK_3_PIN, HIGH);
    break;
  default:
    break;
  }
}
