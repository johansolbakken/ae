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
