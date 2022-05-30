#pragma once
#include "TDataCom.h"
#include "TDataValue.h"
#include <iostream>
#define TabOK 0
#define TabEmpty -101
#define TabFull -102
#define TabNoRecord -103
#define TabRecDbl -104
#define TabNoMem -105
typedef std::string TKey;

class TTable : public TDataCom {
public:
	TTable() {
      m_DataCount = 0;
      m_Efficiency = 0;
	}
    TTable(const TTable &t) = default;
    TTable &operator=(const TTable &t) = default;
	virtual ~TTable() = default;

	[[nodiscard]] int GetDataCount() const {
		return m_DataCount;
	}
	[[nodiscard]] int GetEfficiency() const {
		return m_Efficiency;
	}
	[[nodiscard]] bool IsEmpty() const {
		return m_DataCount == 0;
	}
	[[nodiscard]] virtual bool IsFull() const = 0;
	virtual PTDataValue FindRecord(const TKey &key) = 0;
	virtual void InsertRecord(const TKey &k, PTDataValue pVal) = 0;
	virtual void DeleteRecord(const TKey &k) = 0;
	virtual bool Reset() = 0;
	virtual bool GoNext() = 0;
	[[nodiscard]] virtual bool IsTabEnded() const = 0;
	[[nodiscard]] virtual TKey GetKey() const = 0;
	[[nodiscard]] virtual PTDataValue GetValue() const = 0;

	friend std::ostream &operator<<(std::ostream& os, TTable& tab) {
		for (tab.Reset(); !tab.IsTabEnded(); tab.GoNext()) {
			os << '[' << "Key: " << tab.GetKey() << " Value: " << tab.GetValue()->ToString() << ']' << std::endl;
		}
		return os;
	}

protected:
  int m_DataCount;
  int m_Efficiency;
};

typedef TTable *PTTable;