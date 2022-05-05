#pragma once
#include "TArrayTable.h"

class TScanTable : public TArrayTable {
public:
  explicit TScanTable(int size = 25) : TArrayTable(size) { }
  TScanTable(const TScanTable &t) : TArrayTable(t) {}
  //TODO: ? operator =

  PTDataValue FindRecord(const TKey &k) override {
    SetRetCode(TabOK);
    int i;
    for (i = 0; i < m_DataCount; i++) {
        if (m_Records[i]->m_Key == k) {
          break;
        }
    }
    m_Efficiency = i + 1;
    if (i < m_DataCount) {
      m_CurrentPosition = i;
        return m_Records[i]->m_Value;
    }
    SetRetCode(TabNoRecord);
    return nullptr;
  }

  void InsertRecord(const TKey &k, PTDataValue pVal) override {
    SetRetCode(TabOK);

    if (this->IsFull()) {
      SetRetCode(TabFull);
      return;
    }

    this->m_Records[this->m_DataCount++] = new TTabRecord(k, pVal);
  }

  //найти элемент, который нужно удалить(findrecord) и переместить последний
  // элемент переместить на место удаляемого, очистить память от старого элемента
  int DeleteRecord(const TKey &k) override {
    auto record = FindRecord(k);

    if (record == nullptr) {
      return TabNoRecord;
    }

    SetRetCode(TabOK);
    m_Records[m_CurrentPosition] = dynamic_cast<PTTabRecord>(m_Records[m_DataCount - 1]->GetCopy());
    delete m_Records[m_DataCount - 1];
    delete record;

    m_DataCount--;

    return TabOK;
  }
};
