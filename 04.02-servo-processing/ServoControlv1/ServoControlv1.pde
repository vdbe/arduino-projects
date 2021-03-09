import processing.serial.*;

// Selecteer hier de seriele poort:
int keuze_seriele_poort = 0;

Serial myPort;
int val;

PFont f;
color rectHighlight;
color rectColor;
boolean rectOver1 = false;
boolean rectOver2 = false;
boolean rectOver3 = false;

void setup()
{
	size(600, 300);
	rectColor = color(255, 0, 0);
	rectHighlight = color(255,100,100);
	
	// Create the font
	printArray(PFont.list());
	f = createFont(PFont.list()[0], 24);
	textFont(f);

	// Begin keuze seriele poort
	val = Serial.list().length;
	if (val == 0)
	{
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
	
	if (val > 1)
	{
		println("Waarschuwing! Meer dan 1 seriële poort gevonden:");
		for (int i = 0; i < val; i++)
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
	update(mouseX, mouseY);
	background(255);
	smooth();
	stroke(0);
	fill(255);
	arc(300, 225, 298, 248, -PI, 0); // Quarter circle
	line(150,225,160,225);
	line(450,225, 440,225);
	line(300, 100, 300, 110);
	

	if (rectOver1)
		fill(rectHighlight);
	else
		fill(rectColor);

	// Create red rectangle
	rect(50, 200, 100, 50); // Draw 0 degree rectangle

	if (rectOver2)
		fill(rectHighlight);
	else
		fill(rectColor);

	rect(450, 200, 100, 50); // Draw 180 degree rectangle

	if (rectOver3)
		fill(rectHighlight);
	else
		fill(rectColor);

	rect(250, 50, 100, 50); // Draw 90 degree rectangle

	// Create the black text in rectangle fields
	textAlign(CENTER);
	fill(0);
	text("0 deg", 100, 235);
	text("180 deg", 500, 235);
	text("90 deg", 300, 85);
}

// Check if the mouse cursor over of some rectangle 
void update(int x, int y)
{
	// 0 degree
	rectOver1 = overRect(50, 200, 100, 50);
	

	// 180 degree
	rectOver2 = overRect(450, 200, 100, 50);

	// 90 degree
	rectOver3 = overRect(250, 50, 100, 50);
}

void mousePressed()
{
	if (rectOver1)	// If mouse pressed over square 0 degree,
		myPort.write("0");	// send 0 to serial monitor
	else if (rectOver2)	// If mouse pressed over square 180 degree,
		myPort.write("180");	// send 180 to serial monitor
	else if (rectOver3)	// If mouse pressed over square 90 degree,
		myPort.write("90");	// send 90 to serial monitor

}

boolean overRect(int x, int y, int width, int height)
{
	return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}
