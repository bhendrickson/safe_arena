#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <span.h>

#include "arena.h"

namespace traditional {

struct node {
  std::unique_ptr<node> left;
  std::unique_ptr<node> right;
  std::vector<int> data;
};

std::unique_ptr<node> MakeTree(int& index, int depth) {
  if (depth == 0) return nullptr;

  auto n = std::make_unique<node>();
  n->left = MakeTree(index, depth - 1);
  n->right = MakeTree(index, depth - 1);
  n->data.resize(index % 5, index);
  index++;

  return n;
}

}  // namespace traditional

namespace with_arena {

struct node {
  node* left = nullptr;
  node* right = nullptr;
  gsl::span<int> data;
};

node* MakeTree(abc::arena& arena, int& index, int depth) {
  if (depth == 0) return nullptr;

  node* n = arena.allocate<node>();
  n->left = MakeTree(arena, index, depth - 1);
  n->right = MakeTree(arena, index, depth - 1);
  n->data = arena.allocate_span<int>(index % 5);
  for (auto& i : n->data) i = index;
  index++;

  return n;
}

}  // namespace with_arena

uint64_t now_ms() {
  return std::chrono::system_clock::now().time_since_epoch() /
         std::chrono::milliseconds(1);
}

template<class Func>
static int64_t measure(Func&& func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    return (end - start) / std::chrono::milliseconds(1);
}

void test() {
  int64_t arena_ms = measure([] {
    for (size_t i = 0; i < 30; i ++) {
	  with_arena::node* n = nullptr;
	  {
		  abc::arena arena; 
		  int index = 0;
		  n = with_arena::MakeTree(arena, index, 20);
		  arena.clear(); 
	  }
	  std::cout << "size: " << n->data.size();
    }
  });

  int64_t traditional_ms = measure([] {
    for (size_t i = 0; i < 30; i++) {
      int index = 0;
      traditional::MakeTree(index, 20);
    }
  });

  std::cout << "arena: " << arena_ms << " ms\n"
            << "traditional: " << traditional_ms << " ms\n"
            << "speedup: " << (traditional_ms / double(arena_ms)) << "x\n";
}

int main() {
  test();
  return 0;
}

