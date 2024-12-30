//
//  view_alias.h
//  Created by Ben Westcott on 12/27/24.
//

template<typename X>
using ve_transparent = engine_view<export_views::transparent, X>;

template<typename X>
using vi_transparent = engine_view<inport_views::transparent, X>;

template<typename X>
using ve_negation = engine_view<export_views::negation, X>;

template<typename X>
using ve_conjugate = engine_view<export_views::conjugate, X>;

template<typename X>
using ve_transpose = engine_view<export_views::transpose, X>;

template<typename X>
using vi_transpose = engine_view<inport_views::transpose, X>;

template<typename X>
using ve_hermitian = engine_view<export_views::conjugate, engine_view<export_views::transpose, X>>;

template<typename X>
using ve_row = engine_view<export_views::row, X>;

template<typename X>
using vi_row = engine_view<inport_views::row, X>;

template<typename X>
using ve_col = engine_view<export_views::col, X>;

template<typename X>
using vi_col = engine_view<inport_views::col, X>;

template<typename X>
using ve_box = engine_view<export_views::box, X>;

template<typename X>
using vi_box = engine_view<inport_views::box, X>;

template<typename X, typename Y>
using ve_inner_prod = engine_view<product_views::inner, X, Y>;

template<typename X, typename Y>
using ve_outer_prod = engine_view<product_views::outer, X, Y>;

