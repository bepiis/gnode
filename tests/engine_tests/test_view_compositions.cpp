//
//  test_transpose_view_engine.cpp
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
