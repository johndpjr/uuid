#ifndef UUID_EXEC__UUID_H
#define UUID_EXEC__UUID_H

#include <bitset>
#include <string>
#include <sstream>

class UUID {
private:
    unsigned int m_version;
    std::bitset<128> m_bitset;

public:
    // No-args constructor creates a nil UUID
    UUID();
    // Copy constructor
    UUID(const UUID &uuid);

    // Getters
    unsigned int get_version() const;
    std::bitset<128> get_bitset() const;

    // Get a string representation of the UUID
    std::string str();
};


#endif //UUID_EXEC__UUID_H
