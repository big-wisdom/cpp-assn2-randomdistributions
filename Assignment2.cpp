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

    // Then populate bins with random numbers
    std::random_device rd;
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

// PLOT
void plotDistribution(std::string title,
                      const std::vector<DistributionPair>& distribution,
                      const std::uint8_t maxPlotLineSize)
{
    // find the largest count
    std::uint32_t max = 0;
    std::uint32_t index;
    for (int i = 0; i < distribution.size(); i++)
    {
        if (distribution[i].count > max)
        {
            max = distribution[i].count;
            index = i;
        }
    }

    // find scale to maxPlotLineSize
    std::uint32_t scale = maxPlotLineSize / distribution[index].count;
    for (DistributionPair dp : distribution)
    {
        std::string stars(dp.count * scale, '*');
        std::cout << stars << std::endl;
    }
}

int main()
{
    std::vector<DistributionPair> bins =
        generateUniformDistribution(1000, 0, 100, 10);
    plotDistribution("Uniform Distribution", bins, 150);
}
