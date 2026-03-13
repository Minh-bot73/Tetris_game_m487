/******************************************************************************
 * @file     LogicGame.h
 * @brief    Header file for game logic definitions, structures, and function prototypes.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef LOGICGAME_H
#define LOGICGAME_H


/* ============================================================================
 * INCLUDES
 * ============================================================================ */

#include <stdint.h>


/* ============================================================================
 * DEFINES AND CONSTANTS
 * ============================================================================ */

// Grid dimensions (blocks)
#define BOARD_WIDTH_2D 14
#define BOARD_HEIGHT_2D 32  // exclude 2 unit for top and down border

// Spawn position (centered at top)
#define SPAWN_X   60  // unit(10x10 for each unit)
#define SPAWN_Y   10  // unit(10x10 for each unit)

// Game constants
#define MAX_HIGH_SCORES 5
#define POINTS_PER_LEVEL 5
#define MAX_LEVEL 10

// Colors
#define BACKGROUND_COLOR 0x10a2  // I shape


/* ============================================================================
 * STRUCTURES AND TYPEDEFS
 * ============================================================================ */

// Block structure
typedef struct {
    int x, y;
} Block;

// TetrisBlock structure
typedef struct {
    Block blocks[4];   // A tetris block has 4 blocks
    uint16_t color;    // 16-bit RGB565 color
} TetrisBlock;

// Function pointer for shape drawing
typedef void (*ShapeDrawFunc)(uint16_t x, uint16_t y, uint16_t borderColor);


/* ============================================================================
 * EXTERNAL VARIABLES
 * ============================================================================ */

// Game state variables
extern int nextShapeIndex;     // Holds next shape to appear
extern int currentShapeIndex;  // Holds current shape
extern int currentY;
extern int currentX;

// Board and scoring
extern int board[BOARD_HEIGHT_2D][BOARD_WIDTH_2D + 1];  // 0 = empty, 1 = filled
extern uint32_t score;         // Current score
extern uint8_t level;          // Current level
extern uint32_t highScores[MAX_HIGH_SCORES];

// TetrisBlock instances
extern TetrisBlock currentTetrisBlock;
extern TetrisBlock nextTetrisBlock;

// TetrisBlock shapes array
extern TetrisBlock shapes[7];

// Shape draw functions and names
extern ShapeDrawFunc shapeFunctions[7];
extern const char* shapeNames[7];


/* ============================================================================
 * FUNCTION
 * ============================================================================ */

// Helper functions
void DelayMs(uint32_t ms);

// TetrisBlock manipulation
void Rotate_Clockwise(TetrisBlock* shape);
void DrawTetrisBlock(TetrisBlock shape, uint16_t borderColor);
TetrisBlock CreateTetrisBlock(int shapeIndex, uint16_t pixelX, uint16_t pixelY, uint16_t color);

// Shape drawing functions
void Draw_I_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_J_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_L_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_O_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_S_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_T_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_Z_Shape(uint16_t x, uint16_t y, uint16_t borderColor);

// Game shape controls
void Prepare_Next_Shape(void);
void Next_Shape(void);
void Draw_Current_Shape(int x, int y, uint16_t borderColor, TetrisBlock* currentTetrisBlock);
void Draw_next_Shape(int x, int y, uint16_t borderColor, TetrisBlock* nextTetrisBlock);
void Erase_Current_Shape(int x, int y, uint16_t borderColor, TetrisBlock* currentTetrisBlock);
void Draw_Shadow_TetrisBlock(int x, int y, TetrisBlock *tetrisBlock);
void GetShadowPosition(TetrisBlock *currentTetrisBlock, int *shadowY);

// Game mechanics
void MoveTetrisBlockDown(void);
void Timer1_SetIntervalByLevel(uint8_t level);
void UpdateScore(uint32_t points);
void displayTime(void);

// UI and screens
void Test_Draw_All_Shapes(void);
void DisplayGameOverScreen(void);
void DisplayHighScores(void);
void SaveHighScore(uint32_t newScore);

#endif