//
//  test_transpose_view_engine.cpp
//  Created by Ben Westcott on 11/28/24.
//

TEST_CASE
(
  "IF M is a transpose view engine\n"
  "THEN it has an owning engine type alias whose is_owning member is false\n"
  "THEN it is NOT an owning engine\n"   
  "THEN it has mutable access\n"
  "THEN it has immutable access\n"
  "THEN it has a consistent mutable reference type\n"
  "THEN it has a consistent immutable reference type\n"
  "THEN it has consistent return sizes\n"
  "THEN it has consistent return lenghts\n"
  "THEN it is not reshapeable or row_reshapeable or col_reshapeable.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<K, inport_views::transpose>;
    
    REQUIRE(false == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(false == owning_engine<M>);
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
    "IF M is a type transpose view engine\n"
    "THEN it is a base engine\n"
    "THEN it is a readable engine\n"
    "THEN it is a writable engine.\n"
    "THEN it is a valid mutable view engine\n"
    "THEN it does not have an immutable view.\n"

)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == writable_engine<K>);

    using M = engine_view<K, inport_views::transpose>;

    REQUIRE(true == base_engine<M>);
    REQUIRE(true == readable_engine<M>);
    REQUIRE(true == writable_engine<M>);
    REQUIRE(true == mutable_view<M>);
    REQUIRE(false == immutable_view<M>);
}

TEST_CASE
(
  "IF M is a transpose view engine AND K is its owning engine\n"
  "THEN M and K have the same base types\n"
  "THEN M's owning engine type is K\n"
  "THEN M's engine type is K.\n"
  "THEN IF S is a transpose view engine and M is its owning engine\n"
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
    using M = engine_view<K, inport_views::transpose>;
    
    REQUIRE(true == std::is_same_v<M::data_type, K::data_type>);
    REQUIRE(true == std::is_same_v<M::index_type, K::index_type>);
    REQUIRE(true == std::is_same_v<M::reference, K::reference>);
    REQUIRE(true == std::is_same_v<M::const_reference, K::const_reference>);
    REQUIRE(true == std::is_same_v<M::orientation_type, matrix_orientation::col_major>);
    
    REQUIRE(true == std::is_same_v<M::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<M::engine_type, K>);
    
    using S = engine_view<M, inport_views::transpose>;
    
    REQUIRE(true == std::is_same_v<S::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<S::engine_type, M>);
    REQUIRE(false == std::is_same_v<S::engine_type, K>);
}

TEST_CASE
(
  "IF M is a transpose view engine\n"
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
    using M = engine_view<K, inport_views::transpose>;
    
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
  "IF m is a transpose view engine\n"
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
    using M = engine_view<K, inport_views::transpose>;
    
    M m;
    
    REQUIRE(false == m.has_view());
}


