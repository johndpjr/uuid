#include "UUID.h"

uint64_t UUID::s_last_uuid_time       {get_uuid_ticks()};
unsigned int UUID::s_uuids_this_tick  {0};
uint16_t UUID::s_clock_seq            {get_clock_seq()};
uint8_t* UUID::s_mac_adr              {get_node()};

/*
 * Constructors
 */
UUID::UUID()
    : m_time_low{0},
      m_time_mid{0},
      m_time_hi_and_version{0},
      m_clock_seq_hi_and_reserved{0},
      m_clock_seq_low{0},
      m_node{0}
{
}

UUID::UUID(const Version& ver)
{
    switch(ver) {
        case Version::Nil:
            m_time_low                  = 0;
            m_time_mid                  = 0;
            m_time_hi_and_version       = 0;
            m_clock_seq_hi_and_reserved = 0;
            m_clock_seq_low             = 0;
            m_node[0]=0; m_node[1]=0; m_node[2]=0; m_node[3]=0; m_node[4]=0; m_node[5]=0;
            break;
        case Version::v1:
            v1_uuid();
            break;
        case Version::v3:
            v3_uuid();
            break;
        case Version::v4:
            v4_uuid();
            break;
        case Version::v5:
            v5_uuid();
            break;
    }
}

UUID::UUID(const UUID &u)
    : m_time_low{u.m_time_low}, m_time_mid{u.m_time_mid},
      m_time_hi_and_version{u.m_time_hi_and_version},
      m_clock_seq_hi_and_reserved{u.m_clock_seq_hi_and_reserved},
      m_clock_seq_low{u.m_clock_seq_low},
      m_node{u.m_node[0], u.m_node[1], u.m_node[2],
             u.m_node[3], u.m_node[4], u.m_node[5]}
{
}

UUID::UUID(const std::string& uuid_str)
    : m_time_low{static_cast<uint32_t>(std::stoul(uuid_str.substr(0, 8), nullptr, 16))},
      m_time_mid{static_cast<uint16_t>(std::stoul(uuid_str.substr(9, 4), nullptr, 16))},
      m_time_hi_and_version{static_cast<uint16_t>(std::stoul(uuid_str.substr(14, 4), nullptr, 16))},
      m_clock_seq_hi_and_reserved{static_cast<uint8_t>(std::stoul(uuid_str.substr(19, 2), nullptr, 16))},
      m_clock_seq_low{static_cast<uint8_t>(std::stoul(uuid_str.substr(21, 2), nullptr, 16))},
      m_node{static_cast<uint8_t>(std::stoul(uuid_str.substr(24, 2), nullptr, 16)),
             static_cast<uint8_t>(std::stoul(uuid_str.substr(26, 2), nullptr, 16)),
             static_cast<uint8_t>(std::stoul(uuid_str.substr(28, 2), nullptr, 16)),
             static_cast<uint8_t>(std::stoul(uuid_str.substr(30, 2), nullptr, 16)),
             static_cast<uint8_t>(std::stoul(uuid_str.substr(32, 2), nullptr, 16)),
             static_cast<uint8_t>(std::stoul(uuid_str.substr(34, 2), nullptr, 16))}
{
}

/*
 * Functions
 */
unsigned int UUID::get_version() const {
    return m_time_hi_and_version >> 12;
}

void UUID::v1_uuid() {
    /* time-low
     * time-mid
     * time-hi-and-version */
    uint64_t uuid_time {get_uuid_ticks()};
    ++s_uuids_this_tick;
    // If the system clock hasn't ticked ( >1 UUID requested in 1 microsecond),
    //  increment the timestamp to simulate a 100ns tick passing
    if (uuid_time == s_last_uuid_time) {
        uuid_time += s_uuids_this_tick;
        if (s_uuids_this_tick > MAX_UUIDS_PER_TICK) {
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
    /* clock-seq-hi-and-reserved
     * clock-seq-low */
    // FIXME: this is a hardcoded variant 1 (0b10x)
    m_clock_seq_low = s_clock_seq & 0xFF;
    m_clock_seq_hi_and_reserved = (s_clock_seq>>8) & 0x1F;
    m_clock_seq_hi_and_reserved |= 0x80;

    /* node */
    for (size_t i{0}; i<6; ++i) {
        m_node[i] = s_mac_adr[i];
    }
}

// TODO: implement UUID version 3
void UUID::v3_uuid() {
}

void UUID::v4_uuid() {
    // Set all bits to pseudo-randomly created values
    randomize(m_time_low);
    randomize(m_time_mid);
    randomize(m_time_hi_and_version);
    randomize(m_clock_seq_hi_and_reserved);
    randomize(m_clock_seq_low);
    for (size_t i{0}; i<6; ++i) {
        randomize(m_node[i]);
    }
    // Set version and variant fields
    m_time_hi_and_version &= 0x4FFF;
    // FIXME: this is a hardcoded variant 1 (0b10x)
    m_clock_seq_hi_and_reserved &= 0xBF;
}

// TODO: implement UUID version 5
void UUID::v5_uuid() {
}

std::string UUID::str() const {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');

    ss << setw(8) << m_time_low            << '-'  // time-low
       << setw(4) << m_time_mid            << '-'  // time-mid
       << setw(4) << m_time_hi_and_version << '-'  // time-high-and-version
       << setw(2) << +m_clock_seq_hi_and_reserved  // clock-seq-and-reserved
       << setw(2) << +m_clock_seq_low      << '-'  // clock-seq-low
       << setw(2) << +m_node[0]                    // node
       << setw(2) << +m_node[1]
       << setw(2) << +m_node[2]
       << setw(2) << +m_node[3]
       << setw(2) << +m_node[4]
       << setw(2) << +m_node[5];

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
