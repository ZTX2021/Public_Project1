// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSON_READER_H_INCLUDED
#define JSON_READER_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "value.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <deque>
#include <iosfwd>
#include <istream>
#include <memory>
#include <stack>
#include <string>

#pragma pack(push, 8)

namespace Json {

/** Interface for reading JSON from a char array.
 */
class JSON_API CharReader {
public:
  virtual ~CharReader() = default;
  /** \brief Read a Value from a <a HREF="http://www.json.org">JSON</a>
   * document. The document must be a UTF-8 encoded string containing the
   * document to read.
   *
   * \param      beginDoc Pointer on the beginning of the UTF-8 encoded string
   *                      of the document to read.
   * \param      endDoc   Pointer on the end of the UTF-8 encoded string of the
   *                      document to read. Must be >= beginDoc.
   * \param[out] root     Contains the root value of the document if it was
   *                      successfully parsed.
   * \param[out] errs     Formatted error messages (if not NULL) a user
   *                      friendly string that lists errors in the parsed
   *                      document.
   * \return \c true if the document was successfully parsed, \c false if an
   * error occurred.
   */
  virtual bool parse(char const* beginDoc, char const* endDoc, Value& root,
                     String& errs) = 0;

  class JSON_API Factory {
  public:
    virtual ~Factory() = default;
    /** \brief Allocate a CharReader via operator new().
     * \throw std::exception if something goes wrong (e.g. invalid settings)
     */
    virtual std::unique_ptr<CharReader> newCharReader() const = 0;
  }; // Factory
};   // CharReader

/** \brief Build a CharReader implementation.
 *
 * Usage:
 *   \code
 *   using namespace Json;
 *   CharReaderBuilder builder;
 *   builder["collectComments"] = false;
 *   Value value;
 *   String errs;
 *   bool ok = parseFromStream(builder, std::cin, &value, errs);
 *   \endcode
 */
class JSON_API CharReaderBuilder : public CharReader::Factory {
public:
  // Note: We use a Json::Value so that we can add data-members to this class
  // without a major version bump.
  /** Configuration of this builder.
   * These are case-sensitive.
   * Available settings (case-sensitive):
   * - `"collectComments": false or true`
   *   - true to collect comment and allow writing them back during
   *     serialization, false to discard comments.  This parameter is ignored
   *     if allowComments is false.
   * - `"allowComments": false or true`
   *   - true if comments are allowed.
   * - `"allowTrailingCommas": false or true`
   *   - true if trailing commas in objects and arrays are allowed.
   * - `"strictRoot": false or true`
   *   - true if root must be either an array or an object value
   * - `"allowDroppedNullPlaceholders": false or true`
   *   - true if dropped null placeholders are allowed. (See
   *     StreamWriterBuilder.)
   * - `"allowNumericKeys": false or true`
   *   - true if numeric object keys are allowed.
   * - `"allowSingleQuotes": false or true`
   *   - true if '' are allowed for strings (both keys and values)
   * - `"stackLimit": integer`
   *   - Exceeding stackLimit (recursive depth of `readValue()`) will cause an
   *     exception.
   *   - This is a security issue (seg-faults caused by deeply nested JSON), so
   *     the default is low.
   * - `"failIfExtra": false or true`
   *   - If true, `parse()` returns false when extra non-whitespace trails the
   *     JSON value in the input string.
   * - `"rejectDupKeys": false or true`
   *   - If true, `parse()` returns false when a key is duplicated within an
   *     object.
   * - `"allowSpecialFloats": false or true`
   *   - If true, special float values (NaNs and infinities) are allowed and
   *     their values are lossfree restorable.
   * - `"skipBom": false or true`
   *   - If true, if the input starts with the Unicode byte order mark (BOM),
   *     it is skipped.
   *
   * You can examine 'settings_` yourself to see the defaults. You can also
   * write and read them just like any JSON Value.
   * \sa setDefaults()
   */
  Json::Value settings_;

  CharReaderBuilder();
  ~CharReaderBuilder() override;

  std::unique_ptr<CharReader> newCharReader() const override;

  /** \return true if 'settings' are legal and consistent;
   *   otherwise, indicate bad settings via 'invalid'.
   */
  bool validate(Json::Value& invalid) const;

  /** A simple way to update a specific setting.
   */
  Value& operator[](const String& key);

  /** Called by ctor, but you can use this to reset settings_.
   * \pre 'settings' != NULL (but Json::null is fine)
   * \remark Defaults:
   * \snippet src/lib_json/json_reader.cpp CharReaderBuilderDefaults
   */
  static void setDefaults(Json::Value& settings);
  /** Same as old Features::strictMode().
   * \pre 'settings' != NULL (but Json::null is fine)
   * \remark Defaults:
   * \snippet src/lib_json/json_reader.cpp CharReaderBuilderStrictMode
   */
  static void strictMode(Json::Value& settings);
};

/** Consume entire stream and use its begin/end.
 * Someday we might have a real StreamReader, but for now this
 * is convenient.
 */
bool JSON_API parseFromStream(CharReader::Factory const&, IStream&, Value& root,
                              String& errs);

/** \brief Read from 'sin' into 'root'.
 *
 * Always keep comments from the input JSON.
 *
 * This can be used to read a file into a particular sub-object.
 * For example:
 *   \code
 *   Json::Value root;
 *   cin >> root["dir"]["file"];
 *   cout << root;
 *   \endcode
 * Result:
 * \verbatim
 * {
 * "dir": {
 *    "file": {
 *    // The input stream JSON would be nested here.
 *    }
 * }
 * }
 * \endverbatim
 * \throw std::exception on parse error.
 * \see Json::operator<<()
 */
JSON_API IStream& operator>>(IStream&, Value&);

} // namespace Json

#pragma pack(pop)

#endif // JSON_READER_H_INCLUDED
