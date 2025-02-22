
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "error_def.h"

#define API_VERSION 15
#define CONFIG_VERSION 3
#define PINS_MAX 32

#define DEVICE_NAME_MAX_LEN 32

#define BMP_USER_FLASH_PAGE_SIZE 4096
#define BMP_USER_FLASH_PAGE_LEN 6

#define BMP_SCHEDULE_WAIT_NEXT_EVENT (-1)

typedef uint32_t bmp_api_matrix_row_t;
typedef uint32_t bmp_api_matrix_col_t;

typedef enum {
    BMP_PAIR_DEVICE_RECORD,
} CONFIGURATION_RECORD_ID;

typedef enum {
    I2C_FREQ_100K,
    I2C_FREQ_250K,
    I2C_FREQ_400K,
} bmp_api_i2c_freq_t;

typedef enum {
    SPI_FREQ_125K,
    SPI_FREQ_250K,
    SPI_FREQ_500K,
    SPI_FREQ_1M,
    SPI_FREQ_2M,
    SPI_FREQ_4M,
    SPI_FREQ_8M,
} bmp_api_spi_freq_t;

typedef struct {
    bmp_api_spi_freq_t freq;
    uint8_t            miso;
    uint8_t            mosi;
    uint8_t            sck;
    uint8_t            mode;
} bmp_api_spim_config_t;

typedef struct {
    uint8_t miso;
    uint8_t mosi;
    uint8_t sck;
    uint8_t cs;
    uint8_t cs_pullup;
    uint8_t mode;
    uint8_t default_data;
    uint8_t over_read_data;
    void (*complete_callback)(uint16_t receive_len);
} bmp_api_spis_config_t;

typedef struct {
    bmp_api_i2c_freq_t freq;
    uint8_t            sda;
    uint8_t            scl;
} bmp_api_i2cm_config_t;

typedef struct {
    uint8_t sda;
    uint8_t scl;
    uint8_t address[2];
} bmp_api_i2cs_config_t;

typedef struct {
    uint16_t pid;
    uint16_t vid;
    char     name[32];
    char     manufacture[32];
} bmp_api_device_info_t;

typedef struct {
    uint16_t id;
    uint8_t  role;
    uint8_t  addr[6];
    uint8_t  name[DEVICE_NAME_MAX_LEN];
} bmp_api_bonding_info_t;

typedef enum {
    SINGLE,
    SPLIT_MASTER,
    SPLIT_SLAVE,
    WEBNUS_CONFIG,  // special mode.
} bmp_api_ble_mode_t;

typedef enum {
    STARTUP_NONE,
    STARTUP_ADV_NO_WL,
    STARTUP_ADV_WL,
} bmp_api_ble_startup_t;

typedef enum {
    USB_CONNECTED,
    USB_DISCONNECTED,
    BLE_CONNECTED,
    BLE_DISCONNECTED,
    BLE_ADVERTISING_START,
    BLE_ADVERTISING_STOP,
    USB_CDC_ACM_OPEND,
    USB_CDC_ACM_CLOSED,
    USB_HID_READY,
} bmp_api_event_t;

typedef enum {
    MATRIX_COL2ROW,
    MATRIX_ROW2COL,
    MATRIX_COL2ROW_LPME,
    MATRIX_ROW2COL_LPME,
    MATRIX_COL2ROW2COL,
    MATRIX_ROW2COL2ROW,
    MATRIX_74HC164ROW,
    MATRIX_74HC164COL,
    BMP_MATRIX_CUSTOM,
} bmp_api_diode_direction_t;

typedef struct {
    uint16_t max_interval;
    uint16_t min_interval;
    uint16_t slave_latency;
} bmp_api_ble_conn_param_t;

typedef struct {
    uint8_t rows;
    uint8_t cols;
    uint8_t device_rows;
    uint8_t device_cols;
    uint8_t layer;
    uint8_t debounce;
    uint8_t is_left_hand;
    uint8_t diode_direction;
    uint8_t row_pins[32];
    uint8_t col_pins[32];
} bmp_api_matrix_config_t;

