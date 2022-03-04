#include "HelperFunctions.h"

std::string get_bin_str_from_hex_str(const std::string& hex_str) {
    std::string bin_str;
    for (size_t i{0}; i<hex_str.length(); ++i) {
        switch (hex_str.at(i)) {
            case '0': bin_str.append("0000"); break;
            case '1': bin_str.append("0001"); break;
            case '2': bin_str.append("0010"); break;
            case '3': bin_str.append("0011"); break;
            case '4': bin_str.append("0100"); break;
            case '5': bin_str.append("0101"); break;
            case '6': bin_str.append("0110"); break;
            case '7': bin_str.append("0111"); break;
            case '8': bin_str.append("1000"); break;
            case '9': bin_str.append("1001"); break;
            case 'a':
            case 'A': bin_str.append("1010"); break;
            case 'b':
            case 'B': bin_str.append("1011"); break;
            case 'c':
            case 'C': bin_str.append("1100"); break;
            case 'd':
            case 'D': bin_str.append("1101"); break;
            case 'e':
            case 'E': bin_str.append("1110"); break;
            case 'f':
            case 'F': bin_str.append("1111"); break;
        }
    }

    return bin_str;
}

uint64_t get_uuid_ticks() {
    auto now = std::chrono::system_clock::now();
    uint64_t ticks {constants::DELTA_100ns_TICKS_GREG_AND_UNIX_EPOCH};
    // The system clock has microsecond granularity, so multiply the number
    //  of microsecond ticks since UNIX epoch by 10 to get ticks since epoch
    ticks += std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() * 10;
    return ticks;
}

uint16_t get_clock_seq() {
    srand(get_uuid_ticks());
    return static_cast<uint16_t>(rand() % UINT16_MAX);
}

std::bitset<48> get_node() {
    // FIXME: find actual MAC Address of computer
    // TODO: give option to user to randomly create MAC Address (security concerns)
    srand(get_uuid_ticks());
    std::bitset<48> node;
    // Set the multicast bit (least-significant bit)
    //  since the MAC Address is pseudo-randomly created
    node.set(0);
    for (size_t i{1}; i<48; ++i) {
        // FIXME: using rand() isn't recommended, so use <random>
        if (rand() % 2) {
            node.set(i);
        }
    }

    return node;
}

// FIXME: very slow - 136,550 per second generation (should support 10 million/sec)
template <size_t N>
void randomize_bitset(std::bitset<N>& bitset) {
    srand(get_uuid_ticks());
    for (size_t i{0}; i<bitset.size(); ++i) {
        if (rand() % 2) {
            bitset.set(i);
        }
    }
}
// FIXME: add constants for field sizes, like NODE_SIZE
template void randomize_bitset<32>(std::bitset<32>&);
template void randomize_bitset<16>(std::bitset<16>&);
//template void randomize_bitset<16>(std::bitset<16>&);
template void randomize_bitset<8>(std::bitset<8>&);
//template void randomize_bitset<8>(std::bitset<8>&);
template void randomize_bitset<48>(std::bitset<48>&);