#include <assert.h>
#include <string>

class Person {
    public:
        Person (std::string name);
        std::string name;
};

Person::Person(std::string n) : name(n) {}

class Person2 {
    public:
        Person2 (std::string name) : name(name) {};
        std::string name;
};

class Person3 {
    public:
        Person3 (std::string n) : name(n) {};
        // MUST assign via initalizer list because name is a const
        std::string const name;
};

// Test
int main() {

    // Test with initializer list outside of class definition
    Person alice("Alice");
    Person bob("Bob");
    assert(alice.name != bob.name);

    // Test with initializer list inside class definition
    Person2 brad("Brad");
    Person2 renee("Renee");
    assert(brad.name != renee.name);

    // Test with initializer list and const member variable
    Person3 lucas("Lucas");
    Person3 claire("Claire");
    assert(lucas.name != claire.name);

}