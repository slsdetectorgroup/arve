
#include <fmt/format.h>
// #include "arve/ProducerConsumerQueue.h"
#include "arve/arve.h"
#include "arve/Packets.h"
struct TestType{
    int value{};
    TestType(){};
    TestType(int v):value(v){};
    TestType(const TestType& other){
        fmt::print("CopyConstruct::{}\n", value);
        value = other.value;
    }
    TestType& operator=(const TestType& other){
        fmt::print("CopyAssign::{}\n", value);
        value = other.value;
        return *this;
    }

    TestType(TestType&& other){
        fmt::print("MoveConstruct::{}\n", value);
        value = other.value;
        other.value = -1;
    }

    TestType& operator=(TestType&& other){
        fmt::print("MoveAssign::{}\n", value);
        value = other.value;
        other.value = -1;
        return *this;
    }
    ~TestType(){
        fmt::print("~TestType::{}\n", value);
    }

};

int main(){
    // TestType t{5};
    // TestType t2 = t;
    // t = t2;

    // TestType t3(std::move(t2));
    // t3 = std::move(t2);

    // folly::ProducerConsumerQueue<TestType> q(5);
    // q.write(TestType(1));
    // q.write(TestType(2));
    // TestType t(3);
    // q.read(t);
    constexpr size_t num_packets = 128;
    constexpr size_t payload_size = 8192;
    using Frame = arve::RawFrame<arve::slsPacketHeader, payload_size, num_packets>;
    using Fifo = arve::CircularFifo<Frame>;
    // using Frame = TestType;
    // folly::ProducerConsumerQueue<Frame> q(5);
    // q.write(Frame{});
    // q.write(Frame{});

    // Frame f;
    // q.read(f);

    folly::ProducerConsumerQueue<Frame> q(5);
    Frame f;
    // f.print_addr();
    // auto rc = q.read(f);
    // fmt::print("q.read rc:{}\n", rc);
    // f.print_addr();
    // f[0].header.frameNumber = 5;
    q.write(std::move(f));
    f.print_addr();
    // fmt::print("ho\n");
    //Segfaults with address sanitizer
    // Fifo f;
    // auto raw_frame = f.pop_free();
    // f.push_image(raw_frame);

}