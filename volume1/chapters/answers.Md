#  Answers

## Lesson 1

### Review

1.  Machine code is the computer's native language -- a set of numerical instructions which the computer executes.
2.  Source code is the human-readable text used to create a program.

## Lesson 2

### Review

1.  One line comments can be added using two forward slashes -- *//* -- and comments which can span multiple lines are placed between these markers: */\* \*/*
2.  The four tools used to turn source code into machine code are the preprocessor, the compiler, the assembler, and the linker.
3.  The two classes of programming errors are syntactic and semantic. Syntactic errors are mistakes in the construction of the computer language, such as having the wrong number of parentheses in a statement. Semantic errors are mistakes in the program logic of code that has correct syntax -- bugs in the meaning of the code.

## Lesson 3

### Review

1.  The *%* operator returns the remainder of the division of the two arguments. *10 % 4* is 2.
2.  *i\_ adds one before the rest of the expression is evaluated. \_i* does the addition afterward.
3.  myVar \*= 5
4.  *==* is used for comparison because *=* is used for assigning a value to a variable.
5.  *%f*is the placeholder for a float when using *printf*.
6.  Parameters are information given to a function which it requires to perform its task.
7.  The difference between int and long is that long typically can hold bigger values.
8.  *10 / 4* is integer division, so the result is 2. *10.0 / 4.0* is floating point division, so the result is 2.5.

## Lesson 4

### Assignment

Our job was to use the equation *Interest = Principal \* rate \* time* to calculate and print the simple interest incurred on a principal of \$20000 at a rate of 5% per month for 24 months and we had to create a function to do the actual interest calculations.

Here's one way to do it:

```cpp
#include <stdio.h>


float SimpleInterest(float principal, float rate, int months)
{
    return principal * rate * months;
}


int main(void)
{
    // Our $20000 principal
    float p = 20000.00;

    // 5% interest is .05
    float r = .05;

    int m = 24;
    float interest = SimpleInterest(p, r, m);

    printf("The interest on %f at %f%% interest for %d months is $%f\n",
        p, r * 100, m, interest);
}
```

### Bug Hunt

1.  The variable *c* was not declared. Add a line *int c = 5;* after the one for b and this will compile (and work) properly.
2.  The include for math.h has been forgotten. Add *\#include \<math.h\>* to the top of the example.

### Review

1.  Nothing will be printed. *i* has a value of zero, which, for the purposes of logic, is treated as false.
2.  The boolean OR operator, *||*, should be used here.
3.  The first section of the control block of a *for* loop sets the value of the index variable. The second section is the test to see if another iteration is called for, and the third modifies the index variable.

## Lesson 5

### Assignment

In the last lesson we were trying to make a program which asks for a word from the user and it prints out the integer value of each character.

We were given these steps to follow to write it:

1.  Make a *char* array to hold the user input.
2.  Call *gets()* to get the string from the user.
3.  Make an *int* variable and set it to the string's length
4.  Use a *for* loop to print each character in the string both as a character and its numerical value

```cpp
#include <stdio.h>
#include <string.h>

int main(void)
{
    char inString[1024];

    printf("Type the text to convert and press Enter: ");
    gets(inString);

    // Here was the part we needed to fill in, so here's one way to do it.
    // Step 3.
    int strLength = strlen(inString);

    // Just some fluff to make this program a little nicer
    printf("The character codes for the string \'%s\'\n",inString);

    // Step 4.
    for (int i = 0; i < strLength; i++)
        printf("[%d]: %c\t%d (0x%x)\n", i, inString[i], inString[i], inString[i]);

    return 0;
}
```

This example even includes printing the character code in both decimal and hexadecimal.

### Bug Hunt

1.  The *i++* in the for loop needs to be *i += 2*
2.  The warnings come from using the *%d* -- used for integers -- for a *float* variable. Change the *%d* placeholders in the *printf* statements to *%f* and they'll go away.

### Review

1.  A segmentation fault is an attempt to access memory that doesn't belong to your program.
2.  Because array indices start at 0, the last valid index for a 10-element array is 9.
3.  Pointers should be initialized to NULL or a known-good address so that you always can tell whether or not a pointer is valid and can, thus, avoid segfaults.
4.  *\'a\'* is a character constant. *\"a\"* is a string, which amounts to the *\'a\'* character constant plus a NULL string terminator.
5.  A string is an array of the type *char*.
6.  *string.h* is the header which we have been using that contains helper functions for working with strings.

## Lesson 6

### Bug Hunt

There was only one hunt because of how hard it was. It's a kind of error called an off-by-1 error. These are sometimes are to track down, like this one was. The errors and corrections are commented.

```cpp
char *ReverseString(char *string)
{
    // This function rearranges a string so that it is backwards
    // i.e. abcdef -> fedcba

    if (!string)
        return NULL;

    int length = strlen(string);
    int count = length / 2;

    for (int i = 0; i < count; i++)
    {
        // This line from Lesson 6 is incorrect
        //char temp = string[length - i];
        char temp = string[length - 1 - i];

        // So is this one
        //string[length - i] = string[i];
        string[length - 1 - i] = string[i];
        string[i] = temp;
    }

    return string;
}
```

