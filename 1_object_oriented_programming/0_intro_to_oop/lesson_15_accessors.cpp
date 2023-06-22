#include <iostream>
#include <string>

class BankAccount
{
 private:
      int routing_number{0};
      int account_number{0};
      float balance{0};

 public:
      BankAccount(int routing, int account, float balance) :
        routing_number(routing), account_number(account), balance(balance) {}

      void RoutingNumber(int n);
      void AccountNumber(int n);
      void Balance(float b);

      int RoutingNumber();
      int AccountNumber();
      float Balance();
};

void BankAccount::RoutingNumber(int n) { routing_number = n; }
void BankAccount::AccountNumber(int n) { account_number = n; }
void BankAccount::Balance(float b) { balance = b; }

int BankAccount::RoutingNumber() { return routing_number; }
int BankAccount::AccountNumber() { return account_number; }
float BankAccount::Balance() { return balance; }

int main() {
    int routing = 12345;
    int account = 67890;
    float balance = 1000.00;
    BankAccount my_account(routing, account, balance);
    std::cout << "Routing number = " << my_account.RoutingNumber() << std::endl;
    std::cout << "Account number = " << my_account.AccountNumber() << std::endl;
    std::cout << "Balance number = " << my_account.Balance() << std::endl;
}