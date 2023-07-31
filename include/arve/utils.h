#pragma once
#include <algorithm>
#include <cstddef>
namespace arve {
template <typename Container> bool allEqual(const Container &c) {
    return (!c.empty() &&
            std::all_of(begin(c), end(c),
                        [c](const typename Container::value_type &element) {
                            return element == c.front();
                        }));
}
} // namespace arve