# Eris 
**Version 1.0.0**

C++ interpreter for the Eris programming language with a header-only implementation.

Eris is a general-purpose, lightweight, dynamic programming language with class-based object-oriented programming support.

Compile `src/main.cpp` to get an executable that can run Eris programs on the command-line like `eris filename.eris` (make sure to compile on C++11).

```
class Point {
    def constructor(self, x, y) {
        self.x = x;
        self.y = y;
    }

    def calc(self) {
        return Math::pow(self.x, 2.0) + Math::pow(self.y, 2.0);
    }
}

let p = Point(5, 6);
print(p.calc()); // 61.0
```

## Status
Currently, the interpreter is mostly working, but the implementation is old and unstable, and thus may contain bugs, and documentation is sparse.
A refactoring of the interpreter, the addition of hash maps, and many stability and performance updates, are slated for Eris 2.0.

## Example programs
See the [#examples](examples) directory for example programs.