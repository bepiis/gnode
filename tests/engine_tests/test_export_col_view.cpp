//
//  test_export_col_view_engine.cpp
//  Created by Ben Westcott on 12/1/24.
//

TEST_CASE
(
  "IF M is the type of a a const col view engine\n"
  "THEN it has an owning engine type alias whose is_owning member is false\n"
  "THEN it is NOT an owning engine\n"  
  "THEN M does not have mutable access\n"
  "THEN M has immutable access\n"
  "THEN M does not have a consistent mutable reference type\n"
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
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::col, K>;
    
    REQUIRE(false == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(false == owning_engine<M>);
    REQUIRE(false == mutable_access<M>);
    REQUIRE(true == immutable_access<M>);
    REQUIRE(false == consistent_mutable_ref_type<M>);
    REQUIRE(true == consistent_immutable_ref_type<M>);
    REQUIRE(true == consistent_return_sizes<M>);
    REQUIRE(true == consistent_return_lengths<M>);
    
    REQUIRE(false == reshapeable_engine<M>);
    REQUIRE(false == row_reshapeable_engine<M>);
    REQUIRE(false == col_reshapeable_engine<M>);
}


TEST_CASE
(
    "IF m is a type export col view engine\n"
    "THEN is constexpr col returns true\n"
    "THEN engine ct extents returns constexpr col == 1\n"
    "THEN colvec dimensions concept returns true.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::col, K>;

    REQUIRE(true == engine_ct_extents<M>::is_constexpr_cols());
    REQUIRE(1 == engine_ct_extents<M>::cols());
    REQUIRE(true == colvec_dimensions<M>);
}

TEST_CASE
(
  "IF M is a type const col view engine\n"
  "THEN it is a base engine"
  "THEN it is a readable engine\n"
  "THEN it is NOT a writable engine.\n"
  "THEN it is a basic view engine\n"
  "THEN it is NOT a mutable view engine\n"
  "THEN it has a immutable view.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    
    REQUIRE(true == writable_engine<K>);
    
    using M = engine_view<export_views::col, K>;
    
    REQUIRE(true == base_engine<M>);
    REQUIRE(true == readable_engine<M>);
    REQUIRE(false == writable_engine<M>);
    REQUIRE(true == view_basics<M>);
    REQUIRE(false == mutable_view<M>);
    REQUIRE(true == immutable_view<M>);
}

TEST_CASE
(
  "IF M is a type const col view engine AND K is its owning engine\n"
  "THEN M and K have the same base types\n"
  "THEN M's owning engine type is K\n"
  "THEN M's engine type is K.\n"
  "THEN IF S is a const col view engine and M is its owning engine\n"
  "THEN S's owning engine is K\n"
  "THEN S's engine type is M\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::col, K>;
    
    REQUIRE(true == std::is_same_v<M::data_type, K::data_type>);
    REQUIRE(true == std::is_same_v<M::index_type, K::index_type>);
    REQUIRE(true == std::is_same_v<M::reference, K::const_reference>);
    REQUIRE(true == std::is_same_v<M::const_reference, K::const_reference>);
    REQUIRE(true == std::is_same_v<M::orientation_type, K::orientation_type>);
    
    REQUIRE(true == std::is_same_v<M::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<M::engine_type, K>);
    
    using S = engine_view<export_views::col, M>;
    
    REQUIRE(true == std::is_same_v<S::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<S::engine_type, M>);
    REQUIRE(false == std::is_same_v<S::engine_type, K>);
}

TEST_CASE
(
  "IF M is a type const col view engine\n"
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
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::col, K>;
    
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
  "IF m is a const col view engine\n"
  "THEN IF it is default constructed\n"
  "THEN m's engine ptr is nullptr.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::col, K>;
    
    M m;
    
    REQUIRE(false == m.has_view());
}

TEST_CASE
(
  "IF k is a matrix storage engine which is literal2D constructed\n"
  "THEN IF m is a const col view engine and is constructed with k\n"
  "THEN m's rows equals k's rows\n"
  "THEN m's cols equals k's 1\n"
  "THEN m's size equals k's rows\n"
  "THEN compare exact m, data in's first col returns true\n"
  "THEN IF k is reshaped\n"
  "THEN m's rows equals k's rows\n"
  "THEN m's cols equals k's 1\n"
  "THEN m's size equals k's rows\n"
  "THEN compare exact m, data in's first col returns true\n"
  "THEN IF k_alt is literal2D constructed\n"
  "THEN IF m_alt is constructed with k_alt and col != 0\n"
  "THEN IF m_alt and m are swapped\n"
  "THEN m_alt's rows equals k's rows AND m's rows equals k_alt's rows \n"
  "THEN m_alt's cols equals 1 AND m's cols 1\n"
  "THEN m_alt's size equals k's rows AND m's size equals k_alt's rows\n"
  "THEN compare exact m_alt, data in's first col returns true\n"
  "THEN compare exact m, and data in alt's pth col returns true\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::col, K>;
    
    const literal2D<double> data_in =
    {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
        {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
        {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
        {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
        {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
        {1.30, 1.31, 1.32, 1.33, 1.34, 1.35},
        {1.36, 1.37, 1.38, 1.39, 1.40, 1.41},
        {1.42, 1.43, 1.44, 1.45, 1.46, 1.47}};
    
    const literal2D<double> data_in_col_zero =
    {{1.00}, {1.06}, {1.12}, {1.18}, {1.24}, {1.30}, {1.36}, {1.42}};
    
    const literal2D<double> data_in_alt =
    {{1.00, 1.06, 1.12, 1.18, 1.24, 1.30},
        {1.01, 1.07, 1.13, 1.19, 1.25, 1.31},
        {1.02, 1.08, 1.14, 1.20, 1.26, 1.32},
        {1.03, 1.09, 1.15, 1.21, 1.27, 1.33},
        {1.04, 1.10, 1.16, 1.22, 1.28, 1.34},
        {1.05, 1.11, 1.17, 1.23, 1.29, 1.35},
        {-1.00, -2.00, -3.00, -4.00, -5.00, -6.00},
        {-1.10, -2.20, -3.30, -4.40, -5.50, -6.60}};
    
    size_t p = 5;
    
    const literal2D<double> data_in_alt_pth_col =
    {{1.30}, {1.31}, {1.32}, {1.33}, {1.34}, {1.35}, {-6.00}, {-6.60}};
    
    K k(data_in);
    M m(k);
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == 1);
    REQUIRE(m.size() == k.rows());
    
    REQUIRE(true == eh::compare2D_exact(m, data_in_col_zero));
    
    size_t new_nbr_rows = 50;
    size_t new_row_reach = 400;
    size_t new_nbr_cols = 300;
    size_t new_col_reach = 900;
    
    k.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach);
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == 1);
    REQUIRE(m.size() == k.rows());
    
    M mv(k);
    REQUIRE(true == eh::compare2D_exact(m, mv));
    
    K k_alt(data_in_alt);
    M m_alt(k_alt, p);
    
    m.swap(m_alt);
    
    REQUIRE(m.rows() == k_alt.rows());
    REQUIRE(m.cols() == 1);
    REQUIRE(m.size() == k_alt.rows());
    
    REQUIRE(true == eh::compare2D_exact(m, data_in_alt_pth_col));
    
    REQUIRE(m_alt.rows() == k.rows());
    REQUIRE(m_alt.cols() == 1);
    REQUIRE(m_alt.size() == k.rows());
    
    REQUIRE(true == eh::compare2D_exact(m_alt, mv));
    
    m_alt.swap(m);
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == 1);
    REQUIRE(m.size() == k.rows());
    
    REQUIRE(true == eh::compare2D_exact(m, mv));
    
    REQUIRE(m_alt.rows() == k_alt.rows());
    REQUIRE(m_alt.cols() == 1);
    REQUIRE(m_alt.size() == k_alt.rows());
    
    REQUIRE(true == eh::compare2D_exact(m_alt, data_in_alt_pth_col));
}




