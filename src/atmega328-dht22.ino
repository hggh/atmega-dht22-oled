#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <JeeLib.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SENSOR_PIN 2
#define DHTTYPE DHT22


DHT dht(SENSOR_PIN, DHTTYPE);

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
  // disable ADC
  ADCSRA = 0;
  power_adc_disable();
  power_spi_disable();
  power_usart0_disable();
  power_timer0_enable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_enable();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin();
  display_values(99.0,13.3); 
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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    return;
  }

  display_values(humidity, temperature);
  
  Sleepy::loseSomeTime(60000);
}
