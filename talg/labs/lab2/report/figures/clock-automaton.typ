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
  let action(s, input, index, side, idle: none) = {
    let command = output(input, index: index)
    let caption = if idle == none { command } else {
      align(center, stack(dir: ttb, spacing: 2pt, command, output(idle)))
    }
    transition(s, caption, loop: side, size: 15mm)
  }

  align(center, cetz.canvas(length: 1mm, {
    // Вершины основного цикла имеют углы 0°, 90°, 180° и 270°.
    state((49, 0), 0)
    state((0, 49), 6)
    state((-49, 0), 8)
    state((0, -49), 12)
    state((0, 86), 7)

    state((118, 47), 1)
    state((182, 47), 2)
    state((182, -47), 3)
    state((118, -47), 4)
    state((118, 0), 5)

    state((-98, 39), 9)
    state((-160, 39), 10)
    state((-111, -43), 11)

    transition((8, 0), <s0>, snap-to: (none, auto))
    cycle(<s0>, <s6>, output($a$), label-side: right)
    cycle(<s6>, <s8>, output($a$), label-side: left)
    cycle(<s8>, <s12>, output($a$), label-side: left, label-pos: 67%)
    cycle(<s12>, <s0>, output($a$), label-side: right)
    action(<s0>, $b$, 0, top)

    transition(<s6>, <s7>, output($b$, index: 14), bend: 23deg, label-side: left)
    transition(<s7>, <s6>, output($b$, index: 15), bend: 23deg, label-side: right)
    action(<s6>, $c$, 6, right)
    action(<s7>, $a, c$, 0, right)
    action(<s12>, $b$, 11, left, idle: $c$)

    transition(<s0>, <s1>, output($c$, index: 1), label-side: top)
    transition(<s1>, <s2>, output($c$))
    transition(<s2>, <s3>, output($c$), label-pos: 34%)
    transition(<s3>, <s4>, output($c$))
    action(<s1>, $b$, 2, top, idle: $a$)
    action(<s2>, $b$, 3, top, idle: $a$)
    action(<s3>, $b$, 4, bottom, idle: $a$)
    action(<s4>, $b$, 5, bottom, idle: $a$)

    let confirm = value => text(size: 12pt, output($c space [m = #value]$))
    transition(<s4>, <s0>, confirm(0), label-side: bottom, label-angle: auto, label-pos: 36%)
    transition(<s4>, <s5>, confirm(1), label-side: right)
    transition(<s5>, <s0>, output($b$), label-side: bottom)
    transition(<s5>, <s1>, output($c$), label-side: right)
    action(<s5>, $a$, 0, right)

    transition(<s8>, <s9>, output($c$), label-side: top)
    transition(<s9>, <s10>, output($c$))
    transition(<s10>, <s11>, output($c$), label-side: left)
    transition(<s11>, <s8>, output($c$), label-side: top)
    action(<s8>, $b$, 7, bottom + right)
    action(<s9>, $b$, 8, top, idle: $a$)
    action(<s10>, $b$, 9, top, idle: $a$)
    action(<s11>, $b$, 10, bottom, idle: $a$)
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
      [Выбор мелодии будильника],
      [Ежечасный сигнал],
    )),
    legend(z, 0, (
      [Без импульса],
      [Сбросить признак $m$],
      [Увеличить десятки часов на 1],
      [Увеличить единицы часов на 1],
      [Увеличить десятки минут на 1, $m := 1$],
      [Увеличить единицы минут на 1, $m := 1$],
      [Обнулить секундомер],
      [Включить или выключить будильник],
    )),
    legend(z, 8, (
      [Увеличить часы будильника на 1],
      [Увеличить минуты будильника на 1],
      [Выбрать мелодию будильника],
      [Выбрать мелодию ежечасного сигнала],
      [Запустить мелодию будильника],
      [Запустить мелодию ежечасного сигнала],
      [Запустить или продолжить секундомер],
      [Остановить секундомер с сохранением],
    )),
  )

  v(3mm)
  [
    $c$ обозначает совместное нажатие кнопок $a$ и $b$.
    $m$ показывает, нужно ли после корректировки дождаться запуска часов. \
    $z_(12)$ выдаётся в заданное время при включённом будильнике.
    $z_(13)$ выдаётся в начале часа, если в этот момент будильник не сработал.
    При выдаче звуковых команд состояние сохраняется.
  ]
}

#set page("a3", flipped: true, margin: 15mm)
#align(horizon, clock-automaton())
