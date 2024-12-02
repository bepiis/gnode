//
//  test_boxed_view_engine.cpp
//  Created by Ben Westcott on 12/1/24.
//

TEST_CASE
(
  "IF M is a box view engine\n"
  "THEN it has an owning engine type alias whose is_owning member is false\n"
  "THEN it is NOT an owning engine\n"
  "THEN it has mutable access\n"
  "THEN it has immutable access\n"
  "THEN it has a consistent mutable reference type\n"
  "THEN it has a consistent immutable reference type\n"
  "THEN it has consistent return sizes\n"
  "THEN it has consistent return lenghts\n"
  "THEN it is NOT reshapeable or row_reshapeable or col_reshapeable.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::box>;
    
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
  "IF M is a type box view engine\n"
  "THEN it is a base_engine\n"
  "THEN it is a readable engine\n"
  "THEN it is a writable engine\n"
  "THEN it is a valid mutable view engine.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    
    REQUIRE(true == writable_engine<K>);
    
    using M = matrix_view_engine<K, matrix_view::box>;
    
    REQUIRE(true == base_engine<M>);
    REQUIRE(true == readable_engine<M>);
    REQUIRE(true == writable_engine<M>);
    REQUIRE(true == mutable_view_engine<M>);
}

TEST_CASE
(
  "IF M is a box view engine AND K is its owning engine\n"
  "THEN M and K have the same base types\n"
  "THEN M's owning engine type is K\n"
  "THEN M's engine type is K.\n"
  "THEN IF S is a box view engine and M is its owning engine\n"
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
    using M = matrix_view_engine<K, matrix_view::box>;
    
    REQUIRE(true == std::is_same_v<M::data_type, K::data_type>);
    REQUIRE(true == std::is_same_v<M::index_type, K::index_type>);
    REQUIRE(true == std::is_same_v<M::reference, K::reference>);
    REQUIRE(true == std::is_same_v<M::const_reference, K::const_reference>);
    REQUIRE(true == std::is_same_v<M::orientation_type, K::orientation_type>);
    
    REQUIRE(true == std::is_same_v<M::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<M::engine_type, K>);
    
    using S = matrix_view_engine<M, matrix_view::box>;
    
    REQUIRE(true == std::is_same_v<S::owning_engine_type, K>);
    REQUIRE(true == std::is_same_v<S::engine_type, M>);
    REQUIRE(false == std::is_same_v<S::engine_type, K>);
}

TEST_CASE
(
  "IF M is a box view engine\n"
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
    using M = matrix_view_engine<K, matrix_view::box>;
    
    REQUIRE(true == std::is_trivially_copyable_v<M>);
    REQUIRE(true == std::is_trivially_copy_constructible_v<M>);
    REQUIRE(true == std::is_trivially_move_constructible_v<M>);
    REQUIRE(true == std::is_default_constructible_v<M>);
    REQUIRE(true == std::is_nothrow_default_constructible_v<M>);
    REQUIRE(false == std::is_trivially_default_constructible_v<M>);
    REQUIRE(true == std::constructible_from<M, K &, size_t, size_t, size_t, size_t>);
    REQUIRE(true == std::is_nothrow_swappable_v<M&>);
}

