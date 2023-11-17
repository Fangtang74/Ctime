#include "cuckoofilter.h"

#include <assert.h>
#include <math.h>

#include <sys/time.h>
#include <iostream>
#include <vector>

using cuckoofilter::CuckooFilter;

int main(int argc, char **argv) {
  size_t total_items = 1000000;

  // Create a cuckoo filter where each item is of type size_t and
  // use 12 bits for each item:
  //    CuckooFilter<size_t, 12> filter(total_items);
  // To enable semi-sorting, define the storage of cuckoo filter to be
  // PackedTable, accepting keys of size_t type and making 13 bits
  // for each key:
  //   CuckooFilter<size_t, 13, cuckoofilter::PackedTable> filter(total_items);
  CuckooFilter<size_t, 12> filter(total_items);

  // Insert items to this cuckoo filter
  size_t num_inserted = 0;
  for (size_t i = 0; i < total_items; i++, num_inserted++) {
    if (filter.Add(i) != cuckoofilter::Ok) {
      break;
    }
  }

  struct timeval start, end;
  double sum = 0;
  // Check if previously inserted items are in the filter, expected
  // true for all items
  for (size_t i = 0; i < 1000; i++) {
    gettimeofday(&start, NULL);
    assert(filter.Contain(i) == cuckoofilter::Ok);
    gettimeofday(&end, NULL);
    double t =
        1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    sum += t;
  }
  sum = sum * 0.000001;
  std::cout << "==========Cuckoo Filter Finder Time==========" << std::endl;
  std::cout << std::endl;
  std::cout << "Cuckoo filter runs in ................." << sum << " ms"
            << std::endl;
  std::cout << std::endl;
  // Check non-existing items, a few false positives expected
  // size_t total_queries = 0;
  // size_t false_queries = 0;
  // for (size_t i = total_items; i < 2 * total_items; i++) {
  //   if (filter.Contain(i) == cuckoofilter::Ok) {
  //     false_queries++;
  //   }
  //   total_queries++;
  // }

  // Output the measured false positive rate
  // std::cout << "false positive rate is "
  //           << 100.0 * false_queries / total_queries << "%\n";

  return 0;
}
