#ifndef SEMIGRAPHICS_H
#define SEMIGRAPHICS_H
#include <stdint.h>

static const uint16_t SEMIGRAPHIC_TYPE_MASK = 0xFF;
static const uint16_t SEMIGRAPHIC_TYPE_OFS = 8;
static const uint16_t SEMIGRAPHIC_TYPE_SET_A = 0;
static const uint16_t SEMIGRAPHIC_TYPE_SET_B = 1;

/* 0x000 - 0x0FF - SET A: basic box drawing - (none|thin|thick|double) x (left|up|right|down) */
/*
 * 0000 DDRRUULL
 *       ^ ^ ^ ^-- left
 *       | | +---- up
 *       | +------ right
 *       +---------down 
*/
static const uint16_t SEGMENT_NONE = 0;
static const uint16_t SEGMENT_SINGLE_THIN = 1;
static const uint16_t SEGMENT_SINGLE_THICK = 2;
static const uint16_t SEGMENT_DOUBLE = 3;

static const uint16_t SEGMENT_STYLE_MASK = 0x03;
static const uint16_t SEGMENT_STYLE_OFS_LEFT = 0;
static const uint16_t SEGMENT_STYLE_OFS_UP = 2;
static const uint16_t SEGMENT_STYLE_OFS_RIGHT = 4;
static const uint16_t SEGMENT_STYLE_OFS_DOWN = 6;

/* 0x100 - 0x11F - SET B1: extra box drawing - (double-dot, triple-dot, quad-dot) x (thin, thick) x (horizontal, vertical) */
/*
 * 0001 000SSDTT
 *          ^^^-- line thickness (0, 3 = reserved)
 *          |+--- direction
 *          +---- line style
*/
static const uint16_t LINE_HORIZONTAL = 0x00;
static const uint16_t LINE_VERTICAL = 0x04;
static const uint16_t LINE_SOLID = 0x00;
static const uint16_t LINE_DOUBLE_DOT = 0x08;
static const uint16_t LINE_TRIPLE_DOT = 0x10;
static const uint16_t LINE_QUAD_DOT = 0x18;

/* 0x120 - 0x12F - SET B2: extra box drawing */
/*
 * 0001 001000XX - rounded single thin corners
 * 0001 0010010X - diagonals
 * 0001 00100110 - cross
*/

static const uint16_t RCORNER_LEFT_TOP = 0x120;
static const uint16_t RCORNER_LEFT_BOTTOM = 0x121;
static const uint16_t RCORNER_RIGHT_TOP = 0x122;
static const uint16_t RCORNER_RIGHT_BOTTOM = 0x123;
static const uint16_t DIAGONAL_RISING = 0x124;
static const uint16_t DIAGONAL_FALLING = 0x125;
static const uint16_t DIAGONAL_CROSS = 0x126;

/* 0x130 - 0x14F - SET B3: block characters
/*
 * 0001 0011xxxx - vertical blocks (0..8)
 *                  * x=<0..8> - (x/8) left part
 *                  * x=9 - 50% right
 *                  * x=10 - 25% left top
 *                  * x=11 - 25% right top
 *                  * x=12 - 25% left bottom
 *                  * x=13 - 25% right bottom
 * 0001 0100xxxx - horizontal blocks
*/

extern const char SEMIGRAPHICS[4][];

#endif
