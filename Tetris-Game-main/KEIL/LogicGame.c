/******************************************************************************
 * @file     LogicGame.c
 * @brief    This file is to place  logic functions o// Function to erase the current TetrisBlock by drawing it with background color
void Erase_Current_Shape(int x, int y, uint16_t borderColor, TetrisBlock* currentTetrisBlock) {
    uint16_t originalColor = currentTetrisBlock->color; // Save original color
    currentTetrisBlock->color = BACKGROUND_COLOR; // Temporarily set to background
    DrawTetrisBlock(*currentTetrisBlock, BACKGROUND_COLOR); // Draw to erase
    currentTetrisBlock->color = originalColor; // Restore original color
}

// Function to draw the current TetrisBlock with specified border color
void Draw_Current_Shape(int x, int y, uint16_t borderColor, TetrisBlock* currentTetrisBlock) {
    DrawTetrisBlock(*currentTetrisBlock, borderColor);
}e.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NuMicro.h"
#include "LogicGame.h"
#include "gameStage.h"
#include "gameHeader.h"
#include "../EBI_LCD_Module.h"
#define PREVIEW_WIDTH  (4 * SQUARE_SIZE)
#define PREVIEW_HEIGHT (2 * SQUARE_SIZE)

// Global variables for game state, scoring, and board management
// Array to store the top high scores, initialized to zero
uint32_t highScores[MAX_HIGH_SCORES] = {0};
// Current state of the game system, starts at welcome screen
volatile SystemState currentState = WELCOME_SCREEN; // Actual definition
// Shape Rotation 
// All shapes in default orientation (rotation 0)
// Timer compare values for different levels, decreasing for faster TetrisBlock fall speed
const uint32_t level_cmp[11] = {
    0, // unused, index starts at 1
    5999999, // Level 1: 0.5s 
    5399999, // Level 2: 0.45s
    4799999, // Level 3: 0.4s
    4199999, // Level 4: 0.35s
    3599999, // Level 5: 0.3s
    2999999, // Level 6: 0.25s
    2399999, // Level 7: 0.2s
    1799999, // Level 8: 0.15s
    1199999, // Level 9: 0.1s
    599999   // Level 10: 0.05s
};
// Current player score, accumulates points from cleared lines
uint32_t score = 0;
// Current game level, increases with score
uint8_t level = 1;
// External flags and variables for timer and time tracking from other modules
extern volatile uint8_t Timer0_flag;
extern volatile uint8_t timer_running;
extern uint8_t seconds;
extern uint8_t minutes;
// 2D array representing the game board, where 0 means empty and non-zero means occupied by a TetrisBlock color
int board[BOARD_HEIGHT_2D][BOARD_WIDTH_2D + 1] = {0};


void Rotate_Clockwise(TetrisBlock* shape) {
    if (currentShapeIndex == 3)  // O shape does not rotate
    {
        return;}
        
    TetrisBlock original = *shape;
	int i;

    for (i = 0; i < 4; i++) {
        int x = shape->blocks[i].x - original.blocks[1].x; // Calculate relative x from pivot
        int y = shape->blocks[i].y - original.blocks[1].y; // Calculate relative y from pivot
        shape->blocks[i].x = original.blocks[1].x - y; // New x: pivot_x - relative_y
        shape->blocks[i].y = original.blocks[1].y + x; // New y: pivot_y + relative_x
    }
    // If rotation causes collision, revert to original orientation
    if (Check_Collision(shape)) {
        *shape = original; // Revert if collision
    }
}


// Function to randomly select the next TetrisBlock shape index (0-6)
void Prepare_Next_Shape() {
    nextShapeIndex = rand() % 7;
}

// Function to create a TetrisBlock structure based on shape index, position, and color
TetrisBlock CreateTetrisBlock(int shapeIndex, uint16_t pixelX, uint16_t pixelY, uint16_t color) {
    TetrisBlock t;
    t.color = color;
    
    // Set the positions of the 4 blocks based on the shape index
    switch (shapeIndex) {
        case 0: // I-shape (horizontal) - straight line
            t.blocks[0].x = pixelX + 0*SQUARE_SIZE; t.blocks[0].y = pixelY;
            t.blocks[1].x = pixelX + 1*SQUARE_SIZE; t.blocks[1].y = pixelY;
            t.blocks[2].x = pixelX + 2*SQUARE_SIZE; t.blocks[2].y = pixelY;
            t.blocks[3].x = pixelX + 3*SQUARE_SIZE; t.blocks[3].y = pixelY;
            break;
            
        case 1: // J-shape - L with hook on left
            t.blocks[0].x = pixelX + 0*SQUARE_SIZE; t.blocks[0].y = pixelY;
            t.blocks[1].x = pixelX + 0*SQUARE_SIZE; t.blocks[1].y = pixelY + SQUARE_SIZE;
            t.blocks[2].x = pixelX + 1*SQUARE_SIZE; t.blocks[2].y = pixelY + SQUARE_SIZE;
            t.blocks[3].x = pixelX + 2*SQUARE_SIZE; t.blocks[3].y = pixelY + SQUARE_SIZE;
            break;
            
        case 2: // L-shape - L with hook on right
            t.blocks[0].x = pixelX + 2*SQUARE_SIZE; t.blocks[0].y = pixelY;
            t.blocks[1].x = pixelX + 0*SQUARE_SIZE; t.blocks[1].y = pixelY + SQUARE_SIZE;
            t.blocks[2].x = pixelX + 1*SQUARE_SIZE; t.blocks[2].y = pixelY + SQUARE_SIZE;
            t.blocks[3].x = pixelX + 2*SQUARE_SIZE; t.blocks[3].y = pixelY + SQUARE_SIZE;
            break;
            
        case 3: // O-shape - square, no rotation
            t.blocks[0].x = pixelX + 0*SQUARE_SIZE; t.blocks[0].y = pixelY;
            t.blocks[1].x = pixelX + 1*SQUARE_SIZE; t.blocks[1].y = pixelY;
            t.blocks[2].x = pixelX + 0*SQUARE_SIZE; t.blocks[2].y = pixelY + SQUARE_SIZE;
            t.blocks[3].x = pixelX + 1*SQUARE_SIZE; t.blocks[3].y = pixelY + SQUARE_SIZE;
            break;
            
        case 4: // S-shape - S curve
            t.blocks[0].x = pixelX + 1*SQUARE_SIZE; t.blocks[0].y = pixelY;
            t.blocks[1].x = pixelX + 2*SQUARE_SIZE; t.blocks[1].y = pixelY;
            t.blocks[2].x = pixelX + 0*SQUARE_SIZE; t.blocks[2].y = pixelY + SQUARE_SIZE;
            t.blocks[3].x = pixelX + 1*SQUARE_SIZE; t.blocks[3].y = pixelY + SQUARE_SIZE;
            break;
            
        case 5: // T-shape - T
            t.blocks[0].x = pixelX + 0*SQUARE_SIZE; t.blocks[0].y = pixelY + SQUARE_SIZE;
            t.blocks[1].x = pixelX + 1*SQUARE_SIZE; t.blocks[1].y = pixelY + SQUARE_SIZE;
            t.blocks[2].x = pixelX + 2*SQUARE_SIZE; t.blocks[2].y = pixelY + SQUARE_SIZE;
            t.blocks[3].x = pixelX + 1*SQUARE_SIZE; t.blocks[3].y = pixelY;
            break;
            
        case 6: // Z-shape - Z curve
            t.blocks[0].x = pixelX + 0*SQUARE_SIZE; t.blocks[0].y = pixelY;
            t.blocks[1].x = pixelX + 1*SQUARE_SIZE; t.blocks[1].y = pixelY;
            t.blocks[2].x = pixelX + 1*SQUARE_SIZE; t.blocks[2].y = pixelY + SQUARE_SIZE;
            t.blocks[3].x = pixelX + 2*SQUARE_SIZE; t.blocks[3].y = pixelY + SQUARE_SIZE;
            break;
    }
    
    return t;
}

// Function to promote the next shape to current and prepare a new next shape
void Next_Shape() {
    currentShapeIndex = nextShapeIndex;
    Prepare_Next_Shape(); // Generate new next shape
}
// Function to draw the next TetrisBlock shape at specified position with border color
void Draw_next_Shape(int x, int y, uint16_t borderColor, TetrisBlock* nextTetrisBlock) {
    *nextTetrisBlock = CreateTetrisBlock(nextShapeIndex, x, y, GetShapeColor(nextShapeIndex));
    DrawTetrisBlock(*nextTetrisBlock, borderColor);
}
// Function to erase the next TetrisBlock shape by drawing in background color
void Erase_next_Shape(int x, int y, uint16_t borderColor, TetrisBlock* nextTetrisBlock) {
    // Clear the whole preview area (adjust width/height as needed)
    LCD_BlankArea(x, y, PREVIEW_WIDTH + 5, PREVIEW_HEIGHT + 5, BACKGROUND_COLOR);

    // Now draw the next shape in background color to ensure full erase
    *nextTetrisBlock = CreateTetrisBlock(nextShapeIndex, x, y, BACKGROUND_COLOR);
    DrawTetrisBlock(*nextTetrisBlock, BACKGROUND_COLOR);
}
// Function to erase the current TetrisBlock by drawing in background color
void Erase_Current_Shape(int x, int y, uint16_t borderColor, TetrisBlock* currentTetrisBlock) {
    uint16_t originalColor = currentTetrisBlock->color; // Save original color
    currentTetrisBlock->color = BACKGROUND_COLOR; // Temporarily set to background
    DrawTetrisBlock(*currentTetrisBlock, BACKGROUND_COLOR); // Draw to erase
    currentTetrisBlock->color = originalColor; // Restore original color
}

// Function to draw the current TetrisBlock with specified border color
void Draw_Current_Shape(int x, int y, uint16_t borderColor, TetrisBlock* currentTetrisBlock) {
    DrawTetrisBlock(*currentTetrisBlock, borderColor);
}

// Shape movement
// Function to set Timer1 interval based on current level for TetrisBlock fall speed
void Timer1_SetIntervalByLevel(uint8_t level)
{
    if (level < 1) level = 1; // Clamp level to minimum
    if (level > 10) level = 10; // Clamp level to maximum

    TIMER1->CMP = level_cmp[level]; // Set compare value for timer
}
// Function to update the player's score and level based on points earned
void UpdateScore(uint32_t points) {
    score += points; // Add points to total score
    uint8_t newLevel = (score / POINTS_PER_LEVEL) + 1; // Calculate new level
    if (newLevel > MAX_LEVEL) newLevel = MAX_LEVEL; // Cap at max level
    if (newLevel != level) {
        level = newLevel; // Update level
        Timer1_SetIntervalByLevel(level); // Adjust fall speed
        needsRedraw = 1; // Force UI update for new level
    }
    needsRedraw = 1; // Force UI update for new score
}

// Function to calculate points based on the number of lines cleared
int CalculatePoints(int linesCleared) {
    switch (linesCleared) {
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        default: return 0;
    }
}


// Function to check if a TetrisBlock collides with board boundaries or occupied cells
int Check_Collision(TetrisBlock *t) {
    int i;
    for (i = 0; i < 4; i++) {
        // Convert pixel coordinates to grid coordinates (each square is SQUARE_SIZE pixels)
        int gridX = t->blocks[i].x / SQUARE_SIZE; // Convert pixel x to grid x
        int gridY = t->blocks[i].y / SQUARE_SIZE; // Convert pixel y to grid y

           if (gridX <= 0 || gridX >= BOARD_WIDTH_2D + 1 
                || gridY <= 0 || gridY >= BOARD_HEIGHT_2D - 1) // Check boundary collision

            return 1;
        // Check if cell is occupied (only if within vertical bounds)
        if (gridY >= 0 && board[gridY][gridX] != 0) // Check board collision
           return 1;
    }
    return 0; // No collision
}
// Function to lock the TetrisBlock's position into the board array
void Lock_TetrisBlock_To_Board(TetrisBlock* t) {
	int i;
    for (i = 0; i < 4; i++) {
        int gridX = t->blocks[i].x / SQUARE_SIZE; // Convert to grid coordinates
        int gridY = t->blocks[i].y / SQUARE_SIZE;

        if (gridY >= 1 && gridY <= BOARD_HEIGHT_2D 
            && gridX >= 1 && gridX <= BOARD_WIDTH_2D) { // Ensure within board bounds
                
            board[gridY][gridX] = t->color; // Set board cell to TetrisBlock color
        }
    }
}
// Function to check for and clear full lines, shifting rows down and updating score
int CheckAndClearLines() {
    int row;
    int col;
    int r;
    int linesCleared = 0;
    // Scan from bottom to top (excluding top and bottom borders)
    for (row = BOARD_HEIGHT_2D - 2; row >= 1; row--) {
        int isFull = 1; // Assume row is full
        // Check columns 1 to BOARD_WIDTH_2D for emptiness
        for (col = 1; col <= BOARD_WIDTH_2D; col++) {
            if (board[row][col] == 0) {
                isFull = 0; // Not full if any cell empty
                break;
            }
        }
        if (isFull) {
            // Shift all rows above down by one
            for (r = row; r > 0; r--) {
                memcpy(board[r], board[r - 1], sizeof(board[r]));
            }
            // Clear the top row
            memset(board[0], 0, sizeof(board[0]));
            linesCleared++; // Increment cleared lines
            ++row; // Re-check the same row index after shift
        }
    }
    
    // Update score if lines were cleared
    if (linesCleared > 0) {
        int points = CalculatePoints(linesCleared);
        UpdateScore(points);
    }
    
    return linesCleared;
}


// Function to display and update the game timer
void displayTime(void)
{
	  if (Timer0_flag == 1) // If timer interrupt occurred
    {
        Timer0_flag = 0; // Reset flag

        if (timer_running) // If timer is active
        {
            seconds++; // Increment seconds
            if (seconds >= 60)
            {
                seconds = 0; // Reset seconds
                minutes++; // Increment minutes
            }

            LCD_DisplayTime(minutes, seconds); // Update LCD display
        }
    }
}


// Function to instantly drop the TetrisBlock to the bottom, lock it, and spawn next
void HardDrop() {
    // Loop until collision occurs
    while (1) {
        // Create a temporary copy of the CURRENT (rotated) tetris block
        TetrisBlock temp = currentTetrisBlock;
        int i;
        // Move all blocks down
        for (i = 0; i < 4; i++) {
            temp.blocks[i].y += SQUARE_SIZE; // Simulate drop
        }
        // Check collision with the rotated shape
        if (Check_Collision(&temp)) break; // Stop if collision
        // Erase using rotated state
        Erase_Current_Shape(currentX, currentY, BACKGROUND_COLOR, &currentTetrisBlock);
        currentY += SQUARE_SIZE; // Update position
        currentTetrisBlock = temp; // Preserve rotation
        // Draw using rotated state
        Draw_Current_Shape(currentX, currentY, 0x0000, &currentTetrisBlock);
    }
    // Lock the dropped TetrisBlock
    Lock_TetrisBlock_To_Board(&currentTetrisBlock);
    CheckAndClearLines(); // Clear any full lines
    
    DelayMs(300); // Pause briefly
    // Prepare next shape
    Erase_next_Shape(180, 40, 0x0000, &nextTetrisBlock);
    Next_Shape();
    currentX = SPAWN_X;
    currentY = SPAWN_Y;
    currentTetrisBlock = CreateTetrisBlock(currentShapeIndex, currentX, currentY, GetShapeColor(currentShapeIndex));
    needsRedraw = 1; // Force redraw after hard drop
}
// Function to move the current TetrisBlock down, handle collisions, line clearing, and game over
void MoveTetrisBlockDown() {

    // Create a temporary tetris block by copying the CURRENT (possibly rotated) tetris block
    TetrisBlock temp = currentTetrisBlock;
    int i;

    // Move all blocks of the temporary tetris block down by SQUARE_SIZE
    for (i = 0; i < 4; i++) {
        temp.blocks[i].y += SQUARE_SIZE; // Simulate downward movement
    }

    // Check collision with the TEMPORARY (rotated) tetris block
    if (!Check_Collision(&temp)) {
        // No collision: move down
        // Erase current position
        Erase_Current_Shape(currentX, currentY, BACKGROUND_COLOR, &currentTetrisBlock);
        
        // Update Y position
        currentY += SQUARE_SIZE;
        
        // Update the actual currentTetrisBlock's position
        currentTetrisBlock = temp; // Preserve rotation state
        
        // Redraw at new position
        Draw_Current_Shape(currentX, currentY, 0x0000, &currentTetrisBlock);
    } else {
        // Collision: lock tetris block, check lines, handle game over
        Lock_TetrisBlock_To_Board(&currentTetrisBlock);
        CheckAndClearLines();
        DelayMs(300); // Brief pause after locking
        // Game Over Check (top row) - if any block reaches the top, game ends
        int gameOver = 0;
        int col;
        for (col = 1; col < BOARD_WIDTH_2D; col++) {
            if (board[1][col] != 0) { // If top row has blocks
                gameOver = 1;
                break;
            }
        }
        if (gameOver) {
            currentState = GAME_OVER;  // Transition to game-over state
            level = 1; // Reset level immediately on game over
            TIMER0->CTL &= ~(1 << 30); // Stop timers
            TIMER1->CTL &= ~(1 << 30);
        } else {
            // Normal shape promotion
            Erase_next_Shape(180, 40, 0x0000, &nextTetrisBlock);
            Next_Shape();
            currentX = SPAWN_X;
            currentY = SPAWN_Y;
            currentTetrisBlock = CreateTetrisBlock(currentShapeIndex, currentX, currentY, 
                                       GetShapeColor(currentShapeIndex));
        }
    }
}
// Function to move the TetrisBlock left if no collision
void MoveTetrisBlockLeft() {
    TetrisBlock temp = currentTetrisBlock; // Create temporary copy
    int i;
    // Simulate left movement
    for (i = 0; i < 4; i++) {
        temp.blocks[i].x -= SQUARE_SIZE;
    }
    if (!Check_Collision(&temp)) { // If no collision
        Erase_Current_Shape(currentX, currentY, 
            BACKGROUND_COLOR, &currentTetrisBlock); // Erase old
        currentX -= SQUARE_SIZE; // Update position
        currentTetrisBlock = temp; // Update TetrisBlock
        Draw_Current_Shape(currentX, currentY, 
            0x0000, &currentTetrisBlock); // Draw new
        needsRedraw = 1; // Force redraw
    }
}
// Function to move the TetrisBlock right if no collision
void MoveTetrisBlockRight() {
     TetrisBlock temp = currentTetrisBlock; // Create temporary copy
    int i;
    // Simulate right movement
    for (i = 0; i < 4; i++) {
        temp.blocks[i].x += SQUARE_SIZE;
    }
    if (!Check_Collision(&temp)) { // If no collision
        Erase_Current_Shape(currentX, currentY, BACKGROUND_COLOR, &currentTetrisBlock); // Erase old
        currentX += SQUARE_SIZE; // Update position
        currentTetrisBlock = temp; // Update TetrisBlock
        Draw_Current_Shape(currentX, currentY, 0x0000, &currentTetrisBlock); // Draw new
        needsRedraw = 1; // Force redraw
    }
}

// Function to save a new high score if it qualifies
void SaveHighScore(uint32_t newScore) {
	int i; 
	int j;
    // Check if score is already in the list (prevent duplicates)
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        if (newScore == highScores[i]) {
            return; // Already exists, don't add
        }
    }
    // Insert new score into the high scores array if higher than existing
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        if (newScore > highScores[i]) {
            // Shift lower scores down
            for (j = MAX_HIGH_SCORES - 1; j > i; j--) {
                highScores[j] = highScores[j - 1];
            }
            highScores[i] = newScore; // Insert new score
            break;
        }
    }
}
// Function to calculate the shadow position (hard drop preview) for a TetrisBlock
void GetShadowPosition(TetrisBlock *currentTetrisBlock, int *shadowY) {
    TetrisBlock shadow = *currentTetrisBlock; // Copy the TetrisBlock
    int y = currentY; // Start from current Y
    int i;
    // Drop the shadow until collision
    while (1) {
        TetrisBlock temp = shadow; // Copy for simulation
        for (i = 0; i < 4; i++) {
            temp.blocks[i].y += SQUARE_SIZE; // Move down
        }
        if (Check_Collision(&temp)) break; // Stop on collision
        y += SQUARE_SIZE; // Update shadow Y
        shadow = temp; // Update shadow
    }
    *shadowY = y; // Return the shadow Y position
}
// Function to draw the shadow TetrisBlock at specified position
void Draw_Shadow_TetrisBlock(int x, int y, TetrisBlock *tetrisBlock) {
    TetrisBlock shadow = *tetrisBlock; // Copy TetrisBlock
    int i;
    // Adjust shadow position to the calculated Y
    for (i = 0; i < 4; i++) {
        shadow.blocks[i].y += (y - currentY); // Offset to shadow Y
    }
    // Use a shadow color, e.g., 0xC618 (light gray)
    for (i = 0; i < 4; i++) {
        LCD_Draw_FilledSquare(shadow.blocks[i].x, 
            shadow.blocks[i].y, 0xC618, 0x0000); // Draw shadow squares
    }
}
// Function to create a delay in milliseconds using busy-wait loop
void DelayMs(uint32_t ms) {
    // Simple busy-wait loop for delay (not accurate, but works for short delays)
    volatile uint32_t count;
    while (ms--) { // Loop for each millisecond
        for (count = 0; count < 8000; count++) { // Inner loop calibrated for ~1ms
            __NOP(); // No Operation (adjust 8000 for your MCU speed)
        }
    }
}
