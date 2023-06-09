#include <iostream>
#include <string>
using std::string;

class Vehicle {
public:
    int wheels = 0;
    string color = "blue";
    bool electric = false;
    
    void Print() const
    {
        std::cout << "This " << color << " vehicle has " << wheels << " wheels!\n";
    }
};

class Car : public Vehicle {
public:
    bool sunroof = false;
};

class Bicycle : public Vehicle {
public:
    bool kickstand = true;
};

class Scooter : public Vehicle {
public:
    float length = 0;
    void Print() const {
        Vehicle::Print();
        std::cout << "And it has length = " << length << std::endl;
    }
};

int main() 
{
    Car car;
    car.wheels = 4;
    car.sunroof = true;
    car.Print();
    if(car.sunroof)
        std::cout << "And a sunroof!\n";

    Scooter scooter;
    scooter.electric = true;
    scooter.wheels = 2;
    scooter.color = "green";
    scooter.length = 20.1;
    scooter.Print();
};