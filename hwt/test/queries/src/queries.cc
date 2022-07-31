#include <chrono>
#include <iostream>

#include <order_statistic_set.hpp>

int main(int argc, char *argv[]) {
  if (!std::cin || !std::cout) { std::abort(); }
  throttle::order_statistic_set<int> t{};

  while (true) {
    char query_type{};
    int key{};

    if (!(std::cin >> query_type >> key)) { break; }

    switch (query_type) {
    case 'k': t.insert(key); break;
    case 'm': std::cout << t.select_rank(key) << " "; break;
    case 'n':
      auto closest = t.closest_left(key);
      auto closest_rank = t.get_rank_of(closest);
      std::cout << (closest == key ? closest_rank - 1 : closest_rank) << " ";
      break;
    }
  }

  std::cout << "\n";
}