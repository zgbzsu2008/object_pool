#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "noncopyable.h"

namespace zgb {

template <class T>
class object_pool : noncopyable {
 public:
  using DeleterType = std::function<void(T *)>;

  static constexpr size_t kNum = 128;

  explicit object_pool(size_t num = kNum) : pool_(num) { allocate(); }
  ~object_pool() = default;

  void add(std::unique_ptr<T> ptr) { pool_.emplace_back(std::move(ptr)); }

  std::unique_ptr<T, DeleterType> get() {
    if (empty()) {
      pool_.resize(kNum);
      allocate();
    }

    std::unique_ptr<T, DeleterType> ptr(pool_.back().release(), [this](T *t) {
      pool_.emplace_back(std::unique_ptr<T>(t));
    });
    pool_.pop_back();
    return std::move(ptr);
  }

  bool empty() const { return pool_.empty(); }
  std::size_t size() const { return pool_.size(); }

 private:
  void allocate() {
    for (size_t i = 0; i < size(); i++) {
      std::unique_ptr<T> ptr = std::make_unique<T>();
      pool_[i] = std::move(ptr);
    }
  }
  std::vector<std::unique_ptr<T>> pool_;  // object pool
};

}  // namespace zgb
