#ifndef UUID_EXEC__UUID_H
#define UUID_EXEC__UUID_H

#include <bitset>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "HelperFunctions.h"
#include "constants.h"


class UUID
{
public:
    // The Version affects how the UUID is generated
    // Version 1: Datetime and MAC Address
    // Version 3: Namespace-based (MD5)
    // Version 4: Random
    // Version 5: Namespace-based (SHA-1)
    enum Version {
        Unresolved = 0,
        v1         = 1,
        v3         = 3,
        v4         = 4,
        v5         = 5
    };
    // The Variant affects the format and encoding of the UUID
    // From RFC 4122:
    //      Msb0  Msb1  Msb2  Description
    //      0     x     x     Reserved, NCS backward compatibility.
    //      1     0     x     The variant specified in this document.
    //      1     1     0     Reserved, Microsoft Corporation backward compatibility
    enum Variant {
        Apollo_NCS = 0,
        RFC_4122   = 1,
        Microsoft  = 2
    };

private:
    Version         m_version;
    Variant         m_variant;

    std::bitset<32> m_time_low;
    std::bitset<16> m_time_mid;
    std::bitset<16> m_time_hi_and_version;
    std::bitset<8>  m_clock_seq_hi_and_reserved;
    std::bitset<8>  m_clock_seq_low;
    std::bitset<48> m_node;

    static unsigned int uuids_this_tick;

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
    // Returns the version number of the UUID
    // Ex: For a Version 4 UUID, returns 4
    [[nodiscard]] unsigned int get_version() const;
    // Returns the variant of the UUID
    // Ex: For a Variant 1 UUID, returns 1
    [[nodiscard]] unsigned int get_variant() const;
    // Returns a hexadecimal string representation of the UUID
    // Ex: "123e4567-e89b-12d3-a456-426655440000"
    [[nodiscard]] std::string str() const;
    // Returns a UUID string presented as a URN
    // Ex: "urn:uuid:123e4567-e89b-12d3-a456-426655440000"
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
