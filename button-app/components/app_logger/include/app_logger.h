
#pragma once

#include <esp_log.h>

#define APP_TAG "app"

#define APP_LOGI(...) ESP_LOGI(APP_TAG, __VA_ARGS__)
#define APP_LOGW(...) ESP_LOGW(APP_TAG, __VA_ARGS__)
#define APP_LOGE(...) ESP_LOGE(APP_TAG, __VA_ARGS__)

void app_logger_save(void);
