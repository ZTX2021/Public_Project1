// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

/* This executable is used for testing parser/writer using real JSON files.
 */

#include <algorithm> // sort
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <json/json.h>
#include <memory>
#include <sstream>

struct Options {
  Json::String path;
  Json::Value features;
  bool parseOnly;
  using writeFuncType = Json::String (*)(Json::Value const&);
  writeFuncType write;
};

static Json::String normalizeFloatingPointStr(double value) {
  std::stringstream ss;
  ss << std::setprecision(16) << std::defaultfloat << value;
  Json::String s = ss.str();
  std::cout << "double:" << value << std::endl;
  std::cout << "s: " << s << std::endl;
  Json::String::size_type index = s.find_last_of("eE");
  if (index != Json::String::npos) {
    Json::String::size_type hasSign =
        (s[index + 1] == '+' || s[index + 1] == '-') ? 1 : 0;
    Json::String::size_type exponentStartIndex = index + 1 + hasSign;
    Json::String normalized = s.substr(0, exponentStartIndex);
    Json::String::size_type indexDigit =
        s.find_first_not_of('0', exponentStartIndex);
    Json::String exponent = "0";
    if (indexDigit != Json::String::npos) // There is an exponent different
                                          // from 0
    {
      exponent = s.substr(indexDigit);
    }
    return normalized + exponent;
  }
  return s;
}

static Json::String readInputTestFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (!file)
    return "";
  fseek(file, 0, SEEK_END);
  auto const size = ftell(file);
  auto const usize = static_cast<size_t>(size);
  fseek(file, 0, SEEK_SET);
  auto buffer = new char[usize + 1];
  buffer[size] = 0;
  Json::String text;
  if (fread(buffer, 1, usize, file) == usize)
    text = buffer;
  fclose(file);
  delete[] buffer;
  return text;
}

static void printValueTree(FILE* fout, Json::Value& value,
                           const Json::String& path = ".") {
  if (value.hasComment(Json::commentBefore)) {
    fprintf(fout, "%s\n", value.getComment(Json::commentBefore).c_str());
  }
  switch (value.type()) {
  case Json::nullValue:
    fprintf(fout, "%s=null\n", path.c_str());
    break;
  case Json::intValue:
    fprintf(fout, "%s=%s\n", path.c_str(),
            std::to_string(value.asLargestInt()).c_str());
    break;
  case Json::uintValue:
    fprintf(fout, "%s=%s\n", path.c_str(),
            std::to_string(value.asLargestUInt()).c_str());
    break;
  case Json::realValue:
    fprintf(fout, "%s=%s\n", path.c_str(),
            normalizeFloatingPointStr(value.asDouble()).c_str());
    break;
  case Json::stringValue:
    fprintf(fout, "%s=\"%s\"\n", path.c_str(), value.asString().c_str());
    break;
  case Json::booleanValue:
    fprintf(fout, "%s=%s\n", path.c_str(),
            Json::boolToString(value.asBool()).c_str());
    break;
  case Json::arrayValue: {
    fprintf(fout, "%s=[]\n", path.c_str());
    Json::ArrayIndex size = value.size();
    for (Json::ArrayIndex index = 0; index < size; ++index) {
      printValueTree(fout, value[index],
                     path + "[" + std::to_string(index) + "]");
    }
  } break;
  case Json::objectValue: {
    fprintf(fout, "%s={}\n", path.c_str());
    Json::Value::Members members(value.getMemberNames());
    std::sort(members.begin(), members.end());
    Json::String suffix = *(path.end() - 1) == '.' ? "" : ".";
    for (const auto& name : members) {
      printValueTree(fout, value[name], path + suffix + name);
    }
  } break;
  default:
    break;
  }

  if (value.hasComment(Json::commentAfter)) {
    fprintf(fout, "%s\n", value.getComment(Json::commentAfter).c_str());
  }
}

