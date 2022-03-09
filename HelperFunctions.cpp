#include "HelperFunctions.h"
#include "constants.h"

using namespace constants;

uint64_t get_uuid_ticks() {
    auto now = std::chrono::system_clock::now();
    uint64_t ticks {DELTA_100ns_TICKS_GREG_AND_UNIX_EPOCH};
    // The system clock has microsecond granularity, so multiply the number
    //  of microsecond ticks since UNIX epoch by 10 to get ticks since epoch
    ticks += std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() * 10;
    return ticks;
}

uint16_t get_clock_seq() {
    srand(get_uuid_ticks());
    return rand() % UINT16_MAX;
}

uint64_t get_node() {
    // FIXME: find actual MAC Address of computer
    // TODO: give option to user to randomly create MAC Address (security concerns)
    srand(get_uuid_ticks());
    // FIXME: use <random> instead of rand()
    uint64_t node {rand() % UINT64_MAX};
    // Set the multicast bit (least-significant bit)
    //  since the MAC Address is pseudo-randomly created
    node |= 1;

    return node;
}