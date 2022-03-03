#ifndef UUID__CONSTANTS_H
#define UUID__CONSTANTS_H

namespace constants {
    // Number of 100ns ticks between October 15, 1582 and January 1, 1970
    const uint64_t DELTA_100ns_TICKS_GREG_AND_UNIX_EPOCH{0x1B21DD213814000};
    // Number of UUIDs that can be generated every microsecond
    //     The clock sequence hi and low has 2^13 (8192) distinct values (excluding the variant)
    //     This means 8192 UUIDs can be generated for the same timestamp
    //     (effectively meaning 8192 UUIDs can be generated every microsecond)
    const unsigned long UUIDS_PER_MICROSECOND_TICK{8'192};
}
#endif //UUID__CONSTANTS_H
