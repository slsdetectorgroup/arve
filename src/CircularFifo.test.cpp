#include <catch2/catch_all.hpp>

#include "arve/CircularFifo.h"
#include "arve/ProducerConsumerQueue.h"

using arve::CircularFifo;

// Only for testing. To make sure we can avoid copy constructor
// and copy assignment

struct MoveOnlyInt{
    int value{};

    MoveOnlyInt() = default;
    MoveOnlyInt(int i):value(i){};
    MoveOnlyInt(const MoveOnlyInt&) = delete;
    MoveOnlyInt& operator=(const MoveOnlyInt&) = delete;
    MoveOnlyInt(MoveOnlyInt&& other){
        std::swap(value, other.value);
    }
    MoveOnlyInt& operator=(MoveOnlyInt&& other){
        std::swap(value, other.value);
        return *this;
    }
    bool operator==(int other) const {
        return value == other;
    }

};

TEST_CASE("Default construction"){
    CircularFifo<MoveOnlyInt> f;
}

TEST_CASE("Push move only type"){
    CircularFifo<MoveOnlyInt> f;
    f.push_value(5);
}

TEST_CASE("Push pop"){
    CircularFifo<MoveOnlyInt> f;
    f.push_value(MoveOnlyInt(1));

    auto a = f.pop_value();
    CHECK(a == 1);
    
}

TEST_CASE("Pop free and then push"){
    CircularFifo<MoveOnlyInt> f;

    auto a = f.pop_free();
    a.value = 5;
    f.push_value(std::move(a)); //Explicit move since we can't copy
    auto b = f.pop_value();
    
    CHECK(a == 0); //Moved from value
    CHECK(b == 5); //Original value
    
}

TEST_CASE("Skip the first value"){
    CircularFifo<MoveOnlyInt> f;

    for (int i = 0; i!=10; ++i){
        auto a = f.pop_free();
        a.value = i+1;
        f.push_value(std::move(a)); //Explicit move since we can't copy
    }
   
    auto b = f.pop_value();
    CHECK(b == 1);
    f.next();
    auto c = f.pop_value();
    CHECK(c == 3);
    
}