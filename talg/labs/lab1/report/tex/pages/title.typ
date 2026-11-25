// Титульный лист воспроизводит расположение блоков исходного отчёта.
#import "@local/poly-report:0.1.0": large, latex-size

#set text(size: latex-size.normalsize)
#v(12mm)
#align(center)[
  Министерство образования и науки Российской Федерации \
  Санкт-Петербургский политехнический университет Петра Великого

  #v(4mm)
  Институт компьютерных наук и кибербезопасности \
  Высшая школа технологий искусственного интеллекта \
  Направление: 02.03.01 «Математика и компьютерные науки»
]

#v(32mm)
#align(center, large[
  Отчёт о выполнении лабораторной работы №1 \
  По дисциплине: «Теория алгоритмов» \
  На тему: «Реализация клеточного автомата» \
  Вариант 37
])

#v(46.5mm)
#let signature-line = box(width: 35mm, height: 0.7em, stroke: (bottom: 0.5pt))[]
#grid(
  columns: (1fr,),
  row-gutter: 9mm,
  grid(
    columns: (1fr, 38mm, auto),
    align: (left, center + horizon, left + horizon),
    [Выполнил студент \ группы 5130201/40101],
    signature-line,
    [Семенов И. А.],
  ),
  grid(
    columns: (1fr, 38mm, auto),
    align: (left, center + horizon, left + horizon),
    [Проверил \ преподаватель],
    signature-line,
    [Востров А. В.],
  ),
)

#v(17.5mm)
#align(right)[«#box(width: 9mm, height: 0.7em, stroke: (bottom: 0.5pt))[]» #box(width: 35mm, height: 0.7em, stroke: (bottom: 0.5pt))[] 2026 г.]
#v(1fr)
#align(center)[Санкт-Петербург \ 2026]
#v(-7mm)
#pagebreak()

