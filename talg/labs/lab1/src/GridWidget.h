#pragma once

#include <QWidget>

class CellularAutomaton;

class GridWidget final : public QWidget {
    Q_OBJECT

public:
    explicit GridWidget(QWidget* parent = nullptr);
    void setAutomaton(CellularAutomaton* automaton);

signals:
    void cellEdited();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    QSize sizeHint() const override;

private:
    CellularAutomaton* automaton_ = nullptr;
};
