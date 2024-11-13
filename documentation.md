# 1 Introduction
Tachyon is a dynamic prototype-based programming language. It combines the expressiveness offered by the dynamic prototype-based programming with high performance and support for native threads, in order to be fit for tasks such as numerical computation.

# 2 Lexical Structure
## 2.1 Identifiers
Identifiers are case-sensitive and start with an alphabetic character (A-Z, a-z) or underscore followed by any number of alphanumeric characters (A-Z, a-z, 0-9) or underscore. 

## 2.2 Keywords
The following keywords are reserved and are not permitted as identifiers:
```
null var const block if elif else while for return func afunc throw try catch include continue break
```

(it is woth noting that certain identifiers that collide with C++ keywords or C++ standard library functions in the global scope, such as `enum` or `fopen`, may also not work in the original C++ transpiler implementation of Tachyon.)

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

Tachyon supports number and sting literals. 

## 3 Values and Types
## 3.1 Number
## 3.2 String
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