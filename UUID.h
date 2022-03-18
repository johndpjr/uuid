#ifndef UUID_EXEC__UUID_H
#define UUID_EXEC__UUID_H

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "HelperFunctions.h"
#include "constants.h"

using namespace constants;
using std::setw;

class UUID
{
public:
    /* The Version affects how the UUID is generated
           Version Nil: Special Nil UUID
           Version 1  : Datetime and MAC Address
           Version 3  : Namespace-based (MD5)
           Version 4  : Random
           Version 5  : Namespace-based (SHA-1)
     */
    enum Version {
        Nil = 0,
        v1  = 1,
        v3  = 3,
        v4  = 4,
        v5  = 5
    };

private:
    uint32_t m_time_low;                   // 32 bits
    uint16_t m_time_mid;                   // 16 bits
    uint16_t m_time_hi_and_version;        // 16 bits
    uint8_t  m_clock_seq_hi_and_reserved;  // 8 bits
    uint8_t  m_clock_seq_low;              // 8 bits
    uint8_t  m_node[6];                    // 48 bits

    static uint64_t      s_last_uuid_time;
    static unsigned int  s_uuids_this_tick;
    static uint16_t      s_clock_seq;
    static uint8_t*      s_mac_adr;

    void v1_uuid();
    void v3_uuid();
    void v4_uuid();
    void v5_uuid();

public:
    /*
     * Constructors
     */
    /**
     * No-args constructor creates a Nil Version UUID
     *  where all 128 bits are set to 0 (00000000-0000-0000-0000-000000000000)
     */
    UUID();
    /**
     * Version constructor
     * @param ver
     */
    UUID(const Version& ver);
    /**
     * Copy constructor copies uuid
     * @param uuid
     */
    UUID(const UUID &uuid);
    /**
     * String constructor constructs a UUID from a string
     * @param uuid_str
     */
    UUID(const std::string& uuid_str);

    /*
     * Functions
     */
    /**
     * @return Version number of the UUID
     *  (Ex: returns 4 from Version 4 UUID)
     */
    [[nodiscard]] unsigned int get_version() const;
    /**
     * @return A hexadecimal string representation of the UUID
     *  (Ex: "123e4567-e89b-12d3-a456-426614174000")
     */
    [[nodiscard]] std::string str() const;
    /**
     * @return A Uniform Resource Name (URN) string representation of the UUID
     *  (Ex: "urn:uuid:123e4567-e89b-12d3-a456-426614174000")
     */
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
