#include "UUID.h"

/*
 * Constructors
 */
UUID::UUID()
    : m_version{Version::Unresolved}
{
}

UUID::UUID(const Version& ver)
    : m_version{ver}
{
    switch(ver) {
        // No need to implement Unresolved version - it defaults to nil UUID
        case Version::Unresolved:
            break;
        case Version::v1:
            // FIXME: implement UUID version 1
            break;
        case Version::v2:
            // FIXME: implement UUID version 2
            break;
        case Version::v3:
            // FIXME: implement UUID version 3
            break;
        case Version::v4:
            // FIXME: implement UUID version 4
            break;
        case Version::v5:
            // FIXME: implement UUID version 5
            break;

        default: ;  // Do something more here
    }
}

UUID::UUID(const UUID &u)
    : m_version{u.m_version},
      m_time_low{u.m_time_low}, m_time_mid{u.m_time_mid},
      m_time_hi_and_version{u.m_time_hi_and_version},
      m_clock_seq_hi_and_reserved{u.m_clock_seq_hi_and_reserved},
      m_clock_seq_low{u.m_clock_seq_low},
      m_node{u.m_node}
{
}

UUID::UUID(const unsigned long &time_low, const unsigned long &time_mid, const unsigned long &time_hi_and_version,
           const unsigned long &clock_seq_hi_and_reserved, const unsigned long &clock_seq_low,
           const unsigned long long &node)
    : m_time_low{time_low}, m_time_mid{time_mid}, m_time_hi_and_version{time_hi_and_version},
      m_clock_seq_hi_and_reserved{clock_seq_hi_and_reserved}, m_clock_seq_low{clock_seq_low},
      m_node{node}
{
    parse_version();
}

UUID::UUID(const std::string& uuid_str)
    : m_time_low{get_binary_str_from_hex_str(uuid_str.substr(0,8))},
      m_time_mid{get_binary_str_from_hex_str(uuid_str.substr(9,4))},
      m_time_hi_and_version{get_binary_str_from_hex_str(uuid_str.substr(14,4))},
      m_clock_seq_hi_and_reserved{get_binary_str_from_hex_str(uuid_str.substr(19,2))},
      m_clock_seq_low{get_binary_str_from_hex_str(uuid_str.substr(21,2))},
      m_node{get_binary_str_from_hex_str(uuid_str.substr(24,12))}
{
    parse_version();
}

/*
 * Functions
 */
unsigned int UUID::get_version() const {
    return m_version;
}

void UUID::parse_version() {
    std::bitset<4> ver_set;
    for (size_t i{12}; i<16; ++i) {
        ver_set[i-12] = m_time_hi_and_version[i];
    }

    unsigned long ver {ver_set.to_ulong()};
    switch (ver) {
        case 1: m_version = Version::v1; break;
        case 2: m_version = Version::v2; break;
        case 3: m_version = Version::v3; break;
        case 4: m_version = Version::v4; break;
        case 5: m_version = Version::v5; break;

        default: m_version = Version::Unresolved;
    }
}

std::string UUID::str() const {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    ss << std::setw(8 ) << m_time_low.to_ulong()            << '-'  // time-low
       << std::setw(4 ) << m_time_mid.to_ulong()            << '-'  // time-mid
       << std::setw(4 ) << m_time_hi_and_version.to_ulong() << '-'  // time-high-and-version
       << std::setw(2 ) << m_clock_seq_hi_and_reserved.to_ulong()   // clock-seq-and-reserved
       << std::setw(2 ) << m_clock_seq_low.to_ulong()       << '-'  // clock-seq-low
       << std::setw(12) << m_node.to_ullong();                      // node

    return ss.str();
}

std::string UUID::urn_str() const {
    return "urn:uuid:" + str();
}

/*
 * Operator Overloading
 */
std::ostream& operator<<(std::ostream& os, const UUID& u) {
    os << u.str();
    return os;
}