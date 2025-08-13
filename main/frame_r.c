#include "include/frame_r.h"

frame_type_t type_of_raw_frame(void* buffer) {
    u_int8_t chk = (u_int8_t)((u_int8_t*)buffer)[0] & FC_TYPES_MASK;
    switch(chk) {
        case (FC_TYPE_MANAGEMENT):
            return MGMT_TYPE;
            break;
        case (FC_TYPE_CONTROL):
            return CTRL_TYPE;
            break;
        case (FC_TYPE_DATA):
            return DATA_TYPE;
            break;
        default:
            return RSVD_TYPE;
            break;
    }
}