TEST_CASE
(
  "IF k is a matrix storage engine which is literal2D constructed\n"
  "THEN IF m is a transpose view engine and is constructed with k\n"
  "THEN m has a view\n"
  "THEN m's rows equals k's cols\n"
  "THEN m's cols equals k's rows\n"
  "THEN m's size equals k's size\n"
  "THEN compare exact m, data_in_T returns true\n"
  "THEN IF k is reshaped\n"
  "THRN IF s is a transpose view engine constructed with m"
  "THEN m's rows equals k's cols\n"
  "THEN m's cols equals k's rows\n"
  "THEN m's size equals k's size\n"
  "THEN compare exact k, s returns true\n"
  "THEN IF k_alt is literal2D constructed\n"
  "THEN IF m_alt is constructed with k_alt\n"
  "THEN IF s_alt is constructed with m_alt\n"
  "THEN IF s_alt and s are swapped\n"
  "THEN s_alt's rows equals k's cols AND m's rows equals k_alt's cols\n"
  "THEN s_alt's cols equals k's rows AND m's cols equals k_alt's rows\n"
  "THEN s_alt's size equals k's size AND m's size equals k_alt's size\n"
  "THEN compare exact k, s_alt returns true AND compare exact s, k_alt returns true.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<K, inport_views::transpose>;
    using S = engine_view<M, inport_views::transpose>;
    
    const literal2D<double> data_in  =
       {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
        {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
        {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
        {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
        {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
        {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};
    
    const literal2D<double> data_in_T =
       {{1.00, 1.06, 1.12, 1.18, 1.24, 1.30},
        {1.01, 1.07, 1.13, 1.19, 1.25, 1.31},
        {1.02, 1.08, 1.14, 1.20, 1.26, 1.32},
        {1.03, 1.09, 1.15, 1.21, 1.27, 1.33},
        {1.04, 1.10, 1.16, 1.22, 1.28, 1.34},
        {1.05, 1.11, 1.17, 1.23, 1.29, 1.35},
        {-1.00, -2.00, -3.00, -4.00, -5.00, -6.00},
        {-1.10, -2.20, -3.30, -4.40, -5.50, -6.60}};
    
    K k(data_in);
    M m(k);
    
    REQUIRE(m.rows() == k.cols());
    REQUIRE(m.cols() == k.rows());
    REQUIRE(m.size() == k.size());
    
    REQUIRE(eh::compare2D_exact(m, data_in_T));
    
    size_t new_nbr_rows = 1000;
    size_t new_row_reach = 2000;
    size_t new_nbr_cols = 800;
    size_t new_col_reach = 1000;
    
    k.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach);
    S s(m);
    
    REQUIRE(m.rows() == k.cols());
    REQUIRE(m.cols() == k.rows());
    REQUIRE(m.size() == k.size());
    
    REQUIRE(s.rows() == k.rows());
    REQUIRE(s.cols() == k.cols());
    REQUIRE(s.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(s, k));
    
    const literal2D<double> data_in_alt =
       {{e, pi, e},
        {-e, -pi, -e},
        {pi, pi, pi},
        {e, e, e},
        {-pi, -pi, -pi},
        {-e, -e, -e},
        {pi, e, pi},
        {-pi, -e, -pi}};
    
    K k_alt(data_in_alt);
    M m_alt(k_alt);
    S s_alt(m_alt);
    
    s.swap(s_alt);
    
    REQUIRE(s_alt.rows() == k.rows());
    REQUIRE(s_alt.cols() == k.cols());
    REQUIRE(s_alt.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(s_alt, k));
    
    REQUIRE(s.rows() == k_alt.rows());
    REQUIRE(s.cols() == k_alt.cols());
    REQUIRE(s.size() == k_alt.size());
    
    REQUIRE(true == eh::compare2D_exact(s, k_alt));
    
    s_alt.swap(s);
    
    REQUIRE(s.rows() == k.rows());
    REQUIRE(s.cols() == k.cols());
    REQUIRE(s.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(s, k));
    
    REQUIRE(s.rows() == k.rows());
    REQUIRE(s.cols() == k.cols());
    REQUIRE(s.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(s, k));
}


TEST_CASE
(
  "IF k and kT are matrix storage engines which are literal2D constructed\n"
  "THEN IF m is a transpose view engine and is constructed with k\n"
  "THEN m_ij == kT_ij AND m_i = k_i\n"
  "THEN if m set to NUM for all i and j\n"
  "THEN k is set to NUM for all i and j.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = 4;
    constexpr size_t ncols = 4;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<K, inport_views::transpose>;
    
    const literal2D<std::complex<double>> data_in =
       {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
        {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
        {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
        {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};
    
    const literal2D<std::complex<double>> data_in_T =
       {{1.0 + 0.5i, 3.0 + 2.5i, 5.0 + 4.5i, 7.0 + 6.5i},
        {1.5 + 1.0i, 3.5 + 3.0i, 5.5 + 5.0i, 7.5 + 7.0i},
        {2.0 + 1.5i, 4.0 + 3.5i, 6.0 + 5.5i, 8.0 + 7.5i},
        {2.5 + 2.0i, 4.5 + 4.0i, 6.5 + 6.0i, 8.5 + 8.0i}};
    

    K k(data_in);
    K kT(data_in_T);
    M m(k);
    
    size_t i, j;
    
    i = 0;
    for(; i < m.rows(); i++)
    {
        j = 0;
        for(; j < m.cols(); j++)
        {
            REQUIRE(m(i, j) == kT(i, j));
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