//
//  custom_binary_common_type.h
//  Created by Ben Westcott on 12/18/24.
//

#pragma once

template<typename, typename>
struct patched_common_type : std::false_type
{};

template<typename T1, typename T2>
requires
    type_is_complex<T1> and
    type_is_complex<T2> and
    std::common_with<get_complex_type_t<T1>, get_complex_type_t<T2>>
struct patched_common_type<T1, T2> : std::true_type
{
    using type = std::complex<std::common_type_t<get_complex_type_t<T1>, get_complex_type_t<T2>>>;
};

template<typename T1, typename T2>
requires
    type_is_complex<T1> and
    (not type_is_complex<T2>) and
    std::common_with<get_complex_type_t<T1>, T2>
struct patched_common_type<T1, T2> : std::true_type
{
    using type = std::complex<std::common_type_t<get_complex_type_t<T1>, T2>>;
};

template<typename T1, typename T2>
requires
    (not type_is_complex<T1>) and
    type_is_complex<T2> and
    std::common_with<T1, get_complex_type_t<T2>>
struct patched_common_type<T1, T2> : std::true_type
{
    using type = std::complex<std::common_type_t<T1, get_complex_type_t<T2>>>;
};

template<typename T1, typename T2>
requires
    (not type_is_complex<T1>) and
    (not type_is_complex<T2>) and
    std::common_with<T1, T2>
struct patched_common_type<T1, T2> : std::true_type
{
    using type = std::common_type_t<T1, T2>;
};
