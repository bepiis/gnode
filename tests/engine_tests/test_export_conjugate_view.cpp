//
//  test_export_conjugate_view_engine.cpp
//  Created by Ben Westcott on 11/27/24.
//

TEST_CASE
(
  "IF M is a type const conjugate view engine\n"
  "THEN it has an owning engine type alias whose is_owning member is false\n"
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
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    
    REQUIRE(false == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(false == owning_engine<M>);
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
  "THEN it is a base engine\n"
  "THEN it is a readable engine\n"
  "THEN it is NOT a writable engine\n"
  "THEN it is a basic view engine\n"
  "THEN it is NOT a mutable view engine\n"
  "THEN it has a immutable view.\n"
)
{
    using dtype = std::complex<int64_t>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    
    REQUIRE(true == base_engine<M>);
    REQUIRE(true == readable_engine<M>);
    REQUIRE(false == writable_engine<M>);
    REQUIRE(true == view_basics<M>);
    REQUIRE(false == mutable_view<M>);
    REQUIRE(true == immutable_view<M>);
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
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    
    REQUIRE(true == std::is_same_v<M::data_type, K::data_type>);
    REQUIRE(true == std::is_same_v<M::index_type, K::index_type>);
    REQUIRE(true == std::is_same_v<M::reference, K::data_type>);
    REQUIRE(true == std::is_same_v<M::const_reference, K::data_type>);
    REQUIRE(true == std::is_same_v<M::orientation_type, K::orientation_type>);
    
    REQUIRE(true == std::is_same_v<M::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<M::engine_type, K>);
    
    using S = engine_view<export_views::conjugate, M>;
    
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
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    
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
    
    using ltype = matrix_orientation::col_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    
    M m;
    
    REQUIRE(false == m.has_view());
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN IF m is a const conjugate view engine and is constructed with k\n"
    "THEN m has a view\n"
    "THEN m's rows matches k's rows\n"
    "THEN m's cols matches k's cols\n"
    "THEN m's size matches k's size\n"
    "THEN compare exact m, and conj_data_in\n"
    "THEN IF k is reshaped\n"
    "THEN m's rows matches k's rows\n"
    "THEN m's cols matches k's cols\n"
    "THEN m's size matches k's size\n"
    "THEN IF s is a const conjugate view engine constructed from m\n"
    "THEN compare exact s, m returns true\n"
    "THEN IF k_alt is literal2D constructed\n"
    "THEN IF m_alt is constructed w/ k_alt\n"
    "THEN IF s_alt is constructed w/ m_alt\n"
    "THEN IF s_alt and s are swappd\n"
    "THEN s_alt's rows equals k's rows AND s's rows equals k_alt's rows\n"
    "THEN s_alt's cols equals k's cols AND s's cols equals k_alt's cols\n"
    "THEN s_alt's size equals k's size AND s's size equals k_alt's size\n"
    "THEN compare exact s, k_alt returns true, and compare exact s_alt, k returns true.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 4;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    using S = engine_view<export_views::conjugate, M>;

    const literal2D<std::complex<double>> data_in = 
              {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
               {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
               {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
               {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};

    const literal2D<std::complex<double>> conj_data_in = 
              {{1.0 - 0.5i, 1.5 - 1.0i, 2.0 - 1.5i, 2.5 - 2.0i},
               {3.0 - 2.5i, 3.5 - 3.0i, 4.0 - 3.5i, 4.5 - 4.0i},
               {5.0 - 4.5i, 5.5 - 5.0i, 6.0 - 5.5i, 6.5 - 6.0i},
               {7.0 - 6.5i, 7.5 - 7.0i, 8.0 - 7.5i, 8.5 - 8.0i}};
    
    using cx = std::complex<double>;
    const literal2D<cx> conj_data_in1 = 
              {{cx(1.0, -0.5), cx(1.5, -1.0), cx(2.0, -1.5), cx(2.5, -2.0)},
               {cx(3.0, -2.5), cx(3.5, -3.0), cx(4.0, -3.5), cx(4.5, -4.0)},
               {cx(5.0, -4.5), cx(5.5, -5.0), cx(6.0, -5.5), cx(6.5, -6.0)},
               {cx(7.0, -6.5), cx(7.5, -7.0), cx(8.0, -7.5), cx(8.5, -8.0)}};

    K k(data_in);
    M m(k);

    REQUIRE(true == m.has_view());

    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());

    REQUIRE(true == eh::compare2D_exact(m, conj_data_in));

    size_t new_nbr_cols = 25;
    size_t new_col_reach = 100;

    k.reshape_cols(new_nbr_cols, new_col_reach);

    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());

    S s(m);

    REQUIRE(true == eh::compare2D_exact(s, k));

    const literal2D<std::complex<double>> data_in_alt = 
              {{1.01i, 1.02i, 1.03i, 1.04i, 1.05i, 1.06i},
               {1.07i, 1.08i, 1.09i, 1.10i, 1.11i, 1.12i},
               {1.13i, 1.14i, 1.15i, 1.16i, 1.17i, 1.18i},
               {1.19i, 1.20i, 1.21i, 1.22i, 1.23i, 1.24i}};
    
    const literal2D<std::complex<double>> conj_data_in_alt = 
              {{-1.01i, -1.02i, -1.03i, -1.04i, -1.05i, -1.06i},
               {-1.07i, -1.08i, -1.09i, -1.10i, -1.11i, -1.12i},
               {-1.13i, -1.14i, -1.15i, -1.16i, -1.17i, -1.18i},
               {-1.19i, -1.20i, -1.21i, -1.22i, -1.23i, -1.24i}};

    K k_alt(data_in_alt);
    M m_alt(k_alt);
    S s_alt(m_alt);

    s.swap(s_alt);

    REQUIRE(s.rows() == k_alt.rows());
    REQUIRE(s.cols() == k_alt.cols());
    REQUIRE(s.size() == k_alt.size());

    REQUIRE(true == eh::compare2D_exact(s, k_alt));

    REQUIRE(s_alt.rows() == k.rows());
    REQUIRE(s_alt.cols() == k.cols());
    REQUIRE(s_alt.size() == k.size());

    REQUIRE(true == eh::compare2D_exact(s_alt, k));

    s_alt.swap(s);

    REQUIRE(s.rows() == k.rows());
    REQUIRE(s.cols() == k.cols());
    REQUIRE(s.size() == k.size());

    REQUIRE(true == eh::compare2D_exact(s, k));

    REQUIRE(s_alt.rows() == k_alt.rows());
    REQUIRE(s_alt.cols() == k_alt.cols());
    REQUIRE(s_alt.size() == k_alt.size());

    REQUIRE(true == eh::compare2D_exact(s_alt, k_alt));
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN IF m is a const conjugate view engine and is constructed with k\n"
    "THEN IF s is a const conjugate view engine constructed from m"
    "THEN s_ij == s_ij AND s_i = s_i\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = engine_view<export_views::conjugate, K>;
    using S = engine_view<export_views::conjugate, M>;

    const literal2D<std::complex<double>> data_in = 
              {{1.01i, 1.02i, 1.03i, 1.04i, 1.05i, 1.06i},
               {1.07i, 1.08i, 1.09i, 1.10i, 1.11i, 1.12i},
               {1.13i, 1.14i, 1.15i, 1.16i, 1.17i, 1.18i},
               {1.19i, 1.20i, 1.21i, 1.22i, 1.23i, 1.24i}};

    
    
    K k(data_in);
    M m(k);
    S s(m);
    
    size_t i, j;
    
    i = 0;
    for(; i < s.rows(); i++)
    {
        j = 0;
        for(; j < s.cols(); j++)
        {
            REQUIRE(s(i, j) == k(i, j));
        }
    }

    i = 0;
    for(; i < m.size(); i++)
    {
        REQUIRE(s(i) == s(i));
    }
}



