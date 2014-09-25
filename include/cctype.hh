#pragma once

#include <cctype>

namespace std {

constexpr bool isascii(int c) noexcept {
  return c & 0x7F;
}

} //namespace std
