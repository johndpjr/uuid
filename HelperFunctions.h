#ifndef UUID__HELPERFUNCTIONS_H
#define UUID__HELPERFUNCTIONS_H

#include <bitset>
#include <string>
#include <chrono>

#include "constants.h"


// Returns a binary string from a hexadecimal string
std::string get_bin_str_from_hex_str(const std::string& hex_str);

// Returns the number of 100ns ticks since October 15, 1582
// (with microsecond granularity)
uint64_t get_uuid_ticks();

#endif //UUID__HELPERFUNCTIONS_H
