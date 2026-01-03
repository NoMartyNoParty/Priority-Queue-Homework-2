#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "../library/priority_queue.hpp"
#include <string>
#include <stdexcept>
#include <fstream>

TEST_CASE("Default constructor and empty state", "[constructor]") {
    priority_queue<int> pq;
    REQUIRE(pq.empty());
    REQUIRE(pq.size() == 0);
}

TEST_CASE("Basic enqueue head and size", "[enqueue][head][size]") {
    priority_queue<int> pq;
    pq.enqueue(42, 1);
    REQUIRE_FALSE(pq.empty());
    REQUIRE(pq.size() == 1);
    REQUIRE(pq.head() == 42);
}

TEST_CASE("Priority ordering with multiple elements", "[priority]") {
    priority_queue<std::string> pq;
    pq.enqueue("low", 10);
    pq.enqueue("high", 1);
    pq.enqueue("medium", 5);
    REQUIRE(pq.size() == 3);
    REQUIRE(pq.head() == "high");
}

TEST_CASE("Stability for equal priorities", "[stability]") {
    priority_queue<int> pq;
    pq.enqueue(1, 5);
    pq.enqueue(2, 5);
    pq.enqueue(3, 5);
    pq.enqueue(4, 5);
    REQUIRE(pq.head() == 1);
    pq.dequeue();
    REQUIRE(pq.head() == 2);
    pq.dequeue();
    REQUIRE(pq.head() == 3);
    pq.dequeue();
    REQUIRE(pq.head() == 4);
    pq.dequeue();
}

TEST_CASE("Stability with mixed priorities", "[stability]") {
    priority_queue<std::string> pq;
    pq.enqueue("first-p2", 2);
    pq.enqueue("second-p2", 2);
    pq.enqueue("first-p1", 1);
    pq.enqueue("third-p2", 2);
    pq.enqueue("second-p1", 1);
    REQUIRE(pq.head() == "first-p1");
    pq.dequeue();
    REQUIRE(pq.head() == "second-p1");
    pq.dequeue();
    REQUIRE(pq.head() == "first-p2");
    pq.dequeue();
    REQUIRE(pq.head() == "second-p2");
    pq.dequeue();
    REQUIRE(pq.head() == "third-p2");
    pq.dequeue();
}

TEST_CASE("Dequeue returns elements in priority order", "[dequeue]") {
    priority_queue<int> pq;
    pq.enqueue(100, 5);
    pq.enqueue(200, 2);
    pq.enqueue(300, 8);
    pq.enqueue(400, 1);
    REQUIRE(pq.head() == 400);
    pq.dequeue();
    REQUIRE(pq.head() == 200);
    pq.dequeue();
    REQUIRE(pq.head() == 100);
    pq.dequeue();
    REQUIRE(pq.head() == 300);
    pq.dequeue();
    REQUIRE(pq.empty());
}

TEST_CASE("Clear on non empty and empty queue", "[clear]") {
    priority_queue<int> pq;
    pq.enqueue(1, 1);
    pq.enqueue(2, 2);
    pq.enqueue(3, 3);
    REQUIRE(pq.size() == 3);
    pq.clear();
    REQUIRE(pq.empty());
    REQUIRE(pq.size() == 0);
    REQUIRE_NOTHROW(pq.clear());
    REQUIRE(pq.empty());
}

TEST_CASE("Copy semantics constructor assignment and self assignment", "[copy]") {
    priority_queue<int> pq1;
    pq1.enqueue(10, 3);
    pq1.enqueue(20, 1);
    pq1.enqueue(30, 2);
    priority_queue<int> pq2(pq1);
    REQUIRE(pq2.size() == 3);
    REQUIRE(pq2.head() == 20);
    pq1.dequeue();
    REQUIRE(pq1.size() == 2);
    REQUIRE(pq2.size() == 3);
    REQUIRE(pq2.head() == 20);
    priority_queue<int> pq3;
    pq3.enqueue(1, 10);
    pq3 = pq2;
    REQUIRE(pq3.size() == 3);
    REQUIRE(pq3.head() == 20);
    pq3 = pq3;
    REQUIRE(pq3.size() == 3);
    REQUIRE(pq3.head() == 20);
}

