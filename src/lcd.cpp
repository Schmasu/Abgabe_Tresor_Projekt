#include "lcd.h"
#include "keypad.h"
#include "state_machine.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin für das Keypad
const char correct_pin[] = "2580";
char entered_pin[5];
int pin_index = 0;
bool showPin = false;

void check_pin()
{

  char key = get_key();

  if (key == '#'){      // Bestätigung des eingegebenen PIN
    entered_pin[pin_index] = '\0';
    if (pin_index < sizeof(entered_pin)){
      entered_pin[pin_index] = '\0';
    }
    else{
      entered_pin[sizeof(entered_pin) - 1] = '\0';
    }
    if (strcmp(entered_pin, correct_pin) == 0){     // Wenn PIN korrekt eingegeben wurde...
      Serial.print("PIN korrekt!");
      Serial.print(" Level 2 unlocked \n");
      lcd.clear();
      lcd.print("PIN Korrekt");
      lcd.setCursor(0, 1);
      lcd.print("Level 2 unlocked");
      delay(3000);
      lcd.clear();
      state_machine(INPUT_2_ACCEPTED);
    }
    else{ 	    // Wenn der PIN nicht korrektt eingegeben wurde...
      Serial.print("Falscher PIN!");
      Serial.print(" Zugang verweigert. \n");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Falscher PIN!");
      lcd.setCursor(0, 1);
      lcd.print("Zugang verweigert.");
      delay(3000);
      lcd.clear();
      state_machine(INPUT_REFUSED);
    }
    pin_index = 0;
    }

  else if(key == '*' && pin_index > 0){
    showPin = !showPin;
    updateLCD();
  }

  else if (pin_index < 4 && key >= '0' && key <= '9'){      // PIN Eingabe und Ausgabe des aktuell eingegebenen PIN
    entered_pin[pin_index] = key;
    pin_index++;
    //printEnteredPin();
    updateLCD();
  }
  else if (key == '*'){     // Löschen der aktuellen Eingabe
    pin_index = 0;
    Serial.println("Eingabe geloescht");
    Serial.print(" Bitte PIN eingeben: ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Eingabe geloescht");
    lcd.setCursor(0, 1);
    lcd.print("Bitte PIN eingeben: ");
  }
}

/*void printEnteredPin(){
  // Serielle Ausgabe des PIN
  Serial.print("Eingegebener PIN: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Eingeg. PIN:");
  lcd.setCursor(0, 1);
    for (int i = 0; i < pin_index; i++){
      Serial.print(entered_pin[i]);
      lcd.print(entered_pin[i]);
    }
    Serial.println();
}*/

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Eingegebener PIN: ");
  lcd.setCursor(0, 1);
  if(showPin){
    lcd.print(entered_pin);     // Tatsächlichen PIN anzeigen
  }
  else{
    for(int i = 0; i < pin_index; i++){
      lcd.print('*');     // PIN als '*' anzeigen
    }
  }
}