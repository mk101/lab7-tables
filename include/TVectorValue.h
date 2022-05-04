#pragma once

#include <vector>
#include <sstream>
#include "TDataValue.h"

class TVectorValue : public TDataValue {
 public:
  explicit TVectorValue(const std::vector<PTDataValue> &value) {
    for (auto ptr : value) {
      m_Value.push_back(ptr->GetCopy());
    }
  }

  TVectorValue(const TVectorValue &v) {
    for (auto ptr : v.m_Value) {
      m_Value.push_back(ptr->GetCopy());
    }
  }

  TVectorValue &operator=(const TVectorValue &v) {
    if (this == &v) {
      return *this;
    }

    for (auto value : m_Value) {
      delete value;
    }

    m_Value.clear();

    for (auto ptr : v.m_Value) {
      m_Value.push_back(ptr->GetCopy());
    }

    return *this;
  }

  ~TVectorValue() override {
    for (auto value : m_Value) {
      delete value;
    }
  }

  TDataValue *GetCopy() override {
    return new TVectorValue(m_Value);
  }
  std::string ToString() override {
    std::stringstream ss;
    ss << "{ ";
    for (auto v : m_Value) {
      ss << v->ToString() << " ";
    }

    ss << "}";

    return ss.str();
  }
 private:
  std::vector<PTDataValue> m_Value;
};

typedef TVectorValue *PTVectorValue;