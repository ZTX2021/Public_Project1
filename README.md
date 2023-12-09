[![](https://coverage.cpp.studio/721415197/badge.svg)](https://coverage.cpp.studio/721415197/)

[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/flx__4Zr)
# Project 1: JSON

[![auto judge](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

<sup>This project is based on [jsoncpp](https://github.com/open-source-parsers/jsoncpp) [1.9.5](https://github.com/open-source-parsers/jsoncpp/releases/tag/1.9.5) licensed under [MIT](https://spdx.org/licenses/MIT.html).</sup>

## Tasks

### Mandatory tasks

- (100% off if not done) Add a coverage badge to [README.md](README.md). You can find the exact code in the _Grading_ step of the CI log.
- (20% off if not done) Code coverage $\ge92\\%$
- (10pts) Free-style extension of this library.
  - The score is judged manually by usefulness, difficulty, and implementation quality. For reference, any of the standard subtasks below is sufficient for getting full mark in this task, though they cannot be used in this task. You need to develop your own idea.
  - Fill in the blanks here in `README.md`:
  - Purpose: ______
  - Usage and examples: ______
  - Involved algorithms (if any): _____
  - How you test it: ______

### Standard Subtasks

You are free to choose any subtasks you like, but the final score is capped at 90 points. The subtasks are sorted by expected time consumption.

All standard subtasks have automated grading using [GitHub Actions](../../actions). You can find the result in the _Grading_ step of the CI log. The final score will be manually collected by TA in [PR#1](../../pull/1) so please don't think about cheating the grading program.

> [!IMPORTANT]
> For all subtasks marked with "※", you need to toggle the corresponding option in [CMakeLists.txt](CMakeLists.txt) to enable the grading program. Also, you might find some headers you need to implement in [`src/subtasks`](src/subtasks/).

- (10pts) What is the name of the convention of the class `StreamWriter::Factory` in [`include/json/writer.h`](include/json/writer.h)? (Search keyword hint: factory)
  - _Write you answer here in `README.md`_factory pattern
- (10pts) Clean up code style to comply with [`.clang-format`](.clang-format)
- (15pts)※ Support [user-defined conversion](https://en.cppreference.com/w/cpp/language/cast_operator) to primitive types.
- (15pts)※ Wrap a JSON value inside an object using operator `/`.
  - Example: If `data` is `{"time":100,"from":"Alice"}`, then `"message" / data` becomes `{"message":{"time":100,"from":"Alice"}}`.
- (15pts)※ Define JSON directly in C++ with [User-defined literals](https://en.cppreference.com/w/cpp/language/user_literal) and [String literal](https://en.cppreference.com/w/cpp/language/string_literal).
- (15pts)※ Convert from and to [CSV Files](https://datatracker.ietf.org/doc/html/rfc4180).
  - An implementation as simple as comma-separated values suffices.
  - Only consider array of arrays of strings.
- (20pts)※ Support [XPath](https://developer.mozilla.org/en-US/docs/Web/XPath)-like query to JSON.
    - Required path selector: `..` (parent node), `*` (all children), `name` (child with name `name`), array index (e.g. `[0]`).
    - Don't ignore spaces.
- (20pts) Code coverage $\ge94\\%$
- (30pts)※ Support string concatenation.
    - Example: `{"name"+"1": "San"+"Zhang"}` should be equivalent to `{"name1": "SanZhang"}`.
- (30pts)※ Add a new type `binary` to represent binary data.
  - You need to ensure that the output only consists of [printable ASCII characters](https://en.wikipedia.org/wiki/ASCII#Printable_characters) and newlines.

If you find it hard to follow, we have step-by-step video tutorials for 70 points, but your score will be capped at 80 points if you ask TA for the videos.

## Tutorial

### Software Engineering

> [!TIP]
> **Don't Panic!**[^hg2g]
This is a medium-sized project,
but you don't need to dive into all of the aspects of the project,
or the meaning of some fancy grammar.
You only read the interface you use, and focus on the task you are working on.
A good IDE helps you to navigate the code and find the relevant parts like declarations and definitions.

This project is a C++ library involving the following components:

- **Interface** [`include/json`](include/json/) contains headers defining _what_ this library does.
- **Implementation** [`src/lib_json`](src/lib_json/) defines _how_ this library works.
- **Unit Tests** [`src/unit_test`](src/unit_test/) tests each function to ensure that they work correctly.
- **Integration Tests** [`test`](test/) and [`src/intg_test`](src/intg_test/) tests the library as a whole to ensure that functions interoperate correctly.
- **Code Quality Checks** [`.clang-format`](.clang-format) ensures consistent code style, while the code coverage examines that we have enough tests.
- **Documentation** [`README.md`](README.md) and [`LICENSE`](LICENSE) contain instructions on how to use this library. The top-left corner of this README has a button to open the table of contents.
- **Project Management** Issues and pull requests (like [#1](../../pull/1)) are used to track the progress of coding tasks.
- **Continuous Integration** [`GitHub Actions`](.github/workflows/) automatically builds and tests the project on every commit.

When you are working on a large project,
it is important to keep the project organized.
The above components are common in most software projects.

[^hg2g]: From _The Hitchhiker's Guide to the Galaxy_ by Douglas Adams.

### JSON

> [!TIP]
> You can skip this section if you already know JSON.

[JSON](https://www.json.org/) is probably the most popular data-interchange format in the world.
To see why it is needed, let's imagine a scenario.

You are developing an instant message app, OurChat, which can send and receive messages.
Alice sends a message "Hello World!" to Bob.
The message sent by the program would look like:

```
Hello World!
```

Now Bob wants to know when the message was sent.
Alice can add a timestamp to the message:

```
Hello World! (sent at 2019-01-01 12:00:00)
```

A new user, Charlie, starts using OurChat.
If Charlie also sends a message to Bob,
Bob want to know who sent the message.
You would need to add the sender's name to the message:

```
Hello World! (sent at 2019-01-01 12:00:00) - Charlie
```

The message is getting longer and longer.
The more information is added to the message,
the harder it is to read.
We need to serialize the data into a string at the sender's side,
and deserialize the string into data at the receiver's side,
without losing any information or introducing any ambiguity.

This is where JSON comes in.
JSON is a simple formal language for representing data.
JSON libraries (which are available in almost every programming language) handle the serialization and deserialization of JSON text for you.
JSON defines six types of values:

- `null`: represents the absence of a value.
- `boolean`: Either `true` or `false`.
- `number`: A signed decimal number. Example: `2022`, `-1.5`, `1e9`.
- `string`: A sequence of characters. Example: `"Hello World!"`.
- `array`: An ordered sequence of values (of any type). Examples: `[1, 2, "Hello", null]`.
- `object`: An unordered collection of key-value pairs.
  The keys have to be strings, but the values can be of any type.
  Examples: `{"name": "Alice", "age": 20}`.

You might have noticed that JSON is not type-safe:
any value can appear in any place.
This is a natural consequence of the fact that JSON is a data-interchange format.
It is suitable for dynamic languages like JavaScript and python,
though it's a little bit tricky to use in static languages like C++.
In this project, we work on a library for JSON
to explore how to implement a type-safe dynamic library in C++.

### Running the Program

The core part of the library consists of a JSON parser
(see [`src/lib_json/json_reader.cpp`](src/lib_json/json_reader.cpp))
and a JSON serializer [`src/lib_json/json_writer.cpp`](src/lib_json/json_writer.cpp).
The JSON parser reads from a string (or a file) and generates a JSON value type in C++,
named `Value` in [`src/lib_json/json_reader.cpp`](src/lib_json/json_reader.cpp).
A few examples of using the reader and the parser are given in [`example/`](example/).

As always, you need a working C++ compiler and CMake installed on your system
to compile and run the source code.

We would like to clarify that this project is a *library*, but not an *executable*.
Therefore, there is no `main()` function in the core library,
and nothing for you to execute.
Instead, libraries have a bunch of [tests](test/), [example programs](example/), and benchmarks
(which this project does not contain).

### Unit Tests

The tests are defined in [`src/unit_test/main.cpp`](src/unit_test/main.cpp).
There is a CMake target `json_unit_test` that builds this test program.
You can build and run this test program to see if the unit test passes.

New test cases can be added similarly to the existing ones.

### Integration Tests

It is easy to run the integration tests. It is a CMake target `run_intg_test`.

<details>
<summary>Command Line</summary>

```bash
# Build the project if you haven't done so
mkdir build
cd build
cmake ..
cmake --build .

# Run the integration test
make run_intg_test
```

</details>

<details>
<summary>Visual Studio</summary>

* Open the "Solution Explorer"(解决方案资源管理器) window if it's not already opened: View -> Solution Explorer
* Click on the icon "Switch between solutions and available views"(切换视图)
    * See the [reference](https://learn.microsoft.com/zh-cn/visualstudio/ide/use-solution-explorer?view=vs-2022#solution-explorer-toolbar) for more details
* Choose "CMake Targets View"
* Right click on the target "run_intg_test" and choose "Build".

</details>

<details>
<summary>Visual Studio Code</summary>

You need to have the "CMake Tools" extension installed.

Then open the CMake tab on the left, navigate to `jsoncpp/src/intg_test/run_intg_test`, and click the "Build" button.

</details>

> [!TIP]
> The rest of this section discusses how to add new integration tests.
> They can be more complex to setup compared to unit tests.
> You can skip this if you are not interested, since code coverage can be achieved by unit tests.

Tests are stored in `test/data`.
To add a test, you need to create two files:

* a `TESTNAME.json` file, that contains the input document in JSON format.
* a `TESTNAME.expected` file, that contains a flattened representation of the
  input document.

The `TESTNAME.expected` file format is as follows:

* Each line represents a JSON element of the element tree represented by the
  input document.
* Each line has two parts: the path to access the element separated from the
  element value by `=`. Array and object values are always empty (i.e.
  represented by either `[]` or `{}`).
* Element path `.` represents the root element, and is used to separate object
  members. `[N]` is used to specify the value of an array element at index `N`.

See the examples `test_complex_01.json` and `test_complex_01.expected` to better understand element paths.

When a test is run, output files are generated beside the input test files. Below is a short description of the content of each file:

* `test_complex_01.json`: input JSON document.
* `test_complex_01.expected`: flattened JSON element tree used to check if parsing was correct.
* `test_complex_01.actual`: flattened JSON element tree produced by `json_intg_test` from reading `test_complex_01.json`.
* `test_complex_01.rewrite`: JSON document written by `json_intg_test` using the `Json::Value` parsed from `test_complex_01.json`.
* `test_complex_01.actual-rewrite`: flattened JSON element tree produced by reading `test_complex_01.rewrite`.
* `test_complex_01.process-output`: `json_intg_test` output, typically useful for understanding parsing errors.

To add a test for custom data types,
modify the `printValueTree()` function in `src/intg_test/main.cpp`.
It is used to serialize the values in JSON to `.actual` files.

### Adding a new file to the library

> [!TIP]
> This section might not be useful for this project,
> so you can skip it if you are not interested.

You should add it inside the [`src/lib_json`](src/lib_json/) directory.

Then edit [`CMakeLists.txt`](src/lib_json/CMakeLists.txt) under that library to include this file:

```cmake
set(JSONCPP_SOURCES
    json_tool.h
    json_reader.cpp
    json_valueiterator.inl
    json_value.cpp
    json_writer.cpp
    # Add your new file here
)
```

### Adding a new program that uses this library

> [!TIP]
> This section might not be useful for this project,
> so you can skip it if you are not interested.

To write a program that uses this JSON library,
the headers must be included:

```cpp
#include <json/json.h>
int main() {
    // Some json operations
}
```

and a new entry needs to be added to `CMakeLists.txt`:

```cmake
add_executable(my_executable path/to/source/code.cpp)
target_link_libraries(my_executable jsoncpp_static)
```

in order to compile and link it with the JSON library.
Now a new target has been added to the project,
and you can find it in the list of executables,
as what we have done in the previous assignments (`*_test`).


### The parser and writer

> [!TIP]
> This section might not be useful for this project,
> so you can skip it if you are not interested.

The parser in `src/lib_json/json_reader.cpp` is a recursive descent parser.
It starts from `OurReader::readValue()`, and scans the string for input.
The `readToken()` function fetches a new token from the input stream,
similar to `char c; cin >> c;`.
The difference is that tokens can be something longer than a single character,
such as the number token,
which reads in a whole number from the input stream.

Depending on the first character encountered,
we can distinguish which types of values we are going to encounter in the stream.
For example,
if `{` is found, we know that it's going to be an object;
if `"` is found, we know that it's going to be a string.
Then it calls `readObject()` or `readString()` to continue reading the whole value.

Something interesting about `readObject()` and `readArray()` is that
it's a recursive structure.
For each element in the array,
it calls `readValue()` recursively to read the value inside,
no matter what type it is.
It only stops when it encounters a `]`.

To be more specific about the implementation,
`readValue()` consumes a few characters from the input string,
and store the value in the `Value` object
which the stack top of `OurReader::node_` points to.
Therefore, to use `readValue()` functions:

```cpp
Value val;
nodes_.push(&val);
bool successful = readValue();
nodes_.pop();
if (successful) {
    // val is correctly parsed!
}
```

And to write a `read*()` function:
```cpp
bool readSomething() {
    Value val;
    if (*current_ == 'a') {
        // this is the current character
    }
    if (*(current + 1) == 'b') {
        // this is the next character
    }
    current += 1; // Only do this if you decide that (*current) is part of this value!
    // Do something more to get val
    currentValue().swapPayload(val);
}
```

Finally, add it to a branch in `Reader::readValue()` and `Reader::readToken()`.

Writers are much simpler.
Add a new case in `BuiltStyledStreamWriter::writeValue()`, write a corresponding function,
and you are set.

### More tutorials and Updates

They will be posted on [cpp.studio](https://cpp.studio/assignments/project-1/).
