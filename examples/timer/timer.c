/**
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

// Variables for the timer string, and for hour, minutes, and seconds.
uint8_t timer_str[10];
uint8_t h = 0;
uint8_t m = 0;
uint8_t s = 0;
bool led_status = 0;

struct lcd16x2 lcd;

bool timer_callback(repeating_timer_t *rt);

int main() {
    stdio_init_all();

    // On-board LED
    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    // gpio_put(PICO_DEFAULT_LED_PIN, led_status);

    // I2C Initialisation.
    i2c_init(LCD_I2C_PORT, LCD_I2C_FREQ);
    gpio_set_function(LCD_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(LCD_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(LCD_I2C_SDA);
    gpio_pull_up(LCD_I2C_SCL);
    
    // LCD initialisation.    
    lcd_init(LCD_I2C_PORT, LCD_I2C_ADDRESS, &lcd);  

    // Timer initialisation - repeats every second.
    repeating_timer_t timer;
    int32_t delay_ms = 1000;
    add_repeating_timer_ms(-delay_ms, timer_callback, NULL, &timer);

    // I have found that the repeating timer does not work without this
    // `while` loop, even if there is nothing to put in it.
    while (1) {
        tight_loop_contents();
    }
    
    return 0;
}

bool timer_callback(repeating_timer_t *rt) {
    // Set-up and display the timer
    sprintf(timer_str, "%02u:%02u:%02u", h, m, s);
    lcd_move_cursor(0, 4, &lcd);
    lcd_put_str(timer_str, &lcd);
    
    // Toggle the LED
    // gpio_put(PICO_DEFAULT_LED_PIN, led_status);
    // led_status = !led_status;
    
    // Increase the seconds counter by one.
    s++;
    
    // Every 60 seconds, reset the seconds counter and increment minutes
    // by one.
    if (s == 60) {
        s = 0;
        m++;
    
        // Every 60 minutes, reset the minutes counter and increase hour
        // by one.
        if (m == 60) {
            m = 0;
            h++;
        }
    }
    return true;
}

