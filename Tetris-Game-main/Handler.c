/******************************************************************************
 * @file     Handler.c
 * @brief    This file is to call the function to execute states of Tetris game by user's purposes  .
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
/* Header includes for microcontroller libraries and project modules */
#include "NuMicro.h"
#include <string.h>
#include "EBI_LCD_Module.h"
#include "KEIL/gameHeader.h"
#include "KEIL/gameStage.h"
#include "KEIL/LogicGame.h"

/* External variables from other modules for game state */
extern int isPaused;
extern TetrisBlock currentTetrisBlock;
extern TetrisBlock nextTetrisBlock;
extern int currentX, currentY;
extern int needsRedraw;
/* Defines for clock and PLL status flags */
#define HXTSTB 1 << 0     // HXT Clock Source Stable Flag

#define PLLSTB 1<<2				// Internal PLL Clock Source Stable Flag
#define HXTEN  1<<0				// HXT Enable Bit, write 1 to enable 


volatile    uint8_t     Timer0_flag = 0;
volatile    uint8_t     Timer0_cnt = 0;
volatile uint8_t timer_running = 1; // 1 = running, 0 = paused
volatile uint8_t switchPressed = 0; // Flag for debouncing
uint8_t seconds = 0;
uint8_t minutes = 0;
volatile uint32_t millis = 0;
uint32_t lastLeftTime = 0;
uint32_t lastRightTime = 0;
volatile uint32_t lastSW1Time = 0;
volatile uint32_t lastUpTime = 0;
volatile uint32_t lastDownTime = 0;



