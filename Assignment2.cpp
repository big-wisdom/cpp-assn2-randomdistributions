#include <iostream>
#include <vector>

class DistributionPair
{
  public:
    DistributionPair(std::uint32_t minValue, std::uint32_t maxValue)
        : minValue(minValue), maxValue(maxValue), count(0)
    {
    }

    std::uint32_t minValue;
    std::uint32_t maxValue;
    std::uint32_t count;
};

std::vector<DistributionPair>
generateUniformDistribution(std::uint32_t howMany, std::uint32_t min,
                            std::uint32_t max, std::uint8_t numberBins)
{
    std::vector<DistributionPair> pairs;
    for (int i = 0; i < howMany; i++)
    {
        pairs.push_back(DistributionPair(min, max));
    }

    for (DistributionPair d : pairs)
    {
        std::cout << d.minValue << std::endl;
    }
    return pairs;
}

std::vector<DistributionPair>
generateNormalDistribution(std::uint32_t howMany, float mean, float stdev,
                           std::uint8_t numberBins);
std::vector<DistributionPair>
generatePoissonDistribution(std::uint32_t howMany, std::uint8_t howOften,
                            std::uint8_t numberBins);

int main() { generateUniformDistribution(10, 21, 30, 5); }
