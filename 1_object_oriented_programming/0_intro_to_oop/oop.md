# Intro to Object Oriented Programming

## Overview

Classes are useful for user-defined data types.  Once class is created, you can intstantiate multiple instances of a given class (blue car, red car).  Classes vary a lot depending on the application.

## Structures

Structures are a user-defined type where you can aggregate other data.  Example:

```
struct Rectangle {
  float length;
  float width;
};
```

Then to declare a variable of this type

```
int value; // variable named value of type int
Rectangle rectangle; // variable named rectangle of type Rectangle
rectangle.length = 10; // init length member
rectangle.widgh = 20; // init width member
```

