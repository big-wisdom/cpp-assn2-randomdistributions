#include "distributions.hpp"
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <tuple>

DistributionPair::DistributionPair(std::uint32_t minValue,
                                   std::uint32_t maxValue)
    : minValue(minValue), maxValue(maxValue), count(0) {}


// Generate Bins
std::tuple<std::vector<DistributionPair>, std::uint32_t> myGenerateBins(std::uint32_t max, std::uint32_t min, uint8_t numberBins)
{
    std::vector<DistributionPair> bins;
    std::uint32_t binSize = ((max - min) / numberBins) + 1;
    std::uint32_t previousMax = min - 1;
    for (std::uint32_t i = previousMax + binSize; i <= max; i += binSize)
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
    tie(bins, binSize) = myGenerateBins(max, min, numberBins);

    // Then populate bins with random numbers
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<unsigned int> distInt(min, max);
    for (std::uint32_t i = 0; i < howMany; i++)
    {
        auto rn = distInt(engine);
        auto index = (rn - min) / (binSize); // find the index of the bin

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
    std::uint32_t max = static_cast<std::uint32_t> (mean + (4 * stdev) - 1);
    std::uint32_t min = static_cast<std::uint32_t> (mean - (4 * stdev));

    // generate bins
    std::vector<DistributionPair> bins;
    std::uint32_t binSize;
    tie(bins, binSize) = myGenerateBins(max, min, numberBins);

    // generate random numbers
    std::random_device rd;
    std::mt19937 engine(rd());
    std::normal_distribution<> distNorm{ mean, stdev };
    for (std::uint32_t i = 0; i < howMany; i++)
    {
        auto rn = distNorm(engine);
        int index =
            (unsigned int)((rn - min) / binSize); // find the index of the bin

        if (index >= numberBins) // correct for if it's the max or above
        {
            index = numberBins - 1;
        }

        if (index < 0)
        {
            index = 0;
        }

        bins[index].count += 1;
    }

    return bins;
}

// POISSON DISTRIBUTION
std::vector<DistributionPair>
generatePoissonDistribution(std::uint32_t howMany, std::uint8_t howOften,
                            std::uint8_t numberBins)
{
    std::uint32_t min = 0;
    std::uint32_t max = numberBins - 1;

    std::vector<DistributionPair> bins;
    std::uint32_t binSize;
    tie(bins, binSize) = myGenerateBins(max, min, numberBins);

    // generate random numbers
    std::random_device rd;
    std::mt19937 engine(rd());
    std::poisson_distribution<int> distPoisson(howOften);
    for (std::uint32_t i = 0; i < howMany; i++)
    {
        auto rn = distPoisson(engine);
        int index =
            (unsigned int)((rn - min) / binSize); // find the index of the bin

        if (index >= numberBins) // correct for if it's the max or above
        {
            index = numberBins - 1;
        }

        if (index < 0)
        {
            index = 0;
        }

        bins[index].count += 1;
    }

    return bins;
}


// PLOT
void plotDistribution(std::string title,
                      const std::vector<DistributionPair>& distribution,
                      const std::uint8_t maxPlotLineSize)
{
    // find the largest count
    std::uint32_t max = 0;
    std::uint32_t index = 0;
    for (unsigned int i = 0; i < distribution.size(); i++)
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
