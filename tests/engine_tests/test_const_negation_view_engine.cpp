//
//  test_const_transparent_view_engine.cpp
//  Created by Ben Westcott on 11/26/24.
//

TEST_CASE
(
    "IF M is a type const negation view engine\n"
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
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;

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
    "IF M is a type const negation view engine\n"
    "THEN it is a readable engine\n"
    "THEN it is NOT a writable engine.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;

    REQUIRE(true == readable_engine<M>);
    REQUIRE(false == writable_engine<M>);
}

TEST_CASE
(
    "IF M is a type const negation view engine AND K is its owning engine\n"
    "THEN M and K have the same base types\n"
    "THEN M's owning engine type is K\n"
    "THEN M's engine type is K.\n"
    "THEN IF S is a type const negation view engine and M is its owning engine\n"
    "THEN S's owning engine is K\n"
    "THEN S's engine type is M\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;

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
    "IF M is a type const negation view engine\n"
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

    using ltype = matrix_orientation::col_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;

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
    "IF m is a const negation view engine\n"
    "THEN IF it is default constructed\n"
    "THEN m's engine ptr is nullptr.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;

    M m;

    REQUIRE(false == m.has_view());
}


TEST_CASE
(
      "IF k is a matrix storage engine which is literal2D constructed\n"
      "THEN IF m is a const negation view engine and is constructed with k\n"
      "THEN m has a view\n"
      "THEN m's rows equals k's rows\n"
      "THEN m's cols equals k's cols\n"
      "THEN m's size equals k's size\n"
      "THEN compare exact k, data_in AND m, -data_in return true\n"
      "THEN IF k is reshaped\n"
      "THEN IF s is a const negation view engine and is constructed with m\n"
      "THEN m's rows equals k's rows\n"
      "THEN m's cols equals k's cols\n"
      "THEN m's size equals k's size\n"
      "THEN compare exact k, s returns true\n"
      "THEN IF k_alt is literal2D constructed\n"
      "THEN IF m_alt is constructed with k_alt\n"
      "THEN IF s_alt is constructed with m_alt"
      "THEN IF s_alt and s are swapped\n"
      "THEN s_alt's rows equals k's rows AND s's rows equals k_alt's rows\n"
      "THEN s_alt's cols equals k's cols AND s's cols equals k_alt's cols\n"
      "THEN s_alt's size equals k's size AND s's size equals k_alt's size\n"
      "THEN compare exact k, s_alt returns true AND compare exact s, k_alt returns true.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;

    const literal2D<double> data_in = {{sqrt3, -sqrt3, sqrt3},
                                    {-sqrt3, sqrt3, -sqrt3},
                                    {sqrt3, -sqrt3, sqrt3},
                                    {-sqrt3, sqrt3, -sqrt3},
                                    {sqrt3, -sqrt3, sqrt3}};

    const literal2D<double> minus_data_in = {{-sqrt3, sqrt3, -sqrt3},
                                             {sqrt3, -sqrt3, sqrt3},
                                             {-sqrt3, sqrt3, -sqrt3},
                                             {sqrt3, -sqrt3, sqrt3},
                                             {-sqrt3, sqrt3, -sqrt3}};

    K k(data_in);
    M m(k);

    REQUIRE(true == m.has_view());

    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());

    REQUIRE(true == eh::compare_exact(k, data_in));
    REQUIRE(true == eh::compare_exact(m, minus_data_in));

    size_t new_nbr_rows = 20;
    
    k.reshape_rows(new_nbr_rows, new_nbr_rows);

    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());

    using S = matrix_view_engine<M, matrix_view::const_negation>;

    S s(m);

    REQUIRE(s.rows() == k.rows());
    REQUIRE(s.cols() == k.cols());
    REQUIRE(s.size() == k.size());

    REQUIRE(true == eh::compare_exact(s, k));

    const literal2D<double> data_in_alt = {{sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2}};

    K k_alt(data_in_alt);
    M m_alt(k_alt);
    S s_alt(m_alt);

    s.swap(s_alt);

    REQUIRE(s_alt.rows() == k.rows());
    REQUIRE(s_alt.cols() == k.cols());
    REQUIRE(s_alt.size() == k.size());

    REQUIRE(true == eh::compare_exact(s_alt, k));

    REQUIRE(s.rows() == k_alt.rows());
    REQUIRE(s.cols() == k_alt.cols());
    REQUIRE(s.size() == k_alt.size());

    REQUIRE(true == eh::compare_exact(s, k_alt));

    s_alt.swap(s);

    REQUIRE(s.rows() == k.rows());
    REQUIRE(s.cols() == k.cols());
    REQUIRE(s.size() == k.size());

    REQUIRE(true == eh::compare_exact(s, k));

    REQUIRE(s_alt.rows() == k_alt.rows());
    REQUIRE(s_alt.cols() == k_alt.cols());
    REQUIRE(s_alt.size() == k_alt.size());

    REQUIRE(true == eh::compare_exact(s_alt, k_alt));
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN IF m is a const negation view engine and is constructed with k\n"
    "THEN m_ij == -k_ij AND m_i = -k_i\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = 6;
    constexpr size_t ncols = 6;
    
    using ltype = matrix_orientation::row_major_t;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::const_negation>;
    
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
            REQUIRE(m(i, j) == -k(i, j));
        }
    }

    i = 0;
    for(; i < m.size(); i++)
    {
        REQUIRE(m(i) == -k(i));
    }
    
}