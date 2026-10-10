#include "../../include/CandidateViewport.h"
#include <iostream>

int errors = 0;
#define CHECK(condition)                                          \
  do {                                                            \
    if (!(condition)) {                                           \
      std::cerr << __LINE__ << ": " << #condition << " failed\n"; \
      ++errors;                                                   \
    }                                                             \
  } while (false)

int main() {
  using View = weasel::CandidateViewport;
  CHECK(View::Rows(0, 5, 100) == 1);
  CHECK(View::Rows(5, 0, 100) == 1);
  CHECK(View::Rows(5, 101, 100) == 1);
  CHECK(View::Rows(100, 9, 100) == 11);
  for (int columns : {1, 4, 5, 10}) {
    for (int rows : {2, 3, 5}) {
      View view;
      view.SyncInput("ni");
      view.expanded = true;
      CHECK(view.WindowStart(rows - 1, columns, rows) == 0);
      view.start = view.WindowStart(rows, columns, rows);
      view.count = rows * columns;
      size_t global = 0;
      CHECK(view.GlobalIndex(view.count - 1, global));
      CHECK(global == static_cast<size_t>(2 * rows * columns - 1));
      CHECK(!view.GlobalIndex(view.count, global));
      view.count = 1;  // Partially filled final page.
      CHECK(view.GlobalIndex(0, global));
      CHECK(!view.GlobalIndex(1, global));
      view.start = 0;
      view.count = columns * rows;
      CHECK(view.Move(0, columns, -1, 0) == 0);
      CHECK(view.Move(0, columns, 0, -1) == 0);
      CHECK(view.Move(columns - 1, columns, 1, 0) == columns - 1);
      CHECK(view.Move(0, columns, 0, 1) == columns);
      CHECK(view.Move(columns, columns, 0, -1) == 0);
      CHECK(view.Move(view.count - 1, columns, 0, 1) == view.count - 1);
      if (columns > 1) {
        CHECK(view.Move(0, columns, 1, 0) == 1);
        CHECK(view.Move(1, columns, -1, 0) == 0);
      }
      CHECK(view.PageStart(true, columns, rows) == 0);
      CHECK(view.PageStart(false, columns, rows) == columns * rows);
      view.count = columns + 1;
      CHECK(view.Move(view.count - 1, columns, 0, 1) == view.count - 1);
      view.expanded = false;
      CHECK(view.WindowStart(2, columns, rows) == 2 * columns);
      view.expanded = true;
      view.SyncInput("ni");
      CHECK(view.expanded);
      view.SyncInput("nihao");
      CHECK(!view.expanded && view.count == 0);
      View other;
      other.expanded = true;
      CHECK(other.expanded && !view.expanded);
      other.Reset();
      CHECK(!other.expanded && other.input.empty());
    }
  }
  if (!errors)
    std::cout << "Candidate viewport checks passed\n";
  return errors ? 1 : 0;
}
