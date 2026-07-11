
#include "app_logger.h"
#include "iot_button.h"
#include "button_gpio.h"
#include "sdkconfig.h"

static button_handle_t app_button = NULL;

static void app_single_click_cb(void *button_handle, void *usr_data)
{
    APP_LOGI("single-click");
}

static void app_long_press_cb(void *button_handle, void *usr_data)
{
    APP_LOGI("long-press");
}

void app_main(void)
{
    APP_LOGI("The app is starting...");

    const button_config_t button_cfg = {0};
    const button_gpio_config_t gpio_cfg = {
        .gpio_num = CONFIG_APP_BUTTON};

    iot_button_new_gpio_device(&button_cfg, &gpio_cfg, &app_button);
    iot_button_register_cb(app_button, BUTTON_SINGLE_CLICK, NULL, app_single_click_cb, NULL);
    iot_button_register_cb(app_button, BUTTON_LONG_PRESS_START, NULL, app_long_press_cb, NULL);

    app_logger_save();
}
