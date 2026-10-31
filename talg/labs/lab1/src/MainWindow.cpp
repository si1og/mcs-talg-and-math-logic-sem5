#include "MainWindow.h"

#include "GridWidget.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), automaton_(30), grid_(new GridWidget),
      sizeInput_(new QSpinBox), iterationsInput_(new QSpinBox),
      densityInput_(new QDoubleSpinBox), presetInput_(new QComboBox),
      statusLabel_(new QLabel), timer_(new QTimer(this)),
      animationButton_(new QPushButton("Запустить")) {
    setWindowTitle("клеточный автомат");

    grid_->setAutomaton(&automaton_);
    sizeInput_->setRange(2, 300);
    sizeInput_->setValue(static_cast<int>(automaton_.size()));
    iterationsInput_->setRange(1, 1'000'000);
    iterationsInput_->setValue(100);
    densityInput_->setRange(0.0, 1.0);
    densityInput_->setSingleStep(0.05);
    densityInput_->setValue(0.5);
    presetInput_->addItem("I — неподвижная структура");
    presetInput_->addItem("II — периодическая структура");
    presetInput_->addItem("III — хаотическое поле");
    presetInput_->addItem("IV — сложная локальная структура");

    auto* applySizeButton = new QPushButton("Создать поле");
    auto* presetButton = new QPushButton("Загрузить пресет");
    auto* randomButton = new QPushButton("Случайно");
    auto* clearButton = new QPushButton("Очистить");
    auto* stepButton = new QPushButton("Один шаг");
    auto* runButton = new QPushButton("Выполнить N шагов");

    auto* settings = new QFormLayout;
    settings->addRow("Размер поля:", sizeInput_);
    settings->addRow("Число итераций:", iterationsInput_);
    settings->addRow("Ожидаемая начальная плотность:", densityInput_);
    settings->addRow("Режим поведения:", presetInput_);

    auto* buttons = new QVBoxLayout;
    buttons->addWidget(applySizeButton);
    buttons->addWidget(randomButton);
    buttons->addWidget(clearButton);
    buttons->addSpacing(12);
    buttons->addWidget(presetButton);
    buttons->addSpacing(12);
    buttons->addWidget(stepButton);
    buttons->addWidget(runButton);
    buttons->addWidget(animationButton_);
    buttons->addStretch();
    buttons->addWidget(new QLabel("ЛКМ по клетке — изменить состояние"));
    buttons->addWidget(statusLabel_);

    auto* controls = new QGroupBox("Управление");
    auto* controlsLayout = new QVBoxLayout(controls);
    controlsLayout->addLayout(settings);
    controlsLayout->addLayout(buttons);

    auto* layout = new QHBoxLayout;
    layout->addWidget(grid_, 1);
    layout->addWidget(controls);
    auto* central = new QWidget;
    central->setLayout(layout);
    setCentralWidget(central);

    timer_->setInterval(120);
    connect(applySizeButton, &QPushButton::clicked, this, &MainWindow::applySize);
    connect(presetButton, &QPushButton::clicked, this, &MainWindow::loadPreset);
    connect(randomButton, &QPushButton::clicked, this, &MainWindow::randomize);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearField);
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::makeStep);
    connect(runButton, &QPushButton::clicked, this, &MainWindow::runIterations);
    connect(animationButton_, &QPushButton::clicked, this, &MainWindow::toggleAnimation);
    connect(timer_, &QTimer::timeout, this, &MainWindow::animationStep);
    connect(grid_, &GridWidget::cellEdited, this, &MainWindow::updateStatus);

    updateStatus();
    resize(960, 700);
}

void MainWindow::applySize() {
    resetField(static_cast<std::size_t>(sizeInput_->value()));
    grid_->update();
    updateStatus();
}

void MainWindow::loadPreset() {
    switch (presetInput_->currentIndex()) {
    case 0: {
        resetField(30);
        const std::size_t column = automaton_.size() / 2;
        for (std::size_t row = 0; row < automaton_.size(); row += 2) {
            automaton_.setCell(row, column, true);
        }
        iterationsInput_->setValue(100);
        break;
    }
    case 1: {
        resetField(25);
        const std::size_t center = automaton_.size() / 2;
        automaton_.setCell(center, center, true);
        iterationsInput_->setValue(25);
        break;
    }
    case 2:
        resetField(25);
        automaton_.randomize(0.5, 1u);
        densityInput_->setValue(0.5);
        iterationsInput_->setValue(5000);
        break;
    case 3: {
        resetField(25);
        const std::size_t center = automaton_.size() / 2;
        automaton_.setCell(center, center, true);
        automaton_.setCell(center - 1, center, true);
        automaton_.setCell(center + 1, center, true);
        automaton_.setCell(center, center - 1, true);
        automaton_.setCell(center, center + 1, true);
        iterationsInput_->setValue(1000);
        break;
    }
    default:
        return;
    }
    grid_->update();
    updateStatus();
}

void MainWindow::randomize() {
    automaton_.randomize(densityInput_->value());
    grid_->update();
    updateStatus();
}

void MainWindow::clearField() {
    automaton_.clear();
    grid_->update();
    updateStatus();
}

void MainWindow::makeStep() {
    automaton_.step();
    grid_->update();
    updateStatus();
}

void MainWindow::runIterations() {
    automaton_.run(static_cast<std::size_t>(iterationsInput_->value()));
    grid_->update();
    updateStatus();
}

void MainWindow::toggleAnimation() {
    if (timer_->isActive()) {
        timer_->stop();
        animationButton_->setText("Запустить");
    } else {
        timer_->start();
        animationButton_->setText("Остановить");
    }
}

void MainWindow::animationStep() { makeStep(); }

void MainWindow::updateStatus() {
    const auto alive = std::count(automaton_.cells().begin(), automaton_.cells().end(),
                                  CellularAutomaton::Cell{1});
    const double density = static_cast<double>(alive) / automaton_.cells().size();
    statusLabel_->setText(QString("Поколение: %1\nЖивых клеток: %2\nПлотность: %3")
                              .arg(automaton_.generation())
                              .arg(alive)
                              .arg(density, 0, 'f', 3));
}

void MainWindow::resetField(const std::size_t size) {
    timer_->stop();
    animationButton_->setText("Запустить");
    automaton_.resize(size);
    sizeInput_->setValue(static_cast<int>(size));
}
