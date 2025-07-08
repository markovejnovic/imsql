#ifndef IMSQLITE_OS_PLATFORM_ERROR_HPP
#define IMSQLITE_OS_PLATFORM_ERROR_HPP
#pragma once

#include <string>
#include <stdexcept>

namespace imsql::os {

struct PlatformError : public std::runtime_error {
  explicit PlatformError(const std::string& message)
      : std::runtime_error(message) {}

  explicit PlatformError(const char* message)
      : std::runtime_error(message) {}
};

struct WindowInitializationError : public std::runtime_error {
  explicit WindowInitializationError(const std::string& message)
      : std::runtime_error(message) {}

  explicit WindowInitializationError(const char* message)
      : std::runtime_error(message) {}
};

} // namespace imsql::os

#endif // IMSQLITE_OS_PLATFORM_ERROR_HPP
