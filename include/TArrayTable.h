#pragma once
#include "TTabRecord.h"
#include "TTable.h"
#define TabMaxSize 25;

enum class TDataPos{First_pos,Current_pos,Last_pos};

class TArrayTable : public TTable {
public:
  explicit TArrayTable(int size = 100) : m_TableSize(size) {
    m_Records = new PTTabRecord[m_TableSize];
      for (int i = 0; i < m_TableSize; i++) {
        m_Records[i] = nullptr;
      }
    m_CurrentPosition = 0;
  }
  ~TArrayTable() override {
      for (int i = 0; i < m_DataCount; i++)
      {
          delete m_Records[i];
      }
      delete[] m_Records;
  }

  TArrayTable(const TArrayTable &t)  : TTable(t) {
    m_CurrentPosition = t.m_CurrentPosition;
    m_TableSize = t.m_TableSize;

    m_Records = new PTTabRecord[m_TableSize];
    for (int i = 0; i < m_TableSize; i++) {
      m_Records[i] = dynamic_cast<TTabRecord*>(t.m_Records[i]->GetCopy());
    }
  }

  TArrayTable &operator=(const TArrayTable &t) {
    if (this == &t) {
      return *this;
    }

    m_CurrentPosition = t.m_CurrentPosition;
    m_TableSize = t.m_TableSize;

    m_Records = new PTTabRecord[m_TableSize];
    for (int i = 0; i < m_TableSize; i++) {
      m_Records[i] = dynamic_cast<TTabRecord*>(t.m_Records[i]->GetCopy());
    }

    return *this;
  }
  [[nodiscard]] bool IsFull() const override {
      return m_DataCount >= m_TableSize;
  }
  [[nodiscard]] int GetTabSize() const {
      return m_TableSize;
  }
  [[nodiscard]] TKey GetKey() const override {
      return GetKey(TDataPos::Current_pos); //либо реализовать самим метод получения с позиции
  }
  [[nodiscard]] virtual PTDataValue GetValuePtr() const {
      return GetValuePtr(TDataPos::Current_pos);
  }
  [[nodiscard]] virtual TKey GetKey(TDataPos mode)const {
      int pos = -1;
      if (!IsEmpty()) {
          switch (mode)
          {
          case TDataPos::First_pos:
              pos = 0;
              break;
          case TDataPos::Last_pos:
              pos = m_DataCount - 1;
              break;
          default:
              pos = m_CurrentPosition;
              break;
          }
      }
      return (pos == -1) ? std::string("") : m_Records[pos]->m_Key;
  }

  [[nodiscard]] virtual PTDataValue GetValuePtr(TDataPos mode) const {
      int pos = -1;
      if (!IsEmpty()) {
          switch (mode)
          {
          case TDataPos::First_pos:
              pos = 0;
              break;
          case TDataPos::Last_pos:
              pos = m_DataCount - 1;
              break;
          default:
              pos = m_CurrentPosition;
              break;
          }
      }
      return (pos == -1) ? nullptr : m_Records[pos]->m_Value;
  }
  [[nodiscard]] bool IsTabEnded() const override {
      return m_CurrentPosition >= m_DataCount;
  }
  bool Reset() override {
    m_CurrentPosition = 0;
      return IsTabEnded();
  }
  bool GoNext() override {
      if (!IsTabEnded())
          m_CurrentPosition++;
      return IsTabEnded();
  }
  [[nodiscard]] int GetCurPos() const {
      return m_CurrentPosition;
  }
  virtual bool SetCurPos(int pos) {
    m_CurrentPosition = ((pos > -1) && (pos < m_DataCount)) ? pos : 0;
      return IsTabEnded();
  }

  [[nodiscard]] PTDataValue GetValue() const override {
    return GetValuePtr();
  }

  friend TSortTable;
  friend TScanTable;
 protected:
  PTTabRecord* m_Records;
  int m_TableSize;
  int m_CurrentPosition;
};