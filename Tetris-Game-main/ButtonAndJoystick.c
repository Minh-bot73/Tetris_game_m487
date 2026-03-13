/******************************************************************************
 * @file     ButtonAndJoystick.c
 * @brief    This file handles button and joystick input setup and interrupts.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "NuMicro.h"
#include "KEIL/gameHeader.h"
#include "KEIL/gameStage.h"
#include "KEIL/LogicGame.h"

extern volatile uint32_t millis;
extern volatile uint8_t switchPressed;
extern volatile uint32_t lastSW1Time;
extern uint32_t lastUpTime;
extern uint32_t lastRightTime;
extern uint32_t lastLeftTime;
extern uint32_t lastDownTime;
extern int isPaused;
extern int currentX, currentY;
extern TetrisBlock currentTetrisBlock;
extern int needsRedraw;

void SW1_Interrupt_Setup(void)
{
		//Configure PA.0 (SW1) as input mode
		PA->MODE &= ~(0x3 << 0); 		// Clear bits [1:0] for PA.0
		
	
		PA->INTTYPE &= ~(1 << 0); 		// Edge trigger interrupt for PA.0
		PA->INTEN |= (1 << 0); 		// Falling edge interrupt enable
		PA->INTSRC |= (1 << 0);		// Clear any pending interrupt flag for PA.0

		// NVIC interrupt configuration
		NVIC->ISER[0] |= (1 << 16); 		// Enable NVIC for the GPIO interrupt on Port A 											
}
void GPA_IRQHandler(void) {
    if (PA->INTSRC & (1 << 0)) {
        PA->INTSRC |= (1 << 0); // Clear interrupt flag
        if (millis - lastSW1Time > 150) {
            if (!(PA->PIN & (1 << 0))) {
                switchPressed = 1;
                lastSW1Time = millis;
            }
        }
    }
}

void GPG_IRQHandler(void) {
    if (PG->INTSRC & (1 << 2)) { // UP
        PG->INTSRC |= (1 << 2);
        if (currentState == PLAYING && !isPaused && millis - lastUpTime > 200) {
            Erase_Current_Shape(currentX, currentY, 0x0000, &currentTetrisBlock);
            Rotate_Clockwise(&currentTetrisBlock);
            Draw_Current_Shape(currentX, currentY, 0x0000, &currentTetrisBlock);
            needsRedraw = 1;
            lastUpTime = millis;
        }
    }
    if (PG->INTSRC & (1 << 4)) { // RIGHT
        PG->INTSRC |= (1 << 4);
        if (currentState == PLAYING && !isPaused && millis - lastRightTime > 150) {
            MoveTetrisBlockRight();
            lastRightTime = millis;
        }
    }
}

void GPC_IRQHandler(void) {
    if (PC->INTSRC & (1 << 9)) { // LEFT
        PC->INTSRC |= (1 << 9);
        if (currentState == PLAYING && !isPaused && millis - lastLeftTime > 150) {
            MoveTetrisBlockLeft();
            lastLeftTime = millis;
        }
    }
    if (PC->INTSRC & (1 << 10)) { // DOWN
        PC->INTSRC |= (1 << 10);
        if (currentState == PLAYING && !isPaused && millis - lastDownTime > 150) {
            HardDrop();
            lastDownTime = millis;
        }
    }
}

void Joystick_Init(void)
{
    // Set PG.2(UP), PG.4(RIGHT) as input mode
    PG->MODE &= ~((0x3 << 4) | (0x3 << 8)); // PG.2, PG.4
		PG->PUSEL &= ~((0x3 << 4) | (0x3 << 8));
		PG->PUSEL |=  ((0x1 << 4) | (0x1 << 8));

    // Set PC.9(LEFT) and PC.10(DOWN) as input mode
    PC->MODE &= ~((0x3 << 18) | (0x3 << 20)); // PC.9, PC.10
		PC->PUSEL &= ~((0x3 << 18) | (0x3 << 20));
		PC->PUSEL |=  ((0x1 << 18) | (0x1 << 20));

    // Setup interrupts for PG.2 and PG.4
    PG->INTTYPE &= ~((1 << 2) | (1 << 4)); // Edge trigger
    PG->INTEN |= ((1 << 2) | (1 << 4)); // Falling edge interrupt enable
    PG->INTSRC |= ((1 << 2) | (1 << 4)); // Clear interrupt flags

    // Setup interrupts for PC.9 and PC.10
    PC->INTTYPE &= ~((1 << 9) | (1 << 10)); // Edge trigger
    PC->INTEN |= ((1 << 9) | (1 << 10)); // Falling edge interrupt enable
    PC->INTSRC |= ((1 << 9) | (1 << 10)); // Clear interrupt flags

    // Enable NVIC interrupts
    NVIC->ISER[2] |= (1 << 8); // GPG interrupt
    NVIC->ISER[0] |= (1 << 18); // GPC interrupt
}