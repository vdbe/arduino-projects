#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte IR_RECEIVE_PIN = 4;

void setup()
{
	Serial.begin(9600);
	Serial.println("IR Receive test");
	IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

	lcd.init();
	lcd.clear();
	lcd.backlight();
}

int remoteCode = 0x0;
int counter = 0;

void loop()
{
	if (IrReceiver.decode())
	{
		remoteCode = IrReceiver.decodedIRData.command;
		counter = 0;

		while (remoteCode == 0 && counter > 10)
		{
			delay(10);
			IrReceiver.decode();
			remoteCode = IrReceiver.decodedIRData.command;
			counter++;
		}

		Serial.println(remoteCode, HEX);
		lcd.setCursor(0, 0);
		switch (remoteCode)
		{
		case 0x40:
			lcd.print("OK ");
			break;
		case 0x46:
			lcd.print("^ ");
			break;
		case 0x44:
			lcd.print("<-");
			break;
		case 0x15:
			lcd.print("v ");
			break;
		case 0x43:
			lcd.print("->");
			break;
		case 0x16:
			lcd.print("1 ");
			break;
		case 0x19:
			lcd.print("2 ");
			break;
		case 0x0D:
			lcd.print("3 ");
			break;
		case 0x0C:
			lcd.print("4 ");
			break;
		case 0x18:
			lcd.print("5 ");
			break;
		case 0x5E:
			lcd.print("6 ");
			break;
		case 0x08:
			lcd.print("7 ");
			break;
		case 0x1C:
			lcd.print("8 ");
			break;
		case 0x5A:
			lcd.print("9 ");
			break;
		case 0x42:
			lcd.print("* ");
			break;
		case 0x52:
			lcd.print("0 ");
			break;
		case 0x4A:
			lcd.print("# ");
			break;
		default:
			break;
		}
		IrReceiver.resume();	// Enable receiving of the next value
	}
}