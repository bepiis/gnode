//
//  test_transpose_view_engine.cpp
//  Created by Ben Westcott on 11/29/24.
//

TEST_CASE
(
    "initial concept"
)
{

    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using M = matrix_view_engine<K, matrix_view::transparent>;
    using N = matrix_view_engine<K, matrix_view::const_negation>;

    using S = view_combiner<K, matrix_view::transparent, 
                               matrix_view::const_negation, 
                               matrix_view::const_conjugate>;

    K k(5, 5, 5, 5);
    //typename S::type::type(k);

}
