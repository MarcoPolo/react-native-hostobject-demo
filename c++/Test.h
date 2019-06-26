// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <jsi/jsi.h>
// #include "testnum.h"

namespace example {

class Test {
 private:
  friend class TestBinding;

  int runTest() const;
};

}  // namespace example
