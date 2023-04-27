<div align="center">
    <h1>Æ lang</h1>
</div>

## Hello World

- Need flex and bison

Make compiler

```bash
clone the repo
cd ae
make
```

example/fib.ae

```c++

void main() {
    i32 i = 0;
    i32 a = 0;
    i32 b = 1;
    println(a);
    println(b);
    while (i < 10) {
        i32 c = a + b;
        println(c);
        a = b;
        b = c;
        i = i + 1;
    }
}
```

Build program

```bash
./aec example/fib.ae
./fib
```

Ferdig da.

## What Æ lang supports

- Print statements: `println(x)`,`print(x)`
- Int declaration: `i32 a = 5;`
- Float declaration: `f32 b = 5.0;`
- If, If/else: `if` `else`
- While statement: `while`
- void function: `void a() { println("Hello"); }`
- int/float function: `i32 g() { return 0; }`

## Todo list
* Static checking
    * Check that when we encounter a break, that it is inside a while-loop.
    * Strong type checking
    * bool must end up as a bool. Define everything but 0 as true
    * check parameters types
* Lists
* Heap
    * new and delete keywords
* VS-Code syntax highlight
* export and import
    * only symbols marked as export will be visible when import
* namespaces
* casting
* strings as data type
* class (not struct)
    * everything is private until set public
* scopes
* enum
* Target JavaScript?
* Type Expressions
    * Type constructors (dragon page 371)
* auto for deducing types
* const !!!