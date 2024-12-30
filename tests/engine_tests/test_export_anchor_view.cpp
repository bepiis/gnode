//
//  test_export_anchor_view.cpp
//  Created by Ben Westcott on 12/29/24.
//

TEST_CASE
(
    "IF a col vec type is N anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::N>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a col vec type is S anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::S>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a col vec type is N anchored\n"
    "THEN it is an identity of both\n"
    "the wrapped 1D and 2D access operators.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::N>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(data_in.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};

    m = data_in_2;

    REQUIRE(data_in_2.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2));

}

TEST_CASE
(
    "IF a col vec type is S anchored\n"
    "THEN its elements are accessed in reverse\n"
    "relative to the wrapped 1D and 2D access operators.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const literal1D<double> data_in_rev = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};


    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::S>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(data_in.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_rev));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
    const literal1D<double> data_in_2_rev = {10.6, 10.5, 10.4, 10.3, 10.2, 10.1};

    m = data_in_2;

    REQUIRE(data_in_2.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2_rev));
}

TEST_CASE
(
    "IF a col vec type is NW anchored\n"
    "THEN it is the same as a N anchored colvec type\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::NW>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";
    REQUIRE(data_in.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in));
    
    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};

    m = data_in_2;

    REQUIRE(data_in_2.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2));
}

TEST_CASE
(
    "IF a col vec type is NE anchored\n"
    "THEN it is the same as a N anchored colvec type\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::NE>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";
    REQUIRE(data_in.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};

    m = data_in_2;

    REQUIRE(data_in_2.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2));
}

TEST_CASE
(
    "IF a col vec type is SW anchored\n"
    "THEN it is the same as a S anchored colvec type.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const literal1D<double> data_in_rev = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::SW>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(data_in.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_rev));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
    const literal1D<double> data_in_2_rev = {10.6, 10.5, 10.4, 10.3, 10.2, 10.1};

    m = data_in_2;

    REQUIRE(data_in_2.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2_rev));
}

TEST_CASE
(
    "IF a col vec type is SE anchored\n"
    "THEN it is the same as a S anchored colvec type.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const literal1D<double> data_in_rev = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::SE>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";
    
    REQUIRE(data_in.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_rev));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
    const literal1D<double> data_in_2_rev = {10.6, 10.5, 10.4, 10.3, 10.2, 10.1};

    m = data_in_2;

    REQUIRE(data_in_2.size() == av.rows());
    REQUIRE(1 == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2_rev));
}

TEST_CASE
(
    "IF a col vec type is W anchored\n"
    "THEN accessing elements throws a runtime error.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::W>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0), std::runtime_error);
    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);
}

TEST_CASE
(
    "IF a col vec type is E anchored\n"
    "THEN accessing elements throws a runtime error.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::E>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0), std::runtime_error);
    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);

}

TEST_CASE
(
    "IF a row vec type is E anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::E>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a row vec type is W anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::W>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a row vec type is W anchored\n"
    "THEN it is an identity of both\n"
    "the wrapped 1D and 2D access operators.\n"
)
{

    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::W>;

    AV av(m);

    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(1 == av.rows());
    REQUIRE(data_in.size() == av.cols());
    REQUIRE(1 * data_in.size() == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};

    m = data_in_2;

    REQUIRE(1 == av.rows());
    REQUIRE(data_in_2.size() == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2));

}

TEST_CASE
(
    "IF a row vec type is E anchored\n"
    "THEN its elements are accessed in reverse\n"
    "relative to the wrapped 1D and 2D access operators.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const literal1D<double> data_in_rev = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::E>;

    AV av(m);
    
    //eh::print(av);
    //std::cout << "\n";
    REQUIRE(1 == av.rows());
    REQUIRE(data_in.size() == av.cols());
    REQUIRE(1 * data_in.size() == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_rev));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
    const literal1D<double> data_in_2_rev = {10.6, 10.5, 10.4, 10.3, 10.2, 10.1};

    m = data_in_2;

    REQUIRE(1 == av.rows());
    REQUIRE(data_in_2.size() == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2_rev));
}

