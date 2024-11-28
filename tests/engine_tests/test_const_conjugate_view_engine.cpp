//
//  test_const_conjugate_view_engine.cpp
//  Created by Ben Westcott on 11/27/24.
//

TEST_CASE
(
  "IF M is a type const conjugate view engine\n"
  "THEN it is NOT an owning engine\n"
  "THEN it does NOT have mutable access\n"
  "THEN it has immutable access\n"
  "THEN it does NOT have a consistent mutable reference type\n"
  "THEN it does NOT have a consistent immutable reference type\n"
  "THEN it has consistent return sizes\n"
  "THEN it has consistent return lengths\n"
  "THEN it is not reshapeable, row reshapeable, or col reshapeable.\n"
)
{
    using dtype = std::complex<int64_t>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major_t;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_conjugate>;
    
    REQUIRE(false == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(false == mutable_access<M>);
    REQUIRE(true == immutable_access<M>);
    REQUIRE(false == consistent_mutable_ref_type<M>);
    REQUIRE(false == consistent_immutable_ref_type<M>);
    REQUIRE(true == consistent_return_sizes<M>);
    REQUIRE(true == consistent_return_lengths<M>);
    
    REQUIRE(false == reshapeable_engine<M>);
    REQUIRE(false == row_reshapeable_engine<M>);
    REQUIRE(false == col_reshapeable_engine<M>);
}

TEST_CASE
(
  "IF M is a type const conjugate view engine\n"
  "THEN it is a readable engine\n"
  "THEN it is NOT a writable engine.\n"
)
{
    using dtype = std::complex<int64_t>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major_t;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_conjugate>;
    
    REQUIRE(true == readable_engine<M>);
    REQUIRE(false == writable_engine<M>);
}

TEST_CASE
(
  "IF M is a type const conjugate view engine AND K is its owning engine\n"
  "THEN M and K have the same base types\n"
  "THEN M's owning engine type is K\n"
  "THEN M's engine type is K.\n"
  "THEN IF S is a type const conjugate view engine and M is its owning engine\n"
  "THEN S's owning engine is K\n"
  "THEN S's engine type is M\n"
)
{
    using dtype = std::complex<int64_t>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major_t;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_conjugate>;
    
    REQUIRE(true == std::is_same_v<M::data_type, K::data_type>);
    REQUIRE(true == std::is_same_v<M::index_type, K::index_type>);
    REQUIRE(true == std::is_same_v<M::reference, K::data_type>);
    REQUIRE(true == std::is_same_v<M::const_reference, K::data_type>);
    REQUIRE(true == std::is_same_v<M::orientation_type, K::orientation_type>);
    
    REQUIRE(true == std::is_same_v<M::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<M::engine_type, K>);
    
    using S = matrix_view_engine<M, matrix_view::const_transparent>;
    
    REQUIRE(true == std::is_same_v<S::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<S::engine_type, M>);
    REQUIRE(false == std::is_same_v<S::engine_type, K>);
}

TEST_CASE
(
  "IF M is a type const conjugate view engine\n"
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
    using dtype = std::complex<int64_t>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major_t;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_conjugate>;
    
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
  "IF m is a const conjugate view engine\n"
  "THEN IF it is default constructed\n"
  "THEN m's engine ptr is nullptr.\n"
)
{
    using dtype = std::complex<int64_t>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major_t;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_conjugate>;
    
    M m;
    
    REQUIRE(false == m.has_view());
}



