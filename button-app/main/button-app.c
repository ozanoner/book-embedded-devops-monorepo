
#include "app_logger.h"
#include "iot_button.h"
#include "button_gpio.h"
#include "sdkconfig.h"

#include "esp_insights.h"
#include "esp_rmaker_utils.h"
#include "devops_easy_connect.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *WIFI_SSID = "Wokwi-GUEST";
static const char *WIFI_PASS = "";
extern const char insights_auth_key_start[] asm("_binary_insights_auth_key_txt_start");
extern const char insights_auth_key_end[] asm("_binary_insights_auth_key_txt_end");

static button_handle_t app_button = NULL;
static bool esp_insights_connected = false;
static StaticTask_t insights_task_buf;
static StackType_t insights_task_stack[4096];

static void app_single_click_cb(void *button_handle, void *usr_data);
static void app_long_press_cb(void *button_handle, void *usr_data);
static void on_connected(void *arg);
static void init_insights_agent(void *arg);

void app_main(void)
{
    APP_LOGI("The app is starting...");
    app_wifi_init(WIFI_SSID, WIFI_PASS, on_connected, NULL);
    app_wifi_connect();

    const button_config_t button_cfg = {0};
    const button_gpio_config_t gpio_cfg = {
        .gpio_num = CONFIG_APP_BUTTON};

    iot_button_new_gpio_device(&button_cfg, &gpio_cfg, &app_button);
    iot_button_register_cb(app_button, BUTTON_SINGLE_CLICK, NULL, app_single_click_cb, NULL);
    iot_button_register_cb(app_button, BUTTON_LONG_PRESS_START, NULL, app_long_press_cb, NULL);

    app_logger_save();
}

static void app_single_click_cb(void *button_handle, void *usr_data)
{
    if (esp_insights_connected)
    {
        ESP_DIAG_EVENT("btn", "single-click");
    }
}

static void app_long_press_cb(void *button_handle, void *usr_data)
{
    if (esp_insights_connected)
    {
        ESP_DIAG_EVENT("btn", "long-press");
    }
}

static void on_connected(void *arg)
{

    TaskHandle_t created = xTaskCreateStatic(
        init_insights_agent,
        "insights_init",
        4096,
        NULL,
        5,
        insights_task_stack,
        &insights_task_buf);

    if (created == NULL)
    {
        APP_LOGE("Failed to create insights initialization task");
    }
}

static void init_insights_agent(void *arg)
{
    APP_LOGI("WiFi connected, starting ESP Insights...");

    esp_rmaker_time_sync_init(NULL);

    esp_insights_config_t config = {
        .log_type = ESP_DIAG_LOG_TYPE_EVENT,
        .auth_key = insights_auth_key_start,
    };

    esp_err_t ret = esp_insights_init(&config);
    if (ret != ESP_OK)
    {
        APP_LOGE("Failed to init ESP Insights, err:0x%x", ret);
    }
    else
    {
        esp_insights_connected = true;
        APP_LOGI("ESP Insights initialized successfully");
    }

    vTaskDelete(NULL);
}


