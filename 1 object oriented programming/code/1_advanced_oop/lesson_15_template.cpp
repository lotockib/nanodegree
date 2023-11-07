#include <assert.h>

// TODO: Create a generic function Product that multiplies two parameters
template <typename Type> Type Product(Type a, Type b) {
    return a * b;
}

int main() { 
  assert(Product<int>(10, 2) == 20); 
}