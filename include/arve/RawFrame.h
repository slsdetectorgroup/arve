#pragma once

#include "arve/Packets.h"
#include "arve/defs.h"
#include <cstring>
#include <fmt/format.h>
#include <memory>

namespace arve {

// Data storage to be able to better control allocation
// Could work with pointers to this type?
template <typename Header, size_t PayloadSize, size_t NumPackets>
class RawFrameData {
    Packet<Header, PayloadSize> packets[NumPackets];

  public:
    using header_type = Header;
    using packet_type = Packet<Header, PayloadSize>;
    static constexpr auto payload_size = PayloadSize;
    static constexpr auto num_packets = NumPackets;

    Packet<Header, PayloadSize>& operator[](size_t i){return packets[i];}
};

template <typename Header, size_t PayloadSize, size_t NumPackets>
class RawFrame {
    RawFrameData<Header, PayloadSize, NumPackets> *data_ = nullptr;

  public:
  using header_type = Header;
    using packet_type = Packet<Header, PayloadSize>;
    static constexpr auto payload_size = PayloadSize;
    static constexpr auto num_packets = NumPackets;
    RawFrame() : data_(new RawFrameData<Header, PayloadSize, NumPackets>) {}
    ~RawFrame() { delete data_; }
    RawFrame(const RawFrame &other) : RawFrame() {
        fmt::print("RawFrame was copy constructed!\n");
        print_addr();
        memcpy(data_, other.data_, sizeof(data_));
    }

    RawFrame &operator=(const RawFrame &other) {
        fmt::print("RawFrame was copy assigned!\n");
        memcpy(data_, other.data_, sizeof(data_));
        return *this;
    }

    int64_t frameNumber() const { return operator[](0).header.frameNumber; }

    // Move construction, no allocation just swap the pointer
    // with the other object. The moved from object will have
    // a nullptr and not owning any data
    RawFrame(RawFrame &&other) {
        fmt::print("RawFrame was move constructed!\n");
        std::swap(data_, other.data_);
    }

    RawFrame &operator=(RawFrame &&other) {
        fmt::print("RawFrame was move assigned!\n");
        std::swap(data_, other.data_);
        return *this;
    }

    auto *data() { return data_; }
    const auto &operator[](size_t i) const { return (*data_)[i]; }
    auto &operator[](size_t i) { return (*data_)[i]; }
    void print_addr() {
        fmt::print("RawFrame::packets: {}\n", fmt::ptr(data_));
    }
};

using JungfrauRawFrame = RawFrame<slsPacketHeader, JF_PayloadSize, JF_NumPackets>;

} // namespace arve