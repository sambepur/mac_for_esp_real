#include "frame.h"
#include <stdbool.h>

#ifndef FRAME_R_H
#define FRAME_R_H

#define FRAME_FIRST_MAC_OFFT 4
#define FRAME_SECOND_MAC_OFFT 10
#define FRAME_THIRD_MAC_OFFT 16

#define MORE_LIKELY_MGMT_FRAME_MAP FC_PRESENT | DI_PRESENT | AD1_PRESENT | AD2_PRESENT | AD3_PRESENT | SC_PRESENT | FCS_PRESENT
#define MORE_LIKELY_MGMT_FRAME_SIZE_WITH_NO_DATA 28

#define FC_TYPES_MASK 0b0000000000001100

typedef struct {
    uint8_t bssid[6];
    uint8_t chan;
} filter_t;


typedef struct {
    char ssid[33];
    uint8_t chan;
    vector_t* clients;
} __packed wifi_ap_data;

bool match(ieee_80211_hdr_t*, filter_t*);

#endif