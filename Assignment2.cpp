#include <iostream>
#include <random>
#include <tuple>
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

// Generate Bins
std::tuple<std::vector<DistributionPair>, std::uint32_t>
generateBins(std::uint32_t max, std::uint32_t min, uint8_t numberBins)
{
    std::vector<DistributionPair> bins;
    std::uint32_t binSize = (max - min) / numberBins;
    std::uint32_t previousMax = min - 1;
    for (std::uint32_t i = min + binSize; i <= max; i += binSize + 1)
    {
        bins.push_back(DistributionPair(previousMax + 1, i));
        previousMax = i;
    }
    return std::make_tuple(bins, binSize);
}

// UNIFORM DISTRIBUTION
std::vector<DistributionPair>
generateUniformDistribution(std::uint32_t howMany, std::uint32_t min,
                            std::uint32_t max, std::uint8_t numberBins)
{
    // Generate bins first
    std::vector<DistributionPair> bins;
    std::uint32_t binSize;
    tie(bins, binSize) = generateBins(max, min, numberBins);

    // Then populate bins with random numbers
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<unsigned int> distInt(min, max);
    for (std::uint32_t i = 0; i < howMany; i++)
    {
        auto rn = distInt(engine);
        auto index = (rn - min) / (binSize + 1); // find the index of the bin

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
                           std::uint8_t numberBins)
{
    // calculate max and min
    std::uint32_t max = mean + (4 * stdev) - 1;
    std::uint32_t min = mean - (4 * stdev);

    // generate bins
    std::vector<DistributionPair> bins;
    std::uint32_t binSize;
    tie(bins, binSize) = generateBins(max, min, numberBins);

    // generate random numbers
    // std::random_device rd;
    // std::mt19937 engine(rd());
    // std::uniform_int_distribution<unsigned int> distInt(min, max);
    // for (std::uint32_t i = 0; i < howMany; i++)
    // {
    // auto rn = distInt(engine);
    // auto index = (rn - min) / binSize; // find the index of the bin

    // if (index >= numberBins) // correct for if it's the max or above
    // {
    // index = numberBins - 1;
    // }

    // bins[index].count += 1;
    // }

    return bins;
}
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
    std::double_t scale =
        (double)maxPlotLineSize / (double)distribution[index].count;

    std::cout << title << std::endl;
    for (DistributionPair dp : distribution)
    {
        std::string stars((uint32_t)(dp.count * scale), '*');
        std::cout << "[" << dp.minValue << ", " << dp.maxValue
                  << "] : " << stars << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    auto uniform = generateUniformDistribution(100000, 0, 79, 40);
    plotDistribution("--- Uniform ---", uniform, 80);

    auto normal = generateNormalDistribution(100000, 50, 5, 40);
    plotDistribution("--- Normal ---", normal, 80);

    // auto poisson = generatePoissonDistribution(100000, 6, 40);
    // plotDistribution("--- Poisson ---", poisson, 80);
}
