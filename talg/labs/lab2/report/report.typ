#import "@local/poly-report:0.1.0": report, report-outline
#import "figures/clock-automaton.typ": clock-automaton

#show: report
#set document(title: "Лабораторная работа № 2. Синтез электронных часов")
#show table: set par(justify: false, first-line-indent: 0pt)

#set page(numbering: none)
#include "tex/pages/title.typ"

#set page(numbering: "1")
#counter(page).update(1)
#report-outline()
#pagebreak()

#include "sections/task-statement.typ"
#pagebreak(weak: true)
#include "sections/controller.typ"

#page("a3", flipped: true, margin: 15mm, numbering: "1")[
  #metadata(none) <fig:clock-automaton>
  #align(horizon, clock-automaton())
]
