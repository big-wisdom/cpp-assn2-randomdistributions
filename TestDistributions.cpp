#include "distributions.hpp"

#include "gtest/gtest.h"
#include <numeric>

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

using Bins = std::vector<DistributionPair>;

Bins generateBins(const std::uint32_t min, const std::uint32_t max,
                  const std::uint8_t numberBins) {
  const auto binRange = (max - min) / numberBins;
  auto minBin = min;
  auto maxBin = min + binRange;

  Bins results;
  results.reserve(numberBins);
  for (auto bin = 0u; bin < numberBins; bin++) {
    results.emplace_back(minBin, maxBin);
    minBin = maxBin + 1;
    maxBin = minBin + binRange;
  }

  return results;
}

void checkBins(const Bins &expected, const Bins &actual) {
  ASSERT_EQ(expected.size(), actual.size()) << "Wrong number of bins";
  for (auto i = 0u; i < expected.size(); i++) {
    EXPECT_EQ(expected[i].minValue, actual[i].minValue)
        << "Wrong minimum value for bin " << i;
    EXPECT_EQ(expected[i].maxValue, actual[i].maxValue)
        << "Wrong maximum value for bin " << i;
  }
}

void checkTotal(const std::uint32_t expected, const Bins &bins) {
  const auto add_counts = [](const std::uint32_t total,
                             const DistributionPair &bin) {
    return total + bin.count;
  };
  const auto actual =
      std::accumulate(bins.cbegin(), bins.cend(), 0u, add_counts);
  EXPECT_EQ(expected, actual) << "Wrong number of elements across all bins";
}

TEST(UniformDistribution, ReturnsExpectedBins) {
  const auto bins = generateUniformDistribution(0, 0, 79, 40);
  checkBins(generateBins(0, 79, 40), bins);
}

TEST(UniformDistribution, HasCorrectTotalAcrossAllBins) {
  const auto bins = generateUniformDistribution(100000, 0, 79, 40);
  checkTotal(100000, bins);
}

TEST(NormalDistribution, ReturnsExpectedBins) {
  const auto bins = generateNormalDistribution(0, 50, 5, 40);
  checkBins(generateBins(30, 69, 40), bins);
}

TEST(NormalDistribution, HasCorrectTotalAcrossAllBins) {
  const auto bins = generateNormalDistribution(100000, 50, 5, 40);
  checkTotal(100000, bins);
}

TEST(PoissonDistribution, ReturnsExpectedBins) {
  const auto bins = generatePoissonDistribution(0, 6, 40);
  checkBins(generateBins(0, 39, 40), bins);
}

TEST(PoissonDistribution, HasCorrectTotalAcrossAllBins) {
  const auto bins = generatePoissonDistribution(100000, 6, 40);
  checkTotal(100000, bins);
}
