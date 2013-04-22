/*
 * sd.c
 *
 *  Created on: 04-Apr-2013
 *      Author: vinod
 */
#include "SD.h"
#include "TFT.h"

#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

char mmc_buf[512];

char audio[256];

int countt=0;


/* Peripheral definitions for EK-LM3S3748 board */
// SSI port
#define SDC_SSI_BASE            SSI0_BASE
#define SDC_SSI_SYSCTL_PERIPH   SYSCTL_PERIPH_SSI0
// GPIO for SSI pins
#define SDC_GPIO_PORT_BASE      GPIO_PORTA_BASE
#define SDC_GPIO_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOA
#define SDC_SSI_CLK             GPIO_PIN_2
#define SDC_SSI_TX              GPIO_PIN_5
#define SDC_SSI_RX              GPIO_PIN_4
#define SDC_SSI_FSS             GPIO_PIN_3
#define SDC_SSI_PINS            (SDC_SSI_TX | SDC_SSI_RX | SDC_SSI_CLK |SDC_SSI_FSS)


unsigned int arg = 0;
char IS_SDHC;
unsigned int read_card_size(void)
{
    int i,j,k;
    int count = 0;
    unsigned int C_SIZE = 0;
    unsigned int BLOCK_LEN = 0;
    unsigned int BLOCKNR = 0;
    unsigned int MULT = 0;
    unsigned int C_SIZE_MULT = 0;
    unsigned int READ_BL_LEN = 0;
    unsigned char bf[200];
    unsigned char csd_byte = 0;
    if (command(9, 1,0xff) != 0)
    while(spi_read() != 0 && count++ <100);
    if(count < 90){
        count = 0;
        while(spi_read() != 0xfe && count++ <100);
        if(count >90) {uart_printf("no token!\n"); return 0;}
        uart_printf("GOTTT TOCKEN!!!!\n");
        k = 127;
        for(i = 0; i < 16; i++) {
            csd_byte = spi_read();
            for(j = 0; j < 8; j++) {
                if(csd_byte & (1<<7))
                bf[k--] = 1;
                else
                bf[k--] = 0;
                csd_byte <<= 1;
            }
        }
        if(!IS_SDHC) {
            for(i = 83; i>=80; i--) {
                READ_BL_LEN <<= 1;
                if(bf[i]) READ_BL_LEN++;
            }
            for(i = 73; i>=62; i--) {
                C_SIZE <<= 1;
                if(bf[i]) C_SIZE++;
            }
            for(i = 49; i>=47; i--) {
                C_SIZE_MULT <<= 1;
                if(bf[i]) C_SIZE_MULT++;
            }
//            uart_printf(("READ_BL_LEN = %d\n",READ_BL_LEN); //USED FOR TESTING WHILE CODING
 //           uart_printf(("C_SIZE = %d\n",C_SIZE);
 //           uart_printf(("C_SIZE_MULT = %d\n",C_SIZE_MULT);
            MULT = 1;
            for(i = 0; i < C_SIZE_MULT + 2; i++)
            MULT *= 2;
            BLOCKNR = (C_SIZE+1)*MULT;
            BLOCK_LEN = 1;
            for(i = 0; i<READ_BL_LEN; i++)
            BLOCK_LEN *= 2;
            //uart_printf(("MULT = %d\n",MULT); //USED FOR TESTING WHILE CODING
            //uart_printf(("BLOCK_LEN = %d\n",BLOCK_LEN);
            //uart_printf(("BLOCKNR = %d\n",BLOCKNR);
            return (BLOCKNR*BLOCK_LEN);
            } else {
            for(i = 69; i>=48; i--) {
                C_SIZE <<= 1;
                if(bf[i]) C_SIZE++;
            }
            return ( (((unsigned int)C_SIZE +1)*512)*1024 );
        }
    }
    return 0;
}