TEST_CASE
(
    "IF a row vec type is NW anchored\n"
    "THEN is is the same as a W anchored rowvec type\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::NW>;

    AV av(m);
    
    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(1 == av.rows());
    REQUIRE(data_in.size() == av.cols());
    REQUIRE(1 * data_in.size() == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};

    m = data_in_2;


    REQUIRE(1 == av.rows());
    REQUIRE(data_in_2.size() == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2));
}

TEST_CASE
(
    "IF a row vec type is SW anchored\n"
    "THEN is is the same as a W anchored rowvec type\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::SW>;

    AV av(m);
    
    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(1 == av.rows());
    REQUIRE(data_in.size() == av.cols());
    REQUIRE(1 * data_in.size() == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};

    m = data_in_2;

    REQUIRE(1 == av.rows());
    REQUIRE(data_in_2.size() == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2));
}

TEST_CASE
(
    "IF a row vec type is NE anchored\n"
    "THEN it is the same as a E anchored row vec type\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const literal1D<double> data_in_rev = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::NE>;

    AV av(m);
    
    //eh::print(av);
    //std::cout << "\n";

    REQUIRE(1 == av.rows());
    REQUIRE(data_in.size() == av.cols());
    REQUIRE(1 * data_in.size() == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_rev));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
    const literal1D<double> data_in_2_rev = {10.6, 10.5, 10.4, 10.3, 10.2, 10.1};

    m = data_in_2;

    REQUIRE(1 == av.rows());
    REQUIRE(data_in_2.size() == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2_rev));
}


TEST_CASE
(
    "IF a row vec type is SE anchored\n"
    "THEN it is the same as a E anchored row vec type\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const literal1D<double> data_in_rev = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::SE>;

    AV av(m);
    
    //eh::print(av);
    
    REQUIRE(1 == av.rows());
    REQUIRE(data_in.size() == av.cols());
    REQUIRE(1 * data_in.size() == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_rev));

    const literal1D<double> data_in_2 = {10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
    const literal1D<double> data_in_2_rev = {10.6, 10.5, 10.4, 10.3, 10.2, 10.1};

    m = data_in_2;

    REQUIRE(1 == av.rows());
    REQUIRE(data_in_2.size() == av.cols());
    REQUIRE(data_in_2.size() * 1 == av.size());

    REQUIRE(true == eh::compare1D_exact(av, data_in_2_rev));
}

TEST_CASE
(
    "IF a row vec type is N anchored\n"
    "THEN accessing elements throws a runtime error.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::N>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0), std::runtime_error);
    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);
}

TEST_CASE
(
    "IF a row vec type is S anchored\n"
    "THEN accessing elements throws a runtime error.\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal1D<double> data_in = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in);

    using AV = engine_view<export_views::anchor, M, anchors::S>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0), std::runtime_error);
    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);
}

TEST_CASE
(
    "IF a non vec type is NW anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::NW>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a non vec type is NE anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::NE>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a non vec type is SE anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::SE>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a non vec type is SW anchored\n"
    "THEN the anchor type satisfies:\n"
    "   - view_basics\n"
    "   - unary_view\n"
    "   - immutable_view\n"
    "   - exportable\n"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using AV = engine_view<export_views::anchor, M, anchors::SW>;

    REQUIRE(true == view_basics<AV>);
    REQUIRE(true == unary_view<AV>);
    REQUIRE(true == immutable_view<AV>);
    REQUIRE(true == exportable<AV>);
}

TEST_CASE
(
    "IF a non vec type is NW anchored\n"
    "THEN it is an identity of the\n"
    "wrapped 2D access operator\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal2D<dtype> data_in_nw = 
        {
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i}
        };

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in_nw);

    using AV = engine_view<export_views::anchor, M, anchors::NW>;

    AV av(m);

    REQUIRE(m.rows() == av.rows());
    REQUIRE(m.cols() == av.cols());
    REQUIRE(m.size() == av.size());

    //eh::print(av);
    //println();

    REQUIRE(true == eh::compare2D_exact(av, data_in_nw));
}

