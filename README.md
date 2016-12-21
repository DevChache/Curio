# Curio
A simple compiler design by [DevChache](https://github.com/DevChache), NCEPU;

### Introduction
Curio is a simple compiler and the syntax is as simple as possible.

### Extension
The source code file extension for Curio is `*.curio`, and in default, the compiled file has a extension of `*.run`.

You may specify the file name (including extension) of the source file by add `-o` compile option during compiling.
### Syntax
#### Variables
Curio supports 3 types of variables, they are ***string***, ***number*** and ***boolean***. 

**NOTE**: We strongly recommand you to name the variables in Camel-Case style.

##### string
***string*** is a really normal type of variable and can be splited into several slices (sub-string). A string begins with any kinds of number but ends with `'\0'`.

You may define a string variable as follows:

```
string str;
```
As for initializing during declaration, you may use:
```
string str1 = 'I am a string.';
string str2 = "I am a string, too.";
string str3 = @123.45; // equals to string str3 = "123.45"; 
```

##### number
##### boolean


#### Expressions
##### sentence
A sentence is a sequence of characters or phrases that describes "a thing".
After computer runs over the sentence, the "thing" will be done (unless errors or exceptions happens).

A sentence could be like this:
```
a + b;
number aNumber = 64;
add(7, 5);
exit();
print("Hello world!");
println("Greetings");
```
#### Operations
#### Functions
Functions are a sequence of commands that do sequence of "things". When things are done, the function usually returns a specified value.

**NOTE**: We strongly recommand you to decalre a function with Upper-Camel-Case (alse known as Pascal-Case) style.

Functions with no return value:
```
FunctionName(parameters...):void
{
    // Sequence of things.
}
```

### Usage
#### Compile source file
On your Linux bash, or macOS terminal, use `curioc` to run the compiler.

1. Ignore the `-o` option and its parameter.

    In this case, the `target` file will be named the same as the source.
2. Append the `-o` option and its parameter after the name of the source file.

    In this case, the `target` file will be named exactly as what the parameter specified.
```
// bash on Linux
curioc source
curioc source -o target
```
#### Execute the compiled file
```
./outputfilename
```

### Contact author
My E-mail address is <yangzd1996@outlook.com>.