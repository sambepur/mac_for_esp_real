#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "include/frame.h"
#include "include/frame_r.h"
#include "nvs_flash.h"

#define TARGET_SSID "I HAVE WAN"
#define MACS_BUFFER_SIZE 128

static const char* tag = "SENDER";

static wifi_ap_data;

static void printf_mac(uint8_t* mac) {
    for (int i = 0; i<6; i++) {
        printf("%2x:", mac[i]);
    }
    printf("\n");
    fflush(stdout);
}


static void wifi_promiscuous_pkt_cb(void* buf, wifi_promiscuous_pkt_type_t pkt) {
    if (pkt == WIFI_PKT_MGMT || pkt == WIFI_PKT_CTRL) {
        wifi_promiscuous_pkt_t* pak = (wifi_promiscuous_pkt_t*)buf;
        if (pak->rx_ctrl.sig_len >= 24) {
            ieee_80211_hdr_t* prime = (ieee_80211_hdr_t*) pak->payload;

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
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    uint16_t len = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_stop());
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&len));
    wifi_ap_record_t rec[len];
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&len, rec));
    for (int i = 0; i<len; i++) {
        printf("ITER %d\n", i);
        printf("SSID: %s\n", rec[i].ssid);
        printf_mac(rec[i].bssid);
        printf("CHANNEL: %d\n", rec[i].primary);
    }
    //ESP_ERROR_CHECK(esp_wifi_stop());
    
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_pkt_cb));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&filter));

    /*
    mapper_t mpp;
    mpp.payload_size = 2;
    u_int8_t payload[] = {0x1, 0x0};
    IEEE_80211_frame_t frame = {
        .frame_control = FC_VER | FC_TYPE_MANAGEMENT | FC_SUBTYPE_DEAUTH,
        .frame_id_or_duration = 0x0,
        .addr_1 = NULL,
        .addr_2 = NULL,
        .addr_3 = NULL,
        .seq_control = 0,
        .addr_4 = NO_ADDR_DATA,
        .qos_control = NO_DATA,
        .ht_control = NO_DATA,
        .data = (void*) payload,
        .frame_check_seq = 0
    };

    mpp.frame = frame;
    mapper_init(&mpp);
    void* buffer = to_raw_80211_buffer(&mpp);
    if (buffer == NULL) {
        ESP_LOGE(tag, "buffer init failed %p\n", buffer);
        return ESP_FAIL;
    }
    */
    
}