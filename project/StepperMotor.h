#ifndef StepperMotor_h
#define StepperMotor_h

#ifndef STEPPERDELAY
#define STEPPERDELAY 10;
#endif

class StepperMotor
{
public:
	//StepperMotor();
	void attach(uint8_t pins[4]);
	void setDelay(uint8_t ms);
	void setFullStep(bool);
	void setSteps(int32_t steps);

private:
	byte pins[4];
	bool fullstep;
	uint8_t delayTime;

	byte sequence[9] = {0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001, 0b1001, 0b0000};
	//byte sequence[9] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001, 0b0000};

	void doSteps(int32_t steps);
	void doStep(byte *currentState, byte *targetState);
};

void StepperMotor::attach(uint8_t coilPins[4])
{
	this->fullstep = false;
	this->delayTime = STEPPERDELAY;

	memcpy(this->pins, coilPins, sizeof(uint8_t) * 4);

	for (uint8_t i = 0; i < 4; ++i)
	{
		pinMode(pins[i], OUTPUT);
	}
}

void StepperMotor::setDelay(uint8_t ms)
{
	this->delayTime = ms;
}

void StepperMotor::setFullStep(bool fullstep)
{
	this->fullstep = fullstep;
}

void StepperMotor::setSteps(int32_t steps)
{
	this->doSteps(steps);
}

void StepperMotor::doSteps(int32_t steps)
{
	signed char reverse = (steps >> 15)*2 + 1;

	uint8_t step = fullstep * reverse + reverse;
	uint8_t seqIdx = -step - (steps >> 15 & 0x1) * 2;

	byte *targetState = NULL;
	byte *currentState = sequence + 8;

	steps *= reverse;

	while (steps)
	{
		seqIdx = (seqIdx + step) % 8;
		targetState = &sequence[seqIdx];

		this->doStep(currentState, targetState);

		currentState = targetState;
		steps--;
	}

	this->doStep(currentState, sequence + 8);
}

void StepperMotor::doStep(byte *currentState, byte *targetState)
{
	byte diff = *currentState ^ *targetState;

	for (uint8_t pin = 0; pin < 4; pin++)
	{
		if (diff >> pin & 0x1)
		{
			digitalWrite(this->pins[pin], !(*currentState >> pin & 0x1));
		}
	}

	delay(this->delayTime);
}

#endif
