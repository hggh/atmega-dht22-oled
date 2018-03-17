#include <avr/wdt.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SENSOR_PIN 2
#define DHTTYPE DHT22
#define IS471_PIN 3
#define DISPLAY_POWER_PIN A3


DHT dht(SENSOR_PIN, DHTTYPE);
float temperature = 99.0;
float humidity = 99.0;
volatile unsigned short timer_runs;
volatile unsigned short timer_status;

ISR(TIMER1_OVF_vect) {
  TCNT1 = 0;
  timer_runs++;

  if (timer_runs > 5) {
    timer_status = 100;

    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    sei();
  }
}

void is471_interrupt() {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << TOIE1);
  sei();
  timer_runs = 0;
  timer_status = 1;
}


void setup() {
  pinMode(DISPLAY_POWER_PIN, OUTPUT);
  digitalWrite(DISPLAY_POWER_PIN, LOW);

  pinMode(IS471_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(IS471_PIN), is471_interrupt, FALLING);
  
  dht.begin();
  timer_status = 0;
}


void display_values(float h, float t) {
  String humidity_s = String(int(h));
  String temperature_s = String(int(t));
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  display.setTextSize(1);

  display.setCursor(10, 0);
  display.print("C");
  display.setCursor(15, 0);
  display.drawCircle(20, 2, 1, WHITE);
  display.setCursor(95, 0);
  display.print("rh %");

  // line between description and values
  display.drawLine(0,7,128,7, WHITE);


  display.setTextSize(3);
  display.setCursor(0,10);
  display.print(temperature_s);

  display.drawCircle(temperature_s.length() * 19, 14, 3, WHITE);
  display.setTextSize(2.5);
  display.setCursor(temperature_s.length() * 19, 17);
  display.print('c');


  display.setTextSize(2.5);
  display.setCursor(89, 15);
  display.print(humidity_s);
  display.setTextSize(1);
  display.setCursor(89 + humidity_s.length() * 15, 22);
  display.print('%');

  display.display();
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    return;
  }

  if (timer_status == 1) {
    digitalWrite(DISPLAY_POWER_PIN, HIGH);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display_values(humidity, temperature);

    timer_status = 10;
  }
  if (timer_status == 10) {
    display_values(humidity, temperature);
  }
  if (timer_status == 100) {
    display.clearDisplay();
    display.display();
    digitalWrite(DISPLAY_POWER_PIN, LOW);

    timer_status = 0;
  }

  delay(1000); 
}
