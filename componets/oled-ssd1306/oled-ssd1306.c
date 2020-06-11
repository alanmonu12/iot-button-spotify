#include "oled-ssd1306.h"

//static char *TAG = "OLED";

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size, uint8_t adress)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, adress | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


/**
 * @brief 
 */
void oled_ssd1306_create(oled_ssd1306* self, uint8_t address) {
    self->address = address;
    self->display.CurrentX = 0;
    self->display.CurrentY = 0;
    self->display.Initialized = false;
    self->display.Inverted = false;
    self->handler = NULL;
    ESP_ERROR_CHECK(i2c_master_init());

}

/**
 * @brief 
 */
void oled_ssd1306_Init(oled_ssd1306 *self) {

    uint8_t data[] = {0xAE, 0x20, 0x00, 0xB0, 0xC8, 0x00, 0x10, 0x40, 0x81, 0xFF, 0xA1,
                      0xA6, 0x1F, 0xA4, 0xD3, 0x00, 0xD5, 0xF0, 0xD9, 0x22, 0xDA, 0x02,
                      0xDB, 0x20, 0x8D, 0x14, 0xAF};
    uint8_t x = 0x80;

    for(int i = 0; i < sizeof(data); i++) {
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &data[i], 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &x, 1, self->address));
    }

    oled_ssd1306_Fill(self, White);
    oled_ssd1306_UpdateScreen(self);


    self->display.Initialized = true;
}

void oled_ssd1306_Fill(oled_ssd1306 *self, SSD1306_COLOR color) {
    /* Set memory */
    uint32_t i;

    for(i = 0; i < sizeof(self->buffer); i++) {
        self->buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

void oled_ssd1306_UpdateScreen(oled_ssd1306 *self) {

    uint8_t data[] = {0xB0, 0x00, 0x10};
    uint8_t page_address = 0;
    uint8_t x = 0x80;
    
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        page_address = data[0] + i;
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &x, 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &page_address, 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &x, 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &data[1], 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &x, 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &data[2], 1, self->address));
        ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &self->buffer[SSD1306_WIDTH*i], SSD1306_WIDTH, self->address));
    }
}