static int parseAndSaveValueTree(const Json::String& input,
                                 const Json::String& actual,
                                 const Json::String& kind,
                                 const Json::Value& features, bool parseOnly,
                                 Json::Value& root) {
  Json::CharReaderBuilder builder;
  builder.settings_ = features;

  std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
  Json::String errors;
  const bool parsingSuccessful =
      reader->parse(input.data(), input.data() + input.size(), root, errors);

  if (!parsingSuccessful) {
    std::cerr << "Failed to parse " << kind << " file: " << std::endl
              << errors << std::endl;
    return 1;
  }

  // We may instead check the legacy implementation (to ensure it doesn't
  // randomly get broken).

  if (!parseOnly) {
    FILE* factual = fopen(actual.c_str(), "wt");
    if (!factual) {
      std::cerr << "Failed to create '" << kind << "' actual file."
                << std::endl;
      return 2;
    }
    printValueTree(factual, root);
    fclose(factual);
  }
  return 0;
}
// static Json::String useFastWriter(Json::Value const& root) {
//   Json::FastWriter writer;
//   writer.enableYAMLCompatibility();
//   return writer.write(root);
// }
static Json::String useBuiltStyledStreamWriter(Json::Value const& root) {
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}
static int rewriteValueTree(const Json::String& rewritePath,
                            const Json::Value& root,
                            Options::writeFuncType write,
                            Json::String* rewrite) {
  *rewrite = write(root);
  FILE* fout = fopen(rewritePath.c_str(), "wt");
  if (!fout) {
    std::cerr << "Failed to create rewrite file: " << rewritePath << std::endl;
    return 2;
  }
  fprintf(fout, "%s\n", rewrite->c_str());
  fclose(fout);
  return 0;
}

static Json::String removeSuffix(const Json::String& path,
                                 const Json::String& extension) {
  if (extension.length() >= path.length())
    return Json::String("");
  Json::String suffix = path.substr(path.length() - extension.length());
  if (suffix != extension)
    return Json::String("");
  return path.substr(0, path.length() - extension.length());
}

static int printUsage(const char* argv[]) {
  std::cout << "Usage: " << argv[0] << " [--strict] input-json-file"
            << std::endl;
  return 3;
}

static int parseCommandLine(int argc, const char* argv[], Options* opts) {
  opts->parseOnly = false;
  opts->write = &useBuiltStyledStreamWriter;
  if (argc < 2) {
    return printUsage(argv);
  }
  Json::CharReaderBuilder::setDefaults(opts->features);
  int index = 1;
  if (Json::String(argv[index]) == "--json-checker") {
    Json::CharReaderBuilder::strictMode(opts->features);
    opts->parseOnly = true;
    ++index;
  }
  if (index == argc || index + 1 < argc) {
    return printUsage(argv);
  }
  opts->path = argv[index];
  return 0;
}

static int runTest(Options const& opts) {
  int exitCode = 0;

  Json::String input = readInputTestFile(opts.path.c_str());
  if (input.empty()) {
    std::cerr << "Invalid input file: " << opts.path << std::endl;
    return 3;
  }

  Json::String basePath = removeSuffix(opts.path, ".json");
  if (!opts.parseOnly && basePath.empty()) {
    std::cerr << "Bad input path '" << opts.path
              << "'. Must end with '.expected'" << std::endl;
    return 3;
  }

  Json::String const actualPath = basePath + ".actual";
  Json::String const rewritePath = basePath + ".rewrite";
  Json::String const rewriteActualPath = basePath + ".actual-rewrite";

  Json::Value root;
  exitCode = parseAndSaveValueTree(input, actualPath, "input", opts.features,
                                   opts.parseOnly, root);
  if (exitCode || opts.parseOnly) {
    return exitCode;
  }

  Json::String rewrite;
  exitCode = rewriteValueTree(rewritePath, root, opts.write, &rewrite);
  if (exitCode) {
    return exitCode;
  }

  Json::Value rewriteRoot;
  exitCode = parseAndSaveValueTree(rewrite, rewriteActualPath, "rewrite",
                                   opts.features, opts.parseOnly, rewriteRoot);

  return exitCode;
}

int main(int argc, const char* argv[]) {
  Options opts;
  try {
    int exitCode = parseCommandLine(argc, argv, &opts);
    if (exitCode != 0) {
      std::cerr << "Failed to parse command-line." << std::endl;
      return exitCode;
    }

    const int modern_return_code = runTest(opts);
    if (modern_return_code) {
      return modern_return_code;
    }

    const std::string filename =
        opts.path.substr(opts.path.find_last_of("\\/") + 1);
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception:" << std::endl << e.what() << std::endl;
    return 1;
  }
  return 0;
}
