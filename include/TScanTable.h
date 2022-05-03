#pragma once
#include "TArrayTable.h"

class TScanTable :public TArrayTable {
public:
	TScanTable(int size = 25):TArrayTable(size) { }
	virtual PTDataValue FindRecord(TKey k) {
		SetRetCode(TabOK);
		int i;
		for (i = 0; i < DataCount; i++) {
			if (pRecs[i]->Key == k)
				break;
		}
		Efficiency = i + 1;
		if (i < DataCount)
		{
			CurPos = i;
			return pRecs[i]->pValue;
		}
		SetRetCode(TabNoRecord);
		return nullptr;
	}
	virtual void InsRecord(TKey k, PTDataValue pVal); //создать ttabrecord и вставить в конец
	virtual void DelRecord(TKey k); //найти элемент, который нужно удалить(findrecord) и переместить последний
	                                //элемент переместить на место удаляемого, очистить память от старого элемента
};