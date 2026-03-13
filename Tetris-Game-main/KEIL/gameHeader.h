/******************************************************************************
 * @file     gameHeader.h
 * @brief    Header file for Tetris game definitions, constants, and function prototypes.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


/* ============================================================================
 * DEFINES AND CONSTANTS
 * ============================================================================ */

// Define Standard Colors - RGB565 color codes for LCD display
#define C_WHITE     0xFFFF
#define C_BLACK     0x0000
#define C_BLUE      0x001F
#define C_BLUE2     0x051F
#define C_RED       0xF800
#define C_MAGENTA   0xF81F
#define C_GREEN     0x07E0
#define C_CYAN      0x7FFF
#define C_YELLOW    0xFFE0
#define C_ORANGE    0xFD20
#define C_PURPLE    0x780F
#define SQUARE_SIZE 10


/* ============================================================================
 * EXTERNAL VARIABLES AND CONSTANTS
 * ============================================================================ */

// Characters and Images - Font arrays and screen bitmaps
extern uint8_t Font8x16[];
extern uint16_t Font16x32[];
extern const uint16_t WelcomeScreen[240*320];
extern const uint16_t backgroundScreenPlay[240*320];
extern int needsRedraw;
extern int isPaused;


/* ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================ */

// LCD and Drawing Functions
void LCD_Picture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *img);
void LCD_Draw_FilledSquare(uint16_t x, uint16_t y, uint16_t fillColor, uint16_t borderColor);
void LCD_Draw_SquareBorder(uint16_t x, uint16_t y, uint16_t rows, uint16_t cols, uint16_t fillColor, uint16_t borderColor);

// Tetris Block`s Shape Drawing Functions
void Draw_I_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_J_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_L_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_O_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_S_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_T_Shape(uint16_t x, uint16_t y, uint16_t borderColor);
void Draw_Z_Shape(uint16_t x, uint16_t y, uint16_t borderColor);

// UI and Game Loop Functions
void LCD_Draw_Border(void);
void LCD_DisplayTime(uint8_t min, uint8_t sec);
void gameLoop(void);
uint16_t GetShapeColor(int shapeIndex);
void DrawBoard(void);

// System Initialization and Handlers - Setup functions for MCU peripherals
void SYS_Init(void);
void EBI_Config(void);
void GPIO_Config(void);
void EADC_Config(void);
void Timer0_Init(void);
void Timer1_Init(void);
void Timer2_Init(void);
void enableTimer(void);
void disableTimer(void);

// Button and Joystick - Input device setup
void SW1_Interrupt_Setup(void);
void Joystick_Init(void);