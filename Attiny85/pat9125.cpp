//pat9125.cpp
#include "pat9125.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "Arduino.h"


cPAT9125::cPAT9125 (uint8_t sda, uint8_t scl, uint8_t XRES, uint8_t YRES):swi2c(sda, scl, PAT9125_I2C_ADDR)
{
	swi2c.begin();
  // Verify that the sensor responds with its correct product ID.
	PID1 = rd_reg(PAT9125_PID1);
	PID2 = rd_reg(PAT9125_PID2);
	if (!IsInit())
	{
		PID1 = rd_reg(PAT9125_PID1);
		PID2 = rd_reg(PAT9125_PID2);
	}

	wr_reg(PAT9125_RES_X, XRES);
	wr_reg(PAT9125_RES_Y, YRES);

#ifdef PAT9125_NEW_INIT
	// Switch to bank0, not allowed to perform OTS_RegWriteRead.
	pat9125_wr_reg(PAT9125_BANK_SELECTION, 0);
	// Software reset (i.e. set bit7 to 1). It will reset to 0 automatically.
	// After the reset, OTS_RegWriteRead is not allowed.
	pat9125_wr_reg(PAT9125_CONFIG, 0x97);
	// Wait until the sensor reboots.
	// Delay 1ms.
	_delay_us(1000);
	{
		const uint8_t *ptr = pat9125_init_seq1;
		for (;;) {
			const uint8_t addr = pgm_read_byte_near(ptr ++);
			if (addr == 0x0ff)
				break;
			if (! pat9125_wr_reg_verify(addr, pgm_read_byte_near(ptr ++)))
				// Verification of the register write failed.
				return 0;
		}
	}
	// Delay 10ms.
	_delay_ms(10);
	// Switch to bank1, not allowed to perform OTS_RegWrite.
	pat9125_wr_reg(PAT9125_BANK_SELECTION, 0x01);
	{
		const uint8_t *ptr = pat9125_init_seq2;
		for (;;) {
			const uint8_t addr = pgm_read_byte_near(ptr ++);
			if (addr == 0x0ff)
				break;
			if (! pat9125_wr_reg_verify(addr, pgm_read_byte_near(ptr ++)))
				// Verification of the register write failed.
				return 0;
		}
	}
	// Switch to bank0, not allowed to perform OTS_RegWriteRead.
	pat9125_wr_reg(PAT9125_BANK_SELECTION, 0x00);
	// Enable write protect.
	pat9125_wr_reg(PAT9125_WP, 0x00);

	pat9125_PID1 = pat9125_rd_reg(PAT9125_PID1);
	pat9125_PID2 = pat9125_rd_reg(PAT9125_PID2);
#endif //PAT9125_NEW_INIT

	
	//fprintf_P(uartout, PSTR("PAT9125_RES_X=%hhu\n"), pat9125_rd_reg(PAT9125_RES_X));
	//fprintf_P(uartout, PSTR("PAT9125_RES_Y=%hhu\n"), pat9125_rd_reg(PAT9125_RES_Y));

}

bool cPAT9125::IsInit()
{
	if ((PID1 == 0x31) && (PID2 == 0x91)) return true;
	else false;
}



/*

// Init sequence, address & value.
const PROGMEM uint8_t pat9125_init_seq1[] = {
	// Disable write protect.
	PAT9125_WP, 0x5a,
	// Set the X resolution to zero to let the sensor know that it could safely ignore movement in the X axis.
    PAT9125_RES_X, PAT9125_XRES,
    // Set the Y resolution to a maximum (or nearly a maximum).
    PAT9125_RES_Y, PAT9125_YRES,
    // Set 12-bit X/Y data format.
    PAT9125_ORIENTATION, 0x04,
//	PAT9125_ORIENTATION, 0x04 | (xinv?0x08:0) | (yinv?0x10:0), //!? direction switching does not work
    // Now continues the magic sequence from the PAT912EL Application Note: Firmware Guides for Tracking Optimization.
    0x5e, 0x08,
    0x20, 0x64,
    0x2b, 0x6d,
    0x32, 0x2f,
    // stopper
    0x0ff
};


// Init sequence, address & value.
const PROGMEM uint8_t pat9125_init_seq2[] = {
	// Magic sequence to enforce full frame rate of the sensor.
	0x06, 0x028,
	0x33, 0x0d0,
	0x36, 0x0c2,
	0x3e, 0x001,
	0x3f, 0x015,
	0x41, 0x032,
	0x42, 0x03b,
	0x43, 0x0f2,
	0x44, 0x03b,
	0x45, 0x0f2,
	0x46, 0x022,
	0x47, 0x03b,
	0x48, 0x0f2,
	0x49, 0x03b,
	0x4a, 0x0f0,
	0x58, 0x098,
	0x59, 0x00c,
	0x5a, 0x008,
	0x5b, 0x00c,
	0x5c, 0x008,
	0x61, 0x010,
	0x67, 0x09b,
	0x6e, 0x022,
	0x71, 0x007,
	0x72, 0x008,
	// stopper
    0x0ff
};

*/



