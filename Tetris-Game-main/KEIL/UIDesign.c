/******************************************************************************
 * @file     UID    LCD_PutString(sidebarX, lineY, (uint8_t *)"HISCORE", C_RED, 0x2104);sign.c
 * @brief    This file is to place draw functions, the game loop of the PLAYING state.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
/* Header files */
#include <stdio.h>
#include "NuMicro.h"

#include "gameHeader.h"
#include "../EBI_LCD_Module.h"
#include "LogicGame.h"
#include "gameStage.h"

/* Global variables for game state and UI */
int needsRedraw = 1;
TetrisBlock currentTetrisBlock;
TetrisBlock nextTetrisBlock;
int nextShapeIndex = -1;
int currentShapeIndex = -1;
int currentX, currentY;
int isPaused = 0;
int highestScore = 0;
extern int board[BOARD_HEIGHT_2D][BOARD_WIDTH_2D + 1];
extern uint32_t highScores[MAX_HIGH_SCORES];

/* Function to draw an image on the LCD screen */
void LCD_Picture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *img)
{
    uint32_t i;
    LCD_SetWindow(x, x + width - 1, y, y + height - 1);
    for (i = 0; i < width * height; i++) {
        LCD_WR_DATA(img[i]);
    }
}

/* Function to draw a filled square with border and fill colors */
void LCD_Draw_FilledSquare(uint16_t x, uint16_t y, uint16_t fillColor, uint16_t borderColor) {
    
		uint16_t i;
    LCD_SetWindow(x, x + SQUARE_SIZE - 1, y, y + SQUARE_SIZE - 1);
    for (i = 0; i < SQUARE_SIZE * SQUARE_SIZE; i++) {
        LCD_WR_DATA(borderColor);
    }

    LCD_SetWindow(x + 1, x + SQUARE_SIZE - 2, y + 1, y + SQUARE_SIZE - 2);
    for (i = 0; i < 8 * 8; i++) {
        LCD_WR_DATA(fillColor);
    }
}
/* Function to draw a border of squares around a rectangle */
void LCD_Draw_SquareBorder(uint16_t x, uint16_t y, uint16_t rows, uint16_t cols, uint16_t fillColor, uint16_t borderColor) {
    
    uint16_t i;

    for (i = 0; i < cols; i++) {
        LCD_Draw_FilledSquare(x + i * SQUARE_SIZE, y, fillColor, borderColor);
    }

    for (i = 0; i < cols; i++) {
        LCD_Draw_FilledSquare(x + i * SQUARE_SIZE, y + (rows - 1) * SQUARE_SIZE, fillColor, borderColor);
    }

    for (i = 1; i < rows - 1; i++) {
        LCD_Draw_FilledSquare(x, y + i * SQUARE_SIZE, fillColor, borderColor);
    }

    for (i = 1; i < rows - 1; i++) {
        LCD_Draw_FilledSquare(x + (cols - 1) * SQUARE_SIZE, y + i * SQUARE_SIZE, fillColor, borderColor);
    }
}
/* Function to draw a row of squares */
void LCD_Draw_SquareRow(uint16_t x, uint16_t y, uint16_t cols, uint16_t fillColor, uint16_t borderColor) {
		uint16_t i;
    for (i = 0; i < cols; i++) {
        LCD_Draw_FilledSquare(x + i * SQUARE_SIZE, y, fillColor, borderColor);
    }
}
/* Function to draw a column of squares */
void LCD_Draw_SquareColumn(uint16_t x, uint16_t y, uint16_t rows, uint16_t fillColor, uint16_t borderColor) {
		uint16_t i;
    for (i = 0; i < rows; i++) {
        LCD_Draw_FilledSquare(x, y + i * SQUARE_SIZE, fillColor, borderColor);
    }
}

/* Function to draw the sidebar UI elements */
void Draw_Sidebar(void) {
    uint16_t sidebarX = 180;
    uint16_t sidebarX1 = 165;
    uint16_t lineY;
    lineY = 11;
    LCD_PutString(sidebarX, lineY, (uint8_t *)"NEXT", C_RED, 0x2104);

    lineY = 81;
    LCD_PutString(sidebarX, lineY, (uint8_t *)"HISCO", C_RED, 0x2104);

    lineY = 125;
    LCD_PutString(sidebarX, lineY, (uint8_t *)"SCORE", C_RED, 0x2104);

    lineY = 175;
    LCD_PutString(sidebarX, lineY, (uint8_t *)"TIME", C_RED, 0x2104);

    lineY = 225;
    LCD_PutString(sidebarX, lineY, (uint8_t *)"LEVEL", C_RED, 0x2104);

    lineY = 271;
    LCD_PutString(
        sidebarX1, lineY, (uint8_t *)"PLAY",
        isPaused ? C_WHITE : C_YELLOW,
        0x2104
    );

    lineY = 291;
    LCD_PutString(
        sidebarX1, lineY, (uint8_t *)"PAUSE",
        isPaused ? C_YELLOW : C_WHITE,
        0x2104
    );
}
/* Function to draw the game border and background */
void LCD_Draw_Border(){
	LCD_Picture(0, 0, 240, 320, backgroundScreenPlay);


	LCD_Draw_SquareBorder(0, 0, 32, 24, C_MAGENTA, 0x0000);  


	LCD_Draw_SquareColumn(15 * SQUARE_SIZE, 0, 31, C_MAGENTA, 0x0000);  

	LCD_Draw_SquareRow(16 * SQUARE_SIZE, 7 * SQUARE_SIZE, 7, C_MAGENTA, 0x0000);

	LCD_Draw_SquareRow(16 * SQUARE_SIZE, 26 * SQUARE_SIZE, 7, C_MAGENTA, 0x0000);  	
	Draw_Sidebar();
}

