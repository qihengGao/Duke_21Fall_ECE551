#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct _retire_info retire_info;

double calculate_balance(int startAge, double balance, retire_info info) {
  int age = startAge / 12, month = startAge % 12;
  for (int i = 0; i < info.months; i++) {
    printf("Age %3d month %2d you have $%.2lf\n", age, month, balance);
    month++;
    balance = balance * (1 + info.rate_of_return) + info.contribution;
    if (month == 12) {
      age++;
      month = 0;
    }
  }
  return balance;
}

void retirement(int startAge, double initial, retire_info working, retire_info retired) {
  double balance = calculate_balance(startAge, initial, working);
  int retiredAge = startAge + working.months;
  calculate_balance(retiredAge, balance, retired);
}

int main(void) {
  retire_info working = {489, 1000, 0.045 / 12};
  retire_info retired = {384, -4000, 0.01 / 12};
  retirement(327, 21345.0, working, retired);
  return EXIT_SUCCESS;
}
