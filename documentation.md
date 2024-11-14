# 1 Introduction
Tachyon is a performant dynamic prototype-based programming language. It combines the expressiveness offered by dynamic prototype-based OOP with high performance and support for native multithreading, in order to be fit for tasks such as numerical computation.

# 2 Lexical Structure
## 2.1 Identifiers
Identifiers are case-sensitive and start with an alphabetic character (A-Z, a-z) or underscore followed by any number of alphanumeric characters (A-Z, a-z, 0-9) or underscore.

## 2.2 Keywords
The following keywords are reserved and are not permitted as identifiers:
```
null var const block if elif else while for return func afunc throw try catch include continue break
```

(it is woth noting that certain identifiers that collide with C++ keywords or C++ standard library functions in the global scope, such as `enum` or `fopen`, may also not work in the C++ transpiler implementation of Tachyon.)

## 2.3 Operators
The following operators are recognized as tokens:
```
+ - * / % ~ & | ^ << >> ! && || == !=  = += -= *= /= %= &= |= ^= <<= >>= ++ --
```

## 2.4 Other tokens
These are the remaining non-literal tokens:
```
( ) [ ] { } . , : ; <eof>
```

## 2.5 Literals
Tachyon supports number, string, vector and object literals.


```
13.5; // number literal
1.35e+1; // number literal (scientific E notation)
"string"; // string literal
[1, 2, 3]; // vector literal
{"a": 1, "b": 2}; // object literal
```

Number literals support the standard decimal notation and scientific E notation of the form `<mantissa>e<exponent>`.

String literals are delimited by quotation marks. Tachyon supports the following escape sequences for characters in string literals:

* `\'`- single quote (ASCII byte `0x27`)
* `\"`- double quote (ASCII byte `0x22`)
* `\?`- question mark (ASCII byte `0x3f`)
* `\\` - backslash (ASCII byte `0x5c`)
* `\a` - audible bell (ASCII byte `0x07`)
* `\b` - backslash (ASCII byte `0x5c`)
* `\f` - form feed / new page (ASCII byte `0x0c`)
* `\n` - line feed / new line (ASCII byte `0x0a`)
* `\r` - carriage return (ASCII byte `0x0d`)
* `\t` - horizontal tab (ASCII byte `0x09`)
* `\v` - vertical tab (ASCII byte `0x0b`)
* `\<1-3 octal digits>` - octal value
* `\x<hexadecimal digits>` - hexadecimal value


Vector literals are a comma-separated list of elements delimited by square brackets.

Object literals are a comma-separated list of key-value pairs delimited by square brackets, with the key and value being separated by a colon.

## 2.6 Whitespace
The characters of space, newline and horizontal tab (correesponding to bytes `0x20`, `0x0A`and `0x09` in ASCII respectively) are treated as whitespace and ignored by the transpiler.

## 2.7 Comments
Comments are two forward slashes (`//`) followed by any sequence of characters. A newline terminates a comment. Comments are ignored by the transpiler and are typically used to annotate code.

```
// comment
```

## 3 Values and Types
Tachyon is a dynamically-typed programming language so variables do not have types, only values do, and there are no type annotations. The eight basic types of value are as follows: number, string, vector, function, thread, file, object, and null. Unlike traditional prototype-based programming languages such as Self or Io, Tachyon does not make all values objects as this is a bottleneck on performance.

## 3.1 Number
A number represents an IEEE-754 double-precision floating-point number.

There is no distiction between integer and floating-point types to avoid expensive type-checking during arithmetic operations, and due to the internal "NaN-boxing" representation used in the C++ transpiler implementation. Thus, all numbers are floating-point.

Booleans are not a type, again to avoid expensive typechecking and type conversion. Tachyon simply uses the number 0 for false and the number 1 for true, as in C prior to C99.

```
var a = 0.123;
var b = 5e-1;
```


## 3.2 String
A string represents a mutable character sequence. Strings can have their individual characters accessed and set using the subscript expression.

```
var a = "Hello world":
a[0]; // "H"
```

## 3.3 Vector 
## 3.4 Function
## 3.5 Thread
## 3.6 File
## 3.7 Object
## 3.8 Null
# 4 Execution Context
## 4.1 Variables
# 5 Statements
# 6 Expressions
# 7 The Standard Library
## 7.1 Global Functions
## 7.2 The Math Object
## 7.3 The StringUtils Object
## 7.4 The VectorUtils Object
## 7.5 The ThisThread Object
## 7.6 The ThreadUtils Object
## 7.7 The FileUtils Object