#ifndef UUID_EXEC__UUID_H
#define UUID_EXEC__UUID_H

#include <bitset>
#include <string>
#include <sstream>
#include <iomanip>

#include "VersionBitsets.h"

class UUID {
public:
    enum Version {
        Nil,
        Version1,
        Version2,
        Version3,
        Version4,
        Version5
    };

private:
    Version               m_version;

    const std::bitset<32> m_time_low;
    const std::bitset<16> m_time_mid;
    const std::bitset<16> m_time_hi_and_version;
    const std::bitset<8>  m_clock_seq_hi_and_reserved;
    const std::bitset<8>  m_clock_seq_low;
    const std::bitset<48> m_node;

    void parse_version();

public:
    // No-args constructor creates a nil UUID
    UUID();
    // Version constructor
    UUID(Version ver);
    // Copy constructor
    UUID(const UUID &uuid);
    // Integer constructor
    UUID(const unsigned long& time_low,
         const unsigned long& time_mid,
         const unsigned long& time_hi_and_version,
         const unsigned long& clock_seq_hi_and_reserved,
         const unsigned long& clock_seq_low,
         const unsigned long long& node);
    // String constructor
    UUID(const std::string& time_low,
         const std::string& time_mid,
         const std::string& time_hi_and_version,
         const std::string& clock_seq_hi_and_reserved,
         const std::string& clock_seq_low,
         const std::string& node);

    // Getters
    unsigned int get_version() const;

    // Get a string representation of the UUID
    std::string str();
    // Get a string representation of the UUID as a URN
    // Looks like urn:uuid:{uuid}
    std::string urn_str();
};


#endif //UUID_EXEC__UUID_H
