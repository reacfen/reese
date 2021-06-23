# reese

A "_mostly failed_" attempt at bringing class-like functionality to C99/C11.

## Description

Reese is a library for C99/C11 that introduces a pseudo-class-like structure inside of the C language. Although, the syntax looks far from a native class implementation, there is only so much that can be done using the preprocessor. As such, care has been taken to keep the syntax as concise and clean as possible.

## Installation

Reese is fairly simple to install. Just copy all the above files in the repository into your project and you are all up and ready to use it in your project!

## Usage

> **IMPORTANT:** Make sure to include the header `<reese.h>` if you are going to use any of reese's functionality!

Using reese is very straightforward and simple. A simple example of a "class" definition would look like this:

```c
// Defining a class called "my_class"
REESE_DEFINE_CLASS_INLINE(my_class,
    // Instance variable(s)
    int x;
    , // <- This comma is important!

    // Define the constructor (You must define it before all other member functions!)
    my_class, (int x), {
        self->x = x;
    },
    // Define the member method(s) (Can be defined in any order you want)
    set_x, (int x), {
        self->x = x;
    },
    get_x, (void), {
        /* Note that you have to use 'reese_return' and NOT 'return' when returning a value
         * from a member function! */
        reese_return(self->x);
    }
)
```

This is called an _inline class definition_ where the constructor and all member functions are defined inside the class definition itself.

> **Note:** Due to the nature of preprocessor macros, all separate components of a function i.e., the function name, the parameters and the body, need to be separated using a comma (,).
>
> Therefore, something like `my_function(int foo) {}` needs to be written as `my_function, (int foo), {}` inside of the class definition.

Alternatively, you can also define the member methods outside of the class like this:

```c
// Definition of "my_class"
REESE_DEFINE_CLASS(my_class,
    // Instance variable(s)
    int x;
    , // <- Again, don't forget this comma!

    // Declare the constructor
    my_class, (int x),
    // Declare the member method(s)
    set_x, (int x),
    get_x, (void)
)

// Now you can define them separately outside the class!
REESE_DEFINE_DRAFT(
    // Define the constructor
    my_class, (int x), {
        self->x = x;
    },
    // Define the member method(s)
    set_x, (int x), {
        self->x = x;
    },
    get_x, (void), {
        reese_return(self->x);
    }
)
```

This is especially useful if you want to declare a class inside of a C header and define it inside of a C source file.

> **Note:** There are variety of ways to define a class and its member methods using reese. However, the above two ways are the most straightforward and simple.

### Referencing yet-to-be-defined classes

You can reference classes that are not yet defined by taking a pointer to it:

```c
typedef struct example {
    REESE_CLASS my_class* instance;
} example;
// Defining a class called "my_class"
REESE_DEFINE_CLASS_INLINE(my_class,
    // Instance variable(s)
    int x;
    , // <- This comma is important!

    // Define the constructor (You must define it before all other member functions!)
    my_class, (int x), {
        self->x = x;
    },
    // Define the member method(s) (Can be defined in any order you want)
    set_x, (int x), {
        self->x = x;
    },
    get_x, (void), {
        /* Note that you have to use 'reese_return' and NOT 'return' when returning a value
         * from a member function! */
        reese_return(self->x);
    }
)
int main(void) {
    // ...
    example example_inst;
    my_class inst = create_my_class(123);
    example_inst.instance = &inst;
    printf("%d\n", example_inst.instance);
    // ...
}
```

### Creating an instance of a class

Creating an instance of a class using reese requires a bit getting used to, but is also quite straightforward. You can create an instance of a class using three different ways:

1. Using `create_{class_name}()`:
   ```c
   my_class inst = create_my_class(123);
   ```
2. Using `REESE_CONSTRUCT()`:
   ```c
   my_class inst = REESE_CONSTUCT(my_class, 123);
   ```
3. Using `REESE_FETCH_CONSTRUCTOR()`:
   ```c
   my_class inst = REESE_FETCH_CONSTRUCTOR(my_class) (123);
   ```
   Once you have created the instance, you can call its class's methods like this:

```c
// ...
// To fetch a returned value from a member function returning non-void
const int returned_value = ret_cast(int)capture(my_class, inst).get_x().ret();
printf("%d\n", returned_value); // 123

// If you want to simply invoke a function and do not care about what it returns
capture(my_class, inst).set_x(321);
printf("%d\n", inst.x);         // 321
// ...
finish_capture();
```

> **Note:** If you are using the GCC compiler, you may use the `typeof(inst)` compiler extension to automatically deduce the type of the instance. Hence the above code can be shortened to:
>
> ```c
> #define cap(instance) capture(typeof(instance), instance)
> // ...
> const int returned_value = ret_cast(int)cap(inst).get_x().ret();
> printf("%d\n", returned_value); // 123
>
> cap(inst).set_x(321);
> printf("%d\n", inst.x);         // 321
> // ...
> finish_capture();
> ```

## FAQ

#### Do I need any external libraries to use reese?

No, reese does not have any dependencies. Just fetch the source code and you are ready to go!

#### Is reese cross-platform?

Yes. Reese does not use any platform-specific code to function.

#### Can I use reese with C++?

Though not recommended, but for the sake of the argument, yes.

#### How does it work?

It uses some quirky preprocessor magic to get its job done.

#### Should I use reese in development?

Currently, reese is in an experimental stage, so it is not recommended to use it in a full-fledged development environment.

## Limitation(s)

1. You are *not allowed to define more than 256 member functions* (*including* the constructor) for **each** "class" you create.
2. Reese does some dynamic allocations in the background when return values of instances are fetched. This might prove to be inefficient in situations where conserving memory is important.
3. Reese is not concurrency safe.
