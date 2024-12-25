//
//  test_generators.cpp
//  Created by Ben Westcott on 12/25/24.
//

TEST_CASE
(
    "Basic generator type with static RC satisfy the following concepts:\n"
    "   1) base types\n"
    "   2) base engine\n"
    "   3) readable engine\n"
    "   4) exportable.\n"
)
{
    auto gen_fun = [](size_t i, size_t j) -> double 
    {
        return static_cast<double>(i + j);
    };

    using G = generator<generator_types::basic, decltype(gen_fun), 3, 3, matrix_orientation::row_major>;

    REQUIRE(true == base_types<G>);
    REQUIRE(true == base_engine<G>);
    REQUIRE(true == readable_engine<G>);
    REQUIRE(true == exportable<G>);
}

TEST_CASE
(
    "Basic generator with static R and dynamic C satisfy the following concepts:\n"
    "   1) base types\n"
    "   2) base engine\n"
    "   3) readable engine\n"
    "   4) exportable.\n"
)
{
    auto gen_fun = [](size_t i, size_t j) -> double
    {
        return static_cast<double>(i + j);
    };

    using G = generator<generator_types::basic, decltype(gen_fun), 5, std::dynamic_extent, matrix_orientation::row_major>;

    REQUIRE(true == base_types<G>);
    REQUIRE(true == base_engine<G>);
    REQUIRE(true == readable_engine<G>);
    REQUIRE(true == exportable<G>);
}

TEST_CASE
(
    "Basic generator with dynamic R and static C satisfy the following concepts:\n"
    "   1) base types\n"
    "   2) base engine\n"
    "   3) readable engine\n"
    "   4) exportable.\n"
)
{
    auto gen_fun = [](size_t i, size_t j) -> double
    {
        return static_cast<double>(i + j);
    };

    using G = generator<generator_types::basic, decltype(gen_fun), std::dynamic_extent, 10, matrix_orientation::row_major>;

    REQUIRE(true == base_types<G>);
    REQUIRE(true == base_engine<G>);
    REQUIRE(true == readable_engine<G>);
    REQUIRE(true == exportable<G>);   
}

TEST_CASE
(
    "Basic generator with dynamic R and dynamic C satisfy the following concepts:\n"
    "   1) base types\n"
    "   2) base engine\n"
    "   3) readable engine\n"
    "   4) exportable.\n"
)
{
    auto gen_fun = [](size_t i, size_t j) -> double
    {
        return static_cast<double>(i + j);
    };

    using G = generator<generator_types::basic, decltype(gen_fun), std::dynamic_extent, std::dynamic_extent, matrix_orientation::row_major>;

    REQUIRE(true == base_types<G>);
    REQUIRE(true == base_engine<G>);
    REQUIRE(true == readable_engine<G>);
    REQUIRE(true == exportable<G>);
}