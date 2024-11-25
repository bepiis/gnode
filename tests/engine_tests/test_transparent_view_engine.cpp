//
//  test_transparent_view_engine.cpp
//  Created by Ben Westcott on 11/24/24.
//

TEST_CASE
(
    "IF M is a transparent view engine\n"
    "THEN M is not an owning engine\n"
    "THEN M has mutable access\n"
    "THEN M does not have immutable access\n"
    "THEN M has a consistent mutable reference type\n"
    "THEN M has a consistent immutable reference type\n"
    "THEN M has consistent return sizes\n"
    "THEN M has consistent return lenghts\n"
    "THEN M is not reshapeable or row_reshapeable or col_reshapeable.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;

    REQUIRE(false == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(true == mutable_access<M>);
    REQUIRE(false == immutable_access<M>);
    REQUIRE(true == consistent_mutable_ref_type<M>);
    REQUIRE(true == consistent_immutable_ref_type<M>);
    REQUIRE(true == consistent_return_sizes<M>);
    REQUIRE(true == consistent_return_lengths<M>);

    REQUIRE(false == reshapeable_engine<M>);
    REQUIRE(false == row_reshapeable_engine<M>);
    REQUIRE(false == col_reshapeable_engine<M>);
}

TEST_CASE
(
    "IF M is a type transparent_view_engine\n"
    "THEN M is a readable_engine\n"
    "THEN M is a writable_engine.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == writable_engine<K>);

    using M = matrix_view_engine<K, matrix_view::transparent>;

    REQUIRE(true == readable_engine<M>);
    REQUIRE(true == writable_engine<M>);
}

TEST_CASE
(
    "IF M is a transparent view engine AND K is its owning engine\n"
    "THEN M and K have the same base types\n"
    "THEN M's owning engine type is K\n"
    "THEN M's engine type is K.\n"
    "THEN IF S is a transparent view engine and M is its owning engine\n"
    "THEN S's owning engine is K\n"
    "THEN S's engine type is M\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;

    REQUIRE(true == std::is_same_v<M::data_type, K::data_type>);
    REQUIRE(true == std::is_same_v<M::index_type, K::index_type>);
    REQUIRE(true == std::is_same_v<M::reference, K::reference>);
    REQUIRE(true == std::is_same_v<M::const_reference, K::const_reference>);
    REQUIRE(true == std::is_same_v<M::orientation_type, K::orientation_type>);

    REQUIRE(true == std::is_same_v<M::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<M::engine_type, K>);

    using S = matrix_view_engine<M, matrix_view::transparent>;

    REQUIRE(true == std::is_same_v<S::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<S::engine_type, M>);
    REQUIRE(false == std::is_same_v<S::engine_type, K>);
}

TEST_CASE
(
    "IF M is a transparent view engine\n"
    "THEN it is trivially copyable\n"
    "THEN it is trivially copy constructible\n"
    "THEN it is trivially move constructible\n"
    "THEN it is default constructible\n"
    "THEN it is nothrow default constructible\n"
    "THEN it is NOT trivially default constructible\n"
    "THEN it is constructable from its engine type\n"
    "THEN it is nothrow swappable.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;

    REQUIRE(true == std::is_trivially_copyable_v<M>);
    REQUIRE(true == std::is_trivially_copy_constructible_v<M>);
    REQUIRE(true == std::is_trivially_move_constructible_v<M>);
    REQUIRE(true == std::is_default_constructible_v<M>);
    REQUIRE(true == std::is_nothrow_default_constructible_v<M>);
    REQUIRE(false == std::is_trivially_default_constructible_v<M>);
    REQUIRE(true == std::constructible_from<M, K const&>);
    REQUIRE(true == std::is_nothrow_swappable_v<M&>);
}

TEST_CASE
(
    "IF m is a transparent view engine\n"
    "THEN IF it is default constructed\n"
    "THEN m's engine ptr is nullptr.\n"
)
{

}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN IF m is constructed with k\n"
    "THEN m's rows equals k's rows\n"
    "THEN m's cols equals k's cols\n"
    "THEN m's size equals k's size\n"
    "THEN m_ij == k_ij\n"
    "THEN m_i == k_i.\n"
)
{

}