/* Function to display the game time on the sidebar */
void LCD_DisplayTime(uint8_t min, uint8_t sec)
{
		uint16_t sidebarX = 180;  
		uint16_t sidebarX1 = 170;
    uint16_t lineY;
    char buffer[6];
    sprintf(buffer, "%02d:%02d", min, sec);
		
		lineY = 195;

    LCD_PutString(sidebarX, lineY, (uint8_t *)buffer, C_BLUE, 0x2104);
}

/* Function to draw the current score and level */
void DisplayScoreLevel(void) {
    uint16_t sidebarX = 180;  
    char buffer[16];          

    sprintf(buffer, "%u", score); 
    LCD_PutString(sidebarX, 145, (uint8_t *)buffer, C_BLUE, 0x2104);

    sprintf(buffer, "%u", level);
    LCD_PutString(sidebarX, 245, (uint8_t *)buffer, C_BLUE, 0x2104); 
}

/* Function to draw the highest score */
void DisplayHighestScore(void) {
    uint16_t sidebarX = 180;
    char buffer[16];
    int i;

    if (score > highestScore) {
        highestScore = score;
    }

    sprintf(buffer, "%u", highestScore);
    LCD_PutString(sidebarX, 101, (uint8_t *)buffer, C_BLUE, 0x2104);
}

/* Main game loop for the playing state */
void gameLoop() {
    static uint8_t initialized = 0;
    displayTime(); // Update the displayed time

    if (currentState == PLAYING) {
        if (!initialized) {
            LCD_Draw_Border(); // Draw the initial border
            initialized = 1;
        }

        if (needsRedraw && !isPaused) {
            needsRedraw = 0; // Reset redraw flag
            DrawBoard(); // Draw the game board
            DisplayScoreLevel(); // Draw score and level
            DisplayHighestScore(); // Draw highest score
            
            int shadowY;
            GetShadowPosition(&currentTetrisBlock, &shadowY); // Get shadow position
            Draw_Shadow_TetrisBlock(currentX, shadowY, &currentTetrisBlock); // Draw shadow

            DrawTetrisBlock(currentTetrisBlock, 0x0000); // Draw current tetris block

            uint16_t nextX = 180;
            uint16_t nextY = 40;

            Draw_next_Shape(nextX, nextY, 0x0000, &nextTetrisBlock); // Draw next shape
        }
    } else {
        initialized = 0; // Reset initialization for other states
    }
}

/* Function to draw a TetrisBlock piece */
void DrawTetrisBlock(TetrisBlock shape, uint16_t borderColor) {
	int i;
    for (i = 0; i < 4; i++) {
        LCD_Draw_FilledSquare(shape.blocks[i].x, shape.blocks[i].y, shape.color, borderColor);
    }
}

/* Function to get the color for a shape index */
uint16_t GetShapeColor(int shapeIndex) {
    // Switch statement to return color based on shape index
    switch(shapeIndex) {
        case 0: return C_CYAN;
        case 1: return C_BLUE;
        case 2: return C_ORANGE;
        case 3: return C_YELLOW;
        case 4: return C_GREEN;
        case 5: return C_PURPLE;
        case 6: return C_RED;
        default: return 0xFFFF;
    }
}

/* Function to draw the game board */
void DrawBoard(void) {
    int row, col;
    // Loop through each row and column of the board
    for (row = 1; row < 31; row++) {
        for (col = 1; col <= BOARD_WIDTH_2D; col++) {
            if (board[row][col] != 0) {
                // Draw filled square for occupied cells
                LCD_Draw_FilledSquare(
                    col * SQUARE_SIZE,
                    row * SQUARE_SIZE,
                    board[row][col],
                    0x0000
                );
            } else {
                // Draw empty square for unoccupied cells
                LCD_Draw_FilledSquare(
                    col * SQUARE_SIZE,
                    row * SQUARE_SIZE,
                    0x0000,
                    0x0000
                );
            }
        }
    }
}

/* Function to display the high scores screen */
void DisplayHighScores(void) {
    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
    LCD_PutString(50, 40, (uint8_t *)"TOP 5 SCORES", C_YELLOW, C_BLACK);

    char buffer[32];
    int y = 80;
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        sprintf(buffer, "%d. %u", i + 1, highScores[i]);
        LCD_PutString(60, y, (uint8_t *)buffer, C_WHITE, C_BLACK);
        y += 30;
    }

    LCD_PutString(30, y + 20, (uint8_t *)"Press SW1 to try again.", C_GREEN, C_BLACK);
    LCD_PutString(90, y + 50, (uint8_t *)"GOGOGO!!!", C_GREEN, C_BLACK);
}

/* Function to display the game over screen */
void DisplayGameOverScreen(void) {
    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
    LCD_PutString(40, 100, (uint8_t *)"GAME OVER - NICE TRY :)", C_WHITE, C_BLACK);
    LCD_PutString(40, 140, (uint8_t *)"PRESS SW1 to skip", C_YELLOW, C_BLACK);
}