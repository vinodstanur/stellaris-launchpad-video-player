#include "inc/lm4f120h5qr.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"



#define SET_CHIPSELECT ROM_GPIOPinWrite(SDC_GPIO_PORT_BASE, SDC_SSI_FSS, SDC_SSI_FSS);
#define CLEAR_CHIPSELECT ROM_GPIOPinWrite(SDC_GPIO_PORT_BASE, SDC_SSI_FSS, 0);
#define MMC_NO_RESPONSE -1

unsigned char command(unsigned char, unsigned int, unsigned char);
int mmc_init();
int play_video(unsigned int sector, char *buffer, unsigned int nsect);
int mmc_write_multiple_sector(unsigned int sector, unsigned char *buffer, unsigned int nsect);
unsigned int read_card_size(void);
char * mmc_read_sector(unsigned int sector);
void spi_write(char dat);
char spi_read (void);
void SELECT (void);
void DESELECT (void);
void init_sd();
void init_sd();

void uart_printf(char *);
void uart_print_num(int);

static const unsigned long g_ppulSSIIntMap[][2] =
{
    { SSI0_BASE, INT_SSI0 },
    { SSI1_BASE, INT_SSI1 },
    { SSI2_BASE, INT_SSI2 },
    { SSI3_BASE, INT_SSI3 },
};
