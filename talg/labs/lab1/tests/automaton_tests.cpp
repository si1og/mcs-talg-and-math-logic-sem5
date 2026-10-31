#include "CellularAutomaton.h"

#include <cassert>
#include <cstdint>
#include <iostream>

namespace {
void testTruthTable() {
    for (std::uint32_t value = 0; value < 32; ++value) {
        const bool actual = CellularAutomaton::transition(
            value & 16u, value & 8u, value & 4u, value & 2u, value & 1u);
        assert(actual == (((CellularAutomaton::Rule >> value) & 1u) != 0));
    }
}

void testToroidalBoundary() {
    CellularAutomaton automaton(3);
    automaton.setCell(0, 0, true);
    automaton.setCell(0, 2, true);
    automaton.step();

    // Для (0, 2) правая клетка — (0, 0) из-за склейки границ.
    // Вместе с живым центром это 00110₂ = 6, а бит 6 правила равен 1.
    // Без тороидальной склейки комбинация была бы 00100₂, чей бит равен 0.
    assert(automaton.cell(0, 2));
    assert(automaton.generation() == 1);
}

void testSynchronousUpdate() {
    CellularAutomaton automaton(5);
    automaton.setCell(2, 2, true);
    automaton.run(2);
    assert(automaton.generation() == 2);
    assert(automaton.cells().size() == 25);
}

void testSeededRandomization() {
    CellularAutomaton first(20);
    CellularAutomaton second(20);
    first.randomize(0.42, 12345);
    second.randomize(0.42, 12345);
    assert(first.cells() == second.cells());
}
} // namespace

int main() {
    testTruthTable();
    testToroidalBoundary();
    testSynchronousUpdate();
    testSeededRandomization();
    std::cout << "All automaton tests passed\n";
}
