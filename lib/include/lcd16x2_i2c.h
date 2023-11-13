/*
 * Copyright (c) 2023 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LCD16X2_H_
#define _LCD16X2_H_

#include "pico.h"
#include "hardware/i2c.h"

#define LCD_I2C_ADDRESS 0x3E
#define LCD_NUMBER_OF_ROWS 2
#define LCD_NUMBER_OF_COLS 16

typedef struct lcd16x2 {
    i2c_inst_t *i2c_port;
    uint8_t i2c_addr;
} lcd16x2_t;

/*! \brief LCD register map
 *
 * There are only 2 registers: the instruction register (IR) and the
 * data register (DR). Register Selection Signal (RS) set to 0 means IR
 * selected, RS set to 1 means DR selected.
*/
enum lcd16x2_register {
    LCD_INSTRUCTION_REG = 0x00u,  // Register Select = 0
    LCD_DATA_REG = 0x40u  // Register Select = 1
};

// INSTRUCTION SET
// After Table 6 in Hitachi HD4478 data sheet (p.24)
//
// Clear display
#define LCD_CLEAR_DISPLAY 0x01
// Return home
#define LCD_RETURN_HOME 0x02
// Entry mode set
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_ENTRY_MODE_INCREMENT 0x02
#define LCD_ENTRY_MODE_DECREMENT 0x00
#define LCD_ENTRY_MODE_SHIFT 0x01
#define LCD_ENTRY_MODE_NO_SHIFT 0x00
// Display control
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00
// Cursor or display shift
#define LCD_SHIFT 0x10
#define LCD_MOVE_DISPLAY 0x08
#define LCD_MOVE_CURSOR 0x00
#define LCD_MOVE_RIGHT 0x04
#define LCD_MOVE_LEFT 0x00
// Function set
#define LCD_FUNCTION_SET 0x20
#define LCD_DATA_LEN_8_BITS 0x10
#define LCD_DATA_LEN_4_BITS 0x00
#define LCD_2_LINES 0x08
#define LCD_1_LINE 0x00
#define LCD_CHARACTER_5x10_DOTS 0x04
#define LCD_CHARACTER_5x8_DOTS 0x00


/*! \brief Initialise the LCD and configure default settings
 *
 * \param i2c_port The I2C port instance, either i2c0 or i2c1
 * \param i2c_addr The I2C address
 * \param lcd Pointer to the lcd16x2 structure
*/
void lcd_init(i2c_inst_t *i2c_port, uint8_t i2c_addr, lcd16x2_t *lcd);


/*! \brief Write a value to the specified register
 *
 * \param reg The register address
 * \param val The value to write
 * \param lcd Pointer to the lcd16x2 structure
 */
void lcd_write(enum lcd16x2_register reg, uint8_t val, lcd16x2_t *lcd);


/*! \brief Clears the display
 *
 * \param lcd Pointer to the lcd16x2 structure
 */
void lcd_clear_display(lcd16x2_t *lcd);


/*! \brief Returns cursor home
 *
 * Returns cursor home but does not change the display contents.
 * 
 * \param lcd Pointer to the lcd16x2 structure
*/
void lcd_return_home(lcd16x2_t *lcd);


/*! \brief Moves cursor to the specified position
 * 
 * Moves the cursor to the position specified by row and column. If any
 * of these values are greater than the number of characters in the LCD
 * then they are set to 0.
 * 
 * \param row Row to move the cursor to (top row is 0)
 * \param col Column to move the cursor to (leftmost column is 0)
 * \param lcd Pointer to the lcd16x2 structure
*/
void lcd_move_cursor(uint8_t row, uint8_t col, lcd16x2_t *lcd);


/*! \brief Puts a character on the display
 *
 * \param c Character to put on the display
 * \param lcd Pointer to the lcd16x2 structure 
*/
void lcd_put_char(uint8_t c, lcd16x2_t *lcd);


/*! \brief Puts a string on the display
 *
 * \param s String to put on the display
 * \param lcd Pointer to the lcd16x2 structure
*/
void lcd_put_str(const uint8_t *s, lcd16x2_t *lcd);

#endif