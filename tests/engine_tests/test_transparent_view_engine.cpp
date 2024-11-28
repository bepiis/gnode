//
//  test_transparent_view_engine.cpp
//  Created by Ben Westcott on 11/24/24.
//

TEST_CASE
(
    "IF M is a transparent view engine\n"
    "THEN M is not an owning engine\n"
    "THEN M has mutable access\n"
    "THEN M has immutable access\n"
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

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;

    REQUIRE(false == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(true == mutable_access<M>);
    REQUIRE(true == immutable_access<M>);
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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;

    REQUIRE(true == std::is_trivially_copyable_v<M>);
    REQUIRE(true == std::is_trivially_copy_constructible_v<M>);
    REQUIRE(true == std::is_trivially_move_constructible_v<M>);
    REQUIRE(true == std::is_default_constructible_v<M>);
    REQUIRE(true == std::is_nothrow_default_constructible_v<M>);
    REQUIRE(false == std::is_trivially_default_constructible_v<M>);
    REQUIRE(true == std::constructible_from<M, K &>);
    REQUIRE(true == std::is_nothrow_swappable_v<M&>);
}

TEST_CASE
(
    "IF m is a transparent view engine\n"
    "THEN IF it is default constructed\n"
    "THEN m's engine ptr is nullptr.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;
    
    M m;
    
    REQUIRE(false == m.has_view());
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN IF m is constructed with k\n"
    "THEN m has a view\n"
    "THEN m's rows equals k's rows\n"
    "THEN m's cols equals k's cols\n"
    "THEN m's size equals k's size\n"
    "THEN compare exact k, m returns true\n"
    "THEN IF k is reshaped\n"
    "THEN m's rows equals k's rows\n"
    "THEN m's cols equals k's cols\n"
    "THEN m's size equals k's size\n"
    "THEN compare exact k, m returns true\n"
    "THEN IF k_alt is literal2D constructed\n"
    "THEN IF m_alt is constructed with k_alt\n"
    "THEN IF m_alt and m are swapped\n"
    "THEN m_alt's rows equals k's rows AND m's rows equals k_alt's rows\n"
    "THEN m_alt's cols equals k's cols AND m's cols equals k_alt's cols\n"
    "THEN m_alt's size equals k's size AND m's size equals k_alt's size\n"
    "THEN compare exact k, m_alt returns true AND compare exact m, k_alt returns true.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;
    
    const literal2D<double> data_in = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
                                       {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
                                       {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
                                       {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
                                       {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
                                       {1.30, 1.31, 1.32, 1.33, 1.34, 1.35}};
    
    K k(data_in);
    M m(k);

    REQUIRE(true == m.has_view());
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(k,m));
    
    size_t new_nbr_rows = 8;
    size_t new_row_reach = 50;
    size_t new_nbr_cols = 3;
    size_t new_col_reach = 13;
    
    k.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach);
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(k,m));
    

    const literal2D<double> data_in_alt  = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                        {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                        {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                        {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                        {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                        {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};
    
    K k_alt(data_in_alt);
    M m_alt(k_alt);
    
    m.swap(m_alt);
    
    REQUIRE(m_alt.rows() == k.rows());
    REQUIRE(m_alt.cols() == k.cols());
    REQUIRE(m_alt.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(k,m_alt));
    
    REQUIRE(m.rows() == k_alt.rows());
    REQUIRE(m.cols() == k_alt.cols());
    REQUIRE(m.size() == k_alt.size());
    
    REQUIRE(true == eh::compare2D_exact(k_alt,m));
    
    m_alt.swap(m);
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(k,m));
    
    REQUIRE(m_alt.rows() == k_alt.rows());
    REQUIRE(m_alt.cols() == k_alt.cols());
    REQUIRE(m_alt.size() == k_alt.size());
    
    REQUIRE(true == eh::compare2D_exact(k_alt,m_alt));
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN IF m is constructed with k\n"
    "THEN m_ij == k_ij AND m_i = k_i\n"
    "THEN if m set to zero for all i and j\n"
    "THEN k is set to zero for all i and j.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = 6;
    constexpr size_t ncols = 6;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;
    
    const literal2D<double> data_in = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
                                       {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
                                       {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
                                       {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
                                       {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
                                       {1.30, 1.31, 1.32, 1.33, 1.34, 1.35}};
    
    K k(data_in);
    M m(k);
    
    size_t i, j;
    
    i = 0;
    for(; i < m.rows(); i++)
    {
        j = 0;
        for(; j < m.cols(); j++)
        {
            REQUIRE(m(i, j) == k(i, j));
        }
    }
    
    i = 0;
    for(; i < m.size(); i++)
    {
        REQUIRE(m(i) == k(i));
        m(i) = 0.0;
        REQUIRE(0.0 == m(i));
    }
    
    i = 0;
    for(; i < m.rows(); i++)
    {
        j = 0;
        for(; j < m.cols(); j++)
        {
            m(i, j) = 30.0;
            REQUIRE(30.0 == m(i, j));
        }
    }
}














