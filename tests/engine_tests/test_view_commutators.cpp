//
//  test_view_commutators.cpp
//  Created by Ben Westcott on 11/29/24.
//

TEST_CASE
(
    "Row views and col views count as having row/col vector dimensions\n"
    "even if the underlying storage engine does not\n"
)
{
    using dtype = double;
    using atype = std::allocator<double>;
    
    constexpr size_t nrows = 3;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using MTE = engine_view<M, export_views::transpose>;
    using MTI = engine_view<M, inport_views::transpose>;

    using RVE = engine_view<MTE, export_views::row>;
    REQUIRE(true == rowvec_dimensions<RVE>);

    using RVI = engine_view<MTI, inport_views::row>;    
    REQUIRE(true == rowvec_dimensions<RVI>);

    using RVET = engine_view<RVE, export_views::transpose>;
    REQUIRE(true == colvec_dimensions<RVET>);

    using RVIT = engine_view<RVI, inport_views::transpose>;    
    REQUIRE(true == colvec_dimensions<RVIT>);

    using CVE = engine_view<MTE, export_views::col>;
    REQUIRE(true == colvec_dimensions<CVE>);

    using CVI = engine_view<MTI, inport_views::col>;
    REQUIRE(true == colvec_dimensions<CVI>);

    using CVET = engine_view<CVE, export_views::transpose>;
    REQUIRE(true == rowvec_dimensions<CVET>);

    using CVIT = engine_view<CVI, inport_views::transpose>;
    REQUIRE(true == rowvec_dimensions<CVIT>);

    using RCVE = engine_view<engine_view<MTE, export_views::row>, export_views::col>;
    REQUIRE(true == rowvec_dimensions<RCVE>);
    REQUIRE(true == colvec_dimensions<RCVE>);

    using RCVI = engine_view<engine_view<MTI, inport_views::row>, inport_views::col>;
    REQUIRE(true == rowvec_dimensions<RCVI>);
    REQUIRE(true == colvec_dimensions<RCVI>);

}

/*
 *
 * View commutator tests
 * 
 */

TEST_CASE
(
    "IF M is a storage engine type\n" 
    "and K is a transparent view type\n"
    "then K commutes with transpose view.\n"
)
{
    using dtype = std::complex<double>;
    using atype = std::allocator<dtype>;
    
    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    using COM_TTR = basic_view_commutator<engine_view, export_views::transparent, 
                                     engine_view, export_views::transpose, M>;
    
    const literal2D<std::complex<double>> data_in =
        {{1.0 - 0.5i, 3.0 - 2.5i, 5.0 - 4.5i, 7.0 - 6.5i, 9.0 - 8.5i},
         {1.5 - 1.0i, 3.5 - 3.0i, 5.5 - 5.0i, 7.5 - 7.0i, 9.5 - 9.0i},
         {2.0 - 1.5i, 4.0 - 3.5i, 6.0 - 5.5i, 8.0 - 7.5i, 10.0 - 9.5i},
         {2.5 - 2.0i, 4.5 - 4.0i, 6.5 - 6.0i, 8.5 - 8.0i, 10.5 - 10.0i}};

    M m(data_in);

    auto com = COM_TTR::do_commute(m);

    std::cout << com.first << "\t" << com.second << "\n";

    using COM_TRCNJ = basic_view_commutator<engine_view, export_views::transpose,
                                            engine_view, export_views::conjugate, M>;

    com = COM_TRCNJ::do_commute(m);
    std::cout << com.first << "\t" << com.second << "\n";



}