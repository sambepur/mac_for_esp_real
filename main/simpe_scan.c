#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "include/frame.h"
#include "include/frame_r.h"
#include "nvs_flash.h"

#define MAX_CLIENTS 10

static const char* target = "I HAVE WAN";
static const char* tag = "SENDER";

static wifi_ap_data dta;
static vector_t vec = {
    .d_size = 6,
    .mem_type = MALLOC_CAP_8BIT
};



static void printf_mac(uint8_t* mac) {
    for (int i = 0; i<6; i++) {
        printf("%2x:", mac[i]);
    }
    printf("\n");
    fflush(stdout);
}


static void wifi_promiscuous_pkt_cb(void* buf, wifi_promiscuous_pkt_type_t pkt) {
    static uint8_t cached[MAX_CLIENTS][6];
    static uint8_t counter = 0;
    if (pkt == WIFI_PKT_MGMT || pkt == WIFI_PKT_CTRL) {
        wifi_promiscuous_pkt_t* pak = (wifi_promiscuous_pkt_t*)buf;
        if (pak->rx_ctrl.sig_len >= 24) {
            ieee_80211_hdr_t* prime = (ieee_80211_hdr_t*) pak->payload;
            if (memcmp(prime->addr_3, dta.bssid, 6)) {
                if (!is_cached(cached, prime->addr_1, 6)) {
                    if (vector_add(dta.clients, prime->addr_1) != VEC_OK) {
                        ESP_LOGI
                    }
                }
            }
        }
    }
}


void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    wifi_promiscuous_filter_t filter = {
        .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_CTRL
    };
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    vector_err_t check = vector_init(&vec, MAX_CLIENTS);
    if (check == DRAM_FULL) {
        ESP_LOGI(tag, "DRAM INIT FIALED");
    }


    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    uint16_t len = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_stop());
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&len));
    wifi_ap_record_t rec[len];
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&len, rec));
    for (int i = 0; i<len; i++) {
        wifi_ap_record_t curr = rec[i];
        if (strcmp(target, curr.ssid)) {
            dta.auth = curr.authmode;
            dta.chan = curr.primary;
            memcpy(dta.bssid, curr.bssid, 6);
            ESP_ERROR_CHECK(esp_wifi_set_channel(curr.primary, WIFI_SECOND_CHAN_NONE));
        }
        printf("SSID: %s\n", curr.ssid);
        printf_mac(curr.bssid);
        printf("CHANNEL: %d\n", curr.primary);
    }
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_pkt_cb));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&filter));
    ESP_ERROR_CHECK(esp_wifi_stop());
}