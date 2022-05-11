#include "UUID.h"

uint64_t UUID::s_last_uuid_time       {get_uuid_ticks()};
unsigned int UUID::s_uuids_this_tick  {0};
uint16_t UUID::s_clock_seq            {get_clock_seq()};
uint8_t* UUID::s_mac_adr              {get_node()};

UUID const UUID::NSID_DNS { // "6ba7b810-9dad-11d1-80b4-00c04fd430c8"
    0x6ba7b810, 0x9dad, 0x11d1,
    0x80, 0xb4,
    0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

UUID const UUID::NSID_URL  { // 6ba7b811-9dad-11d1-80b4-00c04fd430c8
    0x6ba7b811, 0x9dad, 0x11d1,
    0x80, 0xb4,
    0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

UUID const UUID::NSID_OID  { // 6ba7b812-9dad-11d1-80b4-00c04fd430c8
    0x6ba7b812, 0x9dad, 0x11d1,
    0x80, 0xb4,
    0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

UUID const UUID::NSID_X500 {  // 6ba7b814-9dad-11d1-80b4-00c04fd430c8
    0x6ba7b814, 0x9dad, 0x11d1,
    0x80, 0xb4,
    0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

/*
 * Constructors
 */
UUID::UUID()
    : m_time_low{0}, m_time_mid{0}, m_time_hi_and_version{0},
      m_clock_seq_hi_and_reserved{0}, m_clock_seq_low{0},
      m_node{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
{
}

UUID::UUID(Version ver)
    : UUID()
{
    switch(ver) {
        case Version::Nil:
            break;
        case Version::v1:
            v1_uuid();
            break;
        case Version::v3:
            break;
        case Version::v4:
            v4_uuid();
            break;
        case Version::v5:
            break;
    }
}

UUID::UUID(Version ver, UUID nsid, std::string name)
    : UUID()
{
    switch (ver) {
        case Version::Nil:
        case Version::v1:
        case Version::v4:
            break;
        case Version::v3:
            v3_uuid(nsid, name);
            break;
        case Version::v5:
            v5_uuid(nsid, name);
            break;
    }
}

UUID::UUID(uint32_t time_low, uint16_t time_mid, uint16_t time_hi_and_version,
           uint8_t clock_seq_hi_and_reserved, uint8_t clock_seq_low,
           uint8_t node_0, uint8_t node_1, uint8_t node_2, uint8_t node_3, uint8_t node_4, uint8_t node_5)
    : m_time_low{time_low}, m_time_mid{time_mid}, m_time_hi_and_version{time_hi_and_version},
      m_clock_seq_hi_and_reserved{clock_seq_hi_and_reserved}, m_clock_seq_low{clock_seq_low},
      m_node{node_0, node_1, node_2, node_3, node_4, node_5}
{
}

UUID::UUID(const UUID &u)
    : m_time_low{u.m_time_low}, m_time_mid{u.m_time_mid}, m_time_hi_and_version{u.m_time_hi_and_version},
      m_clock_seq_hi_and_reserved{u.m_clock_seq_hi_and_reserved}, m_clock_seq_low{u.m_clock_seq_low},
      m_node{u.m_node[0], u.m_node[1], u.m_node[2],
             u.m_node[3], u.m_node[4], u.m_node[5]}
{
}

// FIXME: use regex to confirm that the uuid string is valid
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
void UUID::v1_uuid() {
    /* time-low
     * time-mid
     * time-hi-and-version */
    uint64_t uuid_time {get_uuid_ticks()};
    // If the system clock hasn't ticked ( >1 UUID requested in 1 microsecond),
    //  increment the timestamp to simulate a 100ns tick passing
    if (s_uuids_this_tick >= MAX_UUIDS_PER_TICK) {
        while (uuid_time == s_last_uuid_time) {
            uuid_time = get_uuid_ticks();
        }
        s_uuids_this_tick = 0;
    }
    uuid_time += s_uuids_this_tick;

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

    ++s_uuids_this_tick;
}

void UUID::v3_uuid(UUID nsid, std::string name) {
    unsigned char hash[16];
    HL_MD5_CTX ctx;
    MD5 md5;

    nsid.m_time_low = htonl(nsid.m_time_low);
    nsid.m_time_mid = htons(nsid.m_time_mid);
    nsid.m_time_hi_and_version = htons(nsid.m_time_hi_and_version);

    md5.MD5Init(&ctx);
    md5.MD5Update(&ctx, reinterpret_cast<unsigned char *>(&nsid), sizeof nsid);
    md5.MD5Update(&ctx, reinterpret_cast<unsigned char *>(&name[0]), name.size());
    md5.MD5Final(hash, &ctx);
    format_v3_or_v5(hash, 3);
}

void UUID::v4_uuid() {
    // Randomize all bits
    randomize(m_time_low);
    randomize(m_time_mid);
    randomize(m_time_hi_and_version);
    randomize(m_clock_seq_hi_and_reserved);
    randomize(m_clock_seq_low);
    for (size_t i{0}; i<6; ++i) {
        randomize(m_node[i]);
    }
    // Set version and variant fields
    m_time_hi_and_version &= 0xFFF;
    m_time_hi_and_version |= 0x4000;
    // FIXME: this is a hardcoded variant 1 (0b10x)
    m_clock_seq_hi_and_reserved &= 0x3F;
    m_clock_seq_hi_and_reserved |= 0x80;
}

void UUID::v5_uuid(UUID nsid, std::string name) {
    // SHA-1 hashes to 160 bits, but hash will later be truncated to fit 128 bit uuid size
    unsigned char hash[20];
    HL_SHA1_CTX ctx;
    SHA1 sha1;

    nsid.m_time_low = htonl(nsid.m_time_low);
    nsid.m_time_mid = htons(nsid.m_time_mid);
    nsid.m_time_hi_and_version = htons(nsid.m_time_hi_and_version);

    sha1.SHA1Reset(&ctx);
    sha1.SHA1Input(&ctx, reinterpret_cast<unsigned char *>(&nsid), sizeof nsid);
    sha1.SHA1Input(&ctx, reinterpret_cast<unsigned char *>(&name[0]), name.size());
    sha1.SHA1Result(&ctx, hash);
    format_v3_or_v5(hash, 5);
}

void UUID::format_v3_or_v5(unsigned char* hash, int version) {
    // Copy hash contents to uuid
    std::memcpy(&m_time_low, &hash[0], 4);                   // time-low
    m_time_low = ntohl(m_time_low);
    std::memcpy(&m_time_mid, &hash[4], 2);                   // time-mid
    m_time_mid = ntohs(m_time_mid);
    std::memcpy(&m_time_hi_and_version, &hash[6], 2);        // time-high-and-version
    m_time_hi_and_version = ntohs(m_time_hi_and_version);
    std::memcpy(&m_clock_seq_hi_and_reserved, &hash[8], 1);  // clock-seq-hi-and-reserved
    std::memcpy(&m_clock_seq_low, &hash[9], 1);              // clock-seq-low
    std::memcpy(m_node, &hash[10], 6);                       // node

    // Encode version and variant
    m_time_hi_and_version &= 0x0FFF;
    m_time_hi_and_version |= (version << 12);
    // FIXME: this is a hardcoded variant 1 (0b10x)
    m_clock_seq_hi_and_reserved &= 0x3F;
    m_clock_seq_hi_and_reserved |= 0x80;
}

unsigned int UUID::get_version() const {
    return m_time_hi_and_version >> 12;
}

std::string UUID::str() const {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');

    ss << std::setw(8) << m_time_low            << '-'  // time-low
       << std::setw(4) << m_time_mid            << '-'  // time-mid
       << std::setw(4) << m_time_hi_and_version << '-'  // time-high-and-version
       << std::setw(2) << +m_clock_seq_hi_and_reserved  // clock-seq-hi-and-reserved
       << std::setw(2) << +m_clock_seq_low      << '-'  // clock-seq-low
       << std::setw(2) << +m_node[0]                    // node
       << std::setw(2) << +m_node[1]
       << std::setw(2) << +m_node[2]
       << std::setw(2) << +m_node[3]
       << std::setw(2) << +m_node[4]
       << std::setw(2) << +m_node[5];

    return ss.str();
}

std::string UUID::urn_str() const {
    return "urn:uuid:" + str();
}

/*
 * Operator Overloading
 */
UUID& UUID::operator=(const UUID& rhs) {
    if (this == &rhs) {
        return *this;
    }
    m_time_low = rhs.m_time_low;
    m_time_mid = rhs.m_time_mid;
    m_time_hi_and_version = rhs.m_time_hi_and_version;
    m_clock_seq_hi_and_reserved = rhs.m_clock_seq_hi_and_reserved;
    m_clock_seq_low = rhs.m_clock_seq_low;
    for (size_t i{0}; i < 6; ++i) {
        m_node[i] = rhs.m_node[i];
    }
    return *this;
}

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
