#ifndef ABC_ARENA_H
#define ABC_ARENA_H

#include <cstdlib>
#include <vector>

#include <string_span.h>
#include <span.h>

namespace abc {

class arena {
 public:
  template<class T> gsl::span<T> allocate_span(std::ptrdiff_t count) [[lifetime(const)]] {
	T* t = allocate_unsafe<T>(count);
    return gsl::span<T>(t, count);
  }

  template<class T> T* allocate() [[lifetime(const)]] {
	T* t = allocate_unsafe<T>(1);
    return t;
  }

  void clear() {
	  blocks_.clear();
  }

 private:
  std::vector<std::unique_ptr<char[]>> blocks_;
  size_t space_ = 0;
  char* ptr_ = nullptr;

  template<class T> T* allocate_unsafe(std::ptrdiff_t count) {
    static_assert(std::is_trivially_destructible<T>::value,
	    "can only allocate trivially destructible types");
    static_assert(alignof(T) <= alignof(std::max_align_t),
	    "can only allocate types with a fundamental alignment");

    size_t size = sizeof(T) * count;
    uintptr_t padding = alignment_padding<alignof(T)>(ptr_);
    size_t padded_size = size + padding;

    void* space_to_use = nullptr;
    if (padded_size > space_) {
      space_to_use = fallback_allocate(size);
    } else {
	  space_to_use = ptr_ + padding;
	  ptr_ += padded_size;
	  space_ -= padded_size;
    }
    return new(space_to_use) T[count];
  }

  template<uintptr_t I> static uintptr_t alignment_padding(char* p) {
    auto i = reinterpret_cast<uintptr_t>(p);
    return (i + I - 1) & ~(I - 1) - i;
  }

  char* fallback_allocate(size_t size) {
    std::unique_ptr<char[]> new_block;

    static const size_t kBlockSize = 4096;
    if (size < kBlockSize / 4) {
      new_block = std::make_unique<char[]>(kBlockSize);
      space_ = kBlockSize - size;
      ptr_ = new_block.get() + size;
    } else {
      // Allocate large items by themselves to waste less at ends of blocks.
      new_block = std::make_unique<char[]>(size);
    }

    // Alignment from new char[]'s is sufficent for all fundamental alignments.
    char* ret = new_block.get();
    blocks_.push_back(std::move(new_block));
    return ret;
  }
};

}  // namespace abc

#endif  // ABC_ARENA_H
