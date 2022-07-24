#include <iostream>

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

#include "caches.hpp"
#include "belady.hpp"

struct slow_getter_t {
  int operator()(int p_key) {
    // The answer to “the Ultimate Question of Life, the Universe, and
    // Everything.”
    return 42;
  }
};

int main(int argc, char *argv[]) {
  if (!std::cin || !std::cout) {
    std::abort();
  }

#ifdef BOOST_FOUND__
  po::options_description desc("Available options");
  desc.add_options()("help,h", "Print this help message")("verbose,v", "Output verbose");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
#endif

  std::size_t n{}, m{};

  std::cin >> m >> n;

  if (n == 0 || m == 0) {
    std::abort();
  }

#ifdef BOOST_FOUND__
  std::vector<int> vec{};
  bool verbose = vm.count("verbose");

  if (verbose) {
    vec.reserve(n);
  }

#endif
  caches::lfu_t<int, int> cache{m};
  slow_getter_t g{};

  for (unsigned i = 0; i < n; i++) {
    if (!std::cin || !std::cout) {
      std::abort();
    }

    int temp{};
    std::cin >> temp;

    if (std::cin.fail()) {
      std::abort();
    }
    
    cache.lookup(temp, g);
#ifdef BOOST_FOUND__
    if (verbose) {
      vec.push_back(temp);
    }
#endif
  }

#ifdef BOOST_FOUND__
  if (verbose) {
    auto optimal_hits = caches::get_optimal_hits<int>(m, vec.begin(), vec.end());
    std::cout << "LFU hits: " << cache.get_hits() << "\nMaximum possible hits: " << optimal_hits << "\n";
    return 0;
  }
#endif

  std::cout << cache.get_hits() << std::endl;
}