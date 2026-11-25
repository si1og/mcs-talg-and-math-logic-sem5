#import "@preview/fletcher:0.6.0": cetz, edge, node

#let clock-automaton() = {
  set text(font: "New Computer Modern", size: 14pt)
  show math.equation: set text(font: "New Computer Modern Math")

  let state(pos, index) = node(
    pos, $S_#index$, name: label("s" + str(index)), shape: "ellipse",
    width: 30mm, height: 13mm, fill: white, stroke: 0.7pt + black,
  )
  let transition = edge.with(marks: "-|>", stroke: 0.7pt + black, label-sep: 3pt)
  let cycle = transition.with(bend: 45deg, stroke: 1.2pt + black)
  let output(input, index: 0) = [#input / $z_#index$]
  let action(s, input, index, side) = transition(s, output(input, index: index), loop: side, size: 15mm)

  align(center, cetz.canvas(length: 1mm, {
    // Вершины основного цикла имеют углы 0°, 90°, 180° и 270°.
    state((49, 0), 0)
    state((0, 49), 6)
    state((-49, 0), 8)
    state((0, -49), 12)
    state((0, 86), 7)

    state((102, 47), 1)
    state((172, 47), 2)
    state((172, -47), 3)
    state((102, -47), 4)
    state((137, -18), 5)
    node((137, 16), $m$, name: <check-minutes>, shape: "diamond",
      width: 11mm, height: 9mm, fill: white, stroke: 0.7pt + black)

    state((-98, 39), 9)
    state((-151, 39), 10)
    state((-111, -43), 11)

    transition((8, 0), <s0>, snap-to: (none, auto))
    cycle(<s0>, <s6>, output($a$), label-side: right)
    cycle(<s6>, <s8>, output($a$), label-side: left)
    cycle(<s8>, <s12>, output($a$), label-side: left)
    cycle(<s12>, <s0>, output($a$), label-side: right)

    transition(<s6>, <s7>, output($b$), bend: 23deg)
    transition(<s7>, <s6>, output($b$), bend: 23deg)
    action(<s6>, $a+b$, 6, right)
    action(<s12>, $b$, 11, left)

    transition(<s0>, <s1>, output($a+b$, index: 1), bend: 23deg, label-pos: 76%)
    transition(<s1>, <s2>, output($a$))
    transition(<s2>, <s3>, output($a$), label-pos: 34%)
    transition(<s3>, <s4>, output($a$))
    transition(<s4>, <s1>, output($a$), label-pos: 28%)
    action(<s1>, $b$, 2, top)
    action(<s2>, $b$, 3, top)
    action(<s3>, $b$, 4, bottom)
    action(<s4>, $b$, 5, bottom)

    let confirm = transition.with(label: output($a+b$), label-angle: auto)
    confirm(<s1>, <check-minutes>, label-side: top)
    confirm(<s2>, <check-minutes>, label-side: top)
    confirm(<s3>, <check-minutes>, label-side: right)
    confirm(<s4>, <check-minutes>, label-side: left, label-pos: 27%)
    transition(<check-minutes>, <s0>, $[m = 0]$, label-pos: 72%,
      label-side: bottom, label-angle: auto, layer: 0, crossing: true)
    transition(<check-minutes>, <s5>, $[m = 1]$, label-side: right, label-angle: auto)
    transition(<s5>, <s0>, output($b$), label-pos: 68%, label-side: bottom,
      layer: 0, crossing: true)

    transition(<s8>, <s9>, output($a+b$), bend: 23deg)
    transition(<s9>, <s8>, output($a+b$), bend: 23deg)
    transition(<s10>, (-151, 15), (-135, 0), <s8>, output($a+b$),
      label-pos: 52%, label-side: bottom, corner-radius: 5mm)
    transition(<s9>, <s10>, output($a$), bend: 23deg)
    transition(<s10>, <s9>, output($a$), bend: 23deg)
    action(<s9>, $b$, 8, top)
    action(<s10>, $b$, 9, top)

    transition(<s8>, <s11>, output($b$), bend: 23deg)
    transition(<s11>, <s8>, output($a+b$), bend: 23deg)
    action(<s11>, $a$, 7, left)
    action(<s11>, $b$, 10, bottom)
  }))

  v(6mm)
  set text(size: 12pt)
  set par(justify: false, first-line-indent: 0pt)

  let legend(symbol, first, entries) = grid(
    columns: (auto, 1fr),
    column-gutter: 2mm,
    row-gutter: 1mm,
    ..entries.enumerate().map(((i, description)) => (symbol(first + i), description)).flatten(),
  )
  let s = i => $S_#i$
  let z = i => $z_#i$

  grid(
    columns: (1fr,) * 4,
    column-gutter: 6mm,
    legend(s, 0, (
      [Текущее время],
      [Коррекция десятков часов],
      [Коррекция единиц часов],
      [Коррекция десятков минут],
      [Коррекция единиц минут],
      [Часы остановлены],
      [Секундомер остановлен],
    )),
    legend(s, 7, (
      [Секундомер работает],
      [Будильник],
      [Установка часов будильника],
      [Установка минут будильника],
      [Выбор мелодии и включение будильника],
      [Ежечасный сигнал],
    )),
    legend(z, 0, (
      [Без импульса],
      [Сбросить признак $m$],
      [Увеличить десятки часов],
      [Увеличить единицы часов],
      [Увеличить десятки минут и установить $m$],
      [Увеличить единицы минут и установить $m$],
      [Обнулить секундомер],
    )),
    legend(z, 7, (
      [Включить или выключить будильник],
      [Увеличить часы будильника],
      [Увеличить минуты будильника],
      [Выбрать мелодию будильника],
      [Выбрать мелодию ежечасного сигнала],
      [Запустить мелодию будильника],
      [Запустить мелодию ежечасного сигнала],
    )),
  )

  v(3mm)
  [Ромб обозначает проверку признака изменения минут $m$ при выходе из корректировки.]
}

#set page("a3", flipped: true, margin: 15mm)
#align(horizon, clock-automaton())