TEST_CASE("Swap with non empty and empty queues", "[swap]") {
    priority_queue<int> pq1;
    pq1.enqueue(1, 1);
    pq1.enqueue(2, 2);
    priority_queue<int> pq2;
    pq2.enqueue(10, 5);
    pq2.enqueue(20, 6);
    pq2.enqueue(30, 7);
    pq1.swap(pq2);
    REQUIRE(pq1.size() == 3);
    REQUIRE(pq1.head() == 10);
    REQUIRE(pq2.size() == 2);
    REQUIRE(pq2.head() == 1);
    priority_queue<int> pq3;
    pq3.enqueue(100, 1);
    priority_queue<int> pq4;
    pq3.swap(pq4);
    REQUIRE(pq3.empty());
    REQUIRE(pq4.size() == 1);
    REQUIRE(pq4.head() == 100);
}

TEST_CASE("Serialization and deserialization round trip", "[serialization]") {
    priority_queue<int> pq1;
    pq1.enqueue(1, 5);
    pq1.enqueue(2, 2);
    pq1.enqueue(3, 8);
    const char* filename = "test_roundtrip_int.bin";
    {
        std::ofstream out(filename, std::ios::binary);
        REQUIRE(out.good());
        pq1.serialize(out);
    }
    priority_queue<int> pq2;
    {
        std::ifstream in(filename, std::ios::binary);
        REQUIRE(in.good());
        pq2.deserialize(in);
    }
    REQUIRE(pq2.size() == 3);
    REQUIRE(pq2.head() == 2);
    pq2.dequeue();
    REQUIRE(pq2.head() == 1);
    pq2.dequeue();
    REQUIRE(pq2.head() == 3);
    pq2.dequeue();
}

TEST_CASE("Empty queue serialization and deserialization", "[serialization]") {
    priority_queue<int> pq1;
    const char* filename = "test_empty_queue.bin";
    {
        std::ofstream out(filename, std::ios::binary);
        REQUIRE(out.good());
        pq1.serialize(out);
    }
    priority_queue<int> pq2;
    {
        std::ifstream in(filename, std::ios::binary);
        REQUIRE(in.good());
        pq2.deserialize(in);
    }
    REQUIRE(pq2.empty());
    REQUIRE(pq2.size() == 0);
}

TEST_CASE("Exceptions on empty queue", "[exceptions]") {
    priority_queue<int> pq;
    REQUIRE_THROWS_AS(pq.head(), std::out_of_range);
    REQUIRE_THROWS_AS(pq.dequeue(), std::out_of_range);
    priority_queue<int> pq2;
    pq2.enqueue(42, 1);
    pq2.dequeue();
    REQUIRE_THROWS_AS(pq2.head(), std::out_of_range);
    REQUIRE_THROWS_AS(pq2.dequeue(), std::out_of_range);
}

TEST_CASE("Large number of elements", "[edge-cases]") {
    priority_queue<int> pq;
    for (int i = 0; i < 1000; ++i) {
        pq.enqueue(i, i);
    }
    REQUIRE(pq.size() == 1000);
    for (int i = 0; i < 1000; ++i) {
        REQUIRE(pq.head() == i);
        pq.dequeue();
    }
    REQUIRE(pq.empty());
}

TEST_CASE("Negative priorities", "[edge-cases]") {
    priority_queue<int> pq;
    pq.enqueue(1, -5);
    pq.enqueue(2, -10);
    pq.enqueue(3, 0);
    pq.enqueue(4, 5);
    REQUIRE(pq.head() == 2);
    pq.dequeue();
    REQUIRE(pq.head() == 1);
    pq.dequeue();
    REQUIRE(pq.head() == 3);
    pq.dequeue();
    REQUIRE(pq.head() == 4);
    pq.dequeue();
}

TEST_CASE("Complex type support and operations after clear", "[edge-cases]") {
    struct Person {
        std::string name;
        int age;
        bool operator==(const Person& other) const {
            return name == other.name && age == other.age;
        }
    };
    priority_queue<Person> pq;
    pq.enqueue({"Alice", 30}, 2);
    pq.enqueue({"Bob", 25}, 1);
    pq.enqueue({"Charlie", 35}, 3);
    REQUIRE(pq.head().name == "Bob");
    REQUIRE(pq.head().age == 25);
    pq.dequeue();
    priority_queue<int> pq2;
    pq2.enqueue(1, 1);
    pq2.enqueue(2, 2);
    pq2.clear();
    pq2.enqueue(100, 5);
    pq2.enqueue(200, 3);
    REQUIRE(pq2.size() == 2);
    REQUIRE(pq2.head() == 200);
    pq2.dequeue();
    REQUIRE(pq2.head() == 100);
    pq2.dequeue();
}
