#include "gtest/gtest.h"
#include "Vec2.cpp"

TEST(Vec2, plus) {
    Vec2 base(1.0, 2.5);
    Vec2 vec(5.0, 4.4);

    auto result = base + vec;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 6.9f);
}

TEST(Vec2, equal) {
    struct test{
        Vec2 base, rhs;
    };
    test tests[]{
            {Vec2(0,0), Vec2(0,0)},
            {Vec2(1,100), Vec2(1,100)},
            {Vec2(222.22,10.0), Vec2(222.22,10.0)},
    };

    for (auto tt: tests) {
        EXPECT_TRUE(tt.base == tt.rhs);
    }
}

TEST(Vec2, notEqual) {
    struct test{
        Vec2 base, rhs;
    };
    test tests[]{
            {Vec2(209.4,0), Vec2(0,0)},
            {Vec2(1.5,100), Vec2(1,100)},
            {Vec2(222.22,10.1423), Vec2(222.22,10.0)},
    };

    for (auto tt: tests) {
        EXPECT_TRUE(tt.base != tt.rhs);
    }
}

TEST(Vec2, minus) {
    struct test{
        Vec2 base, rhs, expected;
    };
    test tests[]{
            {
                    .base = Vec2(0.0f, 0.0f),
                    .rhs =Vec2(0.0f, 0.0f),
                    .expected =Vec2(0.0f, 0.0f)
            },
            {
                    .base = Vec2(209.4f, 0.0f),
                    .rhs =Vec2(0.4f, 0.0f),
                    .expected =Vec2(209.0, 0.0f)
            },
            {
                    .base = Vec2(200.4f, 10.0f),
                    .rhs =Vec2(0.0f, 2.0f),
                    .expected =Vec2(200.4f, 8.0f)
            },
            {
                    .base = Vec2(209.4f, 200.0f),
                    .rhs =Vec2(109.0f, 55.0f),
                    .expected =Vec2(100.4f, 145.0f)
            },
    };

    for (auto tt: tests) {
        auto result = tt.base - tt.rhs;
        ASSERT_FLOAT_EQ(result.x, tt.expected.x);
        ASSERT_FLOAT_EQ(result.y, tt.expected.y);
    }
}

TEST(Vec2, div) {
    struct test{
        Vec2 base;
        float div;
        Vec2 expected;
    };
    test tests[]{
            {
                    .base = Vec2(0.0f, 0.0f),
                    .div = 2.0f,
                    .expected =Vec2(0.0f, 0.0f)
            },
            {
                    .base = Vec2(200.4f, 10.40f),
                    .div = 4.0f,
                    .expected =Vec2(50.1f, 2.60f)
            },
    };

    for (auto tt: tests) {
        auto result = tt.base/ tt.div;
        ASSERT_FLOAT_EQ(result.x, tt.expected.x);
        ASSERT_FLOAT_EQ(result.y, tt.expected.y);
    }
}

TEST(Vec2, mult) {
    struct test{
        Vec2 base;
        float val;
        Vec2 expected;
    };
    test tests[]{
            {
                    .base = Vec2(0.0f, 0.0f),
                    .val = 2.0f,
                    .expected =Vec2(0.0f, 0.0f)
            },
            {
                    .base = Vec2(200.4f, 10.40f),
                    .val = 3.3f,
                    .expected =Vec2(661.32f, 34.32f)
            },
    };

    for (auto tt: tests) {
        auto result = tt.base * tt.val;
        ASSERT_FLOAT_EQ(result.x, tt.expected.x);
        ASSERT_FLOAT_EQ(result.y, tt.expected.y);
    }
}

TEST(Vec2, plusEq) {
    struct test{
        Vec2 base, rhs;
        Vec2 expected;
    };
    test tests[]{
            {
                    .base = Vec2(0.0f, 2.0f),
                    .rhs = Vec2(1.0f, 0.0f),
                    .expected =Vec2(1.0f, 2.0f)
            },
            {
                    .base = Vec2(200.4f, 10.40f),
                    .rhs = Vec2(22.6f, 11.5f),
                    .expected =Vec2(223.0f, 21.90f)
            },
    };

    for (auto tt: tests) {
        tt.base += tt.rhs;
        ASSERT_FLOAT_EQ(tt.base.x, tt.expected.x);
        ASSERT_FLOAT_EQ(tt.base.y, tt.expected.y);
    }
}

TEST(Vec2, minusEq) {
    struct test{
        Vec2 base, rhs;
        Vec2 expected;
    };
    test tests[]{
            {
                    .base = Vec2(5.0f, 2.0f),
                    .rhs = Vec2(1.5f, 1.0f),
                    .expected =Vec2(3.5f, 1.0f)
            },
            {
                    .base = Vec2(200.4f, 10.40f),
                    .rhs = Vec2(22.6f, 11.5f),
                    .expected =Vec2(177.8f, -1.1f)
            },
    };

    for (auto tt: tests) {
        tt.base -= tt.rhs;
        ASSERT_FLOAT_EQ(tt.base.x, tt.expected.x);
        ASSERT_FLOAT_EQ(tt.base.y, tt.expected.y);
    }
}

TEST(Vec2, multEq) {
    struct test{
        Vec2 base;
        float val;
        Vec2 expected;
    };
    test tests[]{
            {
                    .base = Vec2(5.0f, 2.0f),
                    .val = 2.0f,
                    .expected =Vec2(10.0f, 4.0f)
            },
            {
                    .base = Vec2(200.4f, 10.40f),
                    .val = 22.6f,
                    .expected =Vec2(4529.04f, 235.04f)
            },
    };

    for (auto tt: tests) {
        tt.base *= tt.val;
        ASSERT_FLOAT_EQ(tt.base.x, tt.expected.x);
        ASSERT_FLOAT_EQ(tt.base.y, tt.expected.y);
    }
}

TEST(Vec2, divEq) {
    struct test{
        Vec2 base;
        float val;
        Vec2 expected;
    };
    test tests[]{
            {
                    .base = Vec2(5.0f, 2.0f),
                    .val = 2.0f,
                    .expected =Vec2(2.5f, 1.0f)
            },
            {
                    .base = Vec2(200.4f, 10.40f),
                    .val = 22.6f,
                    .expected =Vec2(8.8672566372f, 0.4601769912f)
            },
    };

    for (auto tt: tests) {
        tt.base /= tt.val;
        ASSERT_FLOAT_EQ(tt.base.x, tt.expected.x);
        ASSERT_FLOAT_EQ(tt.base.y, tt.expected.y);
    }
}
