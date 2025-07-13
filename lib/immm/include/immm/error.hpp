#ifndef IMMM_ERROR_HPP
#define IMMM_ERROR_HPP
#pragma once

#include "immm/detail/pch.hpp"

namespace immm {

struct PlatformError : public std::runtime_error {
  explicit PlatformError(const std::string& message) : std::runtime_error(message) {}

  explicit PlatformError(const char* message) : std::runtime_error(message) {}
};

struct WindowInitializationError : public std::runtime_error {
  explicit WindowInitializationError(const std::string& message) : std::runtime_error(message) {}

  explicit WindowInitializationError(const char* message) : std::runtime_error(message) {}
};

}  // namespace immm

#endif  // IMMM_ERROR_HPP
