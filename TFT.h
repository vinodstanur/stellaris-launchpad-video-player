
#define RED		0xf800

#define GREEN		0x07e0

#define BLUE		0x001f

#define BLACK		0x0000

#define YELLOW		0xffe0

#define WHITE		0xffff



//Other Colors

#define CYAN		0x07ff

#define BRIGHT_RED	0xf810

#define GRAY1		0x8410

#define GRAY2		0x4208



//TFT resolution 240*320

#define MIN_X	0

#define MIN_Y	0

#define MAX_X	240

#define MAX_Y	320


/////////HARDWARE PART/////////////////////////
//==================/CS===================== PD0


#define CS_OUTPUT   {SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; GPIO_PORTD_DIR_R |= (int)1;  GPIO_PORTD_DEN_R |= (int)1;}while(0);}

#define CS_HIGH     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<0)<<2)) = 255;

#define CS_LOW     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<0)<<2)) = 0;



//------------------RS----------------------



#define RS_OUTPUT   {SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; GPIO_PORTD_DIR_R |= (int)2;  GPIO_PORTD_DEN_R |= (int)2;}

#define RS_HIGH     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<1)<<2)) = 255;

#define RS_LOW     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<1)<<2)) = 0;



//------------------WR----------------------



#define WR_OUTPUT   do{SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; GPIO_PORTD_DIR_R |= (int)4;  GPIO_PORTD_DEN_R |= (int)4;}while(0);


#define WR_HIGH     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<2)<<2)) = 255;

#define WR_LOW     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<2)<<2)) = 0;


#define WR_RISING   {GPIO_PORTD_DATA_R |= (int)1<<2;GPIO_PORTD_DATA_R &= ~((int)1<<2);}



//------------------RD---------------------




#define RD_OUTPUT   do{SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; GPIO_PORTD_DIR_R |= (int)8;  GPIO_PORTD_DEN_R |= (int)8;}while(0);


#define RD_HIGH     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<3)<<2)) = 255;

#define RD_LOW     *(volatile unsigned long *)((volatile unsigned int)(GPIO_PORTD_BASE) + ((1<<3)<<2)) = 0;


#define RD_RISING   {GPIO_PORTD_DATA_R |= (int)1<<3;GPIO_PORTD_DATA_R &= ~((int)1<<3);}




//========================================





/**Macro definitions for char display direction**/

#define LEFT2RIGHT 0

#define DOWN2UP    1

#define RIGHT2LEFT 2

#define UP2DOWN    3






    void init_lcd (void);

    void paintScreenBlack(void);

    void sendCommand(unsigned int index);

    void sendData(unsigned int data);

    void sendByte(unsigned char data);

    void pushData(unsigned char data);

    unsigned char getData(void);

    unsigned int readRegister(unsigned int index);



    void setXY(unsigned int poX, unsigned int poY);

    void setPixel(unsigned int poX, unsigned int poY,unsigned int color);

    void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);

    void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);

    void drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);

    void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color);

    void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, unsigned int color);

    void drawCircle(int poX, int poY, int r,unsigned int color);

    void fillCircle(int poX, int poY, int r,unsigned int color);

    void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor);

    void drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);



    void all_pin_input(void);

    void all_pin_output(void);

    void all_pin_low(void);



    void setOrientation(unsigned int HV);

    void setDisplayDirect(unsigned char);

    void exitStandBy(void);

    unsigned char DisplayDirect;


