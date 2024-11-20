# Tachyon 2.0 Reference Manual

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
Tachyon supports number and string literals.


```
13.5; // number literal
1.35e+1; // number literal (scientific E notation)
"string"; // string literal
```

Number literals support the standard decimal notation and scientific E notation of the form `<mantissa>e<exponent>` with a value of  mantissa*10^exponent. An  optional +/- sign is allowed for the exponent. String literals are delimited by quotation marks. Tachyon supports the following escape sequences for characters in string literals:

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

## 2.6 Whitespace
The characters of space, newline and horizontal tab (corresponding to bytes `0x20`, `0x0A`and `0x09` in ASCII respectively) are treated as whitespace and ignored by the transpiler.

## 2.7 Comments
Comments are two forward slashes (`//`) followed by any sequence of characters. A newline terminates a comment. Comments are ignored by the transpiler and are typically used to annotate code.

```
// comment
```

# 3 Values and Types
Tachyon is a dynamically-typed programming language so variables do not have types, only values do, and there are no type annotations. The eight basic types of value are as follows: number, string, vector, function, thread, file stream, object, and null. Unlike traditional prototype-based programming languages such as Self or Io, Tachyon does not make all values objects as this is a bottleneck on performance. All-values are first-class values and can be stored in object members, variables, vectors and passed as function parameters. Numbers are allocated on the stack, while all other values are allocated on the heap.

## 3.1 Number
A number represents an IEEE-754 double-precision floating-point number. There is no distiction between integer and floating-point types to avoid expensive type-checking during arithmetic operations, and due to the internal "NaN-boxing" representation used in the C++ transpiler implementation. Booleans are not a type, again to avoid expensive typechecking and type conversion. Tachyon simply uses the number 0 for false and the number 1 for true, as in C prior to C99.

```
var a = 0.123;
var b = 5e-1;
```

## 3.2 String
A string represents a mutable character sequence. Strings can have their individual characters accessed and set using the subscript expression.
In the standard library, the `StringUtils` object provides functions to perform operations on strings.

```
var a = "Hello world";
```

## 3.3 Vector 
A vector represents a dynamic arrays of values. Vectors can have their individual characters accessed and set using the subscript expression. In the standard library, the `VectorUtils` object provides functions to perform operations on vectors. The elements of a vector are stored contiguously in memory and the vector is expanded as needed automatically. Reallocations are expensive in terms of performance so the `VectorUtils.reserve` function can be used to reduce reallocations if the number of elements is known beforehand.

```
var a = [1, 2, 3];
var b = [12, null];
```

## 3.4 Function
A function is a reusable piece of code that can be called, and can optionally take arguments and return values. Named functions can be declared with the function definition statement:
```
func add(x, y) {
    return x + y;
}
```

Anonymous functions can be declared with the anonymous function expression:
```
var add = afunc (x, y) {
    return x + y;
};
```

## 3.5 Thread
A thread represents a single thread of execution in a program. In the standard library, the `ThreadUtils` object provides functions to create threads and perform operations on them. Threads are created using the `ThreadUtils.makeThread` function.

```
var t = ThreadUtils.makeThread(afunc () {
    println("Hello world!");
});

```

## 3.6 File Stream
A file stream represents an input/output stream that operates on files. In the standard library, the `FileUtils` object provides functions to create file streams and perform operations on them. File streams are created using the `FileUtils.open`function which accepts a file path and file access mode, both of which are strings.


```
var f = FileUtils.open("fileName.txt", "w");
```

## 3.7 Object
An object is a dictionary that stores a collection of key-value pairs (called members). The key must be a string. Objects in Tachyon are versatile in that they can serve as dictionaries, namespaces and as a mechanism for prototype-based object-oriented programming. Objects are mutable so the members of an object can be modified, and new members can be added. Members can be retrieved both using the subscript expression (`obj["prop]"`) or, as syntactic sugar, the C-style dot syntax (`obj.prop`). The dot syntax does not support keys that are not identifiers.

