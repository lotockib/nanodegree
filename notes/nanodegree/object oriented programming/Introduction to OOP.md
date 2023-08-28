## Overview

Classes are useful for user-defined data types. Once class is created, you can intstantiate multiple instances of a given class (blue car, red car). Classes vary a lot depending on the application.

## Structures

Structures are a user-defined type where you can aggregate other data. Example including initialization:

```cpp
struct Rectangle {
  float length{1};
  float width{1};
};
```

Then to declare a variable of this type

```cpp
int value; // variable named value of type int
Rectangle rectangle; // variable named rectangle of type Rectangle
rectangle.length = 10; // set length member
rectangle.widgh = 20; // set width member
```

By default, all stuct members are public.

### Access Specifiers

Determines if members are private or public. For Structure, members are public by default. Private means access only by object, public means external code can access. Protected means not public, but accessible by friends and children.

Accessors are methods to get and set methods.

Mutator is same public function name with different arguments.

```cpp
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

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_06_access_specifiers.cpp "./lesson_06_access_specifiers.cpp")

### Invariants

An "invariant" is a rule that limits the values of member variables.

For example, in a Date class, an invariant would specify that the member variable day cannot be less than 0. Another invariant would specify that the value of day cannot exceed 28, 29, 30, or 31, depending on the month and year. Yet another invariant would limit the value of month to the range of 1 to 12.

### Encapsulation and Abstraction

[High level explanation](https://youtu.be/KXojXgyW-O8 "https://youtu.be/KXojXgyW-O8")

#### Encapsulation

We bundle related data and operations in a class, and protect it.

[Encapsulation](https://en.wikipedia.org/wiki/Encapsulation "https://en.wikipedia.org/wiki/Encapsulation") is the grouping together of data and logic into a single unit. In object-oriented programming, classes encapsulate data and functions that operate on that data.

This can be a delicate balance, because on the one hand we want to group together relevant data and functions, but on the hand we want to limit member functions to only those functions that need direct access to the representation of a class.

[Video](https://youtu.be/1-j6AxEoK3c "https://youtu.be/1-j6AxEoK3c")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_14_encapsulation.cpp "./lesson_14_encapsulation.cpp")

#### Abstraction

Abstraction refers to the separation of a class's interface from the details of its implementation. The interface provides a way to interact with an object, while hiding the details and implementation of how the class works.

Users only need to understand the interface, they don't need to know the details of how we work with the data.

Example The String() function within this Date class is an example of abstraction.

```cpp
class Date {
 public:
  ...
  std::string String() const;
  ...
};
```

The user is able to interact with the Date class through the String() function, but the user does not need to know about the implementation of either Date or String().

For example, the user does not know, or need to know, that this object internally contains three int member variables. The user can just call the String() method to get data.

If the designer of this class ever decides to change how the data is stored internally -- using a vector of ints instead of three separate ints, for example -- the user of the Date class will not need to know.

In this [lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_22_abstraction.cpp "./lesson_22_abstraction.cpp"), the `String()` function abstracts away things like vectors and other details.

### Constructors

Can make custom contructors. In general, want to instantiate all members.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_10_constructors.cpp "./lesson_10_constructors.cpp")

Default constructor.

```cpp
class Date { 
  int day{1};
  int month{1};
  int year{0};
};
```

### Scope Resolution

C++ allows different identifiers (variable and function names) to have the same name, as long as they have different scope. For example, two different functions can each declare the variable int i, because each variable only exists within the scope of its parent function.

In some cases, scopes can overlap, in which case the compiler may need assistance in determining which identifier the programmer means to use. The process of determining which identifier to use is called "scope resolution".

Scope Resultion Operator :: is the scope resolution operator. We can use this operator to specify which namespace or class to search in order to resolve an identifier.

```cpp
Person::move(); \\ Call the move the function that is a member of the Person class.
std::map m; \\ Initialize the map container from the C++ Standard Library.  
```

Each class provides its own scope. We can use the scope resolution operator to specify identifiers from a class.

This becomes particularly useful if we want to separate class declaration from class definition.

```cpp
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
```

### Namespaces

Namespaces allow programmers to group logically related variables and functions together. Namespaces also help to avoid conflicts between to variables that have the same name in different parts of a program.

```cpp
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
```

In this example, we have two different void Hello() functions. If we put both of these functions in the same namespace, they would conflict and the program would not compile. However, by declaring each of these functions in a separate namespace, they are able to co-exist. Furthermore, we can specify which function to call by prefixing Hello() with the appropriate namespace, followed by the :: operator.

std Namespace You are already familiar with the std namespace, even if you didn't realize quite what it was. std is the namespace used by the C++ Standard Library.

Classes like std::vector and functions like std::sort are defined within the std namespace.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_11_scope_resolution.cpp "./lesson_11_scope_resolution.cpp")

### Initializer Lists

Initializer Lists Initializer lists initialize member variables to specific values, just before the class constructor runs. This initialization ensures that class members are automatically initialized when an instance of the class is created.

```cpp
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

[Video 1](https://youtu.be/cqKuYu1oiow "https://youtu.be/cqKuYu1oiow")

[Video 2](https://youtu.be/Ms4Li58ZvwA "https://youtu.be/Ms4Li58ZvwA")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_12_initializer_lists.cpp "./lesson_12_initializer_lists.cpp")

### Accessor Functions

Accessor Functions Accessor functions are public member functions that allow users to access an object's data, albeit indirectly.

const Accessors should only retrieve data. They should not change the data stored in the object.

The main role of the const specifier in accessor methods is to protect member data. When you specify a member function as const, the compiler will prohibit that function from changing any of the object's member data.

[Video](https://youtu.be/HfVOiSpzFaA "https://youtu.be/HfVOiSpzFaA")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_15_accessors.cpp "./lesson_15_accessors.cpp")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_24_exercise.cpp "./lesson_24_exercise.cpp")

### Mutator Functions

A mutator ("setter") function can apply logic ("invariants") when updating member data.

[Video](https://youtu.be/-WRtCE3MZB8 "https://youtu.be/-WRtCE3MZB8")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_16_mutators.cpp "./lesson_16_mutators.cpp")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_17_exercise.cpp "./lesson_17_exercise.cpp")

### Static Members

Class members can be declared static, which means that the member belongs to the entire class, instead of to a specific instance of the class. More specifically, a static member is created only once and then shared by all instances (i.e. objects) of the class. That means that if the static member gets changed, either by a user of the class or within a member function of the class itself, then all members of the class will see that change the next time they access the static member.

static members are declared within their class (often in a header file) but in most cases they must be defined within the global scope. That's because memory is allocated for static variables immediately when the program begins, at the same time any global variables are initialized.

Here is an example:

```cpp
#include <cassert>

class Foo {
 public:
  static int count;
  Foo() { Foo::count += 1; }
};

int Foo::count{0};

int main() {
  Foo f{};
  assert(Foo::count == 1);
}
```

An exception to the global definition of static members is if such members can be marked as constexpr. In that case, the static member variable can be both declared and defined within the class definition:

```cpp
struct Kilometer {
  static constexpr int meters{1000};
};
```

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_25_static_members.cpp "./lesson_25_static_members.cpp")

### Static Methods

In addition to static member variables, C++ supports static member functions (or "methods"). Just like static member variables, static member functions are instance-independent: they belong to the class, not to any particular instance of the class.

One corollary to this is that we can method invoke a static member function without ever creating an instance of the class.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/0_intro_to_oop/lesson_26_static_methods.cpp "./lesson_26_static_methods.cpp")