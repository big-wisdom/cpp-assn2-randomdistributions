#include <iostream>
#include <random>
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

// UNIFORM DISTRIBUTION
std::vector<DistributionPair>
generateUniformDistribution(std::uint32_t howMany, std::uint32_t min,
                            std::uint32_t max, std::uint8_t numberBins)
{
    // Generate bins first
    std::vector<DistributionPair> bins;
    std::uint32_t binSize = (max - min) / numberBins;
    std::uint32_t previousMax = min;
    for (std::uint32_t i = min + binSize; i <= max; i += binSize)
    {
        bins.push_back(DistributionPair(previousMax, i));
        previousMax = i;
    }

    for (DistributionPair dp : bins)
    {
        std::cout << dp.maxValue << std::endl;
    }
    std::cout << std::endl;

    // Then populate bins with random numbers
    std::random_device rd;
    std::cout << "Entropy " << rd.entropy() << std::endl;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<unsigned int> distInt(min, max);
    for (std::uint32_t i = 0; i < howMany; i++)
    {
        auto rn = distInt(engine);
        auto index = (rn - min) / binSize; // find the index of the bin

        if (index >= numberBins) // correct for if it's the max or above
        {
            index = numberBins - 1;
        }

        bins[index].count += 1;
    }

    std::cout << std::endl << "Bins" << std::endl;
    for (DistributionPair dp : bins)
    {
        std::cout << dp.count << std::endl;
    }
    return bins;
}

// NORMAL DISTRIBUTION
std::vector<DistributionPair>
generateNormalDistribution(std::uint32_t howMany, float mean, float stdev,
                           std::uint8_t numberBins);
// POISSON DISTRIBUTION
std::vector<DistributionPair>
generatePoissonDistribution(std::uint32_t howMany, std::uint8_t howOften,
                            std::uint8_t numberBins);

int main() { generateUniformDistribution(10, 10, 30, 5); }
