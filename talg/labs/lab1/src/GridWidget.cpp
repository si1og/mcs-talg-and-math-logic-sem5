#include "GridWidget.h"

#include "CellularAutomaton.h"

#include <QMouseEvent>
#include <QPainter>

#include <algorithm>

GridWidget::GridWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(360, 360);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GridWidget::setAutomaton(CellularAutomaton* automaton) {
    automaton_ = automaton;
    update();
}

void GridWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor(248, 250, 252));
    if (!automaton_) return;

    const int fieldSize = static_cast<int>(automaton_->size());
    const double cellWidth = static_cast<double>(width()) / fieldSize;
    const double cellHeight = static_cast<double>(height()) / fieldSize;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(22, 101, 52));
    for (int row = 0; row < fieldSize; ++row) {
        for (int column = 0; column < fieldSize; ++column) {
            if (automaton_->cell(row, column)) {
                painter.drawRect(QRectF(column * cellWidth, row * cellHeight,
                                        cellWidth + 0.5, cellHeight + 0.5));
            }
        }
    }

    if (fieldSize <= 80) {
        painter.setPen(QPen(QColor(203, 213, 225), 0));
        for (int i = 0; i <= fieldSize; ++i) {
            painter.drawLine(QPointF(i * cellWidth, 0), QPointF(i * cellWidth, height()));
            painter.drawLine(QPointF(0, i * cellHeight), QPointF(width(), i * cellHeight));
        }
    }
}

void GridWidget::mousePressEvent(QMouseEvent* event) {
    if (!automaton_ || event->button() != Qt::LeftButton) return;
    const auto fieldSize = automaton_->size();
    const auto column = std::min(static_cast<std::size_t>(event->position().x() * fieldSize / width()),
                                 fieldSize - 1);
    const auto row = std::min(static_cast<std::size_t>(event->position().y() * fieldSize / height()),
                              fieldSize - 1);
    automaton_->toggleCell(row, column);
    update();
    emit cellEdited();
}

QSize GridWidget::sizeHint() const { return {640, 640}; }
