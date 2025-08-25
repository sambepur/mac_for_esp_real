#include "include/frame.h"

void mapper_init(mapper_t* mpp) {
    #ifndef FRAME_LEAVE_BLNK_FIELDS
        mpp->map = FULL_MAP_NO_DATA;
        mpp->size = SIZE_WITH_ALL_PRESENT_FIELDS;
    #else
        mpp->size = SIZE_WITH_NESESSARY_FIELDS_ONLY;
        mpp->map = NESESSARY_MAP_FIELD;
        TOTAL_ADDRESSES_SIZE(mpp);
        CHECK_REST(mpp);
    #endif
    if (mpp->frame->data != NULL) {
        mpp->map |= DAT_PRESENT;
        mpp->size += mpp->payload_size;
    }
}

void* to_raw_80211_buffer(mapper_t* mpp) {
    u_int8_t* buff = (u_int8_t*) malloc(mpp->size);
    u_int8_t offset = 0;
    if (mpp->map & FC_PRESENT) {
        BIN_WORD_INTO_ARRAY(mpp->frame->frame_control, buff, offset);
        offset+=2;
    }
    if (mpp->map & DI_PRESENT) {
        BIN_WORD_INTO_ARRAY(mpp->frame->frame_id_or_duration, buff, offset);
        offset+=2;
    }
    if (mpp->map & AD1_PRESENT) {
        if (cut(mpp->frame->addr_1, buff, offset, 6) == -1){
            return NULL;
        }
        offset += 6;
    }
    if (mpp->map & AD2_PRESENT) {
        if(cut(mpp->frame->addr_2, buff, offset, 6) == -1) {
            return NULL;
        }
        offset+=6;
    }
    if (mpp->map & AD3_PRESENT) {
        if (cut(mpp->frame->addr_3, buff, offset, 6) == -1) {
            return NULL;
        }
        offset+=6;
    }
    if (mpp->map & SC_PRESENT) {
        BIN_WORD_INTO_ARRAY(mpp->frame->seq_control, buff, offset);
        offset+=2;
    }
    if (mpp->map & AD4_PRESENT) {
        if (cut(mpp->frame->addr_4, buff, offset, 6) == -1) {
            return NULL;
        }
        offset+=6;
    }
    if (mpp->map & QOS_PRESENT) {
        BIN_WORD_INTO_ARRAY(mpp->frame->qos_control, buff, offset);
        offset+=2;
    }
    if (mpp->map & HT_PRESENT) {
        BIN_DWORD_INTO_ARRAY(mpp->frame->ht_control, buff, offset);
        offset+=4;
    }
    if (mpp->map & DAT_PRESENT) {
        if (cut(mpp->frame->data, buff, offset, mpp->payload_size) == -1) {
            return NULL;
        }
        offset += mpp->payload_size;
    }
    if (mpp->map & FCS_PRESENT) {
        BIN_DWORD_INTO_ARRAY(mpp->frame->frame_check_seq, buff, offset);
        offset+=4;
    }
    return buff;
}