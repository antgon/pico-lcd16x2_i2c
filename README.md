# pico-lcd16x2_i2c

A C library for using SeeedStudio [Grove 16x2 liquid crystal display
(LCD)][grove] with the Raspberry Pi Pico.

This display has 16x2 characters and an I2C interface.

## Wiring

LCD | Pico 
----|-----
SDA | SDA (e.g. pin 8)
SCL | SCL (e.g. pin 9)
VDD | 3V3(OUT)
GND | GND

No additional I2C pull-up resistors are required; the "newer" version of this
display includes these already; see
[Grove 16x2 liquid crystal display (LCD)][grove].

## Notes

The LCD module in this board is Model 1804 from [JHD][jhd]. The module's
data sheet is available for
[downloading](https://files.seeedstudio.com/wiki/Grove-16x2_LCD_Series/res/JDH_1804_Datasheet.pdf)
from SeeedStudio but I cannot make much sense of the information in
there. However, the LCD module is in turn based on Hitachi's HD44780 Dot
Matrix Liquid Crystal Display Controller/Driver ([Wikipedia][hitachi]).
The data sheet for that controller is available from Sparkfun ([HD44780
data sheet, pdf][datasheet]) and it is far more useful and informative.
This C library is thus for the most part based on Hitachi's data sheet.

This library is likely to work with other I2C LCD boards built around
the same Hitachi HD44780 controller, including LCDs with more or less
characters (e.g. 16x1 or 40x2), with only a couple of minor changes to
the code. To date, however, the `lcd16x2` library has only been tested
with the Grove LCD 16x2 board mentioned above.

### See also

There is an example in Raspberry Pi [pico-examples][pico-examples] for
using similar 16x2 LCD modules; see `i2c/lcd_1602_i2c`. I have not been
able to use that example with this LCD board though.

[jhd]: http://www.jhdlcd.com.cn/
[hitachi]: https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
[grove]: https://wiki.seeedstudio.com/Grove-16x2_LCD_Series/
[pico-examples]: https://github.com/raspberrypi/pico-examples
