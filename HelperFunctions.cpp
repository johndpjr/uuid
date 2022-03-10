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

// FIXME: random, but not on each seperate run
template <typename T>
void randomize(T& num) {
    for (size_t i{0}; i<sizeof(num)*8; ++i) {
        if (distribution(generator)) {
            num |= 1;
        }
        num <<= 1;
    }
}
template void randomize<uint32_t>(uint32_t&);
template void randomize<uint16_t>(uint16_t&);
//template void randomize<uint16_t>(uint16_t&);
template void randomize<uint8_t>(uint8_t&);
//template void randomize<uint8_t>(uint8_t&);
template void randomize<uint64_t>(uint64_t&);

uint64_t get_node() {
    // FIXME: find actual MAC Address of computer
    // TODO: give option to user to randomly create MAC Address (security concerns)
    // FIXME: use <random> instead of rand()
    uint64_t node {rand() % UINT64_MAX};
    // Set the multicast bit (least-significant bit)
    //  since the MAC Address is pseudo-randomly created
    node |= 1;

    return node;
}