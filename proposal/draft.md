Fast Ascii Character Manipulation
==========================================

* Document Number: NXXXX
* Date: 2014-08-20
* Programming Language C++, Library Working Group
* Reply-to: Matthew Fioravante <fmatthew5876@gmail.com>

The latest draft, reference header, and links to past discussions on github: 

* <https://github.com/fmatthew5876/stdcxx-ascii>

Introduction
=============================

This proposal adds a set of constexpr free functions for
checking and converting ascii characters.

This proposal is aimed for a C++ Technical Specification.

Impact on the standard
=============================

This proposal is a pure library extension.
It adds a new header `<ascii>` (alternatively reuse `<cctype>`) with
a new namespace `std::ascii`. It adds a set of `constexpr`
`noexcept` free functions which mirror `int isspace(int ch)`
functions in `<cctype>`. Finally, this paper also adds 2
character digit to int conversion functions to the `std::ascii` namespace.

Impact on Implementations
=============================

Portable implementations of these functions are trivial. A full
implementation is included in the reference header on github.

Motivation and Design
================

Computer software problems at their core are about transforming data.
In many domains such as Scientific Computing, Data Mining, and Finance we often
deal with large data files which use ascii delimeters. Ascii text data
files are useful because they are human readable but also reasonably fast
to parse into memory. A popular example file format is CSV files which can be exported from Microsoft Excel.

The C++ standard library currently lacks an efficient and portable
mechanism for checking and converting ascii characters. The standard
set of character routines in `<cctype>` such as `std::isspace()` are
not efficient becuase they have dynamic behavior at runtime based on
the current locale. While we can easily implement these methods
ourselves, it is tedious and error prone to do so.

Supporting dynamic locales requires dynamic dispatch *(function pointer / virtual)*
which is the most restrictive *(read: slowest)* form of branching with repect to the optimizer
and branch optimizing/predicting hardware.
Calling the equivalent of a virtual
function for every character is simply unacceptable for any high
performance parsing routine.

Most crucially, dynamic dispatch prohibits inlining.
On most platforms, a few comparisons is much cheaper than even a direct function call.
Inlining also allows for the possibility of automatic vectorization by an optimizer.
Finally, the `<cctype>` functions can never be `constexpr`.

We propose an additional set of character functions which mirror
the functions in `<cctype>` but are placed in the `std::ascii`
namespace. We also propose 2 additional useful functions
`todigit`, and `toxdigit`.

Technical Specification
====================

We will now describe the additions to `<experimental/ascii>` header.
Alternatively, all of these defintions could be added to the `<experimental/cctype>` header.

*[note-- All of these functions interpret the input character `c` as ascii, regardless of any global locale settings. --end-note]*

Character Checks
-----------------------

    constexpr bool std::ascii:isdigit(int c) noexcept;

**Return**: `c >= 48 && c <= 57`

    constexpr bool std::ascii:isxdigit(int c) noexcept;

**Return**: `std::ascii::isdigit(c) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102)`

    constexpr bool std::ascii:islower(int c) noexcept;

**Return**: `c >= 97 && c <= 122`

    constexpr bool std::ascii:isupper(int c) noexcept;

**Return**: `c >= 65 && c <= 90`

    constexpr bool std::ascii:isalpha(int c) noexcept;

**Return**: `std::ascii::islower(c) || std::ascii::isupper(c)`

    constexpr bool std::ascii:isalnum(int c) noexcept;

**Return**: `std::ascii::isalpha(c) || std::asci::isdigit(c)`

    constexpr bool std::ascii:ispunct(int c) noexcept;

**Return**: `(c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126)`

    constexpr bool std::ascii:isgraph(int c) noexcept;

**Return**: `(c >= 33 && c<= 126)`

    constexpr bool std::ascii:isblank(int c) noexcept;

**Return**: `(c == 9 || c == 32)`

    constexpr bool std::ascii:isspace(int c) noexcept;

**Return**: `(c >= 9 && c <= 13) || c == 32)`

    constexpr bool std::ascii:isprint(int c) noexcept;

**Return**: `(c >= 32 && c <= 126)`

    constexpr bool std::ascii:iscntrl(int c) noexcept;

