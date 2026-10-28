#pragma once

#include <cstdint>
#include <cstring>
#include <string_view>

class PacketReader {
public:
    /**
     * Creates a bounds-checked little-endian reader over a payload.
     * @param data Payload to read.
     */
    explicit PacketReader(std::string_view data) : data_(data) {}

    /**
     * Tells whether every read so far stayed inside the payload.
     * @return True if no read ran past the end.
     */
    bool ok() const { return ok_; }

    /**
     * Reads one byte.
     * @return The byte, or 0 past the end.
     */
    std::uint8_t u8()
    {
        if (pos_ >= data_.size()) {
            ok_ = false;
            return 0;
        }
        return static_cast<std::uint8_t>(data_[pos_++]);
    }

    /**
     * Reads a one-byte bool.
     * @return True for any non-zero byte.
     */
    bool boolean() { return u8() != 0; }

    /**
     * Reads a little-endian uint16.
     * @return The value.
     */
    std::uint16_t u16()
    {
        const std::uint16_t low = u8();
        return static_cast<std::uint16_t>(low | (u8() << 8));
    }

    /**
     * Reads a little-endian float32.
     * @return The value, or 0 past the end.
     */
    float f32()
    {
        float value = 0;
        if (pos_ + sizeof(value) > data_.size()) {
            ok_ = false;
            return 0;
        }
        std::memcpy(&value, data_.data() + pos_, sizeof(value));
        pos_ += sizeof(value);
        return value;
    }

    /**
     * Reads an unsigned LEB128 varint of up to 64 bits.
     * @return The value, or 0 if malformed.
     */
    std::uint64_t varU64()
    {
        std::uint64_t value = 0;
        for (int shift = 0; shift < 70 && ok_; shift += 7) {
            const std::uint8_t b = u8();
            value |= static_cast<std::uint64_t>(b & 0x7F) << shift;
            if ((b & 0x80) == 0) {
                return value;
            }
        }
        ok_ = false;
        return 0;
    }

private:
    std::string_view data_;
    std::size_t pos_ = 0;
    bool ok_ = true;
};
