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
        // No action needed for an Unresolved version - it defaults to a Nil UUID
        case Version::Unresolved:
            break;
        case Version::v1:
            // TODO: implement UUID version 1
            v1_uuid();
            break;
        case Version::v3:
            // TODO: implement UUID version 3
            break;
        case Version::v4:
            // TODO: implement UUID version 4
            break;
        case Version::v5:
            // TODO: implement UUID version 5
            break;
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

UUID::UUID(const std::string& uuid_str)
    : m_time_low{get_bin_str_from_hex_str(uuid_str.substr(0,8))},
      m_time_mid{get_bin_str_from_hex_str(uuid_str.substr(9, 4))},
      m_time_hi_and_version{get_bin_str_from_hex_str(uuid_str.substr(14, 4))},
      m_clock_seq_hi_and_reserved{get_bin_str_from_hex_str(uuid_str.substr(19, 2))},
      m_clock_seq_low{get_bin_str_from_hex_str(uuid_str.substr(21, 2))},
      m_node{get_bin_str_from_hex_str(uuid_str.substr(24, 12))}
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
        case 3: m_version = Version::v3; break;
        case 4: m_version = Version::v4; break;
        case 5: m_version = Version::v5; break;

        default: m_version = Version::Unresolved;
    }
}

void UUID::v1_uuid() {
    uint64_t uuid_time {get_uuid_utc_base_time_ticks()};

    // Low 4 bytes of the time
    m_time_low = std::bitset<32>{uuid_time & 0xFFFFFFFF};
    uuid_time >>= 32;  // shift 32 bits
    // Mid 2 bytes of the time
    m_time_mid = std::bitset<16>{uuid_time & 0xFFFF};
    uuid_time >>= 16;  // shift 16 bits
    // Add 4 bit version and high 12 bits of the time
    m_time_hi_and_version = std::bitset<16>{0x1000 | (uuid_time & 0xFFF)};

    // TODO: clock-seq-and-reserved
    // TODO: clock-seq-low
    // TODO: node
}

void UUID::v3_uuid() {
}

void UUID::v4_uuid() {
}

void UUID::v5_uuid() {
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

std::istream& operator>>(std::istream& is, UUID& u) {
    std::string u_str;
    is >> u_str;
    UUID new_u {u_str};
    u = new_u;
    return is;
}

bool operator==(const UUID& lhs, const UUID& rhs) {
    return (lhs.m_time_low == rhs.m_time_low &&
            lhs.m_time_mid == rhs.m_time_mid &&
            lhs.m_time_hi_and_version == rhs.m_time_hi_and_version &&
            lhs.m_clock_seq_hi_and_reserved == rhs.m_clock_seq_hi_and_reserved &&
            lhs.m_clock_seq_low == rhs.m_clock_seq_low &&
            lhs.m_node == rhs.m_node);
}

bool operator!=(const UUID& lhs, const UUID& rhs) {
    return !(lhs == rhs);
}

bool operator>(const UUID& lhs, const UUID& rhs) {
    return (lhs.str() > rhs.str());
}

bool operator<(const UUID& lhs, const UUID& rhs) {
    return (lhs.str() < rhs.str());
}

bool operator>=(const UUID& lhs, const UUID& rhs) {
    return (lhs.str() >= rhs.str());
}

bool operator<=(const UUID& lhs, const UUID& rhs) {
    return (lhs.str() <= rhs.str());
}
