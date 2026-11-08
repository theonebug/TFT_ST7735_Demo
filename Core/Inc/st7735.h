#ifndef INC_ST7735_H_
#define INC_ST7735_H_

#include "stm32f1xx_hal.h"

#define TFT_WIDTH  128
#define TFT_HEIGHT 160

#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800  // Было 0x00F8
#define COLOR_GREEN   0x07E0  // Было 0xE007
#define COLOR_BLUE    0x001F  // Было 0x1F00
#define COLOR_YELLOW  0xFFE0  // Было 0xE0FF

// Управление пинами по вашему подключению
#define CS_L()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)
#define CS_H()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define DC_C()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define DC_D()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define RST_L() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define RST_H() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)

void TFT_Init(void);
void TFT_FillScreen(uint16_t color);
void TFT_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

#endif
