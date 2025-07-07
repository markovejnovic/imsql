//  Copyright (c) 2008-2023 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/coroutines/detail/coroutine_self.hpp>

namespace hpx::threads::coroutines::detail {

    coroutine_self*& coroutine_self::local_self() noexcept
    {
        static thread_local coroutine_self* local_self_ = nullptr;
        return local_self_;
    }
}    // namespace hpx::threads::coroutines::detail
