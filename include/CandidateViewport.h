#pragma once
#include <algorithm>
#include <cstddef>
#include <string>

namespace weasel {
// Preview several engine pages without changing the page used by number keys.
struct CandidateViewport {
  bool expanded = false;
  int start = 0, count = 0;
  std::string input;

  static int Rows(int requested, int columns, int limit) {
    if (columns <= 0 || columns > limit)
      return 1;
    return std::clamp(requested, 1, limit / columns);
  }
  void Reset() { *this = CandidateViewport(); }
  void SyncInput(const std::string& value) {
    if (input != value) {
      Reset();
      input = value;
    }
  }
  // Stay within the visible grid; horizontal movement never wraps a row.
  int Move(int selected, int columns, int dx, int dy) const {
    int local = selected - start;
    int target = local + dx + dy * columns;
    if (local < 0 || local >= count || target < 0 || target >= count ||
        local / columns != (local + dx) / columns || local + dx < 0)
      return selected;
    return start + target;
  }
  int PageStart(bool backward, int columns, int rows) const {
    return (std::max)(0, start + (backward ? -1 : 1) * columns * rows);
  }
  int WindowStart(int page, int columns, int rows) const {
    return (expanded ? page / rows * rows : page) * columns;
  }
  bool GlobalIndex(size_t displayed, size_t& global) const {
    if (displayed >= static_cast<size_t>(count))
      return false;
    global = start + displayed;
    return true;
  }
};
}  // namespace weasel
