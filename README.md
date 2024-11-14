# Tachyon

**Version 2.0.0**

Tachyon is a performant dynamic prototype-based programming language. This repository provides the implementation as a transpiler to C++. It combines the expressiveness offered by dynamic prototype-based OOP with high performance and support for native multithreading, in order to be fit for tasks such as numerical computation.


Reference document: [REFERENCE.MD]
# Example
```
var Vec2 = {
    mag: lambda(self) {
        return Math.sqrt(self.x * self.x + self.y * self.y);
    }
};

var vec2 = {x: 3, y: 4, proto: Vec2};
System.print(vec2.mag()); // 5
```

# Changelog
* 1.0.0