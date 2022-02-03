#ifndef UUID_EXEC__UUID_H
#define UUID_EXEC__UUID_H

#include <bitset>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "HelperFunctions.h"


class UUID
{
public:
    enum Version {
        Unresolved,
        v1,
        v2,
        v3,
        v4,
        v5
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
    /*
     * Constructors
     */
    // No-args constructor creates a nil UUID: 00000000-0000-0000-0000-000000000000
    // std::bitset defaults to all 0's, so nothing special here
    UUID();
    // Version constructor
    UUID(const Version& ver);
    // Copy constructor
    UUID(const UUID &uuid);
    // Integer piece constructor
    UUID(const unsigned long& time_low,
         const unsigned long& time_mid,
         const unsigned long& time_hi_and_version,
         const unsigned long& clock_seq_hi_and_reserved,
         const unsigned long& clock_seq_low,
         const unsigned long long& node);
    // FIXME: String constructor
    UUID(const std::string& uuid_str);

    /*
     * Functions
     */
    unsigned int get_version() const;
    // Get a string representation of the UUID
    std::string str() const;
    // Get a string representation of the UUID as a URN
    // Ex: urn:uuid:{uuid}
    std::string urn_str() const;

    /*
     * Operator Overloading
     */
    friend std::ostream& operator<<(std::ostream& os, const UUID& u);
    // FIXME: implement >> operator (and have it be case insensitive on input)
};


#endif //UUID_EXEC__UUID_H
