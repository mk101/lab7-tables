#pragma once
#include "TDataValue.h"

class TIntValue : public TDataValue {
 public:
  explicit TIntValue(int value) : m_Value(value) {}

  TDataValue *GetCopy() override {
    return new TIntValue(m_Value);
  }
  std::string ToString() override {
    return std::to_string(m_Value);
  }
  ~TIntValue() override = default;
 private:
  int m_Value;
};

typedef TIntValue *PTIntValue;
