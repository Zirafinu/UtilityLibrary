
#include <doctest/doctest.h>
#include "ring_buffer.hpp"

#include <array>

TEST_SUITE("RingBuffers") {
    // construction
    // push single / block
    // peek block
    // pop block / segment
    // erase block / segment
    TEST_CASE("Construct_uint8") {
        std::array<uint8_t, 3> memory;
        RingBuffer<decltype(memory)> u8_buff{memory};
    }
}
