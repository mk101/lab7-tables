#pragma once
#define DataOk 0
#define DataErr -1

class TDataCom {
public:
  TDataCom() : m_RetCode(DataOk) {}
  [[nodiscard]] int GetRetCode() const {
    return m_RetCode;
  }
protected:
  int m_RetCode;
  void SetRetCode(const int retCode) {
    m_RetCode = retCode;
  };
};