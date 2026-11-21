#include "CellularAutomaton.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
struct Result {
    std::size_t firstSeen = 0;
    std::size_t period = 0;
    std::size_t measuredAt = 0;
    std::size_t alive = 0;
    double minDensity = 1.0;
    double maxDensity = 0.0;
};

std::string key(const CellularAutomaton& automaton) {
    const auto& cells = automaton.cells();
    return {reinterpret_cast<const char*>(cells.data()), cells.size()};
}

double density(const CellularAutomaton& automaton) {
    const auto alive = std::count(automaton.cells().begin(), automaton.cells().end(),
                                  CellularAutomaton::Cell{1});
    return static_cast<double>(alive) / automaton.cells().size();
}

Result observe(CellularAutomaton automaton, const std::size_t limit,
               std::vector<double>* densities = nullptr) {
    std::unordered_map<std::string, std::size_t> seen;
    Result result;
    for (std::size_t step = 0; step <= limit; ++step) {
        const double currentDensity = density(automaton);
        if (densities) densities->push_back(currentDensity);
        result.minDensity = std::min(result.minDensity, currentDensity);
        result.maxDensity = std::max(result.maxDensity, currentDensity);
        const auto [iterator, inserted] = seen.emplace(key(automaton), step);
        if (!inserted) {
            result.firstSeen = iterator->second;
            result.period = step - iterator->second;
            result.measuredAt = step;
            break;
        }
        result.measuredAt = step;
        if (step != limit) automaton.step();
    }
    result.alive = std::count(automaton.cells().begin(), automaton.cells().end(),
                              CellularAutomaton::Cell{1});
    return result;
}

void print(const std::string& name, const std::size_t size, const Result& result) {
    const double finalDensity = static_cast<double>(result.alive) / (size * size);
    std::cout << std::left << std::setw(20) << name << std::right << std::setw(6) << size
              << std::setw(10) << result.measuredAt << std::setw(10) << result.alive
              << std::setw(12) << std::fixed << std::setprecision(4) << finalDensity
              << std::setw(12) << result.minDensity << std::setw(12) << result.maxDensity;
    if (result.period != 0) {
        std::cout << std::setw(10) << result.firstSeen << std::setw(10) << result.period;
    } else {
        std::cout << std::setw(10) << "-" << std::setw(10) << ">limit";
    }
    std::cout << '\n';
}

void analyzeRandomDensity() {
    constexpr std::size_t fieldSize = 10;
    constexpr std::size_t limit = 10'000;
    constexpr std::uint32_t runs = 20;
    std::cout << "probability,generation,mean_density\n";
    std::cerr << "probability cycles seed cycle_start first_repeat period\n";

    for (const double probability : {0.1, 0.3, 0.5, 0.7, 0.9}) {
        std::vector<double> sums(limit + 1);
        Result latestCycle;
        std::uint32_t latestSeed = 0;
        std::size_t cycles = 0;
        for (std::uint32_t seed = 1; seed <= runs; ++seed) {
            CellularAutomaton automaton(fieldSize);
            automaton.randomize(probability, seed);
            std::vector<double> densities;
            densities.reserve(limit + 1);
            const Result result = observe(automaton, limit, &densities);
            if (result.period != 0) {
                ++cycles;
                if (latestSeed == 0 || result.firstSeen > latestCycle.firstSeen) {
                    latestCycle = result;
                    latestSeed = seed;
                }
                // The first repeated configuration fixes the rest of the trajectory.
                while (densities.size() <= limit) {
                    densities.push_back(densities[densities.size() - result.period]);
                }
            }
            for (std::size_t generation = 0; generation <= limit; ++generation) {
                sums[generation] += densities[generation];
            }
        }
        for (std::size_t generation = 0; generation <= limit; ++generation) {
            std::cout << probability << ',' << generation << ','
                      << sums[generation] / runs << '\n';
        }
        std::cerr << probability << ' ' << cycles;
        if (latestSeed != 0) {
            std::cerr << ' ' << latestSeed << ' ' << latestCycle.firstSeen
                      << ' ' << latestCycle.measuredAt << ' ' << latestCycle.period;
        }
        std::cerr << '\n';
    }
}
} // namespace

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--random-density") {
        analyzeRandomDensity();
        return 0;
    }
    std::cout << std::left << std::setw(20) << "scenario" << std::right
              << std::setw(6) << "size" << std::setw(10) << "steps"
              << std::setw(10) << "alive" << std::setw(12) << "density"
              << std::setw(12) << "min" << std::setw(12) << "max"
              << std::setw(10) << "cycle@" << std::setw(10) << "period" << '\n';

    CellularAutomaton empty(25);
    print("empty", empty.size(), observe(empty, 5000));

    CellularAutomaton full(25);
    for (std::size_t row = 0; row < full.size(); ++row)
        for (std::size_t column = 0; column < full.size(); ++column)
            full.setCell(row, column, true);
    print("full", full.size(), observe(full, 5000));

    CellularAutomaton single(25);
    single.setCell(12, 12, true);
    print("single", single.size(), observe(single, 5000));

    CellularAutomaton plus(25);
    plus.setCell(12, 12, true);
    plus.setCell(11, 12, true);
    plus.setCell(13, 12, true);
    plus.setCell(12, 11, true);
    plus.setCell(12, 13, true);
    print("plus", plus.size(), observe(plus, 5000));

    for (const double probability : {0.1, 0.3, 0.5, 0.7, 0.9}) {
        for (const std::uint32_t seed : {1u, 2u, 3u}) {
            CellularAutomaton random(25);
            random.randomize(probability, seed);
            const std::string name = "random p=" + std::to_string(probability).substr(0, 3)
                                   + " s=" + std::to_string(seed);
            print(name, random.size(), observe(random, 5000));
        }
    }
}
