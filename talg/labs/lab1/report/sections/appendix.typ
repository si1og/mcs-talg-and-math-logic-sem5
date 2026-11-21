#heading(numbering: none)[Приложение. Исходный код программы]

#let source(path, lang: "cpp") = {
  block(sticky: true)[*#raw(path)*]
  set text(size: 8.7pt)
  raw(read("../../" + path).trim(), lang: lang, block: true)
}

#source("src/main.cpp")
#source("src/CellularAutomaton.h")
#source("src/CellularAutomaton.cpp")
#source("src/GridWidget.h")
#source("src/GridWidget.cpp")
#source("src/MainWindow.h")
#source("src/MainWindow.cpp")
#source("tools/AutomatonAnalysis.cpp")
#source("tests/automaton_tests.cpp")
#block(breakable: false)[#source("CMakeLists.txt", lang: "cmake")]
