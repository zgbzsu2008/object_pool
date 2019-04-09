#include <iostream>
#include "object_pool.h"

namespace zgb {
template <typename T>
class object {
 public:
  object() : data_(0) {}
  explicit object(T data) : data_(data) {}
  void print() { std::cout << "Object:data= " << data_ << std::endl; }

  T data_;
};
}  // namespace zgb

using namespace zgb;

int main() {
  using T = object<int>;
  object_pool<T> pool(0);

  for (int i = 0; i < 10; ++i) {
    pool.add(std::make_unique<T>(i * i));
  }

  for (int i = 0; i < 10; ++i) {
    auto u = pool.get();
    u->print();
    {
      std::shared_ptr<T> s = pool.get();
      s->print();
    }
  }
  return 0;
}