void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
		CLK->PWRCTL |= HXTEN; // Enable HXT
	
    /* Waiting for 12MHz clock ready */
		while(!(CLK->STATUS & HXTSTB));
	
    /* Switch HCLK clock source to HXT */
		CLK->CLKSEL0 &= ~(0b111 << 0); //clear
		CLK->CLKSEL0 |= (0b000 << 0); //set

    /* Set core clock as PLL_CLOCK from PLL */
	  //Configure PLL for 192 MHz
		CLK->PLLCTL &=(~(0xFFFF << 0));      // Clear PLLCTL[15:0]
		CLK->PLLCTL &= (~(1 << 19)); 				// PLL Source is HXT
		CLK->PLLCTL &= (~(1 << 16));				// PLL is in normal mode
		
		// Configure PLL output frequency
		// FIN = 12 MHZ; FOUT = 192 MHZ
		// Choose:
		// NR = 2 -> INDIV = 1
		// NF = 32 -> FBDIV = 30
		// NO = 2 -> OUTDIV = 01
		CLK->PLLCTL |= (1 << 9); 				// INDIV
		CLK->PLLCTL |= (30 << 0); 			// FBDIV
		CLK->PLLCTL &= (0b01 << 14);    // OUTDIV
		
		CLK->PLLCTL &= ~(1 << 18);   // PLL clock enable
    
    while (!(CLK->STATUS & PLLSTB)); // Wait for PLL to stabilize

		// Set HCLK to PLLFOUT
		CLK->CLKSEL0 &= (~(0x07 << 0)); // Clear current settings for 
    CLK->CLKSEL0 |= 0x02; 					// Set a new value
	
		// Set HCLK Divider to 0
		CLK->CLKDIV0 &= (~(0x0F<<0)); // Clear current settings for HCLKDIV
		CLK->CLKDIV0 |= (0x00 << 0);			// Set new value
		
    /* Set both PCLK0 and PCLK1 as HCLK/2 */
		CLK->PCLKDIV &= ~((0x07 << 0) | (0x07 << 4));
		CLK->PCLKDIV |= ((0x1 << 0) | (0x1 << 4));

		/* EADC clock setting */
		// EADC clock source is PCLK1 96 MHz
		CLK->CLKDIV0 &= ~(0x0FF << 16);		// Clear current settings
		CLK->CLKDIV0 |= (7 << 16); 		// EADC clock divider is (7+1) --> ADC clock is 96/8 = 12 MHz (Maximum is 72)
		CLK->APBCLK0 |= (1 << 28); 			// enable EADC0 clock
		
		// TM1 clock selection 
		CLK->CLKSEL1 &= ~ (0b111 << 12); // clear setting
		CLK->CLKSEL1 |= (0b000 << 12); 	// Clock source from HXT
		CLK->APBCLK0 |= (1 << 3); 		// Clock enable for Timer 1
		
		// TM0 clock selection 
		CLK->CLKSEL1 &= ~ (0b111 << 8); // clear setting
		CLK->CLKSEL1 |= (0b000 << 8); 	// Clock source from HXT
		CLK->APBCLK0 |= (1 << 2); 		// Clock enable for Timer 0
		
		// TM2 clock selection
		CLK->CLKSEL1 &= ~ (0b111 << 16); // clear setting
		CLK->CLKSEL1 |= (0b000 << 16);  // Clock source from HXT
		CLK->APBCLK0 |= (1 << 4);       // Clock enable for Timer 2

		// EBI Controller Clock Enable Bit
		CLK->AHBCLK |= (1 << 3);  

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /*=== EBI (LCD module) mult-function pins ===*/
    /* EBI AD0~5 pins on PG.9~14 */
    SYS->GPG_MFPH &= ~((0xF << 4) | (0xF << 8)  |
                       (0xF << 12) | (0xF << 16)  |
                       (0xF << 20) | (0xF << 24) );
    SYS->GPG_MFPH |= ((0x02 << 4) | (0x02 << 8) |
                      (0x02 << 12) | (0x02 << 16) |
                      (0x02 << 20) | (0x02 << 24));

    /* EBI AD6, AD7 pins on PD.8, PD.9 */
    SYS->GPD_MFPH &= ~((0xF << 0) | (0xF << 4));
    SYS->GPD_MFPH |= ((0x02<<0) | (0x02<<4));

    /* EBI AD8, AD9 pins on PE.14, PE.15 */
    SYS->GPE_MFPH &= ~((0xF << 24) | (0xF << 28));
    SYS->GPE_MFPH |= ((0x02 << 24) | (0x02 << 28));

    /* EBI AD10, AD11 pins on PE.1, PE.0 */
    SYS->GPE_MFPL &= ~((0xF << 4) | (0xF << 0));
    SYS->GPE_MFPL |= ((0x02<<4) | (0x02<<0));

    /* EBI AD12~15 pins on PH.8~11 */
    SYS->GPH_MFPH &= ~((0xF << 0) | (0xF << 4) |
                       (0xF << 8) | (0xF << 12));
    SYS->GPH_MFPH |= ((0x02 << 0) | (0x02 << 4) |
                      (0x02 << 8) | (0x02 << 12));

    /* Configure PH.3 as Output mode for LCD_RS (use GPIO PH.3 to control LCD_RS) */
		PH->MODE &= ~(0x3 << 6);   // Clear bits 7:6
		PH->MODE |=  (0x1 << 6);   // Set bits 7:6 to 01 for push-pull ouput
		PH->DOUT |= (1 << 3); // Set PH.3 high
		
    /* EBI RD and WR pins on PE.4 and PE.5 */
    SYS->GPE_MFPL &= ~((0xF << 16) | (0xF << 20));
    SYS->GPE_MFPL |= ((0x02 << 16) | (0x02 << 20));

    /* EBI CS0 pin on PD.14 */
    SYS->GPD_MFPH &= ~(0xF << 24);
    SYS->GPD_MFPH |= (0x02 << 24);

    /* Configure PB.6 and PB.7 as Output mode for LCD_RST and LCD_Backlight */
		// Clear current mode for PB.6 and PB.7 (2 bits per pin)
		PB->MODE &= ~((0x3 << 12) | (0x3 << 14)); // clear the current bit for PB.6 and PB.7
		PB->MODE |=  ((0x1 << 12) | (0x1 << 14)); // set the push-pull output
		PB->DOUT |= (1 << 6);  // Set PB.6 high
		PB->DOUT &= ~(1 << 7); // Set PB.7 low
}

