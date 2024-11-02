#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/Arguments.hpp"
#include<iostream>
#include "../src/distribution/NormalDistribution.hpp"
#include "../src/distribution/UniformIntDistribution.hpp"
#include "../src/sketches/QSketch.hpp"
#include "../src/sketches/FastExpSketch.hpp"
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("UniformIntDistribution(1,1) should return 1 and correctly compute totalWeight") {
    auto weightDistribution = std::make_unique<UniformIntDistribution>(1, 1, 42);
    for(int i = 0; i < 10; i++){
        weightDistribution->getWeight();
    }
    REQUIRE(weightDistribution->getWeight() == 1);
    REQUIRE(weightDistribution->getTotalWeight() == 11);
}


TEST_CASE("NormalDistribution should work correctly") {
    auto weightDistribution = std::make_unique<NormalDistribution>(100, 20, 42);
    for(int i = 0; i < 100; i++){
        weightDistribution->getWeight();
    }
    REQUIRE_THAT(weightDistribution->getTotalWeight()/100, Catch::Matchers::WithinRel(100.0, 0.01));

}

TEST_CASE("QSketch should closely approximate after arrival of 1 element") {
    auto sketch = new QSketch(400, 1, 42);

    for(uint64_t item=0; item < 1; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }
    // these tests are just ot see if nothing changed. they are ment to fail if something changes.
    REQUIRE_THAT(sketch->estimate(0.5), Catch::Matchers::WithinAbs(1.04330049, 0.0000001));
    REQUIRE_THAT(sketch->estimateNewton(), Catch::Matchers::WithinAbs(0.961076, 0.000001));

    REQUIRE_THAT(sketch->estimate(0.5), Catch::Matchers::WithinRel(1, 0.05));
    REQUIRE_THAT(sketch->estimateNewton(), Catch::Matchers::WithinRel(1, 0.05));
}


TEST_CASE("QSketch should closely approximate after arrival of 10 element") {
    uint am_elems = 10;
    auto sketch = new QSketch(400, 1, 42);

    for(uint64_t item=0; item < am_elems; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }

    REQUIRE_THAT(sketch->estimate(0.5), Catch::Matchers::WithinRel(am_elems, 0.07));
    REQUIRE_THAT(sketch->estimateNewton(), Catch::Matchers::WithinRel(am_elems, 0.07));
}

TEST_CASE("QSketch should closely approximate after arrival of 100 element") {
    uint am_elems = 100;
    auto sketch = new QSketch(400, 1, 42);

    for(uint64_t item=0; item < am_elems; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }

    REQUIRE_THAT(sketch->estimate(0.5), Catch::Matchers::WithinRel(am_elems, 0.06));
    REQUIRE_THAT(sketch->estimateNewton(), Catch::Matchers::WithinRel(am_elems, 0.06));
}

TEST_CASE("FastExpSketch should closely approximate after arrival of 1 element") {
    auto sketch = new FastExpSketch(400, 42);

    for(uint64_t item=0; item < 1; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }
    // this tests are just ot see if nothing changed. they are ment to fail if something changes.
    REQUIRE_THAT(sketch->estimate(), Catch::Matchers::WithinAbs(1.04095648, 0.0000001));

    REQUIRE_THAT(sketch->estimate(), Catch::Matchers::WithinRel(1, 0.05));
}

TEST_CASE("FastExpSketch should closely approximate after arrival of 10 element") {
    uint am_elems = 10;
    auto sketch = new FastExpSketch(400, 42);

    for(uint64_t item=0; item < am_elems; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }
    REQUIRE_THAT(sketch->estimate(), Catch::Matchers::WithinRel(am_elems, 0.07));
}

TEST_CASE("FastExpSketch should closely approximate after arrival of 100 element") {
    uint am_elems = 100;
    auto sketch = new FastExpSketch(400, 42);

    for(uint64_t item=0; item < am_elems; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }

    REQUIRE_THAT(sketch->estimate(), Catch::Matchers::WithinRel(am_elems, 0.07));
}

TEST_CASE("Benchmarking estimates") {
    uint am_elems = 10000;
    auto sketch = new QSketch(400, 1, 42);

    for(uint64_t item=0; item < am_elems; item++){
        sketch->consume((uint8_t*)&item, 8, 1);
    }

    BENCHMARK("estimate(0.5)"){
        return sketch->estimate(0.5);
    };
    BENCHMARK("oldEstimate(x)"){
        return sketch->oldEstimate(0.5);
    };
    BENCHMARK("estimateNewton()"){
        return sketch->estimateNewton();
    };
}