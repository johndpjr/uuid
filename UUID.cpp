#include "UUID.h"

/*
 * Constructors
 */
UUID::UUID()
    : m_version{Version::Nil} {
}

UUID::UUID(Version ver)
    : m_version{ver} {
    switch(ver) {
        case Version::Nil:
            break;
        case Version::Version1:
            break;
        case Version::Version2:
            break;
        case Version::Version3:
            break;
        case Version::Version4:
            break;
        case Version::Version5:
            break;
    }
}

UUID::UUID(const UUID &uuid)
    : m_version{uuid.m_version},
      m_time_low{uuid.m_time_low}, m_time_mid{uuid.m_time_mid},
      m_time_hi_and_version{uuid.m_time_hi_and_version},
      m_clock_seq_hi_and_reserved{uuid.m_clock_seq_hi_and_reserved},
      m_clock_seq_low{uuid.m_clock_seq_low},
      m_node{uuid.m_node} {
}

UUID::UUID(const unsigned long &time_low, const unsigned long &time_mid, const unsigned long &time_hi_and_version,
           const unsigned long &clock_seq_hi_and_reserved, const unsigned long &clock_seq_low,
           const unsigned long long &node)
    : m_time_low{time_low}, m_time_mid{time_mid}, m_time_hi_and_version{time_hi_and_version},
      m_clock_seq_hi_and_reserved{clock_seq_hi_and_reserved}, m_clock_seq_low{clock_seq_low},
      m_node{node} {
    m_version = Version::Nil;
}

UUID::UUID(const std::string &time_low, const std::string &time_mid, const std::string &time_hi_and_version,
           const std::string &clock_seq_hi_and_reserved, const std::string &clock_seq_low, const std::string &node)
   : m_time_low{time_low}, m_time_mid{time_mid}, m_time_hi_and_version{time_hi_and_version},
     m_clock_seq_hi_and_reserved{clock_seq_hi_and_reserved}, m_clock_seq_low{clock_seq_low},
     m_node{node} {
    m_version = Version::Nil;
}

/*
 * Getters
 */
unsigned int UUID::get_version() const {
    return m_version;
}

/*
 * Functions
 */
//void UUID::parse_version() {
//    std::bitset<4> ver_set;
//    for (size_t i{12}; i<16; ++i) {
//        ver_set[i-12] = m_time_hi_and_version[i];
//    }
//}
std::string UUID::str() {
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

std::string UUID::urn_str() {
    return "urn:uuid:" + str();
}