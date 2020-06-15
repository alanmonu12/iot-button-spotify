#include "oled-ssd1306.h"
#include "Spotify_icon.h"

//TODO: Agregar el estado del display on/off a la estructura que define la pantalla
// TODO: Definir los valros validos para la frecuancia y el divisor del reloj para el comando 0xD5
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
static void _send_cmd(uint8_t cmd, oled_ssd1306* self) {
    uint8_t data_to_send[] = {CTRL_CMD, cmd};
    ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, data_to_send, sizeof data_to_send, self->address));
}

/**
 * @brief 
 */
static void _send_with_single_byte_cmd(uint8_t cmd, uint8_t byte1, oled_ssd1306 *self) {
    uint8_t data_to_send[] = {CTRL_CMD, cmd, byte1};
    ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, data_to_send, sizeof data_to_send, self->address));
}

/**
 * @brief 
 */
static void _send_with_double_byte_cmd(uint8_t cmd, uint8_t byte1, uint8_t byte2, oled_ssd1306 *self) {
    uint8_t data_to_send[] = {CTRL_CMD, cmd, byte1, byte2};
    ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, data_to_send, sizeof data_to_send, self->address));
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
    self->display.status = OFF;
    self->handler = NULL;
    ESP_ERROR_CHECK(i2c_master_init());

}

/**
 * @brief 
 */
void oled_ssd1306_Init(oled_ssd1306 *self) {

    oled_ssd1306_display_off(self);
    oled_ssd1306_set_multiplex_ratio(self, _31MUX);
    oled_ssd1306_set_display_offset(self, 0x00);
    oled_ssd1306_set_display_start_line(self, START_LINE_00);
    oled_ssd1306_set_segment_remap(self, REMAP_BIT);
    oled_ssd1306_set_com_output_scan_direction(self, REMAP_BIT);
    oled_ssd1306_set_com_pins_hw_config(self, NO_REMAP_BIT, SEQUENTIAL_PIN_CONFIG);
    oled_ssd1306_set_contrast_control(self, 0x8F);
    oled_ssd1306_entire_display_on_resume(self);
    oled_ssd1306_set_normal_display_mode(self);
    oled_ssd1306_set_osc_frenq(self, 0x00, 0x80);
    oled_ssd1306_set_charge_pump_config(self, CHARGE_PUMP_ENABLE_BIT);
    oled_ssd1306_display_on(self);
    oled_ssd1306_set_memory_addressing_mode(self, HORIZONTAL_ADDRESING_MODE);
    //oled_ssd1306_set_column_address(self, 0x00, 0x10);
    oled_ssd1306_set_page_address(self, 0x00, 0xFF);



    oled_ssd1306_Fill(self, Black);
    oled_ssd1306_UpdateScreen(self);


    self->display.Initialized = true;
}

/**
 * @brief 
 */
void oled_ssd1306_Fill(oled_ssd1306 *self, SSD1306_COLOR color) {
    
    /* Set memory */
    uint32_t i;
    self->buffer[0] = CTRL_DATA;
    ESP_LOGI("OLED", "%u", self->buffer[0]);
    for(i = 1; i < 513; i++) {
        self->buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }

}

/**
 * @brief 
 */
void oled_ssd1306_UpdateScreen(oled_ssd1306 *self) {

    ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, self->buffer, 513, self->address));
    //uint8_t data[] = {0xB0, 0x00, 0x10};
    //uint8_t page_address = 0;
    //uint8_t ctrl_data = 0x40;
    //ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, bitmap, sizeof bitmap, self->address));
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    //for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        //page_address = data[0] + i;
        //ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &page_address, 1, self->address));
        //uint8_t data_to_send[] = {ctrl_data, 0x00};
        //ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, data_to_send, sizeof data_to_send, self->address));
        //ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &data[1], 1, self->address));
        //ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &data[2], 1, self->address));
        //ESP_ERROR_CHECK(i2c_master_write_slave(I2C_MASTER_NUM, &self->buffer[SSD1306_WIDTH*i], SSD1306_WIDTH, self->address));
    //}
}

/**
 * @brief 
 */
void oled_ssd1306_display_on(oled_ssd1306 *self) {
    _send_cmd(SET_DISPLAY_ON_CMD, self);
    self->display.status = ON;
}

