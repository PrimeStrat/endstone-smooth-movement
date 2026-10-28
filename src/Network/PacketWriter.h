#pragma once

#include <cstdint>
#include <string>

class PacketWriter {
public:
    /**
     * Creates a little-endian writer that clears and reuses the given buffer.
     * @param out Buffer to write into.
     */
    explicit PacketWriter(std::string &out) : out_(out) { out_.clear(); }

    /**
     * Writes one byte.
     * @param value Byte to write.
     * @return This writer.
     */
    PacketWriter &u8(std::uint8_t value)
    {
        out_.push_back(static_cast<char>(value));
        return *this;
    }

    /**
     * Writes a little-endian float32.
     * @param value Float to write.
     * @return This writer.
     */
    PacketWriter &f32(float value)
    {
        out_.append(reinterpret_cast<const char *>(&value), sizeof(value));
        return *this;
    }

    /**
     * Writes an unsigned LEB128 varint.
     * @param value Value to write.
     * @return This writer.
     */
    PacketWriter &varU64(std::uint64_t value)
    {
        do {
            const auto b = static_cast<std::uint8_t>(value & 0x7F);
            value >>= 7;
            u8(value ? b | 0x80 : b);
        } while (value);
        return *this;
    }

private:
    std::string &out_;
};
