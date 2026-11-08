#include "st7735.h"

extern SPI_HandleTypeDef hspi2;
volatile uint8_t dma_busy = 0;
uint8_t line_buffer[TFT_WIDTH * 2];

// Callback завершения DMA
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &hspi2) {
        CS_H();
        dma_busy = 0;
    }
}

void TFT_WriteCmd(uint8_t cmd) {
    while(dma_busy);
    DC_C(); CS_L();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    CS_H();
}

void TFT_WriteData(uint8_t data) {
    while(dma_busy);
    DC_D(); CS_L();
    HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
    CS_H();
}

void TFT_WriteDataDMA(uint8_t* data, uint16_t size) {
    while(dma_busy);
    DC_D(); CS_L();
    dma_busy = 1;
    HAL_SPI_Transmit_DMA(&hspi2, data, size);
}

void TFT_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    TFT_WriteCmd(0x2A); // CASET
    TFT_WriteData(0x00); TFT_WriteData(x0 + 0);
    TFT_WriteData(0x00); TFT_WriteData(x1 + 0);
    TFT_WriteCmd(0x2B); // RASET
    TFT_WriteData(0x00); TFT_WriteData(y0 + 0);
    TFT_WriteData(0x00); TFT_WriteData(y1 + 0);
    TFT_WriteCmd(0x2C); // RAMWR
}



void TFT_Init(void) {
    RST_H(); HAL_Delay(10);
    RST_L(); HAL_Delay(10);
    RST_H(); HAL_Delay(120);

    TFT_WriteCmd(0x01); HAL_Delay(120); // Сброс
    TFT_WriteCmd(0x11); HAL_Delay(120); // Выход из сна

    TFT_WriteCmd(0x3A); TFT_WriteData(0x05); // 16-бит цвет

    // Установка правильной адресации пикселей матрицы
    TFT_WriteCmd(0x36); TFT_WriteData(0xC0);

    // Отключение инверсии
    TFT_WriteCmd(0x20);

    TFT_WriteCmd(0x29); // Включить экран
    HAL_Delay(10);
}






void TFT_FillScreen(uint16_t color) {
    for (int i = 0; i < TFT_WIDTH; i++) {
        line_buffer[i*2] = (color >> 8) & 0xFF;
        line_buffer[i*2+1] = color & 0xFF;
    }
    TFT_SetWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
    for (int y = 0; y < TFT_HEIGHT; y++) {
        TFT_WriteDataDMA(line_buffer, TFT_WIDTH * 2);
        while(dma_busy);
    }
}

void TFT_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if(x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
    for (int i = 0; i < w; i++) {
        line_buffer[i*2] = (color >> 8) & 0xFF;
        line_buffer[i*2+1] = color & 0xFF;
    }
    TFT_SetWindow(x, y, x + w - 1, y + h - 1);
    for (int i = 0; i < h; i++) {
        TFT_WriteDataDMA(line_buffer, w * 2);
        while(dma_busy);
    }
}
