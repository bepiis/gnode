//
//  view_engine.h
//  Created by Ben Westcott on 11/23/24.
//

#pragma once

#ifndef ENGINE_SUPPORTED
    #error view engine must be supported... include engine.h instead of its constituient parts!
#endif

/*
 * A view engine must have the following at the minimum:
 *  requires readable_engine for const view types
 *  else requires writable engine for view types
 * {
 *  private type aliases (required for general engine):
 *      using self_type = ...
 *      using orient = matrix_orientation;
 *      using helper = engine_helper;
 *      // using mdhelper = ...     // not implemented yet
 * 
 *      // bool has_mdspan          // not implemented yet
 *  
 *  private type aliases (required for view engine): 
 *      using engine_pointer = ...
 * 
 *  public type aliases (required for general engine): 
 *      using orientation_type = ...
 *      using data_type = ...
 *      using index_type = ...
 *      using reference = ...
 *      using const_reference = ...
 *      // using mdspan_type = ...      // not implemented yet
 *      // using const_mdspan_type = ...    // not implemented yet
 * 
 *  public type aliases (required for view engine):
 *       using engine_type = ...
 *       using owning_engine_type = ...
 *       
 *  public methods (required for general engine):
 *      default destructor
 *      default copy, move constructors
 *      default copy, move assignment
 * 
 *      atleast one element access operator 
 *      (for mutable view engine, returns reference, immutable view engine returns const_reference)
 * 
 *      rows, row_reach, cols, col_reach, size and reach methods
 * 
 *      A swap specialization
 * 
 *  public methods (required for view engine):
 *      default constructor noexcept, initializes engine_ptr to nullptr
 *      engine_type constructor: intializes engine_ptr to address of supplied engine_type reference
 *      has_view: returns whether engine points to a valid engine
 * 
 *  private data (required for view engine):
 *      engine_ptr ptr;
 *      
 * }
 */

//template<typename Egn, typename Vw>
//struct matrix_view_engine; 

// valid view types
struct matrix_view
{
    struct transparent {};
    struct const_transparent {};
    
    struct const_negation {};
    struct const_conjugate {};
    struct const_hermitian {};

    struct banded {};
    struct const_banded {};

    struct transpose {};
    struct const_transpose {};

    struct row {};
    struct const_row {};
    struct col {};
    struct const_col {};

    struct block {};
    struct const_block {};
};

template<typename Egn, typename Vw>
requires
    readable_engine<Egn>
struct view_lookup : public std::false_type
{
    using type = matrix_view::transparent;

    using data_type = typename Egn::data_type;
    using index_type = typename Egn::index_type;
    using reference = typename Egn::reference;
    using const_reference = typename Egn::const_reference;
    using pointer_type = Egn*;
    using rhs_type = Egn &;
    
    using result_type = reference;

    static constexpr result_type eval(data_type & d)
    {
        return d;
    }
};

template<typename Egn>
requires 
    writable_engine<Egn>
struct view_lookup<Egn, matrix_view::transparent> : public std::true_type
{
    using type = matrix_view::transparent;

    using data_type = typename Egn::data_type;
    using index_type = typename Egn::index_type;
    using reference = typename Egn::reference;
    using const_reference = typename Egn::const_reference;
    using pointer_type = Egn*;
    using rhs_type = Egn &;

    using result_type = reference;

    static constexpr result_type eval(data_type & d)
    {
        return d;
    }
};

template<typename Egn>
requires
    readable_engine<Egn>
struct view_lookup<Egn, matrix_view::const_transparent> : public std::true_type
{
    using type = matrix_view::const_transparent;
    
    using data_type = typename Egn::data_type;
    using index_type = typename Egn::index_type;
    using reference = typename Egn::const_reference;
    using const_reference = typename Egn::const_reference;
    using pointer_type = Egn const*;
    using rhs_type = Egn const&;

    using result_type = const_reference;

    static constexpr result_type eval(data_type const& d)
    {
        return d;
    }
};

template<typename Egn>
requires
    readable_engine<Egn> and
    valid_unary_minus_operator<typename Egn::data_type>
struct view_lookup<Egn, matrix_view::const_negation> : public std::true_type
{
    using type = matrix_view::const_negation;
    
    using data_type = typename Egn::data_type;
    using index_type = typename Egn::index_type;
    using reference = typename Egn::data_type;
    using const_reference = typename Egn::data_type;
    using pointer_type = Egn const*;
    using rhs_type = Egn const&;

