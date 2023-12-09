#ifndef JSON_CONFIG_H_INCLUDED
#define JSON_CONFIG_H_INCLUDED
#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

// If non-zero, the library uses exceptions to report bad input instead of C
// assertion macros. The default is to use exceptions.
#ifndef JSON_USE_EXCEPTION
#define JSON_USE_EXCEPTION 1
#endif

/// If defined, indicates that the source file is amalgamated
/// to prevent private header inclusion.
/// Remarks: it is automatically defined in the generated amalgamated header.
// #define JSON_IS_AMALGAMATION

#if !defined(JSON_API)
#define JSON_API
#endif

// JSONCPP_OVERRIDE is maintained for backwards compatibility of external tools.
// C++11 should be used directly in JSONCPP.
#define JSONCPP_OVERRIDE override

#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ >= 6))
#define JSON_USE_INT64_DOUBLE_CONVERSION 1
#endif

namespace Json {
using Int = int;
using UInt = unsigned int;
// For Microsoft Visual use specific types as long long is not supported
using LargestInt = std::int64_t;
using LargestUInt = std::uint64_t;

using String = std::string;
using IStringStream = std::istringstream;
using OStringStream = std::ostringstream;
using IStream = std::istream;
using OStream = std::ostream;
} // namespace Json

#endif // JSON_CONFIG_H_INCLUDED
