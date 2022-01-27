#include "UUID.h"

/*
 * Constructors
 */
UUID::UUID()
    : m_bitset{}, m_version{0} {
}

UUID::UUID(const UUID &uuid)
    : m_bitset{uuid.m_bitset}, m_version{uuid.m_version} {
}

/*
 * Getters
 */
unsigned int UUID::get_version() const {
    return m_version;
}

std::bitset<128> UUID::get_bitset() const {
    return m_bitset;
}

/*
 * Functions
 */

std::string UUID::str() {
    std::string uuid_str{m_bitset.to_string()};

    std::stringstream ss;
    ss << uuid_str.substr(0 , 32) << '-'  // time-low
       << uuid_str.substr(32, 16) << '-'  // time-mid
       << uuid_str.substr(48, 16) << '-'  // time-high-and-version
       << uuid_str.substr(64, 8 )         // clock-seq-and-reserved
       << uuid_str.substr(72, 8 ) << '-'  // clock-seq-low
       << uuid_str.substr(80, 48);        // node

   return ss.str();
}