#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "exlib\array.h"
#include "exlib\tokenizer.h"
#include <exlib/math_expression.h>


Real solve_math_expression_str(const char* str);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, &QPushButton::pressed, this, &MainWindow::solve_expression);
    ui.pushButton->setShortcut(QKeySequence(Qt::Key_Plus));
}


void MainWindow::solve_expression()
{
    std::string expression = ui.lineEdit->text().toStdString();

    Real result = solve_math_expression_str(expression.c_str());
    ui.lineEdit->setText(QString("%1").arg(result));

}


static Real solve_math_expression_str(const char* str)
{
    Tokenizer tokenizer = Tokenizer(str);
    Array<Token> tokens = tokenizer.tokenize();

    return solve_math_expression(tokens);
}