typedef struct {
    uint8_t pin;
    uint8_t num;
} bmp_api_led_config_t;

typedef struct {
    uint8_t  pin_a[8];
    uint8_t  pin_b[8];
    uint8_t  resolution[8];
} bmp_api_encoder_config_t;

typedef struct {
    uint32_t                 version;
    bmp_api_device_info_t    device_info;
    bmp_api_matrix_config_t  matrix;
    bmp_api_ble_mode_t       mode;
    bmp_api_ble_startup_t    startup;
    bmp_api_ble_conn_param_t param_peripheral;
    bmp_api_ble_conn_param_t param_central;  // for split master
    bmp_api_led_config_t     led;
    bmp_api_encoder_config_t encoder;
    uint8_t                  reserved[8];
} bmp_api_config_t;
#define BMP_API_CONFIG_SIZE sizeof(bmp_api_config_t)

typedef struct {
    uint8_t modfier;
    uint8_t reserved;
    uint8_t keys[6];
} __attribute__((packed)) bmp_api_key_report_t;

typedef struct {
    uint8_t buttons;
    int8_t  x;
    int8_t  y;
    int8_t  v;
    int8_t  h;
} __attribute__((packed)) bmp_api_mouse_report_t;

typedef struct {
    uint8_t col;
    uint8_t row;
} bmp_api_keypos_t;

typedef struct {
    uint8_t col;
    uint8_t row;
    uint8_t state;
} bmp_api_key_event_t;

typedef union {
    struct {
        uint8_t time[3];
        uint8_t state : 1;
        uint8_t id : 7;
    };
    uint8_t dat[4];
} bmp_api_switch_state_t;

typedef struct {
    uint8_t  tx_pin;
    uint8_t  rx_pin;
    uint32_t baudrate;
    void (*rx_callback)(uint8_t recv);
    uint8_t  rx_protocol;
} bmp_uart_config_t;

typedef struct {
    const uint8_t* pins;
    uint8_t        cnt;
    uint8_t        adc_port;
    uint8_t        shdn_pin;
    uint8_t        discharge_pin;
} bmp_ecs_config_t;

typedef bmp_error_t (*bmp_api_msc_write_cb_t)(const uint8_t* dat, uint32_t len);
typedef bmp_error_t (*bmp_api_state_change_cb_t)(bmp_api_event_t event);

typedef struct {
    int32_t (*init)(void);
    void (*reset)(uint32_t);
    void (*enter_sleep_mode)(void);
    void (*main_task_init)(void (*main_task)(void*), uint8_t default_interval);
    bool (*has_schedule_in_range)(uint32_t from_ms, uint32_t to_ms);
    void (*schedule_next_task)(int32_t interval_ms);
    void (*process_task)(void);
    bmp_error_t (*push_keystate_change)(bmp_api_key_event_t const* const key);
    uint32_t (*pop_keystate_change)(bmp_api_key_event_t* key, uint32_t len,
                                    uint8_t burst_threshold);
    bmp_error_t (*set_config)(bmp_api_config_t const* const);
    const bmp_api_config_t* (*get_config)(void);
    uint16_t (*get_vcc_mv)(void);
    uint16_t (*get_vcc_percent)(void);
    bmp_error_t (*set_state_change_cb)(bmp_api_state_change_cb_t);
} bmp_api_app_t;

typedef struct {
    void (*init)(bmp_api_config_t const* const config, uint32_t disable_msc);
    void (*enable)(void);
    void (*process)(void);
    void (*send_key)(bmp_api_key_report_t*);
    uint8_t (*get_indicator_led)(void);
    void (*send_mouse)(bmp_api_mouse_report_t*);
    void (*send_system)(uint16_t);
    void (*send_consumer)(uint16_t);
    void (*serial_putc)(char);
    char (*serial_getc)(void);
    void (*serial_puts)(uint8_t const*, uint8_t);
    int (*serial_byte_to_read)(void);
    void (*set_raw_receive_cb)(void (*raw_receive_cb)(const uint8_t* data, uint8_t length));
    int (*raw_send)(const uint8_t* data, uint8_t length);
    bmp_error_t (*create_file)(const char* sfn, const uint8_t* dat, uint32_t size);
    bmp_error_t (*set_msc_write_cb)(bmp_api_msc_write_cb_t);
} bmp_api_usb_t;

