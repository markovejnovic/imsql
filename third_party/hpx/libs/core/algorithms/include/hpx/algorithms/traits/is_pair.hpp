//  Copyright (c) 2023 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>

namespace hpx::traits {

    template <typename T>
    inline constexpr bool is_pair_v = false;

    template <typename T1, typename T2>
    inline constexpr bool is_pair_v<std::pair<T1, T2>> = true;
}    // namespace hpx::traits
