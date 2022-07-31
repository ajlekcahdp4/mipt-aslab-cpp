#include <chrono>
#include <iostream>

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

#include <order_statistic_set.hpp>

int main(int argc, char *argv[]) {
  if (!std::cin || !std::cout) { std::abort(); }

#ifdef BOOST_FOUND__
  po::options_description desc("Available options");
  desc.add_options()("help,h", "Print this help message");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
#endif
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