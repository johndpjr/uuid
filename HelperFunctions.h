#ifndef UUID__HELPERFUNCTIONS_H
#define UUID__HELPERFUNCTIONS_H

#include <bitset>
#include <chrono>
#include <random>
#include <string>

#include "constants.h"

// Returns the number of 100ns ticks since October 15, 1582
// (with microsecond granularity)
uint64_t get_uuid_ticks();

// Returns a random 16-bit number used for the clock sequence
uint16_t get_clock_seq();

template <typename T>
void randomize(T& num);

// Returns the MAC Address of the computer
// Currently just pseudo-randomly generates a MAC Address
uint8_t* get_node();

// Computes the MD5 hash of a message
std::bitset<128> MD5(const std::string& msg);

#endif //UUID__HELPERFUNCTIONS_H
