#import "@local/poly-report:0.1.0": report, report-outline

#show: report
#set document(title: "Лабораторная работа № 1. Клеточный автомат")
#show figure.where(kind: table): set align(center)
#show table: set par(justify: false, first-line-indent: 0pt)

#set page(numbering: none)
#include "tex/pages/title.typ"

#set page(numbering: "1")
#counter(page).update(1)
#report-outline()
#pagebreak()

#include "sections/introduction.typ"
#pagebreak(weak: true)
#include "sections/task-statement.typ"
#pagebreak(weak: true)
#include "sections/mathematical-description.typ"
#pagebreak()
#include "sections/implementation.typ"
#pagebreak(weak: true)
#include "sections/results.typ"
#pagebreak(weak: true)
#include "sections/analysis.typ"
#pagebreak(weak: true)
#include "sections/conclusion.typ"
#pagebreak(weak: true)
#include "sections/sources.typ"
#pagebreak(weak: true)
#include "sections/appendix.typ"
