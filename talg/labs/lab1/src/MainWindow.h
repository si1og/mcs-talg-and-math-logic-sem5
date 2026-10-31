#pragma once

#include "CellularAutomaton.h"

#include <QMainWindow>

class GridWidget;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QTimer;
class QPushButton;

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void applySize();
    void loadPreset();
    void randomize();
    void clearField();
    void makeStep();
    void runIterations();
    void toggleAnimation();
    void animationStep();
    void updateStatus();

private:
    CellularAutomaton automaton_;
    GridWidget* grid_;
    QSpinBox* sizeInput_;
    QSpinBox* iterationsInput_;
    QDoubleSpinBox* densityInput_;
    QComboBox* presetInput_;
    QLabel* statusLabel_;
    QTimer* timer_;
    QPushButton* animationButton_;

    void resetField(std::size_t size);
};
