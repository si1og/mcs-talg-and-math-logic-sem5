#include "MainWindow.h"

#include "GridWidget.h"

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
      probabilityInput_(new QDoubleSpinBox), statusLabel_(new QLabel),
      timer_(new QTimer(this)), animationButton_(new QPushButton("Запустить")) {
    setWindowTitle("Клеточный автомат — правило 55 518 056");

    grid_->setAutomaton(&automaton_);
    sizeInput_->setRange(2, 300);
    sizeInput_->setValue(static_cast<int>(automaton_.size()));
    iterationsInput_->setRange(1, 1'000'000);
    iterationsInput_->setValue(100);
    probabilityInput_->setRange(0.0, 1.0);
    probabilityInput_->setSingleStep(0.05);
    probabilityInput_->setValue(0.5);

    auto* applySizeButton = new QPushButton("Создать поле");
    auto* randomButton = new QPushButton("Случайно");
    auto* clearButton = new QPushButton("Очистить");
    auto* stepButton = new QPushButton("Один шаг");
    auto* runButton = new QPushButton("Выполнить N шагов");

    auto* settings = new QFormLayout;
    settings->addRow("Размер поля:", sizeInput_);
    settings->addRow("Число итераций:", iterationsInput_);
    settings->addRow("Вероятность живой клетки:", probabilityInput_);

    auto* buttons = new QVBoxLayout;
    buttons->addWidget(applySizeButton);
    buttons->addWidget(randomButton);
    buttons->addWidget(clearButton);
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
    timer_->stop();
    animationButton_->setText("Запустить");
    automaton_.resize(static_cast<std::size_t>(sizeInput_->value()));
    grid_->update();
    updateStatus();
}

void MainWindow::randomize() {
    automaton_.randomize(probabilityInput_->value());
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
    statusLabel_->setText(QString("Поколение: %1\nЖивых клеток: %2")
                              .arg(automaton_.generation())
                              .arg(alive));
}
