#ifndef UUID__VERSIONBITSETS_H
#define UUID__VERSIONBITSETS_H

#include <bitset>

struct UUIDVersionBitsets {
    const std::bitset<4> Nil_Bitset     {0b0000};
    const std::bitset<4> Version1_Bitset{0b0001};
    const std::bitset<4> Version2_Bitset{0b0010};
    const std::bitset<4> Version3_Bitset{0b0011};
    const std::bitset<4> Version4_Bitset{0b0100};
    const std::bitset<4> Version5_Bitset{0b0101};
};

#endif //UUID__VERSIONBITSETS_H
