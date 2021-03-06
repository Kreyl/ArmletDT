/*
 * board.h
 *
 *  Created on: 12 ����. 2015 �.
 *      Author: Kreyl
 */

#pragma once

#include <inttypes.h>

// ==== General ====
#define BOARD_NAME          "Armlet3"
#define APP_NAME            "DarkTower"

// MCU type as defined in the ST header.
#define STM32F205xx

// Freq of external crystal if any. Leave it here even if not used.
#define CRYSTAL_FREQ_HZ 12000000

// OS timer settings
#define STM32_ST_IRQ_PRIORITY   2
#define STM32_ST_USE_TIMER      5
#define SYS_TIM_CLK             (Clk.GetTimInputFreq(TIM5))

//  Periphery
#define PILL_ENABLED            TRUE
#define I2C1_ENABLED            FALSE
#define I2C2_ENABLED            PILL_ENABLED
#define I2C3_ENABLED            FALSE
#define SIMPLESENSORS_ENABLED   TRUE
#define BUTTONS_ENABLED         TRUE

#define ADC_REQUIRED            TRUE

#if 1 // ========================== GPIO =======================================
// EXTI
#define INDIVIDUAL_EXTI_IRQ_REQUIRED    FALSE

// UART
#define UART_GPIO       GPIOD
#define UART_TX_PIN     5
#define UART_RX_PIN     6

// Battery measuremrnt
#define BAT_MEAS_PIN    GPIOC, 0

// Buttons
#define BTNA_PIN        GPIOD, 0
#define BTNB_PIN        GPIOD, 1
#define BTNC_PIN        GPIOD, 3
#define BTNL_PIN        GPIOD, 4
#define BTNE_PIN        GPIOD, 7
#define BTNR_PIN        GPIOD, 8
#define BTNX_PIN        GPIOD, 9
#define BTNY_PIN        GPIOD, 10
#define BTNZ_PIN        GPIOD, 11

#define BTN_PWRON       BTNC_PIN

// Usb detect & charging
#define USB_DETECT_PIN  GPIOA, 9
#define IS_CHARGING_PIN GPIOC, 3

// Beeper
#define BEEP_TOP_VALUE  22
#define BEEPER_PIN      { GPIOD, 12, TIM4, 1, invNotInverted, omPushPull, BEEP_TOP_VALUE }

// Vibro
#define VIBRO_PIN       { GPIOE, 14, TIM1, 4, invNotInverted, omPushPull, 207 }
#define VIBRO_TIM_FREQ  171

// I2C
#define I2C1_GPIO       GPIOB
#define I2C1_SCL        6
#define I2C1_SDA        7

#define I2C2_GPIO       GPIOB
#define I2C2_SCL        10
#define I2C2_SDA        11

// USB
#define USB_DM          GPIOA, 11
#define USB_DP          GPIOA, 12
#define USB_AF          AF10

// Radio: SPI, PGpio, Sck, Miso, Mosi, Cs, Gdo0
#define CC_Setup0       SPI1, GPIOA, 5,6,7, 10, 4

// KL NFC
#define KLNFC_TX_PIN    { GPIOA, 2, TIM2, 3, invNotInverted, omPushPull, 1 }

// Pill power
#define PILL_PWR_PIN    { GPIOB, 12, omPushPull }

// SD
#define SD_PWR_PIN      GPIOC, 4
#define SD_AF           AF12
#define SD_DAT0         GPIOC,  8, omPushPull, pudPullUp, SD_AF
#define SD_DAT1         GPIOC,  9, omPushPull, pudPullUp, SD_AF
#define SD_DAT2         GPIOC, 10, omPushPull, pudPullUp, SD_AF
#define SD_DAT3         GPIOC, 11, omPushPull, pudPullUp, SD_AF
#define SD_CLK          GPIOC, 12, omPushPull, pudNone,   SD_AF
#define SD_CMD          GPIOD,  2, omPushPull, pudPullUp, SD_AF

#endif // GPIO

#if 1 // ========================== USART ======================================
#define PRINTF_FLOAT_EN FALSE
#define CMD_UART        USART2
#define UART_USE_INDEPENDENT_CLK    TRUE
#define UART_TXBUF_SZ   2048
#endif

#if I2C2_ENABLED // ====================== I2C ================================
#define I2C2_BAUDRATE   400000
#define I2C_PILL        i2c2
#endif

#if ADC_REQUIRED // ======================= Inner ADC ==========================
#define ADC_MEAS_PERIOD_MS  2007
// Clock divider: clock is generated from the APB2
#define ADC_CLK_DIVIDER		adcDiv4

// ADC channels
#define ADC_CHNL_BATTERY    10

#define ADC_CHANNELS        { ADC_CHNL_BATTERY, ADC_CHNL_VREFINT }
#define ADC_CHANNEL_CNT     2   // Do not use countof(AdcChannels) as preprocessor does not know what is countof => cannot check
#define ADC_SAMPLE_TIME     ast84Cycles
#define ADC_SAMPLE_CNT      8   // How many times to measure every channel

#define ADC_MAX_SEQ_LEN     16  // 1...16; Const, see ref man
#define ADC_SEQ_LEN         (ADC_SAMPLE_CNT * ADC_CHANNEL_CNT)
#if (ADC_SEQ_LEN > ADC_MAX_SEQ_LEN) || (ADC_SEQ_LEN == 0)
#error "Wrong ADC channel count and sample count"
#endif
#endif

#if 1 // =========================== DMA =======================================
#define STM32_DMA_REQUIRED  TRUE
// ==== Uart ====
// Remap is made automatically if required
#define UART_DMA_TX     STM32_DMA1_STREAM6
#define UART_DMA_RX     STM32_DMA1_STREAM5
#define UART_DMA_CHNL   4

#define KLNFC_DMA_TX    STM32_DMA1_STREAM4
#define KLNFC_DMA_RX    STM32_DMA1_STREAM2
#define KLNFC_DMA_CHNL  4

#if I2C1_ENABLED // ==== I2C1 ====
#define I2C1_DMA_TX     STM32_DMA1_STREAM6
#define I2C1_DMA_RX     STM32_DMA1_STREAM5
#endif
#if I2C2_ENABLED // ==== I2C2 ====
#define I2C2_DMA_TX     STM32_DMA1_STREAM7
#define I2C2_DMA_RX     STM32_DMA1_STREAM3
#endif

#if ADC_REQUIRED
#define ADC_DMA         STM32_DMA2_STREAM0
#define ADC_DMA_MODE    STM32_DMA_CR_CHSEL(0) |   /* DMA2 Stream0 Channel0 */ \
                        DMA_PRIORITY_LOW | \
                        STM32_DMA_CR_MSIZE_HWORD | \
                        STM32_DMA_CR_PSIZE_HWORD | \
                        STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                        STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                        STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */
#endif // ADC

#endif // DMA
