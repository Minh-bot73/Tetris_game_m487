/******************************************************************************
 * @file     gameStage.h
 * @brief    Prototype stages of Tetris game.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

 //Stages
 typedef enum {
    WELCOME_SCREEN,
    PLAYING,
    PAUSED,
    GAME_OVER,
    HIGH_SCORE
} SystemState;

extern volatile SystemState currentState;