**Return**: `(c >= 0 && c <= 31) || c == 127`

    constexpr bool std::ascii::isascii(int c) noexcept;

**Return**: `true` if `c >= 0 && c < 128`

###Notes about isascii()

POSIX defines `extern "C" int isascii(int c)` in the global namespace. As of POSIX.1-2008,
`isascii` has been marked obsolete because they claim that it *"cannot be used portably in a localized application."*
\[[Austin\_317](#Austin_317)\]. This function doesn't make sense if the current locale is not ascii compatible.
On the other hand, our version `std::ascii::isascii()` is well defined because it assumes the input character is ascii encoded.

###bool return type

We have decided to return `bool` instead of returning
`int` as is done by the `<cctype>` functions. Returning `bool` is more natural and also
makes these functions easier to use in generic code *(i.e. templates, overloading, predicate functors, generic algorithms, etc..)*.

Ascii Case Conversion
---------------------

    constexpr int std::ascii::tolower(int c) noexcept;

**Return**: `std::ascii::isupper(c) ? c - 32 : c`

    constexpr int std::ascii::toupper(int c) noexcept;

**Return**: `std::ascii::islower(c) ? c + 32 : c`

Ascii digit char to int conversion (new)
---------------------

**Shared Pre-Conditions**: The result is undefined unless `std::ascii::isdigit(c)`

    constexpr int std::ascii::todigit(int c) noexcept;

**Return**: `std::ascii::isdigit(c) ? c - 48 : /* undefined */`

    constexpr int todigit(int c, int m) noexcept;

**Return**: `std::ascii::todigit(c) * m`

Ascii hex digit char to int conversion (new)
---------------------

**Shared Pre-Conditions**: The result is undefined unless `std::ascii::isxdigit(c)`

    constexpr int std::ascii::toxdigit(int c) noexcept;

**Return**: `return (c >= 97 && c <= 102) ? return c - 97 : ((c >= 65 && c <= 70) ? c - 65 : todigit(c));`

    constexpr int std::ascii::toxdigit(int c, int m) noexcept;

**Return**: `std::ascii::toxdigit(c) * m`

Example Usage
=================

Parsing a date:

    auto ymds = "20140517"
    int y = todigit(ymds[0], 1000) + todigit(ymds[1], 100) + todigit(ymds[2], 10) + todigit(ymds[3]);
    int m = todigit(ymds[4], 10) + todigit(ymds[5]);
    int d = todigit(ymds[6], 10) + todigit(ymds[7]);

    assert(y == 2014);
    assert(m == 5);
    assert(d == 17);

Converting an ascii string to upper case:

    std::string s = "abcdefghi";
    std::transform(s.begin(), s.end(), std::ascii::toupper);

Parsing a hex string:

    auto hstr = "F04E";
    int h = toxdigit(hstr[0], 0x1000) + toxdigit(hstr[1], 0x100) + toxdigit(hstr[2], 0x10) + toxdigit(hstr[3]);
    assert(h == 0xF04E);
    

Use Cases
==============

* Date/Time string parsing
* Ascii text file parsers
* Csv file parsers
* Implementing user defined literals
* Faster character to int converstions
* Loading and parsing large text based datasets

Acknowledgments
====================

* Thank you to everyone one the std proposals forum.


References
==================
* <a name="Austin_317"></a>[Austin\_317] Minutes of September 2006 Meeting, The Open Group, Available online at <http://www.opengroup.org/austin/docs/austin_317.html>

<!--
* <a name="N3864"></a>[N3864] Fioravante, Matthew *N3864 - A constexpr bitwise operations library for C++*, Available online at <https://github.com/fmatthew5876/stdcxx-bitops>
* <a name="LXR"></a>[LXR] *Linux/include/linux/kernel.h* Available online at <http://lxr.free-electrons.com/source/include/linux/kernel.h#L50>
* <a name="IsoCpp"></a>[IsoCpp] *ISO C++ standard*
* <a name="clang"></a> *"clang" C Language Family Frontend for LLVM* Available online at <http://clang.llvm.org/>
-->