typedef struct {
    void (*init)(bmp_api_config_t const* const);
    void (*advertise)(uint8_t);
    void (*scan)(void);
    void (*disconnect)(uint8_t disconnect_all);
    bmp_error_t (*get_bonding_info)(bmp_api_bonding_info_t* info, uint32_t* len);
    void (*delete_bond)(uint8_t);
    void (*send_key)(bmp_api_key_report_t*);
    uint8_t (*get_indicator_led)(void);
    void (*send_mouse)(bmp_api_mouse_report_t*);
    void (*send_system)(uint16_t);
    void (*send_consumer)(uint16_t);
    bmp_error_t (*nus_send_bytes)(uint8_t* buf, uint16_t len);
    bmp_error_t (*set_nus_rcv_cb)(bmp_error_t (*callback)(const uint8_t* dat, uint32_t len));
    bmp_error_t (*set_nus_disconnect_cb)(bmp_error_t (*callback)(void));
    void (*set_raw_receive_cb)(void (*raw_receive_cb)(const uint8_t* data, uint8_t length));
    int (*raw_send)(const uint8_t* data, uint8_t length);
    uint16_t (*get_connection_status)(void);
    bmp_error_t (*send_user_data_s2m)(uint8_t id, uint8_t data);
    bmp_error_t (*set_user_data_cb)(void (data_receive_cb)(uint8_t id, uint8_t data));
} bmp_api_ble_t;

typedef struct {
    void (*enter)(void);
    bmp_error_t (*set_rcv_callback)(bmp_error_t (*callback)(const uint8_t* dat, uint32_t len));
    bmp_error_t (*set_send_buffer)(uint8_t* buf, uint16_t len);
    bmp_error_t (*set_disconnect_callback)(bmp_error_t (*callback)(void));
} bmp_api_web_config_t;

typedef enum {
    BMP_MODE_INPUT,
    BMP_MODE_OUTPUT,
} bmp_api_gpio_dir_t;

typedef enum {
    BMP_PULL_NONE,
    BMP_PULLDOWN,
    BMP_PULLUP = 3,
} bmp_api_gpio_pull_t;

typedef enum {
    BMP_PIN_S0S1,
    BMP_PIN_H0S1,
    BMP_PIN_S0H1,
    BMP_PIN_H0H1,
    BMP_PIN_D0S1,
    BMP_PIN_D0H1,
    BMP_PIN_S0D1,
    BMP_PIN_H0D1,
} bmp_api_gpio_drive_t;

typedef struct {
    bmp_api_gpio_dir_t   dir;
    bmp_api_gpio_pull_t  pull;
    bmp_api_gpio_drive_t drive;
} bmp_api_gpio_mode_t;

typedef struct {
    bmp_error_t (*set_mode)(uint8_t pin_num, bmp_api_gpio_mode_t const* const);
    uint32_t (*read_pin)(uint8_t pin_num);
    bmp_error_t (*set_pin)(uint8_t pin_num);
    bmp_error_t (*clear_pin)(uint8_t pin_num);
} bmp_api_gpio_t;

typedef struct {
    bmp_error_t (*init)(uint32_t encoder_scan_ms, const uint8_t* pin_a, const uint8_t* pin_b,
                        uint8_t num_of_enc);
    int32_t (*get_count)(uint8_t idx);
} bmp_api_encoder_t;

typedef struct {
    int (*init)(bmp_api_i2cm_config_t const* const);
    void (*uninit)(void);
    uint8_t (*transmit)(uint8_t address, uint8_t* data, uint16_t length);
    uint8_t (*receive)(uint8_t address, uint8_t* data, uint16_t length);
    uint8_t (*write_reg)(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length,
                         uint16_t timeout);
    uint8_t (*read_reg)(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length,
                        uint16_t timeout);
} bmp_api_i2c_master_t;

