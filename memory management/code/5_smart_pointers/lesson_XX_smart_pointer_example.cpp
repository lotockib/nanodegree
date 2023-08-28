#include <memory>
#include <iostream>

int main() {
  std::unique_ptr<double> my_ptr(new double(5));
  std::cout << "my_ptr stack address (i think) = &my_ptr = " << &my_ptr << std::endl;
  std::cout << "my_ptr heap address = my_ptr.get() = " << my_ptr.get() << std::endl;
  std::cout << "my_ptr value = *my_ptr = " << *my_ptr << std::endl;

  // Can I assign my_ptr heap address elsewhere
  double * my_dumb_ptr = my_ptr.get();
  std::cout << "my_dumb_ptr heap address = my_dumb_ptr = " << my_dumb_ptr << std::endl;
  std::cout << "my_dumb_ptr value = *my_dumb_ptr = " << *my_dumb_ptr << std::endl;

}
