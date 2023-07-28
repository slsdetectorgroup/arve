#include <catch2/catch_all.hpp>
#include "arve/SimpleQueue.h"

using arve::SimpleQueue;
TEST_CASE("push pop"){

    SimpleQueue<int> q(5); 
    int a = 3;
    int b = 8;
    CHECK(q.sizeGuess() == 0);
    CHECK(q.push(a));
    CHECK(q.sizeGuess() == 1);
    CHECK(q.push(b));
    CHECK(q.sizeGuess() == 2);
    int c = 0;

    CHECK(q.pop(c));
    CHECK(c == 3);
    CHECK(q.sizeGuess() == 1);
    CHECK(q.pop(c));
    CHECK(c == 8);
    CHECK(q.sizeGuess() == 0);
}

TEST_CASE("Cannot push to a full queue"){
    SimpleQueue<int> q(2); 
    int a = 3;
    int b = 4;
    int c = 0;
    CHECK(q.push(a));
    CHECK(q.push(b));
    CHECK_FALSE(q.push(a));

    //values are still ok
    CHECK(q.pop(c));
    CHECK(c == 3);
    CHECK(q.pop(c));
    CHECK(c == 4);
}

TEST_CASE("Cannot pop from an empty queue"){
    SimpleQueue<int> q(2);
    int a=0; 
    CHECK_FALSE(q.pop(a));
}