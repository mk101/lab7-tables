#include <iostream>

#include "TScanTable.h"
#include "TIntValue.h"

int main() {
  TScanTable table;

  table.InsertRecord("Petrov", new TIntValue(5));
  table.InsertRecord("Ivanov", new TIntValue(4));

  std::cout << table;
  return 0;
}
