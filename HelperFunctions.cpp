#include "HelperFunctions.h"
#include "constants.h"

using namespace constants;

uint64_t get_uuid_ticks() {
    auto now = std::chrono::system_clock::now();
    uint64_t ticks {DELTA_100ns_TICKS_GREG_AND_UNIX_EPOCH};
    // The system clock has microsecond granularity, so multiply the number
    //  of microsecond ticks since UNIX epoch by 10 (10 100ns ticks/microsecond)
    ticks += std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() * 10;
    return ticks;
}

uint16_t get_clock_seq() {
    uint16_t clock_seq {0};
    randomize(clock_seq);
    return clock_seq;
}

template <typename T>
void randomize(T& num) {
    static std::random_device rd;
    static std::default_random_engine generator {rd()};
    static std::bernoulli_distribution distribution {0.5};
    for (size_t i{0}; i<sizeof(num)*8; ++i) {
        if (distribution(generator)) {
            num |= 1;
        }
        num <<= 1;
    }
}
// Implement randomize() template with fields
template void randomize<uint32_t>(uint32_t&);    // time-low
template void randomize<uint16_t>(uint16_t&);    // time-mid
//template void randomize<uint16_t>(uint16_t&);  // time-high-and-version
template void randomize<uint8_t>(uint8_t&);      // clock-seq-and-reserved
//template void randomize<uint8_t>(uint8_t&);    // clock-seq-low
//template void randomize<uint8_t>(uint8_t&);    // node

uint8_t* get_node() {
    auto node = new uint8_t[6];
    for (size_t i{0}; i<6; ++i) {
        randomize(node[i]);
    }
    // Set the multicast bit (least-significant bit)
    //  since the MAC Address is pseudo-randomly created
    node[6] |= 1;

    return node;
}