int mmc_write_multiple_sector(unsigned int sector, unsigned char *buffer, unsigned int nsect)
{
    int i, cnt;
    sector *= 512;
    if(command(25, sector, 0xff) != 0)
    while (spi_read() != 0 );

    while(nsect) {
        spi_write(0xff);
        spi_write(0xff);
        spi_write(0b11111100); //write token
        for(i = 0; i < 512; i++)
        spi_write(*buffer++);
        spi_write(0xff);
        spi_write(0xff);
        while (((spi_read() & 0b00011111) != 0x05));
        while ((spi_read() != 0xff));
        nsect--;
    }
    spi_write(0xff);
    spi_write(0xff);
    spi_write(0b11111101); //stop token
    spi_write(0xff);
    while ((spi_read() != 0xff));
    return 0;
}

int play_video(unsigned int sector,char *buffer, unsigned int nsect)
{

	unsigned char audio_index=0;

    CS_LOW;
    RS_HIGH;
    RD_HIGH;
    char dat;
    int i, cnt;
    sector *= 512;
    cnt = 0;
    if(command(18, sector, 0xff) != 0 )
    while (spi_read() != 0 && cnt++ <1000 );
    if(cnt >980) return MMC_NO_RESPONSE;
    cnt = 0;
    while(nsect) {
        while (spi_read() != 0xfe);// && cnt++ <1000 );
      //  if(cnt >980){return MMC_NO_RESPONSE;}
        cnt = 0;
        for(i = 0; i < 512; i++) {
        	WR_LOW;

        	__asm__ volatile ("nop");
        	__asm__ volatile ("nop");
        	__asm__ volatile ("nop");
        	__asm__ volatile ("nop");
        	while(!(HWREG(SDC_SSI_BASE + SSI_O_SR) & SSI_SR_TNF));
			HWREG(SDC_SSI_BASE + SSI_O_DR) = 0XFF;
			//while(!(HWREG(SDC_SSI_BASE + SSI_O_SR) & SSI_SR_RNE));
        	dat = HWREG(SDC_SSI_BASE + SSI_O_DR);
        	if(countt++==100) {
        		countt = 0;
//        		audio[audio_index++]=dat;
        		TIMER1_TAMATCHR_R = dat;
        	} else {
        		GPIO_PORTB_DATA_R =  dat;
        		WR_HIGH;
        	}

        }

        while(!(HWREG(SDC_SSI_BASE + SSI_O_SR) & SSI_SR_TNF));
        HWREG(SDC_SSI_BASE + SSI_O_DR) = 0XFF;
        HWREG(SDC_SSI_BASE + SSI_O_DR) = 0XFF;

        while(!(HWREG(SDC_SSI_BASE + SSI_O_SR) & SSI_SR_RNE));
		HWREG(SDC_SSI_BASE + SSI_O_DR);
		HWREG(SDC_SSI_BASE + SSI_O_DR);

        nsect--;
    }
    if(command(12, sector, 0xff) != 0)
    while (spi_read() != 0 && cnt++ <1000 );
    if(cnt >980) return MMC_NO_RESPONSE;
    cnt = 0;
    while (spi_read() != 0xff && cnt++ <1000 );
    if(cnt >980) return MMC_NO_RESPONSE;
  //  CS_HIGH;
    return 100;
}

 //SINGLE SECTOR READING CODE, NOT USED HERE
char * mmc_read_sector(unsigned int sector)
{
    int i;
    sector *= 512;
    if(command(17, sector, 0xff) != 0)
    	while (spi_read() != 0);
    while (spi_read() != 0xfe);
    for(i = 0; i < 512; i++)
    	mmc_buf[i] = spi_read();
    spi_write(0xff);
    spi_write(0xff);
    return mmc_buf;
}


