//  Copyright (c) 2024 Tobias Wukovitsch
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Concerns the mismatch overload taking two ranges:
// If the two ranges are not of the same length the algorithm should still
// proceed as usual and not just return the first iterators of both ranges.

#include <hpx/algorithm.hpp>
#include <hpx/executors/execution_policy.hpp>
#include <hpx/init.hpp>
#include <hpx/modules/testing.hpp>

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

void mismatch_differently_size_ranges_test()
{
    std::vector<std::size_t> a{1, 2, 7, 4};
    std::vector<std::size_t> b{1, 2, 8, 4, 5, 6};

    auto result = hpx::mismatch(
        hpx::execution::par, a.begin(), a.end(), b.begin(), b.end());

    auto expected = std::mismatch(a.begin(), a.end(), b.begin(), b.end());

    HPX_TEST((a.begin() + 2) == result.first);
    HPX_TEST((b.begin() + 2) == result.second);
    HPX_TEST(result == expected);
}

int hpx_main()
{
    mismatch_differently_size_ranges_test();
    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> const cfg = {"hpx.os_threads=all"};

    hpx::local::init_params init_args;
    init_args.cfg = cfg;

    HPX_TEST_EQ_MSG(hpx::local::init(hpx_main, argc, argv, init_args), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}
