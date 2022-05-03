#pragma once
#include <iostream>
#include <sstream>
#include "TDataValue.h"

typedef std::string TKey;
class TTabRecord;
typedef TTabRecord* PTTabRecord;

class TTabRecord : public TDataValue {
public:
	TTabRecord(const TKey &key, PTDataValue val) {
      m_Key = key;
      m_Value = val->GetCopy();
	}

    ~TTabRecord() override {
      delete m_Value;
    }
	void SetKey(const TKey &key) {
      m_Key = key;
	}
	TKey GetKey() {
		return m_Key;
	}
	void SetValue(PTDataValue p) {
      if (m_Value != nullptr) {
        delete m_Value;
      }
      m_Value = p->GetCopy();
	}
	PTDataValue GetValue() {
		return m_Value;
	}
	TDataValue* GetCopy() override {
		return new TTabRecord(m_Key, m_Value);
	}
	TTabRecord& operator=(const TTabRecord& tr) {
      if (this == &tr) {
        return *this;
      }
      m_Key = tr.m_Key;
      m_Value = tr.m_Value->GetCopy();

      return *this;
	}
	virtual bool operator==(const TTabRecord& tr) const {
		return m_Key == tr.m_Key;
	}
	virtual bool operator>(const TTabRecord& tr) const {
		return m_Key > tr.m_Key;
	}
	virtual bool operator<(const TTabRecord& tr) const {
		return m_Key < tr.m_Key;
	}

	std::string ToString() override {
      std::stringstream ss;

      ss << "m_Key: " << m_Key << " Value: " << m_Value->ToString();
      return ss.str();
	}
	friend class TArrayTable;
	friend class TSortTable;
	friend class TTreeNote;
	friend class TTreeTable;
	friend class TScanTable;
	friend class TArrayHashTable;
	friend class TListHashTable;

protected:
  TKey m_Key;
  PTDataValue m_Value;
  virtual void Print() {
    std::cout << ToString() << std::endl;
  }
};