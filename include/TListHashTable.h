#pragma once
#include "THashTable.h"
#include "TTabRecord.h"
#include <list>

class TListHashTable : public THashTable {
 public:
  explicit TListHashTable(int size = 25) : THashTable() {
    m_Lists = new std::list<PTTabRecord>[size];
    m_TableSize = size;
    m_CurrentList = 0;
    for (int i = 0; i < m_TableSize; i++) {
      m_Lists[i] = std::list<PTTabRecord>();
    }
  }
  ~TListHashTable() override {
    delete[] m_Lists;
  }
  bool IsFull() const override {
    return false;
  }
  PTDataValue FindRecord(const TKey &key) override {
    SetRetCode(TabOK);
    PTDataValue tmp = nullptr;
    m_CurrentList = static_cast<int>(HashFunc(key)) % m_TableSize;
    std::list<PTTabRecord>* lst = m_Lists + m_CurrentList;
    for (auto rec : *lst) {
      m_Efficiency++;
      if (rec->m_Key == key) {
        tmp = rec->m_Value;
        return tmp;
      }
    }
    SetRetCode(TabNoRecord);
    return tmp;
  }
  void InsertRecord(const TKey &key, PTDataValue pValue) override {
    m_CurrentList = static_cast<int>(HashFunc(key)) % m_TableSize;
    std::list<PTTabRecord>* lst = m_Lists + m_CurrentList;
    lst->push_back(new TTabRecord(key, pValue));
  }
  void DeleteRecord(const TKey &key) override {
    SetRetCode(TabOK);
    m_CurrentList = static_cast<int>(HashFunc(key)) % m_TableSize;
    PTDataValue tmp=nullptr;
    for (auto rec : m_Lists[m_CurrentList]) {
      m_Efficiency++;
      if (rec->m_Key == key) {
        tmp = rec;
        break;
      }
    }
    if (tmp == nullptr) {
      SetRetCode(TabNoRecord);
      return;
    }
    m_Lists[m_CurrentList].remove((PTTabRecord)tmp);
  }
  TKey GetKey() const override {
    return (*m_StartChain)->m_Key;
  }
  PTDataValue GetValue() const override {
    return (*m_StartChain)->m_Value;
  }
  bool Reset() override {
    m_CurrentList = 0;
    while (m_Lists[m_CurrentList].empty()) {
      m_CurrentList++;
      if (IsTabEnded()) {
        return false;
      }
    }
    m_StartChain = m_Lists[m_CurrentList].begin();
    return !IsTabEnded();
  }
  bool GoNext() override {
    if (++m_StartChain != m_Lists[m_CurrentList].end()){
        return !IsTabEnded();
    }
    m_CurrentList++;

    while (m_Lists[m_CurrentList].empty()) {
      m_CurrentList++;
      if (IsTabEnded()) {
          return false;
      }
    }
    m_StartChain = m_Lists[m_CurrentList].begin();
    return !IsTabEnded();

  }
  bool IsTabEnded() const override {
    return m_CurrentList >= m_TableSize;
  }

 protected:
  int m_CurrentList;
  int m_TableSize;
  std::list<PTTabRecord>* m_Lists;
  std::list<PTTabRecord>::iterator m_StartChain;
};