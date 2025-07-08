#ifndef IMSQLITE_OS_OS_HPP
#define IMSQLITE_OS_OS_HPP
#pragma once

#include "platform/macos.hpp"

namespace imsql::os {

#ifdef __APPLE__
using Provider = MacOSProvider;
#else
#error "Unsupported platform. Please implement the necessary OS provider."
#endif // __APPLE__

} // namespace imsql::os

#endif // IMSQLITE_OS_OS_HPP