/**
 * @brief 
 */
void oled_ssd1306_display_off(oled_ssd1306 *self) {
    _send_cmd(SET_DISPLAY_OFF_CMD, self);
    self->display.status = OFF;
}

/**
 * @brief 
 */
void oled_ssd1306_set_multiplex_ratio(oled_ssd1306 *self, SSD1306_MUX_RATIO_TypeDef mux_ratio) {
    _send_with_single_byte_cmd(SET_MULTIPLEX_RATIO_CMD, mux_ratio, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_display_offset(oled_ssd1306 *self, uint8_t off_set) {
    _send_with_single_byte_cmd(SET_DISPLAY_OFFSET_CMD, off_set, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_display_start_line(oled_ssd1306 *self, SSD1306_START_LINE_TypeDef start_line) {
    _send_cmd(start_line, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_segment_remap(oled_ssd1306 *self, SSD1306_REMAP_BIT_TypeDef bit) {
    uint8_t remap_cmd = 0xA0 | bit;
    _send_cmd(remap_cmd, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_com_output_scan_direction(oled_ssd1306 *self, SSD1306_REMAP_BIT_TypeDef bit) {
    uint8_t remap_cmd = 0xC0 | (bit << 3);
    _send_cmd(remap_cmd, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_com_pins_hw_config(oled_ssd1306 *self, SSD1306_REMAP_BIT_TypeDef remap_bit, SSD1306_PIN_CONFIG_BIT_TypeDef pin_bit) {
    uint8_t remap_cmd = 0x02 | (remap_bit << 5) | (pin_bit << 4);
    _send_with_single_byte_cmd(SET_COM_PINS_HW_CONFIG_CMD, remap_cmd, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_contrast_control(oled_ssd1306 *self, uint8_t contrast) {
    _send_with_single_byte_cmd(SET_CONTRAST_CONTROL_CMD, contrast, self);
}

/**
 * @brief 
 */
void oled_ssd1306_entire_display_on(oled_ssd1306 *self) {
    _send_cmd(ENTIRE_DISPLAY_ON_CMD, self);
}

/**
 * @brief 
 */
void oled_ssd1306_entire_display_on_resume(oled_ssd1306 *self) {
    _send_cmd(ENTIRE_DISPLAY_ON_RESUME_CMD, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_normal_display_mode(oled_ssd1306 *self) {
    _send_cmd(SET_NORMAL_DISPLAY_MODE_CMD, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_inverse_display_mode(oled_ssd1306 *self) {
    _send_cmd(SET_INVERSE_DISPLAY_MODE_CMD, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_osc_frenq(oled_ssd1306 *self, uint8_t divider, uint8_t frequency) {
    uint8_t data = frequency | divider;
    _send_with_single_byte_cmd(SET_CLOCK_FQ_CMD, data, self);

}

/**
 * @brief 
 */
void oled_ssd1306_set_charge_pump_config(oled_ssd1306 *self, SSD1306_CHARGE_PUMP_BIT_TypeDef bit) {
    uint8_t data = 0x10 | (bit << 2);
    _send_with_single_byte_cmd(SET_CHARGE_PUMP_SETTING_CMD, data, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_memory_addressing_mode(oled_ssd1306 *self,SSD1306_ADDRESSING_MODE_TypeDef mode) {
    _send_with_single_byte_cmd(SET_MEMORY_ADDRESSING_MODE_CMD, mode, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_column_address(oled_ssd1306 *self, uint8_t column_start, uint8_t column_end) {
    _send_with_double_byte_cmd(SET_COLUMN_ADDRESS_CMD, column_start, column_start, self);
}

/**
 * @brief 
 */
void oled_ssd1306_set_page_address(oled_ssd1306 *self, uint8_t page_start, uint8_t page_end) {
    _send_with_double_byte_cmd(SET_PAGE_ADDRESS_CMD, page_start, page_end, self);
}

/**
 * @brief 
 */
void oled_ssd1306_draw_bitmap(oled_ssd1306 *self, uint8_t *bitmap, uint16_t width, uint16_t height) {
    uint8_t byte_data = 0x00;
    uint8_t bytes_in_width = width / 8;
    uint8_t bytes_in_height = height / 8;
}
