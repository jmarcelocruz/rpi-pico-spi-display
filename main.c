#include <display/display.h>
#include <spi/spi.h>

#define DISPLAY_RS 12
#define DISPLAY_RW 11
#define DISPLAY_E  10
#define DISPLAY_D0 9
#define DISPLAY_D1 8
#define DISPLAY_D2 7
#define DISPLAY_D3 6
#define DISPLAY_D4 5
#define DISPLAY_D5 4
#define DISPLAY_D6 3
#define DISPLAY_D7 2

void main(void) {
    spi_hw_t spi = spi_init();
    display_hw_t display = display_init(DISPLAY_RS, DISPLAY_RW, DISPLAY_E, DISPLAY_D0, DISPLAY_D1,
            DISPLAY_D2, DISPLAY_D3, DISPLAY_D4, DISPLAY_D5, DISPLAY_D6, DISPLAY_D7);

    char message[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    for (int i = 0; i < sizeof(message); i++) {
        display_putc(display, message[i]);
    }

    while (1) {
        char c = spi_getc(spi);
        display_putc(display, c);
    }
}