void TMR2_IRQHandler(void)
{
		if (TIMER2->INTSTS & (1 << 0))
		{
			TIMER2->INTSTS = (1 << 0); // Clear Timer 0 overflow flag	
			
			millis++;
		}
}

void Timer2_Init(void)
{
		// Set Prescale
		TIMER2->CTL &= ~(0xFF << 0); // clear current setting for Prescale
		TIMER2->CTL |= (0 << 0); // Prescale = (0+1) = 1
	
    // Duration = 1ms => Target Tiner Count = 11999
    TIMER2->CMP = 11999;
	
		// Set TM2 operation mode to Periodic Mode
		TIMER2->CTL &= ~(0b11 << 27); // Clear current settings
		TIMER2->CTL |= (0b01 << 27);	// Periodic Mode
		// The behavior selection in periodic mode is Enabled.
		TIMER2->CTL |= (1 << 20);
		// Enable TM0 interrup flag TIF
		TIMER2->CTL |= (1 << 29);

		// Configure Interrupt
		// Enable TM2 interrup flag TIF
		TIMER2->CTL |= (1 << 29);
		// NVIC interrupt configuration
		NVIC->ISER[1] |= (1 << 2); // (34 - 32 = 2)
		// Clear Timer 2 overflow flag
		TIMER2->INTSTS = (1 << 0); // Write 1 to clear TIF
		
		// TM2 Start Counting
		TIMER2->CTL |= (1 << 30);

}

void TMR1_IRQHandler(void)
{
    // Clear Timer 0 overflow flag
    TIMER1->INTSTS = (1 << 0);
	 // Only move down when timer triggers
    MoveTetrisBlockDown();
    needsRedraw = 1;  // Set flag for main loop to handle movement

}

void Timer1_Init(void)
{
				// Set Prescale
		TIMER1->CTL &= ~(0xFF << 0); // clear current setting for Prescale
		TIMER1->CTL |= (0 << 0); // Prescale = (0+1) = 1 
    /*( 1/12MHz * 1200000) - 1 = 100ms */
    TIMER1->CMP = 5999999;
			// Set TM1 operation mode to Periodic Mode
		TIMER1->CTL &= ~(0b11 << 27); // Clear current settings
		TIMER1->CTL |= (0b01 << 27);	// Periodic Mode
		// The behavior selection in periodic mode is Enabled.
		TIMER1->CTL |= (1 << 20);
				// Enable TM1 interrup flag TIF
		TIMER1->CTL |= (1 << 29);

    /* Enable Timer1 IRQ */
    // NVIC interrupt configuration
		NVIC->ISER[1] |= (1 << 1);

		// TM1 Start Counting
		TIMER1->CTL |= (1 << 30);

}

void TMR0_IRQHandler(void)
{
		if (TIMER0->INTSTS & (1 << 0))
		{
			TIMER0->INTSTS = (1 << 0); // Clear Timer 0 overflow flag	
			
			/* Set Timer0_flag = 1 */
			Timer0_flag = 1;

			/* Timer0_cnt + 1 */
			Timer0_cnt = Timer0_cnt + 1;
		}
}