TEST_CASE
(
  "IF m is a box view engine\n"
  "THEN IF it is default constructed\n"
  "THEN m's engine ptr is nullptr.\n"
  "THEN 0 equals rows, cols and size.\n"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::box>;
    
    M m;
    
    REQUIRE(false == m.has_view());
    REQUIRE(0 == m.rows());
    REQUIRE(0 == m.cols());
    REQUIRE(0 == m.size());
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN if m is a box view engine constructed with k, and a box of size l * w = k's size\n"
    "THEN m's rows equals l equals k's rows\n"
    "THEN m's cols equals w equals k's cols\n"
    "THEN m's size equals l * w equals k's size\n"
    "THEN compare exact m, data_in_sub returns true\n"
    "THEN IF k is reshaped\n"
    "THEN m's rows equals l\n"
    "THEN m's cols equals w\n"
    "THEN m's size equals l * w\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::box>;
    
    const literal2D<double> data_in  =
       {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
        {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
        {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
        {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
        {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
        {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};
    
    size_t l = 6;
    size_t w = 8;
    
    K k(data_in);
    M m(k, 0, l, 0, w);
    
    REQUIRE(m.rows() == k.rows());
    REQUIRE(m.cols() == k.cols());
    REQUIRE(m.size() == k.size());
    
    REQUIRE(true == eh::compare2D_exact(m, k));
    
    size_t new_nbr_rows = 10;
    size_t new_nbr_cols = 10;
    
    k.reshape(new_nbr_rows, new_nbr_rows, new_nbr_cols, new_nbr_cols);
    
    REQUIRE(m.rows() == l);
    REQUIRE(m.cols() == w);
    REQUIRE(m.size() == l * w);
    
    REQUIRE(true == eh::compare2D_exact(m, data_in));
}

TEST_CASE
(
    "IF k is a matrix storage engine which is literal2D constructed\n"
    "THEN if m is a box view engine constructed with k, and a box of size l * w = k's size\n"
    "THEN m's rows equals l equals k's rows\n"
    "THEN m's cols equals w equals k's cols\n"
    "THEN m's size equals l * w equals k's size\n"
    "THEN compare exact m, data_in_sub returns true\n"
    "THEN IF two boxed views, my and mz are swapped\n"
    "THEN their dimensions and data are swapped.\n"
    "THEN IF all boxed views overwrite their view data with 1\n"
    "THEN the result is reflected in k\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;
    
    using ltype = matrix_orientation::row_major;
    
    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::box>;
    
    const literal2D<double> data_in  =
       {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
        {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
        {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
        {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
        {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
        {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};

    K k(data_in);
    
    const literal2D<double> data_in_sub_x =
       {{1.00, 1.01, 1.02, 1.03},
        {1.06, 1.07, 1.08, 1.09},
        {1.12, 1.13, 1.14, 1.15}};

    size_t x_rs = 0;
    size_t x_nr = 3;
    size_t x_cs = 0;
    size_t x_nc = 4;

    M mx(k, x_rs, x_nr, x_cs, x_nc);

    REQUIRE(mx.rows() == x_nr);
    REQUIRE(mx.cols() == x_nc);
    REQUIRE(mx.size() == x_nr * x_nc);

    REQUIRE(true == eh::compare2D_exact(mx, data_in_sub_x));

    const literal2D<double> data_in_sub_y =
       {{1.05, -1.00},
        {1.11, -2.00},
        {1.17, -3.00},
        {1.23, -4.00},
        {1.29, -5.00}};

    size_t y_rs = 0;
    size_t y_nr = 5;
    size_t y_cs = 5;
    size_t y_nc = 2;

    M my(k, y_rs, y_nr, y_cs, y_nc);

    REQUIRE(my.rows() == y_nr);
    REQUIRE(my.cols() == y_nc);
    REQUIRE(my.size() == y_nr * y_nc);

    REQUIRE(true == eh::compare2D_exact(my, data_in_sub_y));

    const literal2D<double> data_in_sub_z =
       {{1.15, 1.16, 1.17, -3.00},
        {1.21, 1.22, 1.23, -4.00},
        {1.27, 1.28, 1.29, -5.00}};

    size_t z_rs = 2;
    size_t z_nr = 3;
    size_t z_cs = 3;
    size_t z_nc = 4;

    M mz(k, z_rs, z_nr, z_cs, z_nc);

    REQUIRE(mz.rows() == z_nr);
    REQUIRE(mz.cols() == z_nc);
    REQUIRE(mz.size() == z_nr * z_nc);

    REQUIRE(true == eh::compare2D_exact(mz, data_in_sub_z)); 

    my.swap(mz);

    REQUIRE(mz.rows() == y_nr);
    REQUIRE(mz.cols() == y_nc);
    REQUIRE(mz.size() == y_nr * y_nc);

    REQUIRE(true == eh::compare2D_exact(mz, data_in_sub_y)); 

    REQUIRE(my.rows() == z_nr);
    REQUIRE(my.cols() == z_nc);
    REQUIRE(my.size() == z_nr * z_nc);

    REQUIRE(true == eh::compare2D_exact(my, data_in_sub_z));   

    const literal2D<double> data_in_modified  =
       {{1.00, 1.00, 1.00, 1.00, 1.04, 1.00, 1.00, -1.10},
        {1.00, 1.00, 1.00, 1.00, 1.10, 1.00, 1.00, -2.20},
        {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, -3.30},
        {1.18, 1.19, 1.20, 1.00, 1.00, 1.00, 1.00, -4.40},
        {1.24, 1.25, 1.26, 1.00, 1.00, 1.00, 1.00, -5.50},
        {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};   

    eh::fill_cols(mx, 0, mx.cols(), 1.00);
    eh::fill_cols(my, 0, my.cols(), 1.00);
    eh::fill_cols(mz, 0, mz.cols(), 1.00);

    REQUIRE(true == eh::compare2D_exact(k, data_in_modified));
}