int mmc_init()
{
    int u = 0;
    unsigned int count;
    unsigned char ocr[10];
    SysCtlDelay(1000);
    count = 0;
    while (command(0, 0, 0x95) != 1 && (count++ < 100));
    if (count > 90) {
        uart_printf("CARD ERROR-CMD0 \n");
        SysCtlDelay(1000);
        return 1;
    }
    //uart_printf(( KERN_ALERT "CMD0 PASSED!\n");
    if (command(8, 0x1AA, 0x87) == 1) { /* SDC ver 2.00 */
        for (u = 0; u < 4; u++) ocr[u] = spi_read();
        if (ocr[2] == 0x01 && ocr[3] == 0xAA) { /* The card can work at vdd range of 2.7-3.6V */
            count = 0;
            do
            	command(55,0,0xff);
            while (command(41, 1UL << 30, 0xff) && count++ < 1000); /* ACMD41 with HCHIP_SELECT bit */
            if(count > 900) {uart_printf("ERROR SDHC 41"); return 1;}
            count = 0;
            if (command(58, 0, 0xff) == 0 && count++ <100) { /* Check CCHIP_SELECT bit */
                for (u = 0; u < 4; u++) ocr[u] = spi_read();
            }
            IS_SDHC = 1;
        }
        } else {
        command(55, 0, 0xff);
        if(command(41, 0, 0xff) >1) {
            count = 0;
            while ((command(1, 0, 0xff) != 0) && (count++ < 100));
            if (count > 90) {
                uart_printf("CARD ERROR-CMD1 \n");
                SysCtlDelay(1000);
                return 1;
            }
            } else {
            count = 0;
            do {
                command(55, 0, 0xff);
            } while(command(41, 0, 0xff) != 0 && count< 100);
        }

        if (command(16, 512, 0xff) != 0) {
            uart_printf("CARD ERROR-CMD16 \n");
            SysCtlDelay(1000);
            return 1;
        }
    }
    uart_printf("CARD INITIALIZED!\n");
    SSIConfigSetExpClk(SDC_SSI_BASE, ROM_SysCtlClockGet(),
    	                           SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER,ROM_SysCtlClockGet()/2 , 8);
    	    ROM_SSIEnable(SDC_SSI_BASE);
    return 0;
}

unsigned char command(unsigned char command, unsigned int fourbyte_arg, unsigned char CRCbits)
{
    unsigned char retvalue,n;
    spi_write(0xff);
    spi_write((unsigned char)(0b01000000 | command));
    spi_write((unsigned char)(fourbyte_arg >>24));
    spi_write((unsigned char)(fourbyte_arg >>16));
    spi_write((unsigned char)(fourbyte_arg >>8));
    spi_write((unsigned char)fourbyte_arg);

    spi_write(CRCbits);

    n = 10;
    do
    retvalue = spi_read();
    while ((retvalue & 0x80) && --n);

    return retvalue;
}

	void send_initial_clock_train(void)
	{
	    unsigned int i;
	    int dat;

	    /* Ensure CS is held high. */
	    DESELECT();

	    /* Switch the SSI TX line to a GPIO and drive it high too. */
	    ROM_GPIOPinTypeGPIOOutput(SDC_GPIO_PORT_BASE, SDC_SSI_TX);
	    ROM_GPIOPinWrite(SDC_GPIO_PORT_BASE, SDC_SSI_TX, SDC_SSI_TX);

	    /* Send 10 bytes over the SSI. This causes the clock to wiggle the */
	    /* required number of times. */
	    for(i = 0 ; i < 10 ; i++)
	    {
	        /* Write DUMMY data. SSIDataPut() waits until there is room in the */
	        /* FIFO. */
	        ROM_SSIDataPut(SDC_SSI_BASE, 0xFF);

	        /* Flush data read during data write. */
	        ROM_SSIDataGet(SDC_SSI_BASE, &dat);
	    }

	    /* Revert to hardware control of the SSI TX line. */
	    ROM_GPIOPinTypeSSI(SDC_GPIO_PORT_BASE, SDC_SSI_TX);
	    SELECT();
	}