void Timer0_Init(void)
{
		// Set Prescale
		TIMER0->CTL &= ~(0xFF << 0); // clear current setting for Prescale
		TIMER0->CTL |= (11 << 0); // Prescale = (11+1) = 12
	
    // Duration = 1s => Target Tiner Count = 499999
    TIMER0->CMP = 999999;
	
		// Set TM0 operation mode to Periodic Mode
		TIMER0->CTL &= ~(0b11 << 27); // Clear current settings
		TIMER0->CTL |= (0b01 << 27);	// Periodic Mode
		// The behavior selection in periodic mode is Enabled.
		TIMER0->CTL |= (1 << 20);
		// Enable TM0 interrup flag TIF
		TIMER0->CTL |= (1 << 29);

		// Configure Interrupt
		// Enable TM0 interrup flag TIF
		TIMER0->CTL |= (1 << 29);
		// NVIC interrupt configuration
		NVIC->ISER[1] |= (1 << 0); // (32 - 32 = 0)
		// Clear Timer 0 overflow flag
		TIMER0->INTSTS = (1 << 0); // Write 1 to clear TIF
		
		// TM0 Start Counting
		TIMER0->CTL |= (1 << 30);

    /* Clear Timer0_flag */
    Timer0_flag = 0;

    /* Reset Timer0_cnt */
    Timer0_cnt = 0;
}

void EBI_Config(void)
{
	  /* Initialize EBI bank0 to access external LCD Module */
		// Configure EBI_CTL0 for bank 0
		EBI->CTL0 &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (0b111 << 8)); // clear the current bit
	
		EBI->CTL0 |= ((1 << 0) 	// Enable EBI
									| (1 << 1) // 16-bit data bus
									| (0 << 2) // CS active low
									| (0 << 3) // Address/Data not separated
									| (1 << 4)) // Continuous access
									| (0b000 << 8); // MCLKDIV = 0 (HCLK/1)
		
		EBI->TCTL0 &= ~((1 << 22) | (1 << 23)); // clear the current bit
		EBI->TCTL0 |= (1 << 22) 	// Data Access Hold Time (tAHD) during EBI reading Disabled
								| (1 << 23);  // Data Access Hold Time (tAHD) during EBI writing Disabled
	
}
void GPIO_Config(void)
{
	  /* Set PH7/PH6 as output mode for LED1/LED2 */
		// Configure PH.7 and PH.6 as output (each pin uses 2 bits in PH->MODE)
		PH->MODE &= ~((0x3 << 14) | (0x3 << 12));  // Clear mode bits for PH.7 and PH.6
		PH->MODE |=  ((0x1 << 14) | (0x1 << 12));  // Set to push-pull output mode 

		// Set PH.7 and PH.6 high
		PH->DOUT |= (1 << 7) | (1 << 6);
}

void EADC_Config(void)
{
		EADC->CTL &= ~(1 << 8);  // Single-end analog input mode
		EADC->CTL &= ~(0b11 << 6); // clear the current bit
		EADC->CTL |= (0b11 << 6);  // 12-bit resolution
	
		EADC->CTL |= (1 << 0);						// Enable EADC
		while (!(EADC0->PWRM & (1 << 0)));			// Wait for EADC is ready for conversion
	
		// Configure sample module 0 for EADC0_CH1, software trigger
		EADC->SCTL[0] &= ~(0x1F << 16);		// TRGSEL = 0 -> Disable trigger or software trigger
		EADC->SCTL[0] &= ~(0xF << 0);		// Clear settings for channel selection
		EADC->SCTL[0] |= (1 << 0);			// Select EADC0_CH1
	
		EADC->STATUS2 = (1 << 0);			// Clear any previous interrupt flags for sure	
		EADC->SWTRG |= (1 << 0);               // Trigger conversion
}
// Function to enable Timer0 and Timer1 by setting the Counter Enable bit
void enableTimer(){
		TIMER0->CTL |= (1 << 30); // Set CEN bit to start Timer0
		TIMER1->CTL |= (1 << 30); // Set CEN bit to start Timer1
}
// Function to disable Timer0 and Timer1 by clearing the Counter Enable bit
void disableTimer(){
		TIMER0->CTL &= ~(1 << 30); // Clear CEN bit to stop Timer0
		TIMER1->CTL &= ~(1 << 30); // Clear CEN bit to stop Timer1
}