TEST_CASE
(
    "IF a non vec type is NE anchored\n"
    "THEN elements are accessed as if\n"
    "the NE anchor was in the NW anchor position.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal2D<dtype> data_in_nw = 
        {
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i}
        };

    const literal2D<dtype> data_in_ne = 
        {
            {4.00 + 5.00i, 3.00 + 4.00i, 2.00 + 3.00i, 1.00 + 2.00i},
            {4.50 + 5.50i, 3.50 + 4.50i, 2.50 + 3.50i, 1.50 + 2.50i},
            {5.00 + 6.00i, 4.00 + 5.00i, 3.00 + 4.00i, 2.00 + 3.00i},
            {5.50 + 6.50i, 4.50 + 5.50i, 3.50 + 4.50i, 2.50 + 3.50i},
            {6.00 + 7.00i, 5.00 + 6.00i, 4.00 + 5.00i, 3.00 + 4.00i},
            {6.50 + 7.50i, 5.50 + 6.50i, 4.50 + 5.50i, 3.50 + 4.50i},
            {7.00 + 8.00i, 6.00 + 7.00i, 5.00 + 6.00i, 4.00 + 5.00i},
            {7.50 + 8.50i, 6.50 + 7.50i, 5.50 + 6.50i, 4.50 + 5.50i},
            {8.00 + 9.00i, 7.00 + 8.00i, 6.00 + 7.00i, 5.00 + 6.00i}
        };

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in_nw);


    using AV = engine_view<export_views::anchor, M, anchors::NE>;

    AV av(m);

    REQUIRE(m.rows() == av.rows());
    REQUIRE(m.cols() == av.cols());
    REQUIRE(m.size() == av.size());

    //eh::print(av);
    //println();
    REQUIRE(true == eh::compare2D_exact(av, data_in_ne));
}

TEST_CASE
(
    "IF a non vec type is SW anchored\n"
    "THEN elements are accessed as if the\n"
    "SW anchor was in the NW anchor position.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal2D<dtype> data_in_nw = 
        {
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i}
        };

    const literal2D<dtype> data_in_sw = 
        {
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i}
        };

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in_nw);

    using AV = engine_view<export_views::anchor, M, anchors::SW>;

    AV av(m);

    REQUIRE(m.rows() == av.rows());
    REQUIRE(m.cols() == av.cols());
    REQUIRE(m.size() == av.size());

    //eh::print(av);
    //println();

    REQUIRE(true == eh::compare2D_exact(av, data_in_sw));

}

TEST_CASE
(
    "IF a non vec type is SE anchored\n"
    "THEN elements are accessed as if the\n"
    "SE anchor was in the NW anchor position.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal2D<dtype> data_in_nw = 
        {
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i}
        };

    const literal2D<dtype> data_in_se = 
        {
            {8.00 + 9.00i, 7.00 + 8.00i, 6.00 + 7.00i, 5.00 + 6.00i},        
            {7.50 + 8.50i, 6.50 + 7.50i, 5.50 + 6.50i, 4.50 + 5.50i},
            {7.00 + 8.00i, 6.00 + 7.00i, 5.00 + 6.00i, 4.00 + 5.00i},
            {6.50 + 7.50i, 5.50 + 6.50i, 4.50 + 5.50i, 3.50 + 4.50i},
            {6.00 + 7.00i, 5.00 + 6.00i, 4.00 + 5.00i, 3.00 + 4.00i},
            {5.50 + 6.50i, 4.50 + 5.50i, 3.50 + 4.50i, 2.50 + 3.50i},
            {5.00 + 6.00i, 4.00 + 5.00i, 3.00 + 4.00i, 2.00 + 3.00i},
            {4.50 + 5.50i, 3.50 + 4.50i, 2.50 + 3.50i, 1.50 + 2.50i},
            {4.00 + 5.00i, 3.00 + 4.00i, 2.00 + 3.00i, 1.00 + 2.00i}
        };

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in_nw);

    using AV = engine_view<export_views::anchor, M, anchors::SE>;

    AV av(m);

    REQUIRE(m.rows() == av.rows());
    REQUIRE(m.cols() == av.cols());
    REQUIRE(m.size() == av.size());

    //eh::print(av);
    //println();
    REQUIRE(true == eh::compare2D_exact(av, data_in_se));
}

