#include <stddef.h>
#include <sys/types.h>
#include "frame_control.h"
#include "binary.h"
#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "feature.h"

#ifndef FRAME_H
#define FRAME_H


#define PAYLOAD_32 0xABCDEFEF
#define PAYLOAD_16 0XABCD
#define NO_ADDR_DATA 0xB
#define NO_DATA 0x6E6F

#define FC_PRESENT  0b10000000000
#define DI_PRESENT  0b01000000000
#define AD1_PRESENT 0b00100000000
#define AD2_PRESENT 0b00010000000
#define AD3_PRESENT 0b00001000000
#define SC_PRESENT  0b00000100000
#define AD4_PRESENT 0b00000010000
#define QOS_PRESENT 0b00000001000
#define HT_PRESENT  0b00000000100
#define DAT_PRESENT 0b00000000010
#define FCS_PRESENT 0b00000000001

#define FULL_MAP_NO_DATA FC_PRESENT | DI_PRESENT | AD1_PRESENT | AD2_PRESENT | AD3_PRESENT | AD4_PRESENT | SC_PRESENT | AD4_PRESENT | QOS_PRESENT | HT_PRESENT | FCS_PRESENT

#define SIZE_WITH_NESESSARY_FIELDS_ONLY 14
#define SIZE_WITH_ALL_PRESENT_FIELDS 40

#define NESESSARY_MAP_FIELD AD1_PRESENT | FC_PRESENT | DI_PRESENT | FCS_PRESENT

#define BROADCAST_MAC {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

#define CHECK_REST(mpp) \
    if (mpp->frame->seq_control != NO_DATA) {\
        mpp->map |= SC_PRESENT; \
        mpp->size += 2; \
    }\
    if (mpp->frame->qos_control != NO_DATA) { \
        mpp->map |= QOS_PRESENT; \
        mpp->size += 2; \
    }\
    if (mpp->frame->ht_control != NO_DATA) {\
        mpp->map |= HT_PRESENT; \
        mpp->size += 4; \
    }\

#define TOTAL_ADDRESSES_SIZE(mpp) \
    if (mpp->frame->addr_2[0] != NO_ADDR_DATA) {\
        mpp->map |= AD2_PRESENT; \
        mpp->size += 6; \
    }\
    if (mpp->frame->addr_3[0] != NO_ADDR_DATA) {\
        mpp->map |= AD3_PRESENT; \
        mpp->size += 6; \
    }\
    if (mpp->frame->addr_4[0] != NO_ADDR_DATA) {\
        mpp->map |= AD4_PRESENT; \
        mpp->size += 6; \
    }\

typedef struct {
    char ssid[33];
    uint8_t channel;
    uint8_t** addrs;
} __packed frm_data_t;

typedef struct {
    u_int16_t frame_control;
    u_int16_t frame_id_or_duration;
    u_int8_t addr_1[6];
    u_int8_t addr_2[6];
    u_int8_t addr_3[6];
    u_int16_t seq_control;
} __packed ieee_80211_hdr_t;

typedef struct {
    u_int16_t frame_control;
    u_int16_t frame_id_or_duration;
    u_int8_t addr_1[6];
    u_int8_t addr_2[6];
    u_int8_t addr_3[6];
    u_int16_t seq_control;
    u_int8_t addr_4[6];
    u_int32_t ht_control;
    u_int16_t qos_control;
    u_int8_t* data;
    u_int32_t frame_check_seq;
} __attribute__((packed, unused)) IEEE_80211_frame_t;

typedef struct {
    IEEE_80211_frame_t* frame;
    u_int16_t map;
    size_t size;
    size_t payload_size;
} __attribute__((packed, unused)) mapper_t;

void mapper_init(mapper_t*);
void* to_raw_80211_buffer(mapper_t*);

#endif