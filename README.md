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

int main() {
    int i = 0;
    int a = 0;
    int b = 1;
    println(a);
    println(b);
    while (i < 10) {
        int c = a + b;
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
- Int declaration: `int a = 5;`
- Float declaration: `float b = 5.0;`
- If, If/else: `if` `else`
- While statement: `while`
- void function: `void a() { println("Hello"); }`
- int/float function: `int g() { return 0; }`

## Todo list
* Static checking
    * Check that when we encounter a break, that it is inside a while-loop.
    * Strong type checking
* builtin data types
    * i8, i16, i32, i64
    * u8, u16, u32, u64
    * f32, f64
    * bool
    * void
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
* void main() { }
* auto for deducing types