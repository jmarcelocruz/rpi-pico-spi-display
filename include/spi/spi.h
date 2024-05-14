#ifndef SPI_H
#define SPI_H

typedef struct spi_hw * spi_hw_t;

spi_hw_t spi_init(void);

char spi_getc(const spi_hw_t spi);

#endif
