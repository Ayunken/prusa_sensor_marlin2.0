#include "SWI2C.h"
#include <inttypes.h>

//PAT9125 configuration
#define PAT9125_I2C_ADDR  0x75  //ID=LO
//#define PAT9125_I2C_ADDR  0x79  //ID=HI
//#define PAT9125_I2C_ADDR  0x73  //ID=NC



//PAT9125 registers
#define PAT9125_PID1			0x00
#define PAT9125_PID2			0x01
#define PAT9125_MOTION			0x02
#define PAT9125_DELTA_XL		0x03
#define PAT9125_DELTA_YL		0x04
#define PAT9125_MODE			0x05
#define PAT9125_CONFIG			0x06
#define PAT9125_WP				0x09
#define PAT9125_SLEEP1			0x0a
#define PAT9125_SLEEP2			0x0b
#define PAT9125_RES_X			0x0d
#define PAT9125_RES_Y			0x0e
#define PAT9125_DELTA_XYH		0x12
#define PAT9125_SHUTTER			0x14
#define PAT9125_FRAME			0x17
#define PAT9125_ORIENTATION		0x19
#define PAT9125_BANK_SELECTION	0x7f




class cPAT9125 
{
    public:

    cPAT9125(uint8_t sda, uint8_t scl, uint8_t XRES, uint8_t YRES);

    void update();
    void update_y();
    void update_x();
    bool IsInit();

    private:
    uint8_t rd_reg(uint8_t addr);
    void wr_reg(uint8_t addr, uint8_t data);
    uint8_t wr_reg_verify(uint8_t addr, uint8_t data);

    public:
    int16_t x = 0;
    int16_t y = 0;
    uint8_t b = 0;
    uint8_t s = 0;

    private:
    uint8_t PID1 = 0;
    uint8_t PID2 = 0;
    SWI2C swi2c;
};

