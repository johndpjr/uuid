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

uint64_t get_uuid_utc_base_time_ticks() {
    time_t timer;
    time(&timer);
    // FIXME: add constants
    // Ticks between October 15, 1582 and January 1, 1970
    uint64_t ticks {0x1B21DD213814000};
    ticks += timer * 0x989680;  // multiply timer by 1e+7 to get intervals
    return ticks;
}