#include <LiquidCrystal_I2C.h>

// No idea how you get to 0x27 but it works
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned char i = 0;

void setup()
{
	lcd.init();
	lcd.backlight();

	lcd.setCursor(0, 0);
	lcd.print("Hello world!");
}


void loop()
{
	lcd.setCursor(0, 1);
	if (i == 10) {
		lcd.printstr("0 ");
		i = 1;
	}
	else {
		lcd.print(i++);
	}

	delay(1000);
}