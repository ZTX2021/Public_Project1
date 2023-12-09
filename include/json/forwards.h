// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSON_FORWARDS_H_INCLUDED
#define JSON_FORWARDS_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "config.h"
#endif // if !defined(JSON_IS_AMALGAMATION)

namespace Json {

// writer.h
class StreamWriter;
class StreamWriterBuilder;
class Writer;

// reader.h
class CharReader;
class CharReaderBuilder;

// value.h
using ArrayIndex = unsigned int;
class StaticString;
class Value;
class ValueIteratorBase;
class ValueIterator;
class ValueConstIterator;

} // namespace Json

#endif // JSON_FORWARDS_H_INCLUDED
