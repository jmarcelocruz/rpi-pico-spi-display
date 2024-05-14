#include <hardware/regs/io_bank0.h>
#include <hardware/regs/pads_bank0.h>
#include <hardware/regs/resets.h>
#include <hardware/regs/spi.h>

#include <spi/spi.h>

spi_hw_t spi_init(void) {
    /* deassert reset on SPI0, PADS and IO_BANK0 */
    RESETS_HW->reset &= ~((1U << 16) | (1U << 8) | (1U << 5));
    while ((RESETS_HW->reset_done & (1U << 16)) == 0 || (RESETS_HW->reset_done & (1U << 8)) == 0 ||
            (RESETS_HW->reset_done & (1U << 5)) == 0) {
        ;
    }
    /* configure GPI0 pins 16 (RX), 17 (CS), 18 (SCK) and 19 (TX) for SPI0 in slave mode */
    IO_BANK0_HW->io[16].ctrl = (1U << 0);
    /* RX: output disable, input enable, everything else default*/
    PADS_BANK0_HW->io[16] = (1U << 7) | (1U << 6) | (1U << 4) | (1U << 2) | (1U << 1);
    IO_BANK0_HW->io[17].ctrl = (1U << 0);
    /* CS: output disable, input enable, everything else default*/
    PADS_BANK0_HW->io[17] = (1U << 7) | (1U << 6) | (1U << 4) | (1U << 2) | (1U << 1);
    IO_BANK0_HW->io[18].ctrl = (1U << 0);
    /* SCK: output disable, input enable, everything else default*/
    PADS_BANK0_HW->io[18] = (1U << 7) | (1U << 6) | (1U << 4) | (1U << 2) | (1U << 1);
    IO_BANK0_HW->io[19].ctrl = (1U << 0);
    /* TX: output enable, input disable, everything else default*/
    PADS_BANK0_HW->io[19] = (0U << 7) | (0U << 6) | (1U << 4) | (1U << 2) | (1U << 1);
    /* configure SPI0 peripheral*/
    SPI0_HW->sspcr[1] &= ~(1U << 1); /* ensure peripheral is disabled before configuration */
    SPI0_HW->sspcr[0] = (0U << 7) | (0U << 6) | (0U << 4) | (7U << 0); /* mode 0, Motorola SPI format, 8 bit */
    SPI0_HW->sspcr[1] = (1U << 3) | (1U << 2); /* output disable, set to slave mode */
    SPI0_HW->sspcr[1] |= (1U << 1); /* enable peripheral */

    return SPI0_HW;
}

char spi_getc(const spi_hw_t spi) {
    while ((spi->sspsr & (1U << 2)) == 0) {
        ; /* receive fifo is empty */
    }
    return spi->sspdr;
}
