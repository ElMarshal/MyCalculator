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

    // Tokenize expression
    Tokenizer tokenizer = Tokenizer(expression.c_str());
    Array<Token> tokens = tokenizer.tokenize();
    Array<std::string> errors = tokenizer.errors();

    if (errors.size() > 0)
    {
        ui.log_label->setText(QString("- Parsing error: %1").arg(QString::fromUtf8(errors[0].c_str())));
        return;
    }

    // Solve expression
    MathExpression math_exp = MathExpression(tokens);
    Real result = math_exp.solve();
    errors = math_exp.errors();
    if (errors.size() > 0)
    {
        ui.log_label->setText(QString("- Expression error: %1").arg(QString::fromUtf8(errors[0].c_str())));
        return;
    }

    ui.lineEdit->setText(QString("%1").arg(result));
}


