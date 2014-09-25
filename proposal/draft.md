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
the current locale.

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

Return type for ascii checks
--------------------

For the `isspace()` class of functions, we have decided to return `bool` instead of returning
`int` as is done by the `<cctype>` functions. Returning `bool` is more natural and also
makes these functions easier to use in generic code *(i.e. templates, overloading, predicate functors, generic algorithms, etc..)*.
While in some cases, returning and directly testing an `int` for `!= 0` may be faster
(e.g. `if(ch & char(0x7F)) { /* inlined isascii(ch) */ }`)
all of these funtions are inline so the optimizer is free to remove the `int` to `bool` conversion
where it makes sense to do so.

Technical Specification
====================

We will now describe the contents of the `<experimental/ascii>` header.
Alternatively, these defintions could be added to the `<experimental/cctype>` header.

*[note-- All of these functions interpret the input character `c` as ascii, regardless of any global locale settings. --end-note]*

Character Checks
-----------------------

**Shared Pre-Condition**: the result is undefined if the value of `c` is not representable as `unsigned char` and is not equal to `EOF`.

    constexpr bool std::ascii:isdigit(int c) noexcept;

**Return**: `true` if `std::isdigit(c) == true` in the default C locale

    constexpr bool std::ascii:isxdigit(int c) noexcept;

**Return**: `true` if `std::isxdigit(c) == true` in the default C locale

    constexpr bool std::ascii:islower(int c) noexcept;

**Return**: `true` if `std::islower(c) == true` in the default C locale

    constexpr bool std::ascii:isupper(int c) noexcept;

**Return**: `true` if `std::isupper(c) == true` in the default C locale

    constexpr bool std::ascii:isalpha(int c) noexcept;

**Return**: `true` if `std::isalpha(c) == true` in the default C locale

    constexpr bool std::ascii:isalnum(int c) noexcept;

**Return**: `true` if `std::isalnum(c) == true` in the default C locale

    constexpr bool std::ascii:ispunct(int c) noexcept;

**Return**: `true` if `std::ispunct(c) == true` in the default C locale

    constexpr bool std::ascii:isgraph(int c) noexcept;

**Return**: `true` if `std::isgraph(c) == true` in the default C locale

    constexpr bool std::ascii:isblank(int c) noexcept;

**Return**: `true` if `std::isblank(c) == true` in the default C locale

    constexpr bool std::ascii:isspace(int c) noexcept;

**Return**: `true` if `std::isspace(c) == true` in the default C locale

    constexpr bool std::ascii:isprint(int c) noexcept;

**Return**: `true` if `std::isprint(c) == true` in the default C locale

    constexpr bool std::ascii:iscntrl(int c) noexcept;

**Return**: `true` if `std::iscntrl(c) == true` in the default C locale

Ascii Case Conversion
---------------------

**Shared Pre-Condition**: the result is undefined if the value of `c` is not representable as `unsigned char` and is not equal to `EOF`.

    constexpr int std::ascii::tolower(int c) noexcept;

**Return**: returns `std::tolower(c)` in the default C locale

    constexpr int std::ascii::toupper(int c) noexcept;

**Return**: returns `std::toupper(c)` in the default C locale

Ascii Character Check (Inspired by POSIX)
---------------------

*[note-- This function is defined on all integer values. --end-node]*

    constexpr bool std::isascii(int c) noexcept;

**Return**: `true` if `c >= 0 && c < 128`

###Compatibility with POSIX

POSIX defines `extern "C" int isascii(int c)` in the global namespace. As of POSIX.1-2008,
`isascii` has been marked obsolete because they claim that it *"cannot be used portably in a localized application."*

`bool std::isascii(int c)` is portable because it only checks if
`c` is part of the basic ascii character set *(i.e. from 0 to 127, inclusive)* and
does not supported any extended ascii character sets.
Users may need to take care to differentiate between
`extern "C" int isascii(int c)` and `bool std::isascii(int c)`.

Ascii digit char to int conversion (new)
---------------------

**Shared Pre-Condition**: The result is undefined unless `std::ascii::isdigit(c) == true`

    constexpr int std::ascii::todigit(int c) noexcept;

**Return**: converts `c` from an ascii character digit to an int representation of that digit

**Example Implementation**: `return (c - '0');`

    constexpr int todigit(int c, int m) noexcept;

**Return**: `std::ascii::todigit(c) * m`

Ascii hex digit char to int conversion (new)
---------------------

**Shared Pre-Condition**: The result is undefined unless `std::ascii::isxdigit(c) == true`

    constexpr int std::ascii::toxdigit(int c) noexcept;

**Return**: converts `c` from an ascii hex character digit to an int representation of that digit

**Example Implementation**: `return (c>='a' && c<='f') ? return c-'a' : ((c>='A' && c<='F') ? c-'A' : todigit(c));`

    constexpr int std::ascii::toxdigit(int c, int m) noexcept;

**Return**: `std::ascii::toxdigit(c) * m`

Example Usage
=================

    auto ymds = "20140517"
    int y = todigit(ymds[0], 1000) + todigit(ymds[1], 100) + todigit(ymds[2], 10) + todigit(ymds[3]);
    int m = todigit(ymds[4], 10) + todigit(ymds[5]);
    int d = todigit(ymds[6], 10) + todigit(ymds[7]);

    assert(y == 2014);
    assert(m == 5);
    assert(d == 17);

<!-- -->

    std::string s = "abcdefghi";
    std::transform(s.begin(), s.end(), std::ascii::toupper);

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


<!--
References
==================

* <a name="N3864"></a>[N3864] Fioravante, Matthew *N3864 - A constexpr bitwise operations library for C++*, Available online at <https://github.com/fmatthew5876/stdcxx-bitops>
* <a name="LXR"></a>[LXR] *Linux/include/linux/kernel.h* Available online at <http://lxr.free-electrons.com/source/include/linux/kernel.h#L50>
* <a name="IsoCpp"></a>[IsoCpp] *ISO C++ standard*
* <a name="clang"></a> *"clang" C Language Family Frontend for LLVM* Available online at <http://clang.llvm.org/>
-->
