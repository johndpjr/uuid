#include "UUID.h"

uint64_t UUID::s_last_uuid_time      {get_uuid_ticks()};
unsigned int UUID::s_uuids_this_tick {0};
uint16_t UUID::s_clock_seq           {get_clock_seq()};
std::bitset<48> UUID::s_mac_adr      {get_node()};

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
    : m_version{u.m_version}, m_variant{u.m_variant},
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

unsigned int UUID::get_variant() const {
    return m_variant;
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
    /*
     * time-low
     * time-mid
     * time-hi-and-version
     */
    uint64_t uuid_time {get_uuid_ticks()};
    ++s_uuids_this_tick;
    // If the system clock hasn't ticked ( >1 UUID requested in 1 microsecond),
    //  increment the timestamp to simulate a 100ns tick passing
    if (uuid_time == s_last_uuid_time) {
        uuid_time += s_uuids_this_tick;
        if (s_uuids_this_tick > constants::MAX_UUIDS_PER_TICK) {
            while (uuid_time == s_last_uuid_time) {
                uuid_time = get_uuid_ticks();
            }
        }
    }
    else {
        s_uuids_this_tick = 0;
    }

    // Construct low 4 bytes of the time
    m_time_low = uuid_time & 0xFFFFFFFF;  // 32 bits
    // Construct mid 2 bytes of the time
    // Shift 32 bits to remove the low 4 bytes
    m_time_mid = (uuid_time>>32) & 0xFFFF;  // 16 bits
    // Construct 4 bit version and high 12 bits of the time
    // Shift 48 bits (32 + 16) to remove the low and mid 6 bytes
    m_time_hi_and_version = 0x1000 | ((uuid_time>>48) & 0xFFF);  // 16 bits

    s_last_uuid_time = uuid_time - s_uuids_this_tick;
    /*
     * clock-seq-hi-and-reserved
     * clock-seq-low
     */
    // FIXME: this is a hardcoded variant 1 (0b10x)
    m_clock_seq_low = s_clock_seq & 0xFF;
    m_clock_seq_hi_and_reserved = (s_clock_seq>>8) & 0x1F;
    m_clock_seq_hi_and_reserved |= 0x80;

    /*
     * node
     */
    m_node = s_mac_adr;
}

void UUID::v3_uuid() {
}

void UUID::v4_uuid() {
}

void UUID::v5_uuid() {
}

std::string UUID::str() const {
    std::ostringstream ss;
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
