#include <avr/pgmspace.h>

typedef byte character_t[8];

const PROGMEM character_t arrow_data = {
	0b11000,
	0b11100,
	0b11110,
	0b11111,
	0b11110,
	0b11100,
	0b11000,
	0b00000
};

const PROGMEM character_t left_arrow = {
	0b00011,
	0b00111,
	0b01111,
	0b11111,
	0b01111,
	0b00111,
	0b00011,
	0b00000
};

const PROGMEM character_t up_arrow = {
	0b00000,
	0b00100,
	0b00100,
	0b01110,
	0b01110,
	0b11111,
	0b11111,
	0b00000
};

const PROGMEM character_t down_arrow = {
	0b00000,
	0b11111,
	0b11111,
	0b01110,
	0b01110,
	0b00100,
	0b00100,
	0b00000
};

// # BIG ARROWS # //
const PROGMEM character_t half_left_up = {
	0b00000,
	0b00000,
	0b00001,
	0b00011,
	0b00111,
	0b01111,
	0b00000,
	0b00000
};

const PROGMEM character_t half_left_down = {
	0b00000,
	0b00000,
	0b01111,
	0b00111,
	0b00011,
	0b00001,
	0b00000,
	0b00000
};

const PROGMEM character_t half_right_up = {
	0b00000,
	0b00000,
	0b10000,
	0b11000,
	0b11100,
	0b11110,
	0b00000,
	0b00000
};
const PROGMEM character_t half_right_down = {
	0b00000,
	0b00000,
	0b11110,
	0b11100,
	0b11000,
	0b10000,
	0b00000,
	0b00000
};

const PROGMEM character_t ask_data =
{
	0b00000,
	0b00100,
	0b00000,
	0b00100,
	0b01000,
	0b10000,
	0b10001,
	0b01110
};

const byte* const byte_table[] = {
	arrow_data, 
	left_arrow, 
	up_arrow, 
	down_arrow, 
	half_left_up, 
	half_left_down, 
	half_right_up, 
	half_right_down,
	ask_data
};

class Characters
{
private:

	void __print__(uint8_t type, uint8_t x, uint8_t y);

	enum CharacterTypes
	{
		ARROW,
		LEFT_ARROW,
		UP_ARROW,
		DOWN_ARROW,
		half__left__up,
		half__left__down,
		half__right__up,
		half__right__down,
		ASK
	};

	uint8_t lastX, lastY, amount;
	uint8_t selected = '>';

public:
	Characters();
	~Characters();

	void init();

	void arrow(uint8_t x, uint8_t y);
	void leftArrow(uint8_t x, uint8_t y);
	void upArrow(uint8_t x, uint8_t y);
	void downArrow(uint8_t x, uint8_t y);

	void bigArrowUp(uint8_t x, uint8_t y);
	void bigArrowDown(uint8_t x, uint8_t y);

	void ask(uint8_t x, uint8_t y);

	void clear();
};

Characters::Characters()
{
}
Characters::~Characters()
{
}

void Characters :: init()
{
	character_t buffer;
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
			buffer[j] = pgm_read_byte(&byte_table[i][j]);
		lcd.createChar(i, buffer);
	}
}


void Characters :: clear()
{
	lcd.setCursor(lastX, lastY);
	for (uint8_t i = 0; i < amount; i++)
		lcd.print(' ');
}

void Characters :: __print__ (uint8_t type, uint8_t x, uint8_t y)
{
	lcd.setCursor(x, y);
	lastX = x;
	lastY = y;

	switch (type)
	{
	case ARROW:
		lcd.print('>');
		break;

	case LEFT_ARROW:
		lcd.print('<');
		break;

	default:
			lcd.write(byte(type));
		break;
	}
	amount = 1;
}

void Characters :: arrow(uint8_t x, uint8_t y)
{
	/* if (selected != '>')
	{
		character_t buffer;
		for (uint8_t j = 0; j < 8; j++)
			buffer[j] = pgm_read_byte(&byte_table[ARROW][j]);
		lcd.createChar(ARROW, buffer);
		selected = '>';
	} */
	__print__(ARROW, x, y);
}

void Characters :: leftArrow (uint8_t x, uint8_t y)
{
	__print__(LEFT_ARROW, x, y);
}

void Characters :: downArrow (uint8_t x, uint8_t y)
{
	__print__(DOWN_ARROW, x, y);
}

void Characters :: upArrow (uint8_t x, uint8_t y)
{
	__print__(UP_ARROW, x, y);
}

void Characters :: bigArrowUp(uint8_t x, uint8_t y)
{
	__print__(half__left__up, x, y);
	__print__(half__right__up, x+1, y);
	amount = 2;
}

void Characters :: bigArrowDown(uint8_t x, uint8_t y)
{
	__print__(half__left__down, x, y);
	__print__(half__right__down, x+1, y);
	amount = 2;
}

void Characters :: ask (uint8_t x, uint8_t y)
{
	if (selected != '?')
	{
		character_t buffer;
		for (uint8_t j = 0; j < 8; j++)
			buffer[j] = pgm_read_byte(&byte_table[ASK][j]);
		lcd.createChar(ASK, buffer);
		selected = '?';
	}
	__print__(ASK, x, y);
}