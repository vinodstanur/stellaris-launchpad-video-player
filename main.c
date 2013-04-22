/*
stellaris launchpad video player

author: Vinod S
email: vinodstanur at gmail dot com
date: 23/4/2013
homepage: http://blog.vinu.co.in
compiler: arm-none-eabi-gcc (summon arm toolchain)
Copyright (C) <2013> <http://blog.vinu.co.in>

(if possible, please try to keep the above details as it is....)

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


#include "inc/lm4f120h5qr.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "inc/hw_timer.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "string.h"
#include "driverlib/timer.h"

#include "inc/hw_ints.h"
#include "TFT.h"
#include "SD.h"

char data[512];



void pwm_enable(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinConfigure(GPIO_PF2_T1CCP0);
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER1_BASE, TIMER_A, 256 -1);
    TimerMatchSet(TIMER1_BASE, TIMER_A, 250); // PWM
    TimerEnable(TIMER1_BASE, TIMER_A);

}

void timer0_enable(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
//	unsigned int period = (SysCtlClockGet()/10)/2;
	TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/22400);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE,TIMER_A);
}

int main()
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_3|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIO_PORTF_DIR_R = 0xff;
	GPIO_PORTF_DEN_R = 0xff;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	GPIOPinConfigure(GPIO_PF2_T1CCP0);
	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);

	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
						 UART_CONFIG_PAR_NONE));
	ROM_IntEnable(INT_UART0);
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	uart_printf("HELLO WORLDdd\n");
	init_lcd();
	init_sd();
	for(int i=0;i<320;i++)
		drawLine(0,i,240,i,BLACK);

	int color=0;
	pwm_enable();
//	timer0_enable();
    TIMER1_TAMATCHR_R = 250;
	setXY(50,0);
	int sector=0;
	while(1) {
		///This is just a demo of video playing, it doesnt have any file system, just read sd card from sector 0
		//to the end and send the data to lcd and audio driver, which results in a video with audio....
		play_video(sector,data,0xffffffff); //send starting sector (sector 0) and number of sectors to read...
    	while(1);
	}

}
