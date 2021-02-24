import processing.serial.*;

// Selecteer hier de seriele poort:
int keuze_seriele_poort = 0;

int width = 800;
int height = 600;

Serial myPort;
int val = 0;

void setup() {
	size(800, 600);
	// Begin keuze seriele poort
	val = Serial.list().length;

	if(val == 0) {
		println("Error: Geen seriële poorten gevonden!");
		exit();

	} else if (keuze_seriele_poort >= val) {
		println("Error: keuze_seriele_poort is verkeerd ingesteld!");
		exit();

	} else if (val > 1) {
		println("Waarschuwing! Meer dan 1 seriële poort gevonden:");

		for (int i = 0; i < val; i++) {
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

	background(200);
	ellipseMode(CENTER);
	fill(255);
	ellipse( width/2, height/2, 200, 200);
	textSize(12);
	fill(0);
	textAlign(CENTER);
	text("laat de data maar komen", width/2, height/2);
}

void draw() {	// Vergelijkbaar met loop in Arduino IDE
	if (myPort.available() == 0) {	// Kijken of er data is op de seriële poort zoniet return
		delay(50);	// No need to check every ms
		return;
	}

	val = myPort.read();	// Data lezen van de seriele poort

	switch (val) {
		case '1':
			option1();
			break;
		case '2':
			option2();
			break;
		case '3':
			option3();
			break;
	}
}

void option1() {
	float x = random(width);	//<>//
	float y = random(height);
	float r = random(50)+10;
	fill( random(255), random(255), random(255));
	ellipse( x, y, r, r);

	x = random(width);	//<>//
	y = random(height);
	float e = random(50)+10;
	fill( random(255), random(255), random(255));
	square(x, y, e);
}

void option2() {
	background(200);
}

void option3() {
	String rarara = "frrnq8--bsaibsaiem,amk-";   // "encrypted" string	python: <encrypted_string> =  "".join(chr(ord(c)-2) for c in <string>)
	String linkje = "";

	for(int i = 0; i< rarara.length(); i++) {
		char c = rarara.charAt(i);
		c += 2;
		linkje += c;
	}
	//link(linkje);
	println(linkje);
}