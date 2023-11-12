/*
 * Copyright (c) 2023 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lcd16x2_i2c.h"

void lcd_write(enum lcd16x2_register reg, uint8_t val, lcd16x2_t *lcd) {
    uint8_t buf[] = {reg, val};
    i2c_write_blocking(lcd->i2c_port, lcd->i2c_addr, buf, 2,
                       false);
};

void lcd_clear_display(lcd16x2_t *lcd) {
    lcd_write(LCD_INSTRUCTION_REG, LCD_CLEAR_DISPLAY, lcd);
}

void lcd_return_home(lcd16x2_t *lcd) {
    lcd_write(LCD_INSTRUCTION_REG, LCD_RETURN_HOME, lcd);
}

void lcd_init(i2c_inst_t *i2c_port, uint8_t i2c_addr,
              lcd16x2_t *lcd) {
    lcd->i2c_port = i2c_port;
    lcd->i2c_addr = i2c_addr;

    // These parameters could potentially be modified by means of some
    // functions, etc, so that he user is able to choose from the
    // various configuration options. At this point, however, I am only
    // interested in using the 16x2 display in the same way (e.g.
    // left-to-right, blinking, etc) so I am for now hardcoding these
    // options here.
    uint8_t lcd_function = (LCD_FUNCTION_SET | LCD_DATA_LEN_8_BITS |
        LCD_2_LINES | LCD_CHARACTER_5x8_DOTS);
    uint8_t lcd_shift = (LCD_SHIFT | LCD_MOVE_CURSOR | LCD_MOVE_RIGHT);
    uint8_t lcd_entry_mode = (LCD_ENTRY_MODE_SET |
        LCD_ENTRY_MODE_INCREMENT | LCD_ENTRY_MODE_NO_SHIFT);
    uint8_t lcd_display_ctrl = (LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON |
        LCD_CURSOR_OFF | LCD_BLINK_OFF);
   
    // initialisation sequence as per the Hitachi manual (Table 13,
    // p.43) is function set -> display control -> entry mode. The time
    // intervals between steps are as in p.13 of the JHD data sheet.
    sleep_ms(15);
    // Function set
    lcd_write(LCD_INSTRUCTION_REG, lcd_function, lcd);
    sleep_us(40);
    // Display control
    lcd_write(LCD_INSTRUCTION_REG, lcd_display_ctrl, lcd);
    sleep_us(40);
    // Display clear
    lcd_clear_display(lcd);
    sleep_ms(2);
    // Entry mode
    lcd_write(LCD_INSTRUCTION_REG, lcd_entry_mode, lcd);
}

void lcd_move_cursor(uint8_t row, uint8_t col, lcd16x2_t *lcd) {
    // To move the cursor to a specific postion the action is "set the
    // DDRAM address", and involves (1) selecting the instruction
    // register (RS = 1), (2) setting clearing the red bit (R/~W = 0),
    // and (3) writing the instruction byte.
    //
    // Within the instruction byte, bit 7 must be set to 1 (i.e. 0x80)
    // and the remaining 7 bits set the address counter, referred in the
    // data sheet as A'AAA'AAA.
    //
    // The leftmost bit (bit 6) sets the row: 0 or 1 for top or bottom
    // row. The remaining 6 bits (5 to 0) define the column position and
    // are 0b000000 to 0b100111, which is the range 0 to 39. This coding
    // thus allows to define column in displays that are up to 40
    // characters wide.
    //
    // From the above it follows that the row (line) can be selected by
    // setting bit 6 to 0 or 1:
    //   row 0 = 0x00
    //   row 1 = 0x40
    // and selecting the column simply involves setting bits 5-0 with
    // the required value (0~39).
    //
    // (Hitachi data sheet p.29)

    // The instruction byte for setting the position of the cursor is

    // Bit 6 selects the row: 0 or 1. Thus, shifting the value by these
    // many places should do the trick. Before that, we check that row
    // is within the expected range; if it isn't it is set to 0.
    if (row >= LCD_NUMBER_OF_ROWS) row = 0;
    row = row << 6;

    // Column position is determined by bits 5-0 in the instruction
    // byte, and the valid range is between 0 and 39.
    if (col >= LCD_NUMBER_OF_COLS) col = 0;

    // Position can now be set the combination of row and column
    // information, This also has to be combined with bit 7 which must
    // be 1
    uint8_t command = (0x80 | row | col);
    lcd_write(LCD_INSTRUCTION_REG, command, lcd);
}

void lcd_put_char(uint8_t c, lcd16x2_t *lcd) {
    // To display characters, the action is "write data to DDRAM". For
    // that, set RS to 1 (i.e. select Data Register), set R/~W to 0
    // (write), then write the 8 bits that code the (ascii) character.
    lcd_write(LCD_DATA_REG, c, lcd);
}

void lcd_put_str(const uint8_t *s, lcd16x2_t *lcd) {
    // TODO Deal with strings longer that 16x2
    while (*s) {
        lcd_put_char(*s++, lcd);
    }
}