/*
TEST_CASE
(
    "IF two nonvec types are have anchor views\n"
    "THEN swapping them swaps their anchors.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    const literal2D<dtype> data_in_nw = 
        {
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i}
        };

    const literal2D<dtype> data_in_se = 
        {
            {8.00 + 9.00i, 7.00 + 8.00i, 6.00 + 7.00i, 5.00 + 6.00i},        
            {7.50 + 8.50i, 6.50 + 7.50i, 5.50 + 6.50i, 4.50 + 5.50i},
            {7.00 + 8.00i, 6.00 + 7.00i, 5.00 + 6.00i, 4.00 + 5.00i},
            {6.50 + 7.50i, 5.50 + 6.50i, 4.50 + 5.50i, 3.50 + 4.50i},
            {6.00 + 7.00i, 5.00 + 6.00i, 4.00 + 5.00i, 3.00 + 4.00i},
            {5.50 + 6.50i, 4.50 + 5.50i, 3.50 + 4.50i, 2.50 + 3.50i},
            {5.00 + 6.00i, 4.00 + 5.00i, 3.00 + 4.00i, 2.00 + 3.00i},
            {4.50 + 5.50i, 3.50 + 4.50i, 2.50 + 3.50i, 1.50 + 2.50i},
            {4.00 + 5.00i, 3.00 + 4.00i, 2.00 + 3.00i, 1.00 + 2.00i}
        };

    const literal2D<dtype> data_in_sw = 
        {
            {5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i, 8.00 + 9.00i},
            {4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i, 7.50 + 8.50i},
            {4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i, 7.00 + 8.00i},
            {3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i, 6.50 + 7.50i},
            {3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i, 6.00 + 7.00i},
            {2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i, 5.50 + 6.50i},
            {2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i, 5.00 + 6.00i},
            {1.50 + 2.50i, 2.50 + 3.50i, 3.50 + 4.50i, 4.50 + 5.50i},
            {1.00 + 2.00i, 2.00 + 3.00i, 3.00 + 4.00i, 4.00 + 5.00i}
        };


    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(data_in_nw);

    using AVA = engine_view<export_views::anchor, M, anchors::SE>;
    using AVB = engine_view<export_views::anchor, M, anchors::SW>;

    AVA ava(m);
    AVB avb(m);

    REQUIRE(ava.rows() == m.rows());
    REQUIRE(ava.cols() == m.cols());
    REQUIRE(ava.size() == m.size());

    REQUIRE(true == eh::compare2D_exact(ava, data_in_se));

    REQUIRE(avb.rows() == m.rows());
    REQUIRE(avb.cols() == m.cols());
    REQUIRE(avb.size() == m.size());

    REQUIRE(true == eh::compare2D_exact(avb, data_in_sw));

    ava.swap(avb);

    REQUIRE(ava.rows() == m.rows());
    REQUIRE(ava.cols() == m.cols());
    REQUIRE(ava.size() == m.size());

    REQUIRE(true == eh::compare2D_exact(ava, data_in_sw));

    REQUIRE(avb.rows() == m.rows());
    REQUIRE(avb.cols() == m.cols());
    REQUIRE(avb.size() == m.size());

    REQUIRE(true == eh::compare2D_exact(avb, data_in_se));  

    avb.swap(ava);

    REQUIRE(ava.rows() == m.rows());
    REQUIRE(ava.cols() == m.cols());
    REQUIRE(ava.size() == m.size());

    REQUIRE(true == eh::compare2D_exact(ava, data_in_se));

    REQUIRE(avb.rows() == m.rows());
    REQUIRE(avb.cols() == m.cols());
    REQUIRE(avb.size() == m.size());

    REQUIRE(true == eh::compare2D_exact(avb, data_in_sw));      

}*/

TEST_CASE
(
    "IF a non vec type is N anchored\n"
    "THEN a runtime error is thrown.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(10, 20, 10, 20);

    using AV = engine_view<export_views::anchor, M, anchors::N>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);

}

TEST_CASE
(
    "IF a non vec type is S anchored\n"
    "THEN a runtime error is thrown.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(10, 20, 10, 20);

    using AV = engine_view<export_views::anchor, M, anchors::S>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);   
}

TEST_CASE
(
    "IF a non vec type is E anchored\n"
    "THEN a runtime error is thrown.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(10, 20, 10, 20);

    using AV = engine_view<export_views::anchor, M, anchors::E>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);      
}

TEST_CASE
(
    "IF a non vec type is W anchored\n"
    "THEN a runtime error is thrown.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    M m(10, 20, 10, 20);

    using AV = engine_view<export_views::anchor, M, anchors::W>;

    AV av(m);

    REQUIRE_THROWS_AS(av(0, 0), std::runtime_error);        
}



