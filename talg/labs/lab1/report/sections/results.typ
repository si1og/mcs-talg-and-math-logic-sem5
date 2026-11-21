#let screenshot(path, label) = align(center, stack(
  dir: ttb,
  spacing: 1.5mm,
  image("../figures/" + path, width: 100%),
  text(size: 10pt, label),
))

#let screenshots(caption, ..frames) = figure(
  grid(
    columns: (1fr, 1fr),
    column-gutter: 4mm,
    row-gutter: 3mm,
    ..frames.pos(),
  ),
  kind: image,
  supplement: [Рисунок],
  caption: caption,
)

= Результат работы программы

== Случайные начальные условия

Поле $10 times 10$ заполнено кнопкой «Случайно» при заданной вероятности
живой клетки 0,5. На рисунке~#ref(<fig:random-start>, supplement: none)
показаны начальное поле и три последовательных шага.
Число живых клеток меняется с 54 до 31, затем до 41 и 40.

#screenshots(
  [Первые три итерации на случайном поле],
  screenshot("random-0.png", [а) $t = 0$ (начальное поле)]),
  screenshot("random-1.png", [б) $t = 1$]),
  screenshot("random-2.png", [в) $t = 2$]),
  screenshot("random-3.png", [г) $t = 3$]),
) <fig:random-start>

#pagebreak(weak: true)
== Ручные начальные условия

На пустом поле $10 times 10$ кликами мыши задана фигура из четырёх
живых клеток в форме буквы Г. На рисунке~#ref(<fig:manual-start>,
supplement: none) видно, как фигура меняется за три шага.
К третьему поколению живых клеток становится шесть.

#screenshots(
  [Первые три итерации на поле, заданном вручную],
  screenshot("manual-0.png", [а) $t = 0$ (начальное поле)]),
  screenshot("manual-1.png", [б) $t = 1$]),
  screenshot("manual-2.png", [в) $t = 2$]),
  screenshot("manual-3.png", [г) $t = 3$]),
) <fig:manual-start>

#pagebreak(weak: true)
== Пресеты с различными типами поведения

В списке «Режим поведения» доступны четыре пресета, иллюстрирующие
устойчивую, периодическую, хаотическую и сложную локальную динамику.
Выбранная конфигурация задаётся кнопкой «Загрузить пресет».
Эволюция этих конфигураций показана на рисунках~#ref(<fig:preset-stationary>, supplement: none)–#ref(<fig:preset-cross>, supplement: none).

*Пресет I. Неподвижная структура.* На поле $30 times 30$ все 15 живых
клеток центрального столбца остаются на своих местах после 100 шагов.

#screenshots(
  [Неподвижная структура],
  screenshot("preset-1-0.png", [а) $t = 0$]),
  screenshot("preset-1-100.png", [б) $t = 100$]),
) <fig:preset-stationary>

*Пресет II. Периодическая структура.* Одиночная клетка на поле
$25 times 25$ за первый шаг порождает пару живых клеток.
Пара смещается по полю. В поколении 26 повторяется конфигурация
поколения 1, период равен 25.

#screenshots(
  [Смещение пары живых клеток],
  screenshot("preset-2-1.png", [а) $t = 1$]),
  screenshot("preset-2-2.png", [б) $t = 2$]),
) <fig:preset-periodic>

#pagebreak(weak: true)
*Пресет III. Хаотическое поле.* Поле $25 times 25$ заполнено с вероятностью
0,5 и фиксированным `seed = 1`. К 100-му поколению число живых клеток
уменьшается с 328 до 284. За первые 5000 шагов повторов конфигурации
не обнаружено.

#screenshots(
  [Изменение хаотического поля],
  screenshot("preset-3-0.png", [а) $t = 0$]),
  screenshot("preset-3-100.png", [б) $t = 100$]),
) <fig:preset-chaotic>

*Пресет IV. Сложная локальная структура.* Начальная конфигурация
на поле $25 times 25$ представляет собой крест из пяти живых клеток.
К 30-му поколению структура распадается на группы с пустыми участками
между ними. Число живых клеток увеличивается до 28.

#screenshots(
  [Развитие структуры из пяти клеток],
  screenshot("preset-4-0.png", [а) $t = 0$]),
  screenshot("preset-4-30.png", [б) $t = 30$]),
) <fig:preset-cross>
