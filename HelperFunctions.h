#ifndef UUID__HELPERFUNCTIONS_H
#define UUID__HELPERFUNCTIONS_H

#include <bitset>
#include <string>
#include <chrono>
#include <random>

#include "constants.h"

static std::random_device rd;
static std::default_random_engine generator {rd()};
static std::bernoulli_distribution distribution {0.5};

// Returns the number of 100ns ticks since October 15, 1582
// (with microsecond granularity)
uint64_t get_uuid_ticks();

// Returns a random 16-bit number used for the clock sequence
uint16_t get_clock_seq();

template <typename T>
void randomize(T& num);

// Returns the MAC Address of the computer
// Currently just pseudo-randomly generates a MAC Address
uint64_t get_node();

#endif //UUID__HELPERFUNCTIONS_H