typedef struct {
    int (*init)(bmp_api_i2cs_config_t const* const);
    void (*uninit)(void);
    void (*prepare)(uint8_t* const dat, uint8_t len);
    void (*receive)(uint8_t* const dat, uint8_t len);
} bmp_api_i2c_slave_t;

typedef struct {
    int (*init)(bmp_api_spim_config_t const* const);
    int (*cs_init)(uint8_t cs_pin);
    int (*start)(uint8_t* p_tx_buffer, size_t tx_length, uint8_t* p_rx_buffer, size_t rx_length,
                 uint8_t cs_pin);
} bmp_api_spi_master_t;

typedef struct {
    int (*init)(bmp_api_spis_config_t const* const);
    int (*start)(uint8_t* p_tx_buffer, size_t tx_length, uint8_t* p_rx_buffer, size_t rx_length);
} bmp_api_spi_slave_t;

typedef struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
} bmp_api_led_t;

typedef struct {
    void (*setleds_pin)(bmp_api_led_t* ledarray, uint16_t number_of_leds, uint8_t pinmask);
} bmp_api_ws2812_t;

typedef struct {
    void (*init)(void);
    void (*info)(const char*);
} bmp_api_logger_t;

typedef struct {
    // void (*init)(void); // initialized in wakeup process
    int (*config_vcc_channel)(uint8_t ain, uint16_t vmax_mv, uint16_t vmin_mv);
    int (*config_channel)(uint8_t channel, uint8_t ain);
    int (*sample_and_convert)(uint8_t channel, int16_t* const result);
} bmp_api_adc_t;

typedef struct {
    void (*init)(bmp_uart_config_t const* const config);
    uint32_t (*send)(uint8_t const* dat, uint32_t len);
} bmp_api_uart_t;

typedef struct {
    void (*init)(bmp_ecs_config_t const* const config);
    void (*assign_drive_pin)(uint32_t pin);
    void (*sw_read)(uint16_t* res);
    void (*clear_drive_pins)(bmp_ecs_config_t const* const config);
    void (*shutdown_amp)(void);
    void (*schedule_next_scan)(void);
    void (*discharge_capacitor)(void);
} bmp_api_ecs_t;

typedef struct {
    float (*read)(void);
} bmp_api_cpu_temp_t;

typedef struct {
    uint32_t (*get_ms)(void);
    void     (*sleep_us)(uint32_t us);
    void     (*sleep_ms)(uint32_t ms);
} bmp_api_timer_t;

typedef struct {
    int  (*write_dword)(uint32_t addr, uint32_t* data);
    int  (*write_page)(uint32_t page, uint32_t* data);
    void (*read)(uint32_t addr, uint8_t* data, uint32_t len);
    int  (*erase_page)(uint32_t page);
    const uint8_t* (*get_base_address)(void);
} bmp_api_user_flash_t;

typedef struct {
    //////DO NOT CHANGE///////
    uint32_t api_version;
    void (*bootloader_jump)(void);
    /////////////////////////
    const char* (*get_bootloader_info)(void);
    bmp_api_app_t        app;
    bmp_api_usb_t        usb;
    bmp_api_ble_t        ble;
    bmp_api_gpio_t       gpio;
    bmp_api_i2c_master_t i2cm;
    bmp_api_i2c_slave_t  i2cs;
    bmp_api_spi_master_t spim;
    bmp_api_ws2812_t     ws2812;
    bmp_api_logger_t     logger;
    bmp_api_web_config_t web_config;
    bmp_api_encoder_t    encoder;
    bmp_api_adc_t        adc;
    bmp_api_uart_t       uart;
    bmp_api_ecs_t        ecs;
    bmp_api_cpu_temp_t   temp;
    bmp_api_timer_t      timer;
    bmp_api_user_flash_t flash;
    bmp_api_spi_slave_t  spis;
} bmp_api_t;

#define BMPAPI ((bmp_api_t*)0xFDE00)
