#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "include/frame.h"
#include "include/frame_r.h"
#include "nvs_flash.h"

#define MACS_BUFFER_SIZE 128

static const char* tag = "SENDER";

//static uint8_t macs[MACS_BUFFER_SIZE];
//static uint8_t macs_offset;

static void printf_mac(uint8_t* mac) {
    printf("BSSID: ");
    for (int i = 0; i<6; i++) {
        printf("%2x:", mac[i]);
    }
    printf("\n");
}


static void wifi_promiscuous_pkt_cb(void* buf, wifi_promiscuous_pkt_type_t pkt) {
    /*
    if (pkt.rx_ctrl.sig_len <= MGMT_FRAME_SIZE) {
        frame_type_t type = type_of_raw_frame(buf);
        if (type == MGMT_TYPE || type == CTRL_TYPE) {
            macs[macs_offset] = pkt.payload + FRAME_FIRST_MAC_OFFT;
            macs_offset++;
            macs[macs_offset] = pkt.payload + FRAME_SECOND_MAC_OFFT;
            macs_offset++;
            macs[macs_offset] = pkt.payload + FRAME_THIRD_MAC_OFFT;
        }
    }
    */
    //frame_type_t type = type_of_raw_frame(buf);

    for (uint8_t d = 0; d< MGMT_FRAME_SIZE; d++) {
        printf("%x ", ((uint8_t*)buf)[d]);
    }
    printf("\n");
}


void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    /*
    wifi_promiscuous_filter_t filter = {
        .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_CTRL
    };
    */
    printf("Free heap size before: %ld\n", esp_get_free_heap_size());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    printf("Free heap size after: %ld\n", esp_get_free_heap_size());
    ESP_LOGI(tag, "%s\n", "INIT");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(tag, "%s\n", "STARTED");
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_LOGI(tag, "%s\n", "SCAN_STARTED");
    uint16_t len = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_stop());
    ESP_LOGI(tag, "%s\n", "SCAN_STOPPED");
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&len));
    wifi_ap_record_t rec[len];
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&len, rec));
    ESP_LOGI(tag, "%s\n", "AP_REC1");
    for (int i = 0; i<len; i++) {
        printf("ITER %d\n", i);
        printf("SSID: %s\n", rec[i].ssid);
        printf_mac(rec[i].bssid);
        printf("CHANNEL: %d\n", rec[i].primary);
    }

    wifi_scan_config_t scfg = {
        .channel_bitmap = {
            .ghz_2_channels = 0b0000000000000000 | (1 << rec[0].primary)
        },
        .bssid = rec[0].bssid,
        .channel = rec[0].primary,
        .ssid = rec[0].ssid,
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {
                .max = 500,
                .min = 100
            },
            .passive = 1000
        },
        .home_chan_dwell_time = 200
    };

    //ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, wifi_scan_done, NULL));
    
    memset(rec, 0, (size_t) len);
    len = 0;
    ESP_LOGI(tag, "%s\n", "CLEARED");
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scfg, true));
    ESP_LOGI(tag, "%s\n", "SCAN2_STARTED");
    ESP_ERROR_CHECK(esp_wifi_scan_stop());
    ESP_LOGI(tag, "%s\n", "SCAN2_STOPPED");
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&len));
    ESP_LOGI(tag, "%s: %d\n", "LEN2", len);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&len, rec));
    ESP_LOGI(tag, "%s\n", "AP_REC2");
    for (int i = 0; i<len; i++) {
        printf("SSID: %s\n", rec[i].ssid);
        printf_mac(rec[i].bssid);
        printf("CHANNEL: %d\n", rec[i].primary);
    }
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_LOGI(tag, "%s\n", "WIFI_STOPPED");
    /*
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_pkt_cb));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&filter));
    */

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