The reason why this causes the program to not print anything requires a little thought. The first line in the loop is supposed to save the last character of the string. By not subtracting 1 from the length, it saves the *NULL* terminator for the string into *temp* instead. The rest of the loop continues on pretty much as it should. *abcdef\\0* becomes *\\0fedcba*. Because the *NULL* terminator is at the very beginning of the string, when *printf()* goes to print it, it sees the terminator and stops, resulting in the reversed string not getting printed.

### Review

1.  for, while, and do.
2.  A switch is better than if when there needs to be a choice made from several possible options.
3.  Any cases in a switch after the default case will never be executed.
4.  myVariable = (myInt > 5) ? 50 : 100


## Lesson 7

### Bug Hunt

1.  The pointer *combinedString* doesn't point to a valid memory address. It needs to either be given heap memory by *malloc()* -- which is later freed -- or declared on the stack as an array.
2.  The size of *binaryString* array in *main()* is too small. It needs to be at least big enough to hold 1 character per bit in a byte plus 1 for the *NULL* terminator, so *binaryString* must be at least 9 characters instead of 6.

### Review

1.  The heap is bigger.
2.  A memory leak is when memory is allocated from the heap and is not freed.
3.  Portability is the quality of being able to compile your code on more than one operating system with as little modification as possible, preferably without any.
4.  Typecasting when you tell the compiler to treat one type as another.
5.  malloc just allocates memory from the heap. calloc allocates memory and fills it with 0's.
6.  The result of 7 & 0 is 0.
7.  The result of 7 | 8 is 15.
8.  33 looks like 100001 in binary?
9.  The binary number 1000010 is 66 in regular decimal notation?
10. The result of 1 << 5 is 32.

## Lesson 8

### Bug Hunt

1.  The problem is that the parameter string is a *const char \** instead of a just *char \**. This means that the string given to *ReverseString()* can't be changed and the compiler complains when we try to change it in the *for* loop. Remove the *const* keyword and everything is happy.

### Review

1.  A variable's scope is its lifespan from when it is allocated and declared to when it is freed and how it can be accessed by code.
2.  The three types of scope are local, global, and static.
3.  The declaration int * const myPointer; is a pointer whose value can be changed but whose address cannot be changed.
4.  Data written to stderr normally is printed to the screen.
5.  A handle is an arbitrary number used to uniquely identify an object, such as a file.
6.  If you want to write data to the end of an existing file, you must use the "a" or "a+" modes in fopen().

## Lesson 9

### Review

1.  There are ten rows in the array float myFloatArray[10][20].

## Lesson 10

### Review

1.  Default parameter values let you leave out one or more parameters when calling a function.
2.  References have an advantage over pointers in that they are always initialized and can't be uninitialized.
3.  A reference is declared like this: int &reference = someOtherInteger;
4.  The difference between passing a parameter by value and passing one by reference is that when a parameter is passed by value, a copy of the parameter is given to the function. This allows the function to change the parameter given to it without changing the variable that was passed to it.
5.  Declaration A is for a function pointer

## Lesson 11

### Review

1.  An enumerated type is one whose value is one of a set of specified values.
2.  By specifying an integer value for an identifier in an enumerated type, it is possible to use the enumerated type for case values in a switch statement.
3.  A variable inside a struct is accessed using the dot operator or the arrow operator. The form is like this: myStruct.someVariable or myStructPtr->someVariable.
4.  The dot operator is used for structures allocated on the stack and the arrow operator is used for structures allocated on the heap.

## Lesson 12

### Review

1.  Object-oriented programming is a programming paradigm which thinks in terms of the interaction of objects as a means to accomplish a task.
2.  A class is a C++ object definition which consists of a name, inheritance, and a list of methods and properties.
3.  A method is a function which belongs to a class.
4.  A property is a data variable which belongs to a class.
5.  A class' constructor is for initializing its properties and allocating memory for heap-allocated properties. The destructor is for freeing pointers and performing any other tear-down for an object before it is freed.
6.  new and delete are the C++ counterparts to malloc() and free().
7.  If malloc() is used to allocate an object, its constructor is never called. If free() is used to free an object, its destructor is never called.
8.  A public method can be called by functions outside the object. A private method can only be called by other methods owned by the class.
9.  Arrays allocated with new are properly freed by calling delete [] on the array pointer.
10. Data abstraction is the practice of using hiding properties from public access using methods.

## Lesson 13

### Review

1.  Inheritance is the practice of creating a class by extending another one.
2.  Private methods and properties can be accessed from within the class' methods. Protected methods and properties can be accessed from the class' methods and those of any child classes.
3.  A virtual function is one whose behavior can be redefined by a child class.
4.  A static function is one which can be called without having an instance of its owning class.
5.  Function overloading is the practice of declaring and implementing more than one function with the same name and different parameter lists.
6.  Protected inheritance forces all public methods and properties of the parent class to have protected access in the child class, restricting outside access but allowing access from its methods and those of its child classes.
