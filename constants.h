#ifndef UUID_CONSTANTS_H
#define UUID_CONSTANTS_H

namespace constants {
    // Number of 100ns ticks between October 15, 1582 and January 1, 1970
    const uint64_t DELTA_100ns_TICKS_GREG_AND_UNIX_EPOCH {0x1B21DD213814000};
    // Number of UUIDs that can be generated every tick
    //  given the same timestamp (and microsecond clock granularity)
    const unsigned long MAX_UUIDS_PER_TICK {10};
}
#endif //UUID_CONSTANTS_H
