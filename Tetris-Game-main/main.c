/******************************************************************************
 * @file     main.c
 * @brief    This file is to call the function to execute states of Tetris game by user's purposes  .
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "NuMicro.h"
#include <string.h>
#include "EBI_LCD_Module.h"
#include "KEIL/gameHeader.h"
#include "KEIL/gameStage.h"
#include "KEIL/LogicGame.h"

// External variables declared in other modules for game state management
extern int isPaused;
extern TetrisBlock currentTetrisBlock;
extern TetrisBlock nextTetrisBlock;
extern int currentX, currentY;
extern int needsRedraw;
extern volatile uint8_t switchPressed;
extern uint8_t seconds;
extern uint8_t minutes;

// Main entry point of the Tetris game application
int32_t main(void)
{
    // Unlock protected registers for configuration
    SYS_UnlockReg();
    
    // Initialize system, clocks, and peripherals
    SYS_Init();
    Joystick_Init();
    
    // Configure external bus interface for LCD
    EBI_Config();

    // Initialize LCD display
    ILI9341_Initial();

    // Set backlight control pin high
    PB->DOUT |= (1 << 7);

    // Configure GPIO for LEDs
    GPIO_Config();

    // Configure ADC for touchscreen
    EADC_Config();
    
    // Initialize timers for game timing and display
    Timer1_Init();
    Timer1_SetIntervalByLevel(1);
    SW1_Interrupt_Setup();
    
    Timer0_Init();
    Timer2_Init();
    
    // Lock protected registers after configuration
    SYS_LockReg();
    
    // Stop timers initially to prevent premature triggering
    TIMER0->CTL &= ~(1 << 30);
    TIMER1->CTL &= ~(1 << 30);

    // Main game loop - runs indefinitely handling game states
    while (1) {
        // Handle different game states based on currentState variable
        switch (currentState) {
            case WELCOME_SCREEN:
                // Display welcome screen image
                LCD_Picture(0, 0, 240, 320, WelcomeScreen);
                if (switchPressed) {
                    // User pressed button to start new game - reset game variables
                    score = 0;
                    level = 1;
                    seconds = 0;
                    minutes = 0;
                    memset(board, 0, sizeof(board)); // Clear game board
                    Prepare_Next_Shape(); // Prepare first next shape
                    Next_Shape(); // Move next to current
                    currentX = SPAWN_X; // Set spawn position
                    currentY = SPAWN_Y;
                    currentTetrisBlock = CreateTetrisBlock(currentShapeIndex, currentX, currentY, 
                                             GetShapeColor(currentShapeIndex)); // Create current TetrisBlock
                    needsRedraw = 1; // Flag to redraw UI
                    isPaused = 1; // Start paused
                    currentState = PLAYING; // Transition to playing state
                    switchPressed = 0; // Clear button press
                }
                break;

            case PLAYING:
                // Clear any pending switch presses to avoid multiple triggers
                switchPressed = 0;
                if (!isPaused) {
                    // Enable game timers when not paused to allow TetrisBlock movement
                    enableTimer();
                }
                // Run main game logic (drawing, input handling, etc.)
                gameLoop();
                if (switchPressed) {
                    if (isPaused) {
                        // Unpause the game - resume timers and set state
                        isPaused = 0;
                        enableTimer();
                        needsRedraw = 1; // Force UI redraw to update level display
                    } else {
                        // Pause the game - stop timers and transition to paused state
                        isPaused = 1;
                        disableTimer();
                        currentState = PAUSED;
                    }
                    LCD_Draw_Border(); // Redraw border after state change
                    switchPressed = 0; // Clear button press
                }
                break;

            case PAUSED:
                // Display pause message on screen
                LCD_PutString(60, 140, (uint8_t *)"Paused", C_YELLOW, C_BLACK);

                if (switchPressed) {
                    // User pressed button to resume - go back to playing state
                    currentState = PLAYING;
                    switchPressed = 0;
                    TIMER0->CTL |= (1 << 30); // Manually enable Timer0
                    TIMER1->CTL |= (1 << 30); // Manually enable Timer1
                    isPaused = 0; // Clear pause flag
                    LCD_Draw_Border(); // Redraw border
                }
                break;

            case GAME_OVER: {
                // Stop all timers to prevent further game progression
                disableTimer();
                static uint8_t gameOverDrawn = 0; // Flag to draw game over screen only once
                if (!gameOverDrawn) {
                    // Display game over screen with final score
                    DisplayGameOverScreen();
                    gameOverDrawn = 1; // Prevent re-drawing
                }
                if (switchPressed) {
                    // User pressed button - save high score and go to high score screen
                    SaveHighScore(score);
                    level = 1; // Reset level on game over
                    currentState = HIGH_SCORE;
                    switchPressed = 0;
                    gameOverDrawn = 0; // Reset flag for next game
                }
                break;
            }
            case HIGH_SCORE: {
                static uint8_t highScoreDrawn = 0; // Flag to draw high scores only once
                if (!highScoreDrawn) {
                    // Clear screen and display high scores list
                    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
                    DisplayHighScores();
                    highScoreDrawn = 1; // Prevent re-drawing
                }
                if (switchPressed) {
                    // User pressed button - start new game from high score screen
                    score = 0;
                    level = 1;
                    Timer1_SetIntervalByLevel(level); // Set timer speed for level 1
                    seconds = 0;
                    minutes = 0;
                    memset(board, 0, sizeof(board)); // Reset board
                    Prepare_Next_Shape();
                    Next_Shape(); 
                    currentX = SPAWN_X;
                    currentY = SPAWN_Y;
                    currentTetrisBlock = CreateTetrisBlock(currentShapeIndex, currentX, currentY, 
                                           GetShapeColor(currentShapeIndex));
                    isPaused = 1;
                    currentState = PLAYING;
                    switchPressed = 0;
                    highScoreDrawn = 0; // Reset flag
                    needsRedraw = 1;
                }
                break;
            }
        }
    }
}