Objects can optionally inherit from a prototype object, which  is stored in the `proto` member. If an object has a prototype, it will automatically inherit all the members of its prototype. 

```
var obj = {"x": 2, "y": 3};
var derivedObj = {"proto": obj, "y": 4};
println(derivedObj.x); // 2
println(derivedObj.y); // 4
println(derivedObj["y"]); // 4
```

# 4 Statements
## 4.1 Expression statement
The expression statement is an expression followed by a semicolon. The value of the expression is discarded aside from potential errors.
```
5;
```


## 4.2 Variable definition statement
The variable definiion statement is used to define mutable variables. Global variables exist from their definition to the end of execution. Local variables exist from their definition to the end of the block which they were defined in. 

```
var x = 5;
```


## 4.2 Constant definition statement
The constant definiion statement is used to define immutable constants. Global constants exist from their definition to the end of execution. Local constants exist from their definition to the end of the block which they were defined in. Trying to modify a constant after it was defined results in an error.

```
const x = 5;
```

## 4.3 Block statement
The block statement is a group of zero or more statements put inside their own scope. Unlike in most C-like languages, the block statement requires `block` keyword to disambiguate from object literals.
```
block {
    var x = 5;
}
```


## 4.3 If statement
The if statement executes the body if the condition is true. Optionally, an `else` clause can be added, which executes if the condition is false, and zero or more `elif` clauses, which execute if the condition is true and none of the above clauses were executed.
```
if(2 + 2 == 4) {
    println("2 + 2 is 4");
} elif(2 + 2 == 5) {
    println("2 + 2 is 5"); 
} else {
    println("2 + 2 is not 4");
}
```

## 4.4 While statement
The while statement repeatedly executes the body as long as the condition is true. `while(true)` can be used to create an infinite loop.
```
var x = 0;
while (x < 10) {
    x++;
}
```

## 4.5 For statement
The for statement repeatedly executes the body and the update expression as long as the condition is true. Additionally, the initialization statement (such as `var x = 0`in the example) is executed once before everything else.
```
var sum = 0;
for(var i = 0; i < 10; i++) {
    sum += i;
}
```


## 4.6 Continue statement
The continue statement jumps to the next iteration in a loop, skipping the following statements.

## 4.7 Break statement
The break statement unconditionally terminates execution of a loop.
```
var x = 0;
while (true) {
    x++;
    if(x >= 10) {
        break;
    }
}
```

## 4.8 Function definition statement
The function definition statement defines a new function.
```
func successor(x) {
    return x + 1;
}
```


## 4.9 Throw statement
Throws an error. Only strings can be thrown.
```
throw "sample error";
```

## 4.10 Try-catch statement
Executes the try clause if no errors are encountered, otherwise executes the catch clause. The error message as a string is also stored in a variable specified in the catch clause.

```
try  {
    throw "sample error";
} catch(e) {
    println(e); // prints "sample error"
}
```


## 4.11 Include statement
Includes the contents of another Tachyon program file.

```
include "filename.tachyon";
```


# 5 Expressions

## 5.1 Unary operators
Tachyon supports the unary operators `+` (unary plus), `-` (unary minus), `~` (bitwise NOT), and `!` (logical NOT). Unary plus has no effect. Unary NOT converts the numeric argument from float64 to int32 and back. Additionally there is the in-place operator `++`for incrementing a variable, vector element or object property by 1, and `--` for decrementing.

