import processing.serial.*;

//selecteer hier de seriele poort:
int keuze_seriele_poort = 0;

Serial myPort;
int val = 0;
boolean overButtonR = false;
boolean overButtonG = false;
boolean overButtonB = false;
boolean overButtonRoff = false;
boolean overButtonGoff = false;
boolean overButtonBoff = false;

void setup() {
	size(190 , 150);
	//begin keuze seriele poort
	val = Serial.list().length;
	if (val == 0) {
		println("Error: Geen seriële poorten gevonden!");
		exit();

	} else if (keuze_seriele_poort >= val) {
		println("Error: keuze_seriele_poort is verkeerd ingesteld!");
		exit();

	} else if (val > 1) {
		println("Waarschuwing! Meer dan 1 seriële poort gevonden:");
		for(int i = 0; i < val; i++){
			print(i);
			print(": ");
			println(Serial.list()[i]);
		}
	}

	print("Seriële poort ");
	print(keuze_seriele_poort);
	print(" geselecteerd: ");
	println(Serial.list()[keuze_seriele_poort]);

	//Poort openen op Baud rate van 9600
	myPort = new Serial(this, Serial.list()[keuze_seriele_poort], 9600);

	textAlign(CENTER);
	background(200);

	fill(255,0,0);
	rect(10,10,50,50); //rood
	fill(0,255,0);
	rect(70,10,50,50); //groen
	fill(0,0,255);
	rect(130,10,50,50); //blauw

	fill(0);
	stroke(255, 0, 0);
	rect(10,70,50,50); //rood
	stroke(0,255,0);
	rect(70,70,50,50); //groen
	stroke(0,0,255);
	rect(130,70,50,50); //blauw
}

void mousePressed() {
	if (overButtonR) {
		myPort.write("r=1\n");
		return;
	}

	if (overButtonG) {
		myPort.write("g=1\n");
		return;
	}

	if (overButtonB) {
		myPort.write("b=1\n");
		return;
	}

	if (overButtonRoff) {
		myPort.write("r=0\n");
		return;
	}

	if (overButtonGoff) {
		myPort.write("g=0\n");
		return;
	}

	if (overButtonBoff) {
		myPort.write("b=0\n");
		return;
	}
}

void draw() {

}

void mouseMoved() {
	checkButtons();
}

void mouseDragged() {
	checkButtons();
}

void checkButtons() {
	print(mouseX);
	print(" ");
	println(mouseY);

	if (mouseX > 10 && mouseX < 60 && mouseY > 10 && mouseY < 60) {
		overButtonR = true;
		text("  Rood aan  ", 95, 140);
		return;
	} else {
		overButtonR = false;
	}

	if (mouseX > 70 && mouseX < 120 && mouseY > 10 && mouseY < 60) {
		overButtonG = true;
		text("  Groen aan  ", 95, 140);
		return;
	} else {
		overButtonG = false;
	}

	if (mouseX > 130 && mouseX < 180 && mouseY > 10 && mouseY < 60) {
		overButtonB = true;
		text("  Blauw aan  ", 95, 140);
		return;
	} else {
		overButtonB = false;
	}

	if (mouseX > 10 && mouseX < 60 && mouseY > 70 && mouseY < 120) {
		overButtonRoff = true;
		text("  Rood uit  ", 95, 140);
		return;
	} else {
		overButtonRoff = false;
	}

	if (mouseX > 70 && mouseX < 120 && mouseY > 70 && mouseY < 120) {
		overButtonGoff = true;
		text("  Groen uit  ", 95, 140);
		return;
	} else {
		overButtonGoff = false;
	}

	if (mouseX > 130 && mouseX < 180 && mouseY > 70 && mouseY < 120) {
		overButtonBoff = true;
		text("  Blauw uit  ", 95, 140);
		return;
	} else {
		overButtonBoff = false;
	}

	// Not over any button: cover text
	fill(200);
	noStroke();
	rect(60,130,70,20);
	stroke(0);
	fill(0);
}