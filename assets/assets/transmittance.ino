#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int sensorPin = A0;    // 포토다이오드 전압 입력
const int buttonPin = 2;     // 버튼 입력 핀

float V_ref = -1.0;          // 기준 전압
bool isRefSaved = false;     // 기준값 저장 여부

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD 주소

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Press button to");
  lcd.setCursor(0, 1);
  lcd.print("set reference");

  Serial.begin(9600);
}

void loop() {
  float voltage = analogRead(sensorPin) * (5.0 / 1023.0);

  // 기준값 저장이 안 되었을 때
  if (!isRefSaved) {
    if (digitalRead(buttonPin) == HIGH) {
      V_ref = voltage;
      isRefSaved = true;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ref Saved:");
      lcd.setCursor(0, 1);
      lcd.print(V_ref, 3); // 소수점 3자리까지 출력
      delay(1500);
      lcd.clear();
    }
  }

  // 기준값 저장 후 → 실시간 투과율 계산 & 출력
  if (isRefSaved) {
    float transmittance = (voltage / V_ref) * 100.0;

    lcd.setCursor(0, 0);
    lcd.print("Transmittance:  ");

    lcd.setCursor(0, 1);
    lcd.print(transmittance, 2);
    lcd.print(" %   ");

    // 시리얼로도 출력 (선택)
    Serial.print("Voltage: ");
    Serial.print(voltage, 4);
    Serial.print(" V | ");
    Serial.print("Trans: ");
    Serial.print(transmittance, 2);
    Serial.println(" %");
  }

  delay(300);  // 0.3초마다 측정
}
