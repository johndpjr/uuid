#ifndef UUID__CONSTANTS_H
#define UUID__CONSTANTS_H

namespace constants {
    // Number of 100ns ticks between October 15, 1582 and January 1, 1970
    const uint64_t DELTA_100ns_TICKS_GREG_AND_UNIX_EPOCH {0x1B21DD213814000};
    // Number of UUIDs that can be generated every tick
    //  given the same timestamp (and microsecond clock granularity)
    const unsigned long MAX_UUIDS_PER_TICK {10};
    // Masks out the 3 most-significant bits of the clock sequence
    //  to leave room for the variant
    const unsigned int CLOCK_SEQ_MASK {0x1FFF};

    /* Field Sizes
     */
    const size_t TIME_LOW_SIZE                  {32};
    const size_t TIME_MID_SIZE                  {16};
    const size_t TIME_HI_AND_VERSION_SIZE       {16};
    const size_t CLOCK_SEQ_HI_AND_RESERVED_SIZE { 8};
    const size_t CLOCK_SEQ_LOW_SIZE             { 8};
    const size_t NODE_SIZE                      {48};
}
#endif //UUID__CONSTANTS_H
