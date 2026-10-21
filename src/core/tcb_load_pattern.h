// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>
#include <cstring>
#include <span>

namespace Core {
inline bool IsAbsoluteTcbAccess(std::span<const uint8_t> bytes, uint32_t tcb_size) {
    if (bytes.size() < 9 || bytes[0] != 0x64 ||
        (bytes[1] != 0x48 && bytes[1] != 0x4c) ||
        (bytes[2] != 0x8b && bytes[2] != 0x33 && bytes[2] != 0x3b) ||
        (bytes[3] & 0xc7) != 0x04 || bytes[4] != 0x25) return false;
    uint32_t offset{};
    std::memcpy(&offset, bytes.data() + 5, sizeof(offset));
    return offset < tcb_size;
}
}
