#ifndef SbrYYY_h
#define SbrYYY_h

#include "SbrBase.h"

class SbrYYY : public SbrBase<SbrYYY>
{
public:
  SbrYYY(std::string name)
      : SbrBase(name)
  {
  }

  void setup() override;
  void loop() override;

private:
};

#endif