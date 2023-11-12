/*
 * Copyright (c) 2023 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd16x2_i2c.h"

// I2C defines
#define LCD_I2C_PORT i2c1
#define LCD_I2C_SDA 14
#define LCD_I2C_SCL 15
#define LCD_I2C_FREQ 100000

int main() {
    stdio_init_all();

     // I2C Initialisation.
    i2c_init(LCD_I2C_PORT, LCD_I2C_FREQ);
    gpio_set_function(LCD_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(LCD_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(LCD_I2C_SDA);
    gpio_pull_up(LCD_I2C_SCL);

    // LCD initialisation.
    struct lcd16x2 lcd;
    lcd_init(LCD_I2C_PORT, LCD_I2C_ADDRESS, &lcd);

    // Write to display
    lcd_put_str("Hello", &lcd);
    lcd_move_cursor(1, 0, &lcd);
    lcd_put_str("world!", &lcd);

    return 0;
}
