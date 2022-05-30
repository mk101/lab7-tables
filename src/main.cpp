#include <iostream>

#include "TScanTable.h"
#include "TSortTable.h"
#include "TListHashTable.h"
#include "CSVParser.h"

int main() {
  auto *table = new TListHashTable();

  std::cout << "============Import CSV============" << std::endl;
  std::cout << "Importing...";

  CSVParser parser("D:\\test.csv");
  if (!parser.ParseAndInsert(table)) {
    std::cout << "Failed" << std::endl;
    delete table;
    return -1;
  }

  std::cout << "Done" << std::endl;
  std::cout << "Table view:" << std::endl;
  std::cout << *table;

  std::cout << "============Find============" << std::endl;

  std::cout << "Ivanov: " << table->FindRecord("Ivanov")->ToString() << std::endl;
  std::cout << "Unknown: ";

  auto unknown = table->FindRecord("Unknown");
  if (table->GetRetCode() == TabNoRecord) {
    std::cout << "No record" << std::endl;
  } else {
    std::cout << unknown->ToString();
  }

  std::cout << "============Delete============" << std::endl;
  std::cout << "Deleting Petrov...";
  table->DeleteRecord("Petrov");
  if (table->GetRetCode() == TabOK) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "Failed" << std::endl;
  }

  std::cout << "============Insert============" << std::endl;
  std::cout << "Insert Petrov...";
  auto newPetrovMarks = new TVectorValue({new TIntValue(5), new TIntValue(5)});
  table->InsertRecord("Petrov", newPetrovMarks);
  delete newPetrovMarks;

  if (table->GetRetCode() == TabFull) {
    std::cout << "Failed" << std::endl;
  } else {
    std::cout << "OK" << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Final table view:" << std::endl;
  std::cout << *table;

  delete table;
  return 0;
}
