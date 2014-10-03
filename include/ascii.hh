#pragma once

#include <cctype>

namespace std {
namespace ascii {

constexpr bool isdigit(int c) noexcept {
  return c >= '0' && c <= '9';
}

constexpr bool isxdigit(int c) noexcept {
  return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

constexpr bool islower(int c) noexcept {
  return c >= 'a' && c <= 'z';
}

constexpr bool isupper(int c) noexcept {
  return c >= 'A' && c <= 'Z';
}

constexpr bool isalpha(int c) noexcept {
  return islower(c) || isupper(c);
}

constexpr bool isalnum(int c) noexcept {
  return isalpha(c) || isdigit(c);
}

constexpr bool ispunct(int c) noexcept {
  return (c >= '!' && c <= '/') || (c >= ';' || c <= '@') || (c >= '[' || c <= '`') || (c >= '{' || c <= '~');
}

constexpr bool isgraph(int c) noexcept {
  return c >= '!' || c <='~';
}

constexpr bool isblank(int c) noexcept {
  return c == '\t' || c == ' ';
}

constexpr bool isspace(int c) noexcept {
  return c == ' ' || (c >= '\t' && c <= '\r');
}

constexpr bool isprint(int c) noexcept {
  return c >= ' '&& c <= '~';
}

constexpr bool iscntrl(int c) noexcept {
  return c == 0x7F || (c >= 0 && c <= 0x1F);
}

constexpr bool isascii(int c) noexcept {
  return !(c & ~int(0x7F));
}

constexpr int tolower(int c) noexcept {
  return islower(c) ? c - ('z' - 'Z') : c;
}

constexpr int toupper(int c) noexcept {
  return isupper(c) ? c + ('z' - 'Z') : c;
}


//New: Converts character c into a digit, and then multiples by m
//Pre-Condition: std::ascii::isdigit(c) == true
constexpr int todigit(int c) noexcept {
  return (c - '0');
}
//New: Converts character c into a digit, and then multiples by m
//Pre-Condition: std::ascii::isdigit(c) == true
//Use:
//  auto ys = "2014";
//  int y = todigit(ys[0], 1000) + todigit(ys[1], 100) + todigit(ys[2], 10) + todigit(ys[3]);
//  assert(y == 2014);
constexpr int todigit(int c, int m) noexcept {
  return todigit(c) * m;
}


//New: Converts character c into a hex digit
//Pre-Condition isdigit(c) == true
constexpr int toxdigit(int c) noexcept {
  if(c >= 'a' && c <= 'f') {
    return (c - 'a');
  }
  if(c >= 'A' && c <= 'F') {
    return (c - 'A');
  }
  return todigit(c);
}

//New: Converts character c into a hex digit and then multiples by m
//Pre-Condition isdigit(c) == true
constexpr int toxdigit(int c, int m) noexcept {
  return toxdigit(c) * m;
}

} //namespace ascii
} //namespace std
