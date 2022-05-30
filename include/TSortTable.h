#pragma once
#include "TScanTable.h"
#include <fstream>
#include <iostream>
enum class TSortMethod{ Quick,Insert, Merge };

class TSortTable : public TScanTable {
 public:
  explicit TSortTable(int Size = 20) : TScanTable(Size) { };
  explicit TSortTable(const TScanTable& tab) {
    *this = tab;
  }
  TSortTable& operator=(const TScanTable& tab) {
    if (this == &tab)
      return *this;
    if (m_Records != nullptr) {
      for (int i = 0; i < m_DataCount; i++) {
        delete m_Records[i];
      }
      delete[] m_Records;
      m_Records = nullptr;
    }
    m_TableSize = tab.GetTabSize();
    m_DataCount = tab.GetDataCount();
    m_Records = new PTTabRecord[m_TableSize];
    for (int i = 0; i < m_DataCount; i++) {
      m_Records[i] = (PTTabRecord)tab.m_Records[i]->GetCopy();
    }
    SortData();
    m_CurrentPosition = 0;
    return *this;
  }
  TSortMethod GetSortMethod(){
    return m_SortMethod;
  }
  void SetSortMethod(TSortMethod sm) {
    m_SortMethod = sm;
  }
  virtual PTDataValue FindRecord(const TKey &key) override {
    int i = 0, i1 = 0, i2 = m_DataCount - 1;
    m_Efficiency = 0;
    while (i1 <= i2) {
      m_Efficiency++;
      i = (i1 + i2) / 2;
      if (m_Records[i]->m_Key == key) {
        i1 = i + 1;
        i2 = i;
        break;
      }
      if (m_Records[i]->m_Key > key) {
        i2 = i - 1;
      }
      else {
        i1 = i + 1;
      }
    }
    if (i2 < 0 || (m_Records[i2]->m_Key) < key) {
      i2++;
    }
    m_CurrentPosition = i2;
    if (i2 < m_DataCount && m_Records[i2]->m_Key == key) {
      SetRetCode(TabOK);
      return m_Records[i2]->m_Value;
    }
    SetRetCode(TabNoRecord);
    return nullptr;
  }
  void InsertRecord(const TKey &key, PTDataValue pValue) override {
    if (IsFull())
      SetRetCode(TabFull);
    else {
      PTDataValue temp = FindRecord(key);
      if (temp != nullptr)
        SetRetCode(TabRecDbl);
      else {
        for (int i = m_DataCount; i > m_CurrentPosition; i--) {
          m_Records[i] = m_Records[i - 1];
        }
        m_Records[m_CurrentPosition] = new TTabRecord(key, pValue);
        m_DataCount++;
        SetRetCode(TabOK);
      }
    }
  }
  void DeleteRecord(const TKey &key) override {
    PTDataValue temp = FindRecord(key);
    if (temp == nullptr) {
      SetRetCode(TabNoRecord);
    }
    else {
      for (int i = m_CurrentPosition; i < m_DataCount - 1; i++) {
        m_Records[i] = m_Records[i + 1];
      }
      delete m_Records[--m_DataCount];
      m_Records[m_DataCount] = nullptr;
      SetRetCode(TabOK);
    }
  }

 protected:
  TSortMethod m_SortMethod;
  void SortData() {
    m_Efficiency = 0;
    switch (m_SortMethod) {
      case TSortMethod::Insert:
        InsertSort(m_Records, m_DataCount);
        break;
      case TSortMethod::Merge:
        MergeSort(m_Records, m_DataCount);
        break;
      case TSortMethod::Quick:
        QuickSort(m_Records, m_DataCount);
        break;
    }
  }
  void InsertSort(PTTabRecord* pMem, int DataCount) {
    PTTabRecord pR;
    for (int i = 1, j; i < DataCount; i++) {
      pR = pMem[i];
      for (j = i - 1; j > -1; j--) {
        if (pMem[j]->GetKey() > pR->GetKey()) {
          pMem[j + 1] = pMem[j];
          m_Efficiency++;
        }
        else
          break;
      }
      pMem[j + 1] = pR;
    }
  }
  void MergeSort(PTTabRecord* pMem, int DataCount) {
    PTTabRecord *pData = pMem;
    auto *pBuf = new PTTabRecord[DataCount];
    PTTabRecord* pTemp = pBuf;
    MergeSorter(pData, pBuf, DataCount);
    delete pTemp;
  }
  void QuickSort(PTTabRecord* pMem, int DataCount) {
    int piv = DataCount / 2;
    if (DataCount > 1) {
      QuickSplit(pMem, DataCount, piv);
      QuickSort(pMem, DataCount - piv);
      if (DataCount % 2 == 0)
        QuickSort(pMem + piv, piv);
      else
        QuickSort(pMem + piv + 1, piv);
    }
  }

  void MergeSorter(PTTabRecord*& pData, PTTabRecord*& pBuf, int Size) { //������ ����������
    int n1 = (Size + 1) / 2;
    int n2 = Size - n1;
    if (Size > 2) {
      PTTabRecord* pDat2 = pData + n1;
      PTTabRecord* pBuf2 = pBuf + n1;
      MergeSorter(pData, pBuf, n1);
      MergeSorter(pDat2, pBuf2, n2);
    }
    MergeData(pData, pBuf, n1, n2);
  }
  void MergeData(PTTabRecord*& pData, PTTabRecord*& pBuf, int n1, int n2) {
    int i = 0, i1 = 0, i2 = 0;
    PTTabRecord* pData1 = pData;
    PTTabRecord* pData2 = pData + n1;
    while (i1 < n1 && i2 < n2) {
      if (pData1[i1]->GetKey() < pData2[i2]->GetKey()) {
        pBuf[i++] = pData1[i1++];
      }
      else {
        pBuf[i++] = pData2[i2++];
      }
    }
    while (i1<n1) {
      pBuf[i++] = pData1[i1++];
    }
    while (i2 < n2) {
      pBuf[i++] = pData2[i2++];
    }
    /*pData = pBuf;
    pBuf = pData1;*/
    for (int j = 0; j < i; j++) {
      pData[j] = pBuf[j];
    }
    m_Efficiency += n1 + n2;
  }
  void QuickSplit(PTTabRecord* pData, int Size, int& pivot) {
    int i = 0, j = Size - 1;
    PTTabRecord x = pData[pivot];
    do {
      while ((pData[i]->GetKey()) < (x->GetKey())) i++;
      while ((pData[j]->GetKey()) > (x->GetKey())) j--;

      if (i <= j) {
        if ((pData[i]->GetKey()) > (pData[j]->GetKey())) {
          std::swap(pData[i], pData[j]);
          m_Efficiency += 2;
        }
        x = pData[pivot];
        if ((pData[i]->GetKey()) == (x->GetKey())) j--;
        else if ((pData[j]->GetKey()) == (x->GetKey())) i++;
        else {
          i++;
          j--;
        }
      }
    } while (i <= j);
  }
};