
/*
tft library ported from seedstudio arduino library....
by author: Vinod S
email: vinodstanur at gmail dot com
date: 23/4/2013
homepage: http://blog.vinu.co.in
compiler: arm-none-eabi-gcc (summon arm toolchain)
Copyright (C) <2013> <http://blog.vinu.co.in>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "TFT.h"
#include "inc/lm4f120h5qr.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

void delay(volatile unsigned int b) {
	while(b--) {
		for(volatile int x=0;x<10000;x++);
	}
}

void pushData(unsigned char data)
{
	GPIO_PORTB_DATA_R = data;
}


void sendCommand(unsigned int index)
{
    CS_LOW;
    RS_LOW;
    RD_HIGH;
    WR_HIGH;
    WR_LOW;
    pushData(0);
    WR_HIGH;
    WR_LOW;
    pushData(index&0xff);
    WR_HIGH;
    CS_HIGH;
}

void sendData(unsigned int data)
{
    CS_LOW;
    RS_HIGH;
    RD_HIGH;

    WR_LOW;
    pushData((data&0xff00)>>8);
    WR_HIGH;

    WR_LOW;
    pushData(data&0xff);
    WR_HIGH;

    CS_HIGH;
}

void sendByte(unsigned char data)
{
    WR_LOW;
	CS_LOW;
    RS_HIGH;
    RD_HIGH;
    pushData((data));
    WR_HIGH;

    CS_HIGH;
}

void paintScreenBlack(void)

{

	setXY(0,0);
    for(unsigned char i=0;i<2;i++)

    {

        for(unsigned int f=0;f<38400;f++)

        {
            sendData(BLACK);
        }

    }

}


void init_lcd (void)
{
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD|SYSCTL_PERIPH_GPIOB|SYSCTL_PERIPH_GPIOC);
	  GPIO_PORTD_DIR_R |= 15;
	  GPIO_PORTD_DEN_R |= 15;

    all_pin_output();
    all_pin_low();
    delay(100);
    sendCommand(0x0001);
    sendData(0x0100);
    sendCommand(0x0002);
    sendData(0x0700);
    sendCommand(0x0003);
    sendData(0x0030);
    //sendData(0xD030);
    sendCommand(0x0004);
    sendData(0x0000);
    sendCommand(0x0008);
    sendData(0x0302);
    sendCommand(0x000A);
    sendData(0x0000);
    sendCommand(0x000C);
    sendData(0x0000);
    sendCommand(0x000D);
    sendData(0x0000);
    sendCommand(0x000F);
    sendData(0x0000);

    delay(100);

    sendCommand(0x0030);
    sendData(0x0000);
    sendCommand(0x0031);
    sendData(0x0405);
    sendCommand(0x0032);
    sendData(0x0203);
    sendCommand(0x0035);
    sendData(0x0004);
    sendCommand(0x0036);
    sendData(0x0B07);
    sendCommand(0x0037);
    sendData(0x0000);
    sendCommand(0x0038);
    sendData(0x0405);
    sendCommand(0x0039);
    sendData(0x0203);
    sendCommand(0x003c);
    sendData(0x0004);
    sendCommand(0x003d);
    sendData(0x0B07);
    sendCommand(0x0020);
    sendData(0x0000);
    sendCommand(0x0021);
    sendData(0x0000);
    sendCommand(0x0050);
 //   sendData(0x0000);
    sendData(50);
    sendCommand(0x0051);
    sendData(240-51);
    sendCommand(0x0052);
    sendData(0x0000);
    sendCommand(0x0053);
    sendData(0x013f);

    delay(100);

    sendCommand(0x0060);
    sendData(0xa700);
    sendCommand(0x0061);
    sendData(0x0001);
    sendCommand(0x0090);
    sendData(0x003A);
    sendCommand(0x0095);
    sendData(0x021E);
    sendCommand(0x0080);
    sendData(0x0000);
    sendCommand(0x0081);
    sendData(0x0000);
    sendCommand(0x0082);
    sendData(0x0000);
    sendCommand(0x0083);
    sendData(0x0000);
    sendCommand(0x0084);
    sendData(0x0000);
    sendCommand(0x0085);
    sendData(0x0000);
    sendCommand(0x00FF);
    sendData(0x0001);
    sendCommand(0x00B0);
    sendData(0x140D);
    sendCommand(0x00FF);
    sendData(0x0000);
    delay(100);
    sendCommand(0x0007);
    sendData(0x0133);
    delay(50);
    exitStandBy();
    sendCommand(0x0022);

    //paint screen black
    paintScreenBlack();

}

void exitStandBy(void)
{
    sendCommand(0x0010);
    sendData(0x14E0);
    delay(100);
    sendCommand(0x0007);
    sendData(0x0133);
}

void setOrientation(unsigned int HV)//horizontal or vertical
{
    sendCommand(0x03);
    if(HV==1)//vertical
    {
        sendData(0x5038);
    }
    else//horizontal
    {
        sendData(0x5030);
    }
    sendCommand(0x0022); //Start to write to display RAM
}

void setDisplayDirect(unsigned char Direction)
{
  DisplayDirect = Direction;
}

void setXY(unsigned int poX, unsigned int poY)
{
    sendCommand(0x0020);//X
    sendData(poX);
    sendCommand(0x0021);//Y
    sendData(poY);
    sendCommand(0x0022);//Start to write to display RAM
}

void setPixel(unsigned int poX, unsigned int poY,unsigned int color)
{
    setXY(poX,poY);
    sendData(color);
}

void all_pin_output(void)
{
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	  GPIO_PORTB_DIR_R = 0b11111111;
	  GPIO_PORTB_DEN_R = 0b11111111;
}

void all_pin_low(void)
{
	GPIO_PORTB_DATA_R &= ~(0xff);
}

void drawCircle(int poX, int poY, int r,unsigned int color)
{
    int x = -r, y = 0, err = r, e2;
    do {
        setPixel(poX-x, poY+y,color);
        setPixel(poX+x, poY+y,color);
        setPixel(poX+x, poY-y,color);
        setPixel(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

void fillCircle(int poX, int poY, int r,unsigned int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {

        drawVerticalLine(poX-x,poY-y,2*y,color);
        drawVerticalLine(poX+x,poY-y,2*y,color);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}


void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color)
{
    int x = x1-x0;
    int y = y1-y0;
    int dx = abs(x), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    for (;;){ /* loop */
        setPixel(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}


