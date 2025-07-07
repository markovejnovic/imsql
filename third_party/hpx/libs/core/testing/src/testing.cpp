//  Copyright (c) 2020 ETH Zurich
//  Copyright (c) 2013-2022 Hartmut Kaiser
//  Copyright (c) 2017 Denis Blank
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define HPX_NO_VERSION_CHECK

#include <hpx/assert.hpp>
#include <hpx/modules/format.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/modules/util.hpp>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

namespace hpx::util {

    static test_failure_handler_type test_failure_handler;

    void set_test_failure_handler(test_failure_handler_type f)
    {
        test_failure_handler = HPX_MOVE(f);
    }

    namespace detail {

        std::atomic<std::size_t> sanity_failures_(0);
        std::atomic<std::size_t> test_failures_(0);

        void fixture::increment(counter_type c) noexcept
        {
            if (test_failure_handler)
            {
                test_failure_handler();
            }

            switch (c)
            {
            case counter_type::sanity:
                ++sanity_failures_;
                return;
            case counter_type::test:
                ++test_failures_;
                return;
            default:
                break;
            }
            HPX_ASSERT(false);
        }

        std::size_t fixture::get(counter_type c) noexcept
        {
            switch (c)
            {
            case counter_type::sanity:
                return sanity_failures_;
            case counter_type::test:
                return test_failures_;
            default:
                break;
            }
            HPX_ASSERT(false);
            return static_cast<std::size_t>(-1);
        }

        fixture& global_fixture() noexcept
        {
            static fixture fixture_(std::cerr);
            return fixture_;
        }
    }    // namespace detail

    ////////////////////////////////////////////////////////////////////////////
    int report_errors()
    {
        return report_errors(std::cerr);
    }

    int report_errors(std::ostream& stream)
    {
        std::size_t const sanity =
            detail::global_fixture().get(counter_type::sanity);
        std::size_t const test =
            detail::global_fixture().get(counter_type::test);
        if (sanity == 0 && test == 0)
            return 0;

        hpx::util::ios_flags_saver ifs(stream);
        stream << sanity << " sanity check"    //-V128
               << ((sanity == 1) ? " and " : "s and ") << test << " test"
               << ((test == 1) ? " failed." : "s failed.") << std::endl;
        return 1;
    }

    void print_cdash_timing(char const* name, double time)
    {
        // use format followed by single cout for better multi-threaded output
        std::string const temp =
            hpx::util::format("<DartMeasurement name=\"{}\" "
                              "type=\"numeric/double\">{}</DartMeasurement>",
                name, time);
        std::cout << temp << std::endl;
    }

    void print_cdash_timing(char const* name, std::uint64_t time)
    {
        print_cdash_timing(name, static_cast<double>(time) / 1e9);
    }
}    // namespace hpx::util
