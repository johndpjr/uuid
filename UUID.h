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
        Unresolved = 0,
        v1         = 1,
        v3         = 3,
        v4         = 4,
        v5         = 5
    };

private:
    Version         m_version;

    std::bitset<32> m_time_low;
    std::bitset<16> m_time_mid;
    std::bitset<16> m_time_hi_and_version;
    std::bitset<8>  m_clock_seq_hi_and_reserved;
    std::bitset<8>  m_clock_seq_low;
    std::bitset<48> m_node;

    void parse_version();
    void v1_uuid();
    void v3_uuid();
    void v4_uuid();
    void v5_uuid();

public:
    /*
     * Constructors
     */
    // No-args constructor creates a Nil UUID
    UUID();
    // Version constructor
    UUID(const Version& ver);
    // Copy constructor
    UUID(const UUID &uuid);
    // String constructor
    UUID(const std::string& uuid_str);

    /*
     * Functions
     */
    [[nodiscard]] unsigned int get_version() const;
    // Get a string representation of the UUID
    [[nodiscard]] std::string str() const;
    // Get a string representation of the UUID as a URN
    // Ex: urn:uuid:{uuid}
    [[nodiscard]] std::string urn_str() const;

    /*
     * Operator Overloading
     */
    friend std::ostream& operator<<(std::ostream& os, const UUID& u);
    friend std::istream& operator>>(std::istream& is, UUID& u);
    friend bool operator==(const UUID& lhs, const UUID& rhs);
    friend bool operator!=(const UUID& lhs, const UUID& rhs);
    friend bool operator> (const UUID& lhs, const UUID& rhs);
    friend bool operator< (const UUID& lhs, const UUID& rhs);
    friend bool operator>=(const UUID& lhs, const UUID& rhs);
    friend bool operator<=(const UUID& lhs, const UUID& rhs);
};


#endif //UUID_EXEC__UUID_H
