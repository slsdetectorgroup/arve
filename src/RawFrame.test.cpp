#include <catch2/catch_all.hpp>
#include "arve/RawFrame.h"

struct DummyHeader{
    int frameNumber{};
};

constexpr size_t payload_size = 512;
constexpr size_t num_packets = 10;
using DummyFrame = arve::RawFrame<DummyHeader, payload_size, num_packets>;

TEST_CASE("Construct"){
    DummyFrame f;
    CHECK(f[0].header.frameNumber == 0);
}

TEST_CASE("Copy construct"){
    DummyFrame f;
    DummyFrame f2 = f;
}

TEST_CASE("Move construct"){
    DummyFrame f(DummyFrame{});
    CHECK(f[0].header.frameNumber == 0);
}

TEST_CASE("Move assign"){
    DummyFrame f;
    DummyFrame f2;
    f2 = DummyFrame{};
    CHECK(f[0].header.frameNumber == 0);
}