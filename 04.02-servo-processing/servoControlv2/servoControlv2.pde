import processing.serial.*;

//!!! selecteer hier de seriele poort:
int keuze_seriele_poort = 0;

//!!! Deze lib moet je installeren via de menu Schets -> Bibliotheek importeren... ->
// Bibliotheek toevoegen -> zoeken achter controlP5 en dan installeren.
// informatie over deze bibliotheek: http://www.sojamo.de/libraries/controlP5/
import controlP5.*;	// import controlP5 library

ControlP5 cp5;	// controlP5 GUI object
int servoAngle = 0;
boolean directSend = false;
Serial myPort;

void setup() {
	int val;
	size(400,130);
	noStroke();
	cp5 = new ControlP5(this);

	cp5.addToggle("direct_send")
		.setPosition(50,70)
		.setSize(20,20)
		.getCaptionLabel().set("direct send");


	cp5.addSlider("angle")
		.setBroadcast(false)	// Events tijdelijk uitschakelen
		.setPosition(50,30)
		.setSize(200,20)
		.setRange(0,180)
		.setNumberOfTickMarks(5)
		.snapToTickMarks(false)
		.setBroadcast(true);	// Terug events toelaten


	cp5.addButton("send")
		 .setBroadcast(false)	// Events tijdelijk uitschakelen
		 .setValue(0)
		 .setPosition(300,30)
		 .setSize(50,20)
		 .setBroadcast(true);	// Terug events toelaten

	// begin keuze seriele poort
	val = Serial.list().length;
	if (val == 0){
		println("Error: Geen seriële poorten gevonden!");
		exit();
		return;
	}

	if (keuze_seriele_poort >= val)
	{
		println("Error: keuze_seriele_poort is verkeerd ingesteld!");
		exit();
		return;
	}

	if(val > 1)
	{
		println("Waarschuwing! Meer dan 1 seriële poort gevonden:");
		for(int i = 0; i < val; i++)
		{
			print(i);
			print(": ");
			println(Serial.list()[i]);
		}
	}

	print("Seriële poort ");
	print(keuze_seriele_poort);
	print(" geselecteerd: ");
	println(Serial.list()[keuze_seriele_poort]);

	// Poort openen op Baud rate van 9600
	myPort = new Serial(this, Serial.list()[keuze_seriele_poort], 9600);
}


void draw()
{
	background(100, 100, 100);
}

void send()
{
	println("Send servo angle: " + servoAngle);
	myPort.write(servoAngle + "\n");
}

void direct_send(boolean value)
{
	directSend = value;
}

void angle(int angle)
{
	println("Changed servo angle: " + angle);
	servoAngle = angle;

	if(directSend)
		myPort.write(servoAngle + "\n");
}
