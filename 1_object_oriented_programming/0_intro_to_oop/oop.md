# Intro to Object Oriented Programming

## Overview

Classes are useful for user-defined data types.  Once class is created, you can intstantiate multiple instances of a given class (blue car, red car).  Classes vary a lot depending on the application.

## Structures

Structures are a user-defined type where you can aggregate other data.  Example including initialization:

```
struct Rectangle {
  float length{1};
  float width{1};
};
```

Then to declare a variable of this type

```
int value; // variable named value of type int
Rectangle rectangle; // variable named rectangle of type Rectangle
rectangle.length = 10; // set length member
rectangle.widgh = 20; // set width member
```

By default, all stuct members are public.

### Access Specifiers

Determines if members are private or public.  For Structure, members are public by default.  Private means access only by object, public means external code can access.  Protected means not public, but accessible by friends and children.

Accessors are methods to get and set methods.

Mutator is same public function name with different arguments.

```
struct Date {
  private:
    int day{1};
  public:
    int month{1};
    int year{0};
    int Day() { return day; }; // Getter
    void Day(int d) { day = d }; // Mutator for setting, can prevent certain values from being set, ie invariant
}
```

## Classes

By convention, programmers use structures when member variables are independent of each other, and use classes when member variables are related by an "invariant".

By default, all class members are private.

[Lesson file](./lesson_06_access_specifiers.cpp)

### Invariants

An "invariant" is a rule that limits the values of member variables.

For example, in a Date class, an invariant would specify that the member variable day cannot be less than 0. Another invariant would specify that the value of day cannot exceed 28, 29, 30, or 31, depending on the month and year. Yet another invariant would limit the value of month to the range of 1 to 12.

### Encapsulation and Abstraction

[High level explanation](https://youtu.be/KXojXgyW-O8)

#### Encapsulation

We bundle related data and operations in a class, and protect it.

[Encapsulation](https://en.wikipedia.org/wiki/Encapsulation) is the grouping together of data and logic into a single unit. In object-oriented programming, classes encapsulate data and functions that operate on that data.

This can be a delicate balance, because on the one hand we want to group together relevant data and functions, but on the hand we want to limit member functions to only those functions that need direct access to the representation of a class.

[Video](https://youtu.be/1-j6AxEoK3c)

[Lesson file](./lesson_14_encapsulation.cpp)

#### Abstraction 

Users only need to understand the interface, they don't need to know the details of how we work with the data.

### Constructors

Can make custom contructors.  In general, want to instantiate all members.

[Lesson file](./lesson_10_constructors.cpp)

Default constructor.

```
class Date { 
  int day{1};
  int month{1};
  int year{0};
};
```

### Scope Resolution

C++ allows different identifiers (variable and function names) to have the same name, as long as they have different scope. For example, two different functions can each declare the variable int i, because each variable only exists within the scope of its parent function.

In some cases, scopes can overlap, in which case the compiler may need assistance in determining which identifier the programmer means to use. The process of determining which identifier to use is called "scope resolution".

Scope Resultion Operator
:: is the scope resolution operator. We can use this operator to specify which namespace or class to search in order to resolve an identifier.

```
Person::move(); \\ Call the move the function that is a member of the Person class.
std::map m; \\ Initialize the map container from the C++ Standard Library.  
```

Each class provides its own scope. We can use the scope resolution operator to specify identifiers from a class.

This becomes particularly useful if we want to separate class declaration from class definition.

class Date {
 public:
  int Day() const { return day; }
  void Day(int day);  // Declare member function Date::Day().
  int Month() const { return month; }
  void Month(int month) {
    if (month >= 1 && month <= 12) Date::month = month;
  }
  int Year() const { return year; }
  void Year(int year) { Date::year = year; }

 private:
  int day{1};
  int month{1};
  int year{0};
};

// Define member function Date::Day().
void Date::Day(int day) {
  if (day >= 1 && day <= 31) Date::day = day;
}

### Namespaces

Namespaces allow programmers to group logically related variables and functions together. Namespaces also help to avoid conflicts between to variables that have the same name in different parts of a program.

namespace English {
void Hello() { std::cout << "Hello, World!\n"; }
}  // namespace English

namespace Spanish {
void Hello() { std::cout << "Hola, Mundo!\n"; }
}  // namespace Spanish

int main() {
  English::Hello();
  Spanish::Hello();
}
In this example, we have two different void Hello() functions. If we put both of these functions in the same namespace, they would conflict and the program would not compile. However, by declaring each of these functions in a separate namespace, they are able to co-exist. Furthermore, we can specify which function to call by prefixing Hello() with the appropriate namespace, followed by the :: operator.

std Namespace
You are already familiar with the std namespace, even if you didn't realize quite what it was. std is the namespace used by the C++ Standard Library.

Classes like std::vector and functions like std::sort are defined within the std namespace.

[Lesson file](./lesson_11_scope_resolution.cpp)

### Initializer Lists

Initializer Lists
Initializer lists initialize member variables to specific values, just before the class constructor runs. This initialization ensures that class members are automatically initialized when an instance of the class is created.

```
Date::Date(int day, int month, int year) : year_(y) {
  Day(day);
  Month(month);
}
```

In this example, the member value year is initialized through the initializer list, while day and month are assigned from within the constructor. Assigning day and month allows us to apply the invariants set in the mutator.

In general, prefer initialization to assignment. Initialization sets the value as soon as the object exists, whereas assignment sets the value only after the object comes into being. This means that assignment creates and opportunity to accidentally use a variable before its value is set.

In fact, initialization lists ensure that member variables are initialized before the object is created. This is why class member variables can be declared const, but only if the member variable is initialized through an initialization list. Trying to initialize a const class member within the body of the constructor will not work.

Initializer lists exist for a number of reasons. First, the compiler can optimize initialization faster from an initialization list than from within the constructor.

A second reason is a bit of a technical paradox. If you have a const class attribute, you can only initialize it using an initialization list. Otherwise, you would violate the const keyword simply by initializing the member in the constructor!

The third reason is that attributes defined as references must use initialization lists.

[Video 1](https://youtu.be/cqKuYu1oiow)

[Video 2](https://youtu.be/Ms4Li58ZvwA)

[Lesson file](./lesson_12_initializer_lists.cpp)

## Accessor Functions

[Video](https://youtu.be/HfVOiSpzFaA)
