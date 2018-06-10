#include "die.h"
#include "displaytoolbox.h"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <algorithm>
#include <deque>
#include <string>

#include "QList"

std::string roll(const char* command, RandomGenerator::result_type maxDiceValue, std::vector<RandomGenerator::result_type> numbers) {
    std::deque<RandomGenerator::result_type> q;
    q.assign(begin(numbers), end(numbers));
    auto gen = CustomRandomGenerator(maxDiceValue, q);
    Die::setRandomGenerator(gen);

    DiceParser parser;
    auto ret = parse(parser, command);
    return ret.scalarText.toStdString();
}

TEST_CASE("keep", "operator") {
    CHECK( roll("4d10k1", 10, {3,4,6,1}) == "6" );
    CHECK( roll("4d10k2", 10, {3,4,6,1}) == "4,6" );
    CHECK( roll("4d10k[>5]", 10, {3,4,6,1}) == "6" );
    CHECK( roll("4d10k[>2]", 10, {3,4,6,1}) == "3,4,6" );
    CHECK( roll("4d10k[>7]", 10, {3,4,6,1}) == "0" );

    // k require a range
    CHECK_THROWS( roll("4d10ks", 10, {3,4,6,1}));
}

// deprecate Keep and explode

TEST_CASE("keep lower", "operator") {
    CHECK( roll("4d10kl1", 10, {3,4,6,1}) == "1" );
    CHECK( roll("4d10kl2", 10, {3,4,6,1}) == "1,3" );
}

TEST_CASE("sort", "operator") {
    CHECK( roll("4d10s", 10, {3,4,6,1}) == "1,3,4,6" );
    CHECK( roll("4d10s", 10, {1,3,4,6}) == "1,3,4,6" );
}

TEST_CASE("count", "operator") {
    CHECK( roll("4d10c[>5]", 10, {3,4,6,1}) == "1" );
    CHECK( roll("4d10c[>2]", 10, {3,4,6,1}) == "3" );

    // interaction with other operators
    CHECK( roll("4d10k[>7]c", 10, {3,4,6,1}) == "0" );
}

TEST_CASE("reroll", "operator") {
    CHECK( roll("4d10r6", 10, {3,4,6,1,7}) == "3,4,7,1" );
    CHECK( roll("4d10r6", 10, {3,4,6,1,6}) == "3,4,6,1" ); // only one reroll
    CHECK( roll("4d10r[>5]", 10, {3,4,6,1,7}) == "3,4,7,1" );
    CHECK( roll("4d10r[>2]", 10, {3,4,6,1,1,2,3}) == "1,2,6,3" );
}

TEST_CASE("reroll until", "operator") {
    CHECK( roll("4d10R6", 10, {3,4,6,1,7}) == "3,4,7,1" );
    CHECK( roll("4d10R6", 10, {3,4,6,1,6,6,5}) == "3,4,5,1" );
    CHECK( roll("4d10R[>5]", 10, {3,4,6,1,7}) == "3,4,7,1" );
    CHECK( roll("4d10R[>2]", 10, {3,4,6,1,1,2,3}) == "1,2,6,3" );
    CHECK( roll("4d10R[>2]", 10, {3,4,6,1,1,6,2,6,6,3}) == "1,2,6,3" );
}

TEST_CASE("explode", "operator") {
    CHECK( roll("4d10e", 10, {3,4,6,1}) == "3,4,6,1" );
    CHECK( roll("4d10e", 10, {3,4,6,10,1}) == "3,4,6,11" );
    CHECK( roll("4d10e6", 10, {3,4,6,1,5}) == "3,4,11,1" );
    //CHECK( roll("4d10e6", 10, {3,4,6,1,6,5}) == "3,4,17,1" );
    //CHECK( roll("4d10e[>5]", 10, {3,4,6,1,5}) == "3,4,11,1" );
    //CHECK( roll("4d10e[>2]", 10, {3,4,6,1,5,1,1,1}) == "9,5,7,1" );
}

TEST_CASE("backward jump", "operator") {
    CHECK( roll("4d10c[>2]c+@c[>=4]", 10, {3,4,6,1}) == "5" );
    CHECK( roll("6d10kl4c[>2]c+@c[>=4]", 10, {3,4,6,1,9,8}) == "5" );
}

/* not testable
TEST_CASE("paint", "operator") {
}
*/

/* not testable
TEST_CASE("merge", "operator") {
    CHECK( roll("4d10m1", 10, {3,4,6,1}) == "1" );
    CHECK( roll("4d10m2", 10, {3,4,6,1}) == "1,3" );
}
*/

TEST_CASE("if", "operator") {
    CHECK( roll("4d10i[>2]{\"x\"", 10, {3,4,6,1}) == "x" );
    CHECK( roll("4d10i2", 10, {3,4,6,1}) == "1,3" );
}

TEST_CASE("group", "operator") {
    CHECK( roll("5d10g7", 10, {3,4,6,1,2}) == "2" );
    CHECK( roll("3d10g10", 10, {9,9,2}) == "1" );
    CHECK( roll("4d10g10", 10, {9,9,2,2}) == "2" );
    CHECK( roll("4d10g10", 10, {2,2,9,9}) == "2" );
    CHECK( roll("4d10g10", 10, {7,4,3,6}) == "2" );
    CHECK( roll("4d10g10", 10, {8,4,3,7}) == "2" );
    CHECK( roll("13d100g100", 100, {/* group A = 100 */ 50, 25, 25, /* group B = 100 */ 33, 34, 32, 1, /* group C = 100 */ 10, 11, 12, 13, 54}) == "3" );
    // CHECK( roll("12d100g100", 100, {/* group A = 103 */ 50, 28, 25, /* group B = 104 */ 33, 34, 32, 5, /* group C = 101 */ 9, 11, 12, 13, 56}) == "3" );
}

TEST_CASE("comment", "operator") {
    CHECK( roll("4d10#comment", 10, {3,4,6,1}) == "comment\n3,4,6,1" );
}