    using result_type = const_reference;

    static constexpr result_type eval(data_type const& d)
    {
        return -d;
    }
};

template<typename Egn>
requires
    readable_engine<Egn> and
    has_conjugate<typename Egn::data_type>
struct view_lookup<Egn, matrix_view::const_conjugate> : public std::true_type
{
    using type = matrix_view::const_conjugate;
    
    using data_type = typename Egn::data_type;
    using index_type = typename Egn::index_type;
    using reference = typename Egn::data_type;
    using const_reference = typename Egn::data_type;
    using pointer_type = Egn const*;
    using rhs_type = Egn const&;


    using result_type = const_reference;

    static constexpr result_type eval(data_type const& d)
    {
        return std::conj(d);
    }
};

template<typename Egn, typename Vw>
requires
    readable_engine<Egn>
struct matrix_view_engine
{
    
/* view engine private type alias requirements */
private:
    using view = view_lookup<Egn, Vw>;
    using engine_ptr = typename view::pointer_type;
    
/* view engine private data requirements */
private:
    engine_ptr m_eng_ptr;
    
/* engine private type alias requirements */
private:
    
    using self_type = matrix_view_engine<Egn, typename view::type>;
    using orient = matrix_orientation;
    using helper = engine_helper;
    // using mdspan_stuff = ...
    
    /* view engine specific type aliases */
//private:
    // ...
    
/* view engine public type alias requirements */
public:
    using owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;
    using engine_type = Egn;
    
/* engine public type alias requirements */
public:
    // engine_type is not required to have a valid orientation type to be readable, so it may
    // not have one defined, thus we have to preform type detection to extract its orientation type
    // correctly.
    using orientation_type = typename get_engine_orientation<Egn>::type;
    
    // Egn must have these by readable_engine requirement:
    using data_type = typename view::data_type;
    using index_type = typename view::index_type;
    using reference = typename view::reference;
    using const_reference = typename view::const_reference;
    // using mdspan_type = ...
    // using const_mdspan_type = ...

    using result_type = typename view::result_type;
    
/* view engine public method requirements */
public:
    
    constexpr matrix_view_engine() noexcept
    : m_eng_ptr(nullptr)
    {}
    
    explicit
    constexpr matrix_view_engine(typename view::rhs_type rhs)
    : m_eng_ptr(&rhs)
    {}
    
    constexpr bool has_view() const
    {
        return m_eng_ptr != nullptr;
    }
    
/* engine public method requirements */
public:
    
    /* rule of zero */
    
    // required for readable_engine (consistant_return_lengths):
    //      rows() -> index_type
    constexpr index_type rows() const noexcept
    {
        /*
         if constexpr(is_row_ct_v<engine_type>)
         {
            return engine_ct_extents<engine_type>::rows();
         }
         else
         {
            return m_eng_ptr->rows();
         }*/
        return m_eng_ptr->rows();
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      cols() -> index_type
    constexpr index_type cols() const noexcept
    {
        /*
         if constexpr(is_col_ct_v<engine_type> and engine_ct_extents<engine_type>::cols() != 0)
         {
            return engine_ct_extents<engine_type>::cols();
         }
         else
         {
            return m_eng_ptr->cols();
         }*/
        return m_eng_ptr->cols();
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
        /*
         if constexpr(is_size_ct_v<engine_type>)
         {
            return engine_ct_extents<engine_type>::size();
         }
         else
         {
            return m_eng_ptr->size();
         }*/
        return m_eng_ptr->size();
    }
    
    // required or readable_engine (immutable_access):
    //      (*this)(i, j) -> reference or const_reference or data_type
    constexpr result_type operator()(index_type i, index_type j) const
    {
        return view::eval((*m_eng_ptr)(i, j));
    }
    
    // required or readable_engine (immutable_access):
    //      (*this)(i) -> reference or const_reference or data_type
    constexpr result_type operator()(index_type i) const
    {
        return view::eval((*m_eng_ptr)(i));
    }
    
    constexpr void swap(matrix_view_engine & rhs) noexcept
    {
        helper::swap(*this, rhs);
    }
};





//#include "transparent_view_engine.h"
//#include "const_transparent_view_engine.h"
//#include "const_negation_view_engine.h"
