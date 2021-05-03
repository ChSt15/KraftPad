#ifndef PINOUTS_H
#define PINOUTS_H



//SD-Card
#define SD_CARD_CS_PIN 22

//LCD
#define LCD_CS_PIN 15
#define LCD_RST_PIN 2
#define LCD_DC_PIN 5
#define LCD_LED_PIN 4

//SPI Bus is standard pins

//I2C
#define I2C_SCL_PIN 27
#define I2C_SDA_PIN 26

//LoRa
#define SX1280_NSS_PIN 14
#define SX1280_NRESET_PIN 16
#define SX1280_TXEN_PIN 17
#define SX1280_RXEN_PIN 12
#define SX1280_RFBUSY_PIN 34
#define SX1280_DIO1_PIN 35
#define SX1280_SPIBUS SPI

//GPIO Testing
#define GENERAL_INPUT_PIN 33
#define GENERAL_OUTPUT_PIN 13

//Battery voltage divider
#define VBAT_DIV_ADC_PIN 2

#define VBAT_DIV_R1 75e3
#define VBAT_DIV_R2 150e3

#define VBAT_DIV_FACTOR (VBAT_DIV_R2/(VBAT_DIV_R2 + VBAT_DIV_R1)) 

#define VBAT_CORRECTION_SCALER 1.0
#define VBAT_CORRECTION_OFFSET 0.042

//Analog stick
#define JOYSTICK_Y_ADC_PIN 0
#define JOYSTICK_X_ADC_PIN 2
#define JOYSTICK_BUTTON_PIN 36

//Prog/Kill button
#define PROG_KILL_BUTTON_PIN 0

//Vext pin
#define VEXT_SWITCH_PIN 21

//BNO080
#define IMU_INT_PIN 37

//Buzzer pin on DAC
#define BUZZER_PIN 25













#endif