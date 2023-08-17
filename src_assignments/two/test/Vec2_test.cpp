#include "gtest/gtest.h"
#include "Vec2.cpp"

TEST(Vec2, plus) {
    Vec2 base(1.0, 2.5);
    Vec2 vec(5.0, 4.4);

    auto result = base + vec;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 6.9f);
}