void spi_init (void)
{
	    /*
	     * This doesn't really turn the power on, but initializes the
	     * SSI port and pins needed to talk to the card.
	     */

	    /* Enable the peripherals used to drive the SDC on SSI */
	    ROM_SysCtlPeripheralEnable(SDC_SSI_SYSCTL_PERIPH);
	    ROM_SysCtlPeripheralEnable(SDC_GPIO_SYSCTL_PERIPH);

	    /*
	     * Configure the appropriate pins to be SSI instead of GPIO. The FSS (CS)
	     * signal is directly driven to ensure that we can hold it low through a
	     * complete transaction with the SD card.
	     */
	    ROM_GPIOPinTypeSSI(SDC_GPIO_PORT_BASE, SDC_SSI_TX | SDC_SSI_RX | SDC_SSI_CLK);
	    ROM_GPIOPinTypeGPIOOutput(SDC_GPIO_PORT_BASE, SDC_SSI_FSS);

	    /*
	     * Set the SSI output pins to 4MA drive strength and engage the
	     * pull-up on the receive line.
	     */
	    ROM_GPIOPadConfigSet(SDC_GPIO_PORT_BASE, SDC_SSI_RX, GPIO_STRENGTH_4MA,
	                         GPIO_PIN_TYPE_STD_WPU);
	    ROM_GPIOPadConfigSet(SDC_GPIO_PORT_BASE, SDC_SSI_CLK | SDC_SSI_TX | SDC_SSI_FSS,
	                         GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

	    /* Configure the SSI0 port */
	    ROM_SSIConfigSetExpClk(SDC_SSI_BASE, ROM_SysCtlClockGet(),
	                           SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 400000, 8);
	    ROM_SSIEnable(SDC_SSI_BASE);

	    /* Set DI and CS high and apply more than 74 pulses to SCLK for the card */
	    /* to be able to accept a native command. */
	    send_initial_clock_train();

}


void spi_write(char dat)
{
    int rcvdat;

    ROM_SSIDataPut(SDC_SSI_BASE, dat); /* Write the data to the tx fifo */

    ROM_SSIDataGet(SDC_SSI_BASE, &rcvdat); /* flush data read during the write */
}


char spi_read (void)
{
    int rcvdat;

    ROM_SSIDataPut(SDC_SSI_BASE, 0xFF); /* write dummy data */

    ROM_SSIDataGet(SDC_SSI_BASE, &rcvdat); /* read data frm rx fifo */

    return (char)rcvdat;
}

// asserts the CS pin to the card

void SELECT (void)
{
    ROM_GPIOPinWrite(SDC_GPIO_PORT_BASE, SDC_SSI_FSS, 0);
}

// de-asserts the CS pin to the card
void DESELECT (void)
{
    ROM_GPIOPinWrite(SDC_GPIO_PORT_BASE, SDC_SSI_FSS, SDC_SSI_FSS);
}

void init_sd() {
	spi_init();
	mmc_init();
}


void uart_printf(char *p)
{
	while(*p)
		UARTCharPut(UART0_BASE,*(p++));
}

void uart_print_num(int i)
{
	if (i < 0) {
		UARTCharPut(UART0_BASE,'0');
		i *= -1;
	} else if (i == 0) {
		UARTCharPut(UART0_BASE,'+');
		UARTCharPut(UART0_BASE,'0');
	} else
		UARTCharPut(UART0_BASE,'+');
	int count = 0;
	unsigned char b[5];
	b[0]=b[1]=b[2]=b[3]=b[4]=0;
	while (i) {
		b[count++] = i % 10;
		i /= 10;
	}
	for (count = 4; count >= 0; count--)
		UARTCharPut(UART0_BASE,(b[count] + '0'));
}


void TIMER0_ISR_A(void)
{
	static unsigned char a_index1=0;
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	TIMER1_TAMATCHR_R = audio[a_index1+=1];
}
