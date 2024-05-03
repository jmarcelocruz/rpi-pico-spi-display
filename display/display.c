#include <hardware/regs/io_bank0.h>
#include <hardware/regs/pads_bank0.h>
#include <hardware/regs/resets.h>
#include <hardware/regs/sio.h>

#include <display/display.h>

struct display_hw {
    uint8_t rs;
    uint8_t rw;
    uint8_t e;
    uint8_t d[8];
};

static struct display_hw display;

static void _display_cmd(const display_hw_t display, uint8_t rs, uint8_t rw, const uint8_t d[8]) {
    /* configure rs and rw pins */
    uint32_t io_out = SIO_HW->io_out & ~((1U << display->rs) | (1U << display->rw));
    SIO_HW->io_out = io_out | (rs ? (1U << display->rs) : 0) | (rw ? (1U << display->rw) : 0);
    /* set chip enable high before loading data on the bus */
    SIO_HW->io_out_set = (1U << display->e);
    /* put data on the bus */
    io_out = SIO_HW->io_out;
    for (int i = 0; i < 8; i++) {
        io_out &= ~(1U << display->d[i]);
    }
    for (int i = 0; i < 8; i++) {
        io_out |= (d[i] ? (1U << display->d[i]) : 0);
    }
    SIO_HW->io_out = io_out;
    /* set chip enable low, causing display to process data on the bus according to rs and rw */
    SIO_HW->io_out_clr = (1U << display->e);
}

display_hw_t display_init(uint8_t rs, uint8_t rw, uint8_t e, uint8_t d0, uint8_t d1,
        uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
    display = (struct display_hw) {rs, rw, e, {d0, d1, d2, d3, d4, d5, d6, d7}};
    /* deassert reset on IO_BANK0 and PADS_BANK0 */
    RESETS_HW->reset &= ~((1U << 8) | (1U << 5));
    while ((RESETS_HW->reset_done & (1U << 8)) == 0 || (RESETS_HW->reset_done & (1U << 5)) == 0) {
        ;
    }
    /* configure pins passed as arguments */
    IO_BANK0_HW->io[display.rs].ctrl = (5U << 0);
    IO_BANK0_HW->io[display.rw].ctrl = (5U << 0);
    IO_BANK0_HW->io[display.e].ctrl = (5U << 0);
    for (uint8_t i = 0; i < 8; i++) {
        IO_BANK0_HW->io[display.d[i]].ctrl = (5U << 0);
    }
    SIO_HW->io_out_clr = (1U << rs) | (1U << rw) | (1U << e) | (1U << d0) | (1U << d1) |
            (1U << d2) | (1U << d3) | (1U << d4) | (1U << d5) | (1U << d6) | (1U << d7);
    SIO_HW->io_oe = (1U << rs) | (1U << rw) | (1U << e) | (1U << d0) | (1U << d1) |
            (1U << d2) | (1U << d3) | (1U << d4) | (1U << d5) | (1U << d6) | (1U << d7);

    /* follow initialization sequence from display datasheet */
    _display_cmd(&display, 0, 0, (uint8_t []){0, 0, 0, 1, 1, 1, 0, 0});
    _display_cmd(&display, 0, 0, (uint8_t []){0, 0, 0, 1, 1, 1, 0, 0});
    _display_cmd(&display, 0, 0, (uint8_t []){1, 1, 1, 1, 0, 0, 0, 0});
    _display_cmd(&display, 0, 0, (uint8_t []){1, 0, 0, 0, 0, 0, 0, 0});
    /* TODO: replace with proper delay implementation */
    for (int i = 0; i < 1000000; i++) {
        (void) 0;
    }
    _display_cmd(&display, 0, 0, (uint8_t []){0, 1, 1, 0, 0, 0, 0, 0});

    return &display;
}

void display_putc(const display_hw_t display, char c) {
    uint8_t d[8];
    for (int i = 0; i < 8; i++) {
        d[i] = (c & 1U);
        c = c >> 1;
    }
    _display_cmd(display, 1, 0, d);
}
