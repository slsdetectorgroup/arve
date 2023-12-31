#pragma once
#include <cstdint>
#include <cstddef>
namespace arve{
// copy of slsDetectorDefs::sls_detector_header; be careful with versions!

struct slsPacketHeader {
    uint64_t frameNumber;
    uint32_t expLength; /* Eiger: numsubframes, exptime x 100ns others) */
    uint32_t packetNumber;
    uint64_t bunchId;
    uint64_t timestamp;
    uint16_t modId;
    uint16_t row;
    uint16_t column;
    uint16_t reserved;
    uint32_t debug;
    uint16_t roundRNumber;
    uint8_t detType;
    uint8_t version;
};



//TODO! how much care do we need to take with alignment?
template<typename Header, size_t PayloadSize>
struct Packet{
        Header header;
        char data[PayloadSize];

        //For outside access
        using header_type = Header;
        static constexpr auto payload_size = PayloadSize;
};

using JungfrauPacket = Packet<slsPacketHeader, 8192>;


}