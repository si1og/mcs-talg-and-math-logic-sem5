#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

class CellularAutomaton {
public:
    using Cell = std::uint8_t;
    using Grid = std::vector<Cell>;

    static constexpr std::uint32_t Rule = 55'518'056u;

    explicit CellularAutomaton(std::size_t size = 30);

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::uint64_t generation() const noexcept;
    [[nodiscard]] bool cell(std::size_t row, std::size_t column) const;
    [[nodiscard]] const Grid& cells() const noexcept;

    void resize(std::size_t size);
    void clear() noexcept;
    void setCell(std::size_t row, std::size_t column, bool alive);
    void toggleCell(std::size_t row, std::size_t column);
    void randomize(double aliveProbability = 0.5);
    void randomize(double aliveProbability, std::uint32_t seed);
    void step();
    void run(std::size_t iterations);

    [[nodiscard]] static bool transition(bool top, bool left, bool center,
                                         bool right, bool bottom) noexcept;

private:
    std::size_t size_;
    std::uint64_t generation_ = 0;
    Grid cells_;
    std::mt19937 randomEngine_;

    [[nodiscard]] std::size_t index(std::size_t row, std::size_t column) const;
    void validateCoordinates(std::size_t row, std::size_t column) const;
    void fillRandom(double aliveProbability);
};
