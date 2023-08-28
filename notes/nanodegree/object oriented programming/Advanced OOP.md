## Summary

[Summary video](https://youtu.be/QR68Vcr-XTw "https://youtu.be/QR68Vcr-XTw")

[Another video](https://youtu.be/gWcAMxhNOcg "https://youtu.be/gWcAMxhNOcg")

## Inheritance

Inheritence In our everyday life, we tend to divide things into groups, based on their shared characteristics. Here are some groups that you have probably used yourself: electronics, tools, vehicles, or plants.

Sometimes these groups have hierarchies. For example, computers and smartphones are both types of electronics, but computers and smartphones are also groups in and of themselves. You can imagine a tree with "electronics" at the top, and "computers" and "smartphones" each as children of the "electronics" node.
![[Pasted image 20230827205540.png]]

Object-oriented programming uses the same principles! For instance, imagine a Vehicle class:

```cpp
class Vehicle {
public:
  int wheels = 0;
  string color = "blue";

  void Print() const
  {
    std::cout << "This " << color << " vehicle has " << wheels << " wheels!\n";
  }
};
```

We can derive other classes from Vehicle, such as Car or Bicycle. One advantage is that this saves us from having to re-define all of the common member variables - in this case, wheels and color - in each derived class.

Another benefit is that derived classes, for example Car and Bicycle, can have distinct member variables, such as sunroof or kickstand. Different derived classes will have different member variables:

```cpp
class Car : public Vehicle {
public:
  bool sunroof = false;
};

class Bicycle : public Vehicle {
public:
  bool kickstand = true;
};
```

[Lesson example](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_03_inheritance.cpp "./lesson_03_inheritance.cpp")

### Inherited Access Specifiers

Just as access specifiers (i.e. public, protected, and private) define which class members users can access, the same access modifiers also define which class members users of a derived classes can access.

[Public inheritance](https://en.cppreference.com/w/cpp/language/derived_class#Public_inheritance "https://en.cppreference.com/w/cpp/language/derived_class#Public_inheritance"): the public and protected members of the base class listed after the specifier keep their member access in the derived class

[Protected inheritance](https://en.cppreference.com/w/cpp/language/derived_class#Protected_inheritance "https://en.cppreference.com/w/cpp/language/derived_class#Protected_inheritance"): the public and protected members of the base class listed after the specifier are protected members of the derived class

[Private inheritance](https://en.cppreference.com/w/cpp/language/derived_class#Private_inheritance "https://en.cppreference.com/w/cpp/language/derived_class#Private_inheritance"): the public and protected members of the base class listed after the specifier are private members of the derived class

[Lesson example](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_04_access_specifiers.cpp "./lesson_04_access_specifiers.cpp")

[Lesson example](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_05_exercise.cpp "./lesson_05_exercise.cpp")

## Composition

Composition is a closely related alternative to inheritance. Composition involves constructing ("composing") classes from other classes, instead of inheriting traits from a parent class.

A common way to distinguish "composition" from "inheritance" is to think about what an object can do, rather than what it is. This is often expressed as "has a" versus "is a".

From the standpoint of composition, a cat "has a" head and "has a" set of paws and "has a" tail.

From the standpoint of inheritance, a cat "is a" mammal.

There is no hard and fast rule about when to prefer composition over inheritance. In general, if a class needs only extend a small amount of functionality beyond what is already offered by another class, it makes sense to inherit from that other class. However, if a class needs to contain functionality from a variety of otherwise unrelated classes, it makes sense to compose the class from those other classes.

[Lesson example](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_06_composition.cpp "./lesson_06_composition.cpp")

## Friends

In C++, friend classes provide an alternative inheritance mechanism to derived classes. The main difference between classical inheritance and friend inheritance is that a friend class can access private members of the base class, which isn't the case for classical inheritance. In classical inheritance, a derived class can only access public and protected members of the base class.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_08_friends.cpp "./lesson_08_friends.cpp")

## Polymorphism

Polymorphism is means "assuming many forms".

In the context of object-oriented programming, polymorphism describes a paradigm in which a function may behave differently depending on how it is called. In particular, the function will perform differently based on its inputs.

Polymorphism can be achieved in two ways in C++: overloading and overriding. In this exercise we will focus on overloading.

## Overloading

In C++, you can write two (or more) versions of a function with the same name. This is called "overloading". Overloading requires that we leave the function name the same, but we modify the function signature. For example, we might define the same function name with multiple different configurations of input arguments.

This example of class Date overloads:

```cpp
#include <ctime>

class Date {
public:
    Date(int day, int month, int year) : day_(day), month_(month), year_(year) {}
    Date(int day, int month) : day_(day), month_(month)  // automatically sets the Date to the current year
    {
        time_t t = time(NULL);
	    tm* timePtr = localtime(&t);
        year_ = timePtr->tm_year;
    }

private:
    int day_;
    int month_;
    int year_;
};
```

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_09_overloading.cpp "./lesson_09_overloading.cpp")

## Operator Overloading

In this exercise you'll see how to achieve polymorphism with operator overloading. You can choose any operator from the ASCII table and give it your own set of rules!

Operator overloading can be useful for many things. Consider the + operator. We can use it to add ints, doubles, floats, or even std::strings.

In order to overload an operator, use the operator keyword in the function signature:

```cpp
Complex operator+(const Complex& addend) {
  //...logic to add complex numbers
}
```

Imagine vector addition. You might want to perform vector addition on a pair of points to add their x and y components. The compiler won't recognize this type of operation on its own, because this data is user defined. However, you can overload the + operator so it performs the action that you want to implement.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_10_operator_overloading.cpp "./lesson_10_operator_overloading.cpp")

## Virtual Functions

Virtual functions are a polymorphic feature. These functions are declared (and possibly defined) in a base class, and can be overridden by derived classes.

This approach declares an interface at the base level, but delegates the implementation of the interface to the derived classes.

In this exercise, class Shape is the base class. Geometrical shapes possess both an area and a perimeter. Area() and Perimeter() should be virtual functions of the base class interface. Append = 0 to each of these functions in order to declare them to be "pure" virtual functions.

A pure virtual function is a virtual function that the base class declares but does not define.

A pure virtual function has the side effect of making its class abstract. This means that the class cannot be instantiated. Instead, only classes that derive from the abstract class and override the pure virtual function can be instantiated.

```cpp
class Shape {
    public:
        Shape() {}
        virtual double Area() const = 0;
        virtual double Perimeter() const = 0;
};
```

Virtual functions can be defined by derived classes, but this is not required. However, if we mark the virtual function with = 0 in the base class, then we are declaring the function to be a pure virtual function. This means that the base class does not define this function. A derived class must define this function, or else the derived class will be abstract.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_11_virtual_functions.cpp "./lesson_11_virtual_functions.cpp")

## Polymorphism: Overriding

"Overriding" a function occurs when:

A base class declares a [virtual function]([http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#glossary](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#glossary "http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#glossary") function). A derived class overrides that virtual function by defining its own implementation with an identical function signature (i.e. the same function name and argument types).

```cpp
class Animal {
public:
  virtual std::string Talk() const = 0;
};

class Cat {
public:
  std::string Talk() const { return std::string("Meow"); }
};
```

In this example, Animal exposes a virtual function: Talk(), but does not define it. Because Animal::Talk() is undefined, it is called a pure virtual function, as opposed to an ordinary (impure? 😉) virtual function.

Furthermore, because Animal contains a pure virtual function, the user cannot instantiate an object of type Animal. This makes Animal an abstract class.

Cat, however, inherits from Animal and overrides Animal::Talk() with Cat::Talk(), which is defined. Therefore, it is possible to instantiate an object of type Cat.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_12_overriding.cpp "./lesson_12_overriding.cpp")

## Function Hiding

Function hiding is closely related, but distinct from, overriding.

A derived class hides a base class function, as opposed to overriding it, if the base class function is not specified to be virtual.

```cpp
class Cat { // Here, Cat does not derive from a base class
public:
  std::string Talk() const { return std::string("Meow"); }
};

class Lion : public Cat {
public:
  std::string Talk() const { return std::string("Roar"); }
};
```

In this example, Cat is the base class and Lion is the derived class. Both Cat and Lion have Talk() member functions.

When an object of type Lion calls Talk(), the object will run Lion::Talk(), not Cat::Talk().

In this situation, Lion::Talk() is hiding Cat::Talk(). If Cat::Talk() were virtual, then Lion::Talk() would override Cat::Talk(), instead of hiding it. Overriding requires a virtual function in the base class.

The distinction between overriding and hiding is subtle and not terribly significant, but in certain situations hiding can lead to bizarre errors, particularly when the two functions have slightly different function signatures.

## More Override

"Overriding" a function occurs when a derived class defines the implementation of a virtual function that it inherits from a base class.

It is possible, but not required, to specify a function declaration as override.

```cpp
class Shape {
public:
  virtual double Area() const = 0;
  virtual double Perimeter() const = 0;
};

class Circle : public Shape {
public:
  Circle(double radius) : radius_(radius) {}
  double Area() const override { return pow(radius_, 2) * PI; } // specified as an override function
  double Perimeter() const override { return 2  *radius_*  PI; } // specified as an override function

private:
  double radius_;
};
```

This specification tells both the compiler and the human programmer that the purpose of this function is to override a virtual function. The compiler will verify that a function specified as override does indeed override some other virtual function, or otherwise the compiler will generate an error.

Specifying a function as override is good practice, as it empowers the compiler to verify the code, and communicates the intention of the code to future users.

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_13_overriding.cpp "./lesson_13_overriding.cpp")

## Multiple Inheritance

A single class inherits from multiple classes.

[Video](https://youtu.be/jEoPLBdLLsw "https://youtu.be/jEoPLBdLLsw")

[The "diamond problem"](https://youtu.be/p29phGPfKnQ "https://youtu.be/p29phGPfKnQ")

The Core Guidelines have some worthwhile recommendations about how and when to use multiple inheritance:

[Use multiple inheritance to represent multiple distinct interfaces](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c135-use-multiple-inheritance-to-represent-multiple-distinct-interfaces "http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c135-use-multiple-inheritance-to-represent-multiple-distinct-interfaces")

["Use multiple inheritance to represent the union of implementation attributes"](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c136-use-multiple-inheritance-to-represent-the-union-of-implementation-attributes "http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c136-use-multiple-inheritance-to-represent-the-union-of-implementation-attributes")

## Generic Programming

Can use Template to build classes with different types of data. This is generic programming.

[Video](https://youtu.be/k2Hai5sBemU "https://youtu.be/k2Hai5sBemU")

## Templates

Templates enable generic programming by generalizing a function to apply to any class. Specifically, templates use types as parameters so that the same implementation can operate on different data types.

For example, you might need a function to accept many different data types. The function acts on those arguments, perhaps dividing them or sorting them or something else. Rather than writing and maintaining the multiple function declarations, each accepting slightly different arguments, you can write one function and pass the argument types as parameters. At compile time, the compiler then expands the code using the types that are passed as parameters.

```cpp
template <typename Type> Type Sum(Type a, Type b) { return a + b; }

int main() { std::cout << Sum<double>(20.0, 13.7) << "\n"; }
Because Sum() is defined with a template, when the program calls Sum() with doubles as parameters, the function expands to become:

double Sum(double a, double b) {
    return a+b;
}
```

Or in this case:

```cpp
std::cout << Sum<char>(‘Z’, ’j’) << "\n";
The program expands to become:

char Sum(char a, char b) {
    return a+b;
}
```

We use the keyword template to specify which function is generic. Generic code is the term for code that is independent of types. It is mandatory to put the template<> tag before the function signature, to specify and mark that the declaration is generic.

Besides template, the keyword typename (or, alternatively, class) specifies the generic type in the function prototype. The parameters that follow typename (or class) represent generic types in the function declaration.

In order to instantiate a templatized class, use a templatized constructor, for example: Sum(20.0, 13.7). You might recognize this form as the same form used to construct a vector. That's because vectors are indeed a generic class!

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_15_template.cpp "./lesson_15_template.cpp")

[Lesson file](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/1_object_oriented_programming/1_advanced_oop/lesson_16_template.cpp "./lesson_16_template.cpp")

## Deduction

In this example, you will see the difference between total and partial deduction.

Deduction occurs when you instantiate an object without explicitly identifying the types. Instead, the compiler "deduces" the types. This can be helpful for writing code that is generic and can handle a variety of inputs.

## Useful Resources From Reviewer

[Basic Concepts of OOP using C++](https://www.geeksforgeeks.org/basic-concepts-of-object-oriented-programming-using-c/ "https://www.geeksforgeeks.org/basic-concepts-of-object-oriented-programming-using-c/")

[Short video on OOP](https://www.youtube.com/watch?v=ZOKLjJF54Xc "https://www.youtube.com/watch?v=ZOKLjJF54Xc")

[4 Advantages of Object Oriented Programming](https://www.roberthalf.com/blog/salaries-and-skills/4-advantages-of-object-oriented-programming "https://www.roberthalf.com/blog/salaries-and-skills/4-advantages-of-object-oriented-programming")

[Procedural vs OOP](https://www.geeksforgeeks.org/differences-between-procedural-and-object-oriented-programming/ "https://www.geeksforgeeks.org/differences-between-procedural-and-object-oriented-programming/")

[Modern C++ Object Oriented Programming](https://www.ms.sapientia.ro/~manyi/teaching/c++/CPP_v1.2.pdf "https://www.ms.sapientia.ro/~manyi/teaching/c++/CPP_v1.2.pdf")

[C++ Beginner Game Programming](https://www.youtube.com/playlist?list=PLqCJpWy5FohcehaXlCIt8sVBHBFFRVWsx "https://www.youtube.com/playlist?list=PLqCJpWy5FohcehaXlCIt8sVBHBFFRVWsx")

[OOP Overview for Python](https://www.youtube.com/watch?v=-DP1i2ZU9gk "https://www.youtube.com/watch?v=-DP1i2ZU9gk")

[Code it Yourself - Role Playing Game](https://www.youtube.com/watch?v=xXXt3htgDok&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO&index=18 "https://www.youtube.com/watch?v=xXXt3htgDok&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO&index=18")

[Polymorphism](https://www.youtube.com/watch?v=kxKKHKSMGIg&t=651s "https://www.youtube.com/watch?v=kxKKHKSMGIg&t=651s")

[Pointers](https://www.youtube.com/watch?v=iChalAKXffs "https://www.youtube.com/watch?v=iChalAKXffs")

[CppCon 2019 – Back to Basics Object Oriented Programming](https://youtu.be/32tDTD9UJCE "https://youtu.be/32tDTD9UJCE")

[SOLID Design Principles](https://www.youtube.com/watch?v=rtmFCcjEgEw "https://www.youtube.com/watch?v=rtmFCcjEgEw")

[Design Patterns](https://www.geeksforgeeks.org/introduction-to-pattern-designing/ "https://www.geeksforgeeks.org/introduction-to-pattern-designing/")

[Code Complete Book](http://aroma.vn/web/wp-content/uploads/2016/11/code-complete-2nd-edition-v413hav.pdf "http://aroma.vn/web/wp-content/uploads/2016/11/code-complete-2nd-edition-v413hav.pdf")

[Pragmatic Programmer Book](https://www.cin.ufpe.br/~cavmj/104The%20Pragmatic%20Programmer,%20From%20Journeyman%20To%20Master%20-%20Andrew%20Hunt,%20David%20Thomas%20-%20Addison%20Wesley%20-%201999.pdf "https://www.cin.ufpe.br/~cavmj/104The%20Pragmatic%20Programmer,%20From%20Journeyman%20To%20Master%20-%20Andrew%20Hunt,%20David%20Thomas%20-%20Addison%20Wesley%20-%201999.pdf")

[Design Patterns Book](http://www.uml.org.cn/c++/pdf/DesignPatterns.pdf "http://www.uml.org.cn/c++/pdf/DesignPatterns.pdf")

[Linux Shell Workshop](https://classroom.udacity.com/courses/ud206 "https://classroom.udacity.com/courses/ud206")

[How to view running processes on linux](https://www.howtoforge.com/how-to-view-running-processes-on-linux/ "https://www.howtoforge.com/how-to-view-running-processes-on-linux/")

[Learning Bash](https://guide.bash.academy/ "https://guide.bash.academy/")