void cPAT9125::update()
{
	if (IsInit())
	{
		uint8_t ucMotion = rd_reg(PAT9125_MOTION);
		b = rd_reg(PAT9125_FRAME);
		s = rd_reg(PAT9125_SHUTTER);

		if (PID1 == 0xff) return;

		if (ucMotion & 0x80)
		{
			uint8_t ucXL = rd_reg(PAT9125_DELTA_XL);
			uint8_t ucYL = rd_reg(PAT9125_DELTA_YL);
			uint8_t ucXYH = rd_reg(PAT9125_DELTA_XYH);

			if (PID1 == 0xff) return;

			int16_t iDX = ucXL | ((ucXYH << 4) & 0xf00);
			int16_t iDY = ucYL | ((ucXYH << 8) & 0xf00);

			if (iDX & 0x800) iDX -= 4096;
			if (iDY & 0x800) iDY -= 4096;

			x -= iDX;
			y -= iDY; //negative number, because direction switching does not work
		}
		
	}
	
}

void cPAT9125::update_y()
{
	if (IsInit())
	{
		uint8_t ucMotion = rd_reg(PAT9125_MOTION);
		if (PID1 == 0xff) return;
		if (ucMotion & 0x80)
		{
			uint8_t ucYL = rd_reg(PAT9125_DELTA_YL);
			uint8_t ucXYH = rd_reg(PAT9125_DELTA_XYH);

			if (PID1 == 0xff) return;

			int16_t iDY = ucYL | ((ucXYH << 8) & 0xf00);
			if (iDY & 0x800) iDY -= 4096;
			y -= iDY; //negative number, because direction switching does not work
		}
	}
}

void cPAT9125::update_x()
{
	if (IsInit())
	{
		uint8_t ucMotion = rd_reg(PAT9125_MOTION);
		if (PID1 == 0xff) return;
		if (ucMotion & 0x80)
		{
			uint8_t ucXL = rd_reg(PAT9125_DELTA_XL);
			uint8_t ucXYH = rd_reg(PAT9125_DELTA_XYH);

			if (PID1 == 0xff) return;

			int16_t iDX = ucXL | ((ucXYH << 4) & 0xf00);
			if (iDX & 0x800) iDX -= 4096;
			x -= iDX; //negative number, because direction switching does not work
		}
	}
}

uint8_t cPAT9125::rd_reg(uint8_t addr)
{
	uint8_t data = 0;
  if (!swi2c.read1bFromRegister(addr,&data))
  {
    PID1 = 0xff;
    PID2 = 0xff;
    return 0;
  }
	/*
	if (!swi2c_readByte_A8(PAT9125_I2C_ADDR, addr, &data)) //NO ACK error
	{
		pat9125_PID1 = 0xff;
		pat9125_PID2 = 0xff;
		return 0;
	}
 */
	return data;
  
}

void cPAT9125::wr_reg(uint8_t addr, uint8_t data)
{
	if (!swi2c.writeToRegister(addr,data))
	{
    	PID1 = 0xff;
    	PID2 = 0xff;
    	return;
	}
   /*
	if (!swi2c_writeByte_A8(PAT9125_I2C_ADDR, addr, &data)) //NO ACK error
	{
		pat9125_PID1 = 0xff;
		pat9125_PID2 = 0xff;
		return;
	}
*/
}

uint8_t cPAT9125::wr_reg_verify(uint8_t addr, uint8_t data)
{
	wr_reg(addr, data);
	return rd_reg(addr) == data;
}