## 5.2 Binary operators
For binary operators, Tachyon supports:
* the arithmetic operators `+` (add), `-` (subtract), `*` (multiply),  `/` (divide), and `%`(modulo)
* the bitwise operators `&` (bitwise AND), `|`(bitwise OR), `^`(bitwise XOR), `<<` (left-shift) and `>>`(right-shift)–these convert the numeric arguments from float64 to int32 and back
* the logical operators `&&` (logical AND) and `|`(bitwise OR)-always returns 0 (false) or 1 (true)
* the equality operators `==`(equals) and `!=`(not equals)–for non-numeric values these compare by reference, and `StringUtils.eq`and `VectorUtils.eq`can be used for ocomparison of strings and vectors by content
* the relational operators `<` (less than), `<=` (less than or equal), `>` (greater than), and  `>=`(greater than or equal)
* the assignment operator `=`, which sets the value of a variable, string character, vector element or object property–the return value is the new value
* the compound assignment operators `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, and `|=`, which set the value of a variable, vector element or object property to the application of the operator with the current value and the second argument (for a concrete example, `x *= 2`is just a syntactic sugar for `x = x * 2`)–the return value is the new value

## 5.3 Vector expression
Creates a new vector. The syntax is a comma-separated list of the elements enclosed by square brackets.
```
[1, 2, 3]
[[3, "b"], [4, "g"]]
```

## 5.4 Object expression
Crates a new object. The syntax is a comma-separated list of key-value pairs enclosed by curly brackets, with the key and value separated from each other by a colon. Oject and vector expression syntax is almost always compatible with JSON, although no concerted effort to comply to the JSON standard was made.

```
{"position: {"x": 9, "y": 7, "z:" 12}}
```

## 5.5 Anonymous function expression
The anonymous function expression defines an anonymous function. It has similar syntax to the function definition statement.

```
afunc(x) {
    return x + 1;
}
```


## 5.6 Call expression
The call expression is used to call a function.

```
add(4, 5)
```

Calling an object member function will add the object as the automatic first argument (but only if it is actually done using the object member expression or subscript expression syntax). This is so that member functions can access the object with an initial "self" (or any other name) argument. Example:

```
var MyObject = {
    "getID": afunc(self) {
        return self.id();
    }
};
var x = {"proto": MyObject "id": 5};
println(x.getID()); // implicit first argument, makes it like x.getID(x) and the self argument will be x
```

## 5.7 Object member expression
Accesses a member of an object. This is a syntatic notation for the subscript notation, for example `a.b`is essentially a syntactic sugar for `a["b"]`.

```
Math.pow
```


## 5.8 Subscript expression
Accesses a character at the provided index from a string, an element at the provied index from a vector, or member at the provided key from an object.

```
vec[0];
obj["key"];
```

# 6 The Standard Library
## 6.1 Global Functions
`typeOf(val)`
Returns a type ID depending on the type of `val`:
* 0 if `val`is a number
* 1 if `val`is a string
* 2 if `val`is a vector
* 3 if `val`is a function
* 4 if `val`is a thread
* 5 if `val`is a file stream
* 6 if `val`is an object
* 7 if `val`is `null`

`hasMember(obj, member)`
Returns if object `obj` has a member with the name of `member`.

`getTimeMillis()`

Returns the number of milliseconds since the UNIX epoch (January 1, 1970 12:00 AM UTC).

`input()`

Reads a string from standard input.

`print(val)`

Prints a string representation of `val` (as according to `StringUtils.repr`) to standard output.

`println(val)`

Prints a string representation of `val` as according to (`StringUtils.repr`) and a newline to standard output.

`exitProgram(code)`

Exits the program with exict code number `code`.

## 6.2 The Math Object
The `Math` object includes member functions that compute common mathematical operations, as well as members defining common mathematical consontants.

`Math.abs(self, a)`

Returns the absolute value of `a`.

`Math.exp(self, a)`

Returns the base-e exponential function of `a`.

`Math.exp2(self, a)`

Returns the base-2 exponential function of `a`.

`Math.expm1(self, a)`
Returns the base-e exponential function of `a`, minus one (e^a - 1). This function is more precise than using the expression `Math.exp(a) - 1`if a is near zero.

`Math.log(self, a)`

Returns the natural logarithm of  `a`.

`Math.log10(self, a)`

Returns the base-10 logarithm or common logarithm of  `a`.

`Math.log2(self, a)`

Returns the base-2 logarithm of  `a`.

`Math.log1p(self, a)`

Returns the natural logarithm of  `a + 1` (log(a + 1)). This function is more precise than using the expression `Math.log(a + 1)`if a is near zero.

`Math.pow(self, a, b)`

Returns `a` raised to the power of `b`.

`Math.sqrt(self, a)`

Returns the square root of `a`.

`Math.cbrt(self, a)`

Returns the cube root of `a`.

`Math.hypot(self, a)`

Returns the square root of the sum of the squares of `a` and `b` (√(a^2 + b^2)).

`Math.cos(self, a)`

Returns the cosine of `a`.

`Math.sin(self, a)`

Returns the sine of `a`.

`Math.tan(self, a)`

Returns the tangent of `a`.

`Math.acos(self, a)`

Returns the inverse cosine of `a`.

`Math.asin(self, a)`

Returns the inverse sine of `a`.

`Math.atan(self, a)`

Returns the inverse tangent of `a`.

`Math.atan2(self, a, b)`

Returns the inverse tangent of `b/a`.

`Math.cosh(self, a)`

Returns the hyperbolic cosine of `a`.

`Math.sinh(self, a)`

Returns the hyperbolic sine of `a`.

`Math.cosh(self, a)`

Returns the hyperbolic cosine of `a`.

`Math.acosh(self, a)`

Returns the inverse hyperbolic cosine of `a`.

`Math.asinh(self, a)`

Returns the inverse hyperbolic sine of `a`.

`Math.atanh(self, a)`

Returns the inverse hyperbolic tangent of `a`.

`Math.erf(self, a)`

Returns the error function of `a`.

`Math.erfc(self, a)`

Returns the complementary error function of `a`.

`Math.gamma(self, a)`

Returns the gamma function of `a`. For integer values of `a`, this is equal to the factorial of `(a-1)`.


`Math.lgamma(self, a)`

Returns the natural logarithm of the gamma function of `a`.


`Math.ceil(self, a)`

Returns the ceiling function of `a`, or the smallest integer greater than `a`.


`Math.floor(self, a)`

Returns the floor function of `a`, or the largest integer less than `a`.


`Math.trunc(self, a)`

Returns the truncation of `a`, or the nearest integer that is less is magnitude than `a`.  Acts like ceiling for negative a and floor for positive a.


`Math.round(self, a)`

Returns the round function of `a`, or the nearest integer to `a`.  Halfway cases are rounded away from zero, so `Math.round(1.5)` is 2 and `Math.round(-1.5)`is -2.


`Math.isFinite(self, a)`

Returns 1 if `a` is finite and 0 if `a` is infinite.


`Math.isInf(self, a)`

Returns 1 if `a` is infinite and 0 if `a` is finite.

`Math.isNaN(self, a)`

Returns 1 if `a` is a floating-point NaN value or is non-numeric, and 0 otherwise.

`Math.random(self, a)`

Returns a pseudo-random number between 0 and 1.

`Math.INFINITY`

`Math.INFINITY` is the floating-point positive infinity value.


`Math.NAN`

`Math.NAN`is a floating-point quiet not-a-number (QNaN) value.

`Math.E`

`Math.E` is the closest possible approximation to e, the base of natural logarithms.


`Math.PI`

`Math.PI`is closest possible approximation to pi, the ratio of the circumference of a circle to its diameter.


## 6.3 The StringUtils Object
The `StringUtils` object includes member functions that implement string operations.

`StringUtils.front(self, str)`

Returns the first character in `str`.


`StringUtils.back(self, str)`

Returns the last character in `str`.


`StringUtils.empty(self, str)`

Returns if `str`is empty.


`StringUtils.size(self, str)`

Returns the number of characters in `str`.

`StringUtils.clear(self, str)`

Clears the contents of `str`, resulting in it becoming an empty string.


`StringUtils.insert(self, idx, str2)`

Inserts string `str2` at the index `idx` in `str`.


`StringUtils.erase(self, idx, count)`

Removes `count` (or `StringUtils.size(str) - idx` if it is less) characters from `str` starting at position `idx`.


`StringUtils.popBack(self, str)`

Removes the last character of `str`. If `str` is empty, the behavior is undefined.


`StringUtils.append(self, str, str2)`

Appends `str2`to `str`. 

`StringUtils.replace(self, idx, count, str)`

Replaces the characters in `str` from indices `idx` to `idx + count - 1` (or `StringUtils.size(str) - 1` if it is less) with `str2`.

`StringUtils.find(self, str, str2)`

Returns the index of the first occurrence of `str2`in `str`. If `str2`is not contained within `str`, returns `null`.

`StringUtils.rfind(self, str, str2)`

Returns the index of the last occurrence of `str2`in `str`. If `str2`is not contained within `str`, returns `null`.

`StringUtils.substr(self, str, idx, count)`

Returns a substring of `str` containing the characters at indices `idx` to `idx + count`.

`StringUtils.compare(self, str, str2)`

Compares `str` with `str2` lexicographically, returning -1 if `str`is less than `str2`, 0 if they are equal by content, and 1 if `str`is greater than `str2`.

`StringUtils.split(self, str, sep)`

Returns a vector that splits up `str`into substrings separated by `sep`. For example `StringUtils.split("1|2|3", "|")` returns `[1, 2, 3]`. Using the empty string as a  separator results in being split up into each individual character.

`StringUtils.eq(self, str, str2)`

Returns if `str` and `str2` are equal by content (`str == str2` compares by reference).

`StringUtils.concat(self, str, str2)`

Returns the concatenation of `str`and `str2`. Also see `VectorUtils.join`.

`StringUtils.repeat(self, str, n)`

Returns `str` repeated `n` times. For example, `StringUtils.repeat("abc", 3)` returns `"abcabcabc"`.

`StringUtils.repr(self, val)`

Returns a string representation of `val`:
* If `val`is a number, then returns a decimal or scientific notation representation of `val`.
* If `val`is an object and does not have a `toString` member function, then returns `"object at [address of val]"`.
* If `val`is an object and has a `toString` member function, then returns `val.toString()`.
* If `val`is a vector, then returns a string containing a representation of each element of `vector`, with each element separated by commas, and everything enclosed in square brackets.
* If `val`is a function, then returns `"function at [address of val]"`.
* If `val`is a thread, then returns `"thread at [address of val]"`.
* If `val`is a file stream, then returns `"file stream at [address of val]"`.

`StringUtils.toNumber(self, str)`

Converts a string representation of a number `str` to a number.

`StringUtils.copy(self, str)`

Returns a new copy of `str`.

`StringUtils.fromCodePoint(self, code)`

Converts the character code point `code` to a string.


`StringUtils.toCodePoint(self, str)`

Returns the code point of the first character in `str`.


## 6.4 The VectorUtils Object
The `VectorUtils` object includes member functions that implement vector operations.

`VectorUtils.size(self, vec)`

Returns the number of elements in `vec`.

`VectorUtils.front(self, vec)`

Returns the first element of `vec`.

`VectorUtils.back(self, vec)`

Returns the last element of `vec`.

`VectorUtils.pushBack(self, vec, elem)`

Appends the element `elem` to the end of `vec`.

`VectorUtils.popBack(self, vec)`

Removes the last character of `vec`. If `vec` is empty, the behavior is undefined.

`VectorUtils.insert(self, vec, idx, val)`

Inserts a copy of `val`before index `īdx` in `vec`.

`VectorUtils.subvec(self, vec, idx, count)`

Returns a subvector of `vec` with the elements corresponding to the indices `idx` to `idx.count-1`(or `vec.size()-1`if it is less).

`VectorUtils.join(self, vec, sep)`

Joins a vector of strings into one string with each element separated by `sep`. For example, `VectorUtils.join(["a","b","c"],"|")`is `"abc"`.

`VectorUtils.transform(self, vec, func)`

Returns a new vector which is populated with the result of applying `func` to each element of `vec`.

`VectorUtils.accumulate(self, vec, init, func)`

Eecutes the binary function `func` on the initial value and each element of the vector, in order, passing on the return value from the preceding calculation. Returns the final result after reaching the end of the vector. For example, if the vector was all-numbers and `func` added the two arguments, then this would result in the sum of all the elements in the vector and `init`.

`VectorUtils.eq(self, vec, vec2)`

Eecutes the binary function `func` on the initial value and each element of the vector, in order, passing on the return value from the preceding calculation. Returns the final result after reaching the end of the vector. For example, if the vector was all-numbers and `func` added the two arguments, then this would result in the sum of all the elements in the vector and `init`.

`VectorUtils.concat(self, vec, vec2)`

Returns the concatenation of `vec`and `vec2`.

`VectorUtils.repeat(self, vec, n)`

Returns `vec` repeated `n` times.

`VectorUtils.capacity(self, vec)`

Returns the number of elements that `vec` has currently allocated memory far.


`VectorUtils.reserve(self, vec, newCap)`

Increases the capacity of `vec` to a value greater than or equal to `newCap`. Does not change the vector's size.


## 6.5 The ThisThread Object
The `ThisThread` object includes member functions that access this thread.

`ThisThread.yield(self)`

Hints to the implementation to reschedule thread execution.

`ThisThread.getID(self)`

Returns the ID of the current thread as a string.

`ThisThread.sleepFor(self, ms)`

Makes the current thread sleep for at least the specified number of milliseconds.


`ThisThread.sleepUntil(self, sleepTime)`

Makes the current thread sleep until the specified time `sleepTime`, which should be specified as a UNIX timestamp in milliseconds, has been reached.

## 6.6 The ThreadUtils Object
The `ThreadUtils` object includes member functions that implement thread operations.

`ThreadUtils.makeThread(self, func)`

Creates a new thread. It will start executing immediately after being created (pending any OS scheduling delays), starting at the function `func`. The return value of `func`is ignored.

`ThreadUtils.joinable(self), thread`

Returns if the thread `thread` identifies an active thread of execution.

`ThreadUtils.getID(self, thread)`

Returns the ID of the thread `thread` as a string.

`ThreadUtils.hardwareConcurrecy(self)`

Returns the number of concurrent threads that the implementation supports. If the value isn't computable or well-defined, 

`ThreadUtils.join(self, thread)`

Blocks the current thread until the thread `thread` finishes executing.

`ThreadUtils.detach(self, thread)`

Detaches the actual OS thread from the thread `thread`, allowing it to continue execution independently. Once detached, the OS thread will run in the background, and `thread` becomes non-joinable, meaning you cannot join it later.

## 6.7 The FileUtils Object 
The `FileUtils` object includes member functions that implement file input and output operations.

`FileUtils.open(self, filename, mode)`

Opens a file with the path `filename` using the mode `mode` and returns an associated file stream for performing file input and output operations.

The file access flags used in the `mode` argument are the following:
* `"r":` Open for reading. The file must exist.
* `"w"`: Open for writing. Creates a new file or truncates an existing file to zero length.
* `"a"`: Open for appending. Creates a new file if it doesn't exist.
* `"r+"`: Open for reading and writing. The file must exist.
* `"w+"`: Open for reading and writing. Creates a new file or truncates an existing file.
* `"a+"`: Open for reading and appending. Creates a new file if it doesn't exist.
* `"b`": Can optionally be specified to open in binary mode.


`FileUtils.reopen(self, filename, mode, stream)`

Closes the file currently associated with `stream`, then opens the file with path `filename` using `mode` and associates this file to `stream`. 

`FileUtils.close(self, stream)`

Closes the file stream `stream`.


`FileUtils.flush(self, stream)`

Used to flush the output buffer of the file stream `stream`, ensuring that all buffered data is written to the underlying file or device.

If `stream` is an input stream or an update streams on which input was the last operation, the behavior is undefined.


`FileUtils.getc(self, stream)`

Reads the next character from the file stream `stream`.

`FileUtils.gets(self, stream, count)`

Reads at most `count - 1` character sfrom the file stream `stream`, returning them all as a string.
