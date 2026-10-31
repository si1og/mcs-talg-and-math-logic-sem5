#include "CellularAutomaton.h"

#include <algorithm>
#include <stdexcept>

CellularAutomaton::CellularAutomaton(const std::size_t size, const std::uint32_t rule)
    : size_(size), rule_(rule), cells_(size * size),
      randomEngine_(std::random_device{}()) {
    if (size == 0) {
        throw std::invalid_argument("Field size must be positive");
    }
    buildTruthTable();
}

std::size_t CellularAutomaton::size() const noexcept { return size_; }

std::uint64_t CellularAutomaton::generation() const noexcept { return generation_; }

std::uint32_t CellularAutomaton::rule() const noexcept { return rule_; }

const CellularAutomaton::TruthTable& CellularAutomaton::truthTable() const noexcept {
    return truthTable_;
}

bool CellularAutomaton::cell(const std::size_t row, const std::size_t column) const {
    validateCoordinates(row, column);
    return cells_[index(row, column)] != 0;
}

const CellularAutomaton::Grid& CellularAutomaton::cells() const noexcept { return cells_; }

void CellularAutomaton::resize(const std::size_t size) {
    if (size == 0) {
        throw std::invalid_argument("Field size must be positive");
    }
    size_ = size;
    generation_ = 0;
    cells_.assign(size_ * size_, 0);
}

void CellularAutomaton::clear() noexcept {
    std::fill(cells_.begin(), cells_.end(), 0);
    generation_ = 0;
}

void CellularAutomaton::setCell(const std::size_t row, const std::size_t column,
                                const bool alive) {
    validateCoordinates(row, column);
    cells_[index(row, column)] = static_cast<Cell>(alive);
}

void CellularAutomaton::toggleCell(const std::size_t row, const std::size_t column) {
    setCell(row, column, !cell(row, column));
}

void CellularAutomaton::randomize(const double aliveProbability) {
    fillRandom(aliveProbability);
}

void CellularAutomaton::randomize(const double aliveProbability, const std::uint32_t seed) {
    randomEngine_.seed(seed);
    fillRandom(aliveProbability);
}

void CellularAutomaton::fillRandom(const double aliveProbability) {
    if (aliveProbability < 0.0 || aliveProbability > 1.0) {
        throw std::invalid_argument("Probability must be in [0, 1]");
    }
    std::bernoulli_distribution distribution(aliveProbability);
    for (Cell& value : cells_) {
        value = static_cast<Cell>(distribution(randomEngine_));
    }
    generation_ = 0;
}

void CellularAutomaton::step() {
    Grid next(cells_.size());
    const auto previous = [this](const std::size_t row, const std::size_t column) {
        return cells_[index(row, column)] != 0;
    };

    for (std::size_t row = 0; row < size_; ++row) {
        const std::size_t topRow = (row + size_ - 1) % size_;
        const std::size_t bottomRow = (row + 1) % size_;
        for (std::size_t column = 0; column < size_; ++column) {
            const std::size_t leftColumn = (column + size_ - 1) % size_;
            const std::size_t rightColumn = (column + 1) % size_;
            next[index(row, column)] = static_cast<Cell>(transition(
                previous(row, column), previous(topRow, column),
                previous(bottomRow, column), previous(row, leftColumn),
                previous(row, rightColumn)));
        }
    }
    cells_.swap(next);
    ++generation_;
}

void CellularAutomaton::run(const std::size_t iterations) {
    for (std::size_t i = 0; i < iterations; ++i) {
        step();
    }
}

bool CellularAutomaton::transition(const bool center, const bool top, const bool bottom,
                                   const bool left, const bool right) const noexcept {
    // порядок аргументов: центр, верх, низ, лево, право
    const std::uint32_t combination =
        (static_cast<std::uint32_t>(center) << 4u) |
        (static_cast<std::uint32_t>(top) << 3u) |
        (static_cast<std::uint32_t>(bottom) << 2u) |
        (static_cast<std::uint32_t>(left) << 1u) |
        static_cast<std::uint32_t>(right);
    return truthTable_[combination] != 0;
}

std::size_t CellularAutomaton::index(const std::size_t row,
                                     const std::size_t column) const {
    return row * size_ + column;
}

void CellularAutomaton::buildTruthTable() noexcept {
    for (std::size_t combination = 0; combination < truthTable_.size(); ++combination) {
        truthTable_[combination] =
            static_cast<Cell>((rule_ >> combination) & 1u);
    }
}

void CellularAutomaton::validateCoordinates(const std::size_t row,
                                            const std::size_t column) const {
    if (row >= size_ || column >= size_) {
        throw std::out_of_range("Cell coordinates are outside the field");
    }
}
