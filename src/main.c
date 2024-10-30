#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include "LCD.h"

#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_SCREEN_NODE DT_ALIAS(lcd_screen)

const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, (0));
static const struct i2c_dt_spec lcd_screen = I2C_DT_SPEC_GET(LCD_SCREEN_NODE);

#define SLEEP_TIME_MS 10000  

int main(void) {
    if (gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH) < 0) {
        printk("Failed to configure LED pin\n");
        return;
    }

 
    printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

   
    init_lcd(&lcd_screen);

    
    const struct device *dht11 = DEVICE_DT_GET_ONE(aosong_dht);
    if (!device_is_ready(dht11)) {
        printk("DHT11 sensor not ready\n");
        return;
    }

    printk("DHT11 sensor is ready\n");

   
    while (1) {
 
    if (sensor_sample_fetch(dht11) < 0) {
        printk("Failed to fetch data from sensor\n");
        continue;
    }

    struct sensor_value temp, humidity;


    if (sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
        printk("Failed to get temperature\n");
        continue;
    }


    if (sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY, &humidity) < 0) {
        printk("Failed to get humidity\n");
        continue;
    }

 
    

    printk("Temperature: %d.%06d C, Humidity: %d.%06d%%\n", temp.val1, temp.val2, humidity.val1,humidity.val2 );

    k_msleep(SLEEP_TIME_MS);
}
}

