#pragma once
#include <concepts>
#include <compare>
namespace CustomConcepts{
    inline namespace latestModifyCode{
        template<typename T>
        concept copy_constructable_and_three_way_comparable=std::three_way_comparable<T>&&std::copy_constructible<T>;
    }
}