void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
    setXY(poX,poY);
    setOrientation(1);
    if(length+poY>MAX_Y)
    {
        length=MAX_Y-poY;
    }

    for(unsigned int i=0;i<length;i++)
    {
        sendData(color);
    }
}

void  drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
    setXY(poX,poY);
    setOrientation(0);
    if(length+poX>MAX_X)
    {
        length=MAX_X-poX;
    }
    for(unsigned int i=0;i<length;i++)
    {
        sendData(color);
    }
}


void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color)
{
    drawHorizontalLine(poX, poY, length, color);
    drawHorizontalLine(poX, poY+width, length, color);

    drawVerticalLine(poX, poY, width,color);
    drawVerticalLine(poX + length, poY, width,color);
}

void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, unsigned int color)
{
    for(unsigned int i=0;i<width;i++)
    {
        if(DisplayDirect == LEFT2RIGHT)
          drawHorizontalLine(poX, poY+i, length, color);
          else if (DisplayDirect ==  DOWN2UP)
            drawHorizontalLine(poX, poY-i, length, color);
            else if(DisplayDirect == RIGHT2LEFT)
              drawHorizontalLine(poX, poY-i, length, color);
              else if(DisplayDirect == UP2DOWN)
                drawHorizontalLine(poX, poY+i, length, color);

    }
}
