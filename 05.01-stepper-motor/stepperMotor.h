class StepperMotor
{
public:
	//StepperMotor();
	void attach(unsigned char pins[4]);
	void setDelay(unsigned char ms);
	void setFullStep(bool);
	void setSteps(int steps);

private:
	byte pins[4];
	bool fullstep;
	unsigned char delayTime;

	byte sequence[9] = {0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001, 0b1001, 0b0000};
	//byte sequence[9] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001, 0b0000};

	void doSteps(int steps);
	void doStep(byte *currentState, byte *targetState);
};

void StepperMotor::attach(unsigned char coilPins[4])
{
	fullstep = false;
	memcpy(pins, coilPins, sizeof(coilPins[0]) * 4);
	delayTime = 10;

	for (unsigned char i = 0; i < 4; ++i)
		pinMode(pins[i], OUTPUT);
}

void StepperMotor::setDelay(unsigned char ms)
{
	delayTime = ms;
}

void StepperMotor::setFullStep(bool b)
{
	fullstep = b;
}

void StepperMotor::setSteps(int steps)
{
	doSteps(steps);
}

void StepperMotor::doSteps(int steps)
{
	signed char reverse = (steps >> 15)*2 + 1;
	unsigned char step = fullstep * reverse + reverse;
	unsigned char seqIdx = -step - (steps >> 15 & 0x1)*2;
	steps *= reverse;

	byte *targetState = NULL;
	byte *currentState = sequence + 8;

	while (steps > 0)
	{
		seqIdx = (seqIdx + step) % 8;
		targetState = &sequence[seqIdx];
		Serial.println(seqIdx);

		doStep(currentState, targetState);

		currentState = targetState;
		steps--;
	}
	doStep(currentState, sequence + 8);
}

void StepperMotor::doStep(byte *currentState, byte *targetState)
{
	byte diff = *currentState ^ *targetState;
	for (unsigned char pin = 0; pin < 4; pin++)
	{
		
		if (diff >> pin & 0x1)
		{
			digitalWrite(pins[pin], !(*currentState >> pin & 0x1));
		}
	}

	delay(delayTime);
}
