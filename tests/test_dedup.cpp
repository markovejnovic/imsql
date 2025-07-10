/// @note AI generated
#include "imsqlite/dsalg/views/dedup.hpp"
#include <gtest/gtest.h>
#include <ranges>
#include <vector>
#include <list>
#include <algorithm>

using imsql::vw::dedup;

TEST(DedupViewTest, RemovesConsecutiveDuplicatesVector) {
  // Test with consecutive duplicates interleaved with non-duplicate values.
  std::vector<int> input{1, 1, 2, 2, 2, 3, 3, 1};
  auto result = input | dedup;
  std::vector<int> output(std::from_range, result);
  std::vector<int> expected{1, 2, 3, 1};

  EXPECT_EQ(output, expected);
}

TEST(DedupViewTest, AllUniqueVector) {
  // For input with all unique values, output should be the same.
  std::vector<int> input{1, 2, 3, 4};
  auto result = input | dedup;
  std::vector<int> output(std::from_range, result);

  EXPECT_EQ(output, input);
}

TEST(DedupViewTest, AllDuplicatesVector) {
  // For a sequence with all duplicates, output should only contain a single element.
  std::vector<int> input{5, 5, 5, 5};
  auto result = input | dedup;
  std::vector<int> output(std::from_range, result);
  std::vector<int> expected{5};

  EXPECT_EQ(output, expected);
}

TEST(DedupViewTest, EmptyInput) {
  // Ensure an empty container produces an empty view.
  std::vector<int> input{};
  auto result = input | dedup;
  std::vector<int> output(std::from_range, result);

  EXPECT_TRUE(output.empty());
}

TEST(DedupViewTest, SingleElement) {
  // A single element should remain unchanged.
  std::vector<int> input{42};
  auto result = input | dedup;
  std::vector<int> output(std::from_range, result);
  std::vector<int> expected{42};

  EXPECT_EQ(output, expected);
}

TEST(DedupViewTest, WorksWithList) {
  // Confirm that it works with other containers, e.g., std::list.
  std::list<int> input{1, 1, 2, 3, 3, 4};
  auto result = input | dedup;
  // Convert the result to a vector for easy comparison.
  std::vector<int> output(std::from_range, result);
  std::vector<int> expected{1, 2, 3, 4};

  EXPECT_EQ(output, expected);
}

TEST(DedupViewTest, NonConsecutiveDuplicatesRemain) {
  std::vector<int> input{1, 2, 1, 2, 1};
  auto result = input | dedup;
  std::vector<int> output(std::from_range, result);
  std::vector<int> expected{1, 2, 1, 2, 1}; // no deduping happens

  EXPECT_EQ(output, expected);
}

TEST(DedupViewTest, LazinessBehavior) {
  int eval_count = 0;
  std::vector<int> input{1, 1, 2, 2, 3, 3};

  auto lazy = input
    | std::views::transform([&](int x) {
        ++eval_count;
        return x;
      })
    | dedup;

  auto it = lazy.begin();
  EXPECT_EQ(*it, 1);
  EXPECT_GT(eval_count, 0); // Confirm something was evaluated
  EXPECT_LT(eval_count, static_cast<int>(input.size())); // But not everything
}

TEST(DedupViewTest, ViewCanBeIteratedTwice) {
  std::vector<int> input{1, 1, 2, 2, 3};

  auto view = input | dedup;

  std::vector<int> first(std::from_range, view);
  std::vector<int> second(std::from_range, view);

  EXPECT_EQ(first, second);
}

struct MyType {
  int id;
  friend bool operator==(const MyType& a, const MyType& b) {
    return a.id == b.id;
  }
};

TEST(DedupViewTest, WorksWithUserDefinedType) {
  std::vector<MyType> input{{1}, {1}, {2}, {2}, {3}};
  auto result = input | dedup;

  std::vector<int> ids;
  for (const auto& val : result) {
    ids.push_back(val.id);
  }

  EXPECT_EQ(ids, std::vector<int>({1, 2, 3}));
}
