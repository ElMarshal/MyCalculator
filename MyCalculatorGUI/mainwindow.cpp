#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <functional>

#include "exlib\array.h"
#include "exlib\tokenizer.h"
#include <exlib/math_expression.h>


Real solve_math_expression_str(const char* str);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // bind button callbacks
    connect(ui.btn_0, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "0"));
    connect(ui.btn_1, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "1"));
    connect(ui.btn_2, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "2"));
    connect(ui.btn_3, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "3"));
    connect(ui.btn_4, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "4"));
    connect(ui.btn_5, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "5"));
    connect(ui.btn_6, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "6"));
    connect(ui.btn_7, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "7"));
    connect(ui.btn_8, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "8"));
    connect(ui.btn_9, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "9"));

    connect(ui.btn_dot, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "."));
    connect(ui.btn_exp, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "e"));

    connect(ui.btn_e, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "e"));
    connect(ui.btn_pi, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "pi"));

    connect(ui.btn_plus, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "+"));
    connect(ui.btn_minus, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "-"));
    connect(ui.btn_mul, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "*"));
    connect(ui.btn_div, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "/"));

    connect(ui.btn_lp, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "("));
    connect(ui.btn_rp, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, ")"));

    connect(ui.btn_sin, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "sin("));
    connect(ui.btn_cos, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "cos("));

    connect(ui.btn_ans, &QPushButton::pressed, this, std::bind(&MainWindow::exp_append_str, this, "ans"));

    connect(ui.btn_del, &QPushButton::pressed, this, std::bind(&MainWindow::exp_delete, this));
    connect(ui.btn_cl, &QPushButton::pressed, this, std::bind(&MainWindow::exp_clear, this));

    connect(ui.btn_equal, &QPushButton::pressed, this, std::bind(&MainWindow::solve_expression, this));


    // return pressed in edit box
    connect(ui.edit_exp, &QLineEdit::returnPressed, this, std::bind(&MainWindow::solve_expression, this));


    // button shortcuts
    ui.btn_0->setShortcut(QKeySequence(Qt::Key_0));
    ui.btn_1->setShortcut(QKeySequence(Qt::Key_1));
    ui.btn_2->setShortcut(QKeySequence(Qt::Key_2));
    ui.btn_3->setShortcut(QKeySequence(Qt::Key_3));
    ui.btn_4->setShortcut(QKeySequence(Qt::Key_4));
    ui.btn_5->setShortcut(QKeySequence(Qt::Key_5));
    ui.btn_6->setShortcut(QKeySequence(Qt::Key_6));
    ui.btn_7->setShortcut(QKeySequence(Qt::Key_7));
    ui.btn_8->setShortcut(QKeySequence(Qt::Key_8));
    ui.btn_9->setShortcut(QKeySequence(Qt::Key_9));

    ui.btn_dot->setShortcut(QKeySequence(Qt::Key_Period));
    ui.btn_e->setShortcut(QKeySequence(Qt::Key_E));

    ui.btn_plus->setShortcut(QKeySequence(Qt::Key_Plus));
    ui.btn_minus->setShortcut(QKeySequence(Qt::Key_Minus));
    ui.btn_mul->setShortcut(QKeySequence(Qt::Key_multiply));
    ui.btn_div->setShortcut(QKeySequence(Qt::Key_division));

    ui.btn_del->setShortcut(QKeySequence(Qt::Key_Delete));
    ui.btn_del->setShortcut(QKeySequence(Qt::Key_Backspace));


    // setup log lable color
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::red);
    ui.lbl_log->setAutoFillBackground(true);
    ui.lbl_log->setPalette(sample_palette);
    ui.lbl_log->clear();

    // initialize variables
    exp_clear();
    m_last_result = 0.0;

    // initialize math symbols
    m_symbols = default_math_symbols();
    m_symbols.constants.add("ans", m_last_result);
}


void MainWindow::solve_expression()
{
    std::string expression = ui.edit_exp->text().toStdString();

    // Tokenize expression
    Tokenizer tokenizer = Tokenizer(expression.c_str());
    Array<Token> tokens = tokenizer.tokenize();
    Array<std::string> errors = tokenizer.errors();

    if (errors.size() > 0)
    {
        ui.lbl_log->setText(QString("- Parsing error: %1").arg(QString::fromUtf8(errors[0].c_str())));
        return;
    }

    // update ans
    if (m_symbols.constants.find("ans"))
    {
        m_symbols.constants.find("ans")->value = m_last_result;
    }
    else
    {
        m_symbols.constants.add("ans", m_last_result);
    }

    // Solve expression
    MathExpression math_exp = MathExpression(tokens);
    Real result = math_exp.solve(&m_symbols);
    errors = math_exp.errors();
    if (errors.size() > 0)
    {
        ui.lbl_log->setText(QString("- Expression error: %1").arg(QString::fromUtf8(errors[0].c_str())));
        return;
    }

    m_last_result = result;

    ui.lbl_result->setText(QString("= %1").arg(result));
    ui.lbl_log->clear();
}

void MainWindow::exp_append_str(const QString& str)
{
    ui.edit_exp->setText(ui.edit_exp->text() + str);
}

void MainWindow::exp_append_real(Real num)
{
    exp_append_str(QString("%1").arg(num));
}

void MainWindow::exp_append_int(int num)
{
    exp_append_str(QString("%1").arg(num));
}

void MainWindow::exp_delete()
{
    QString str = ui.edit_exp->text();
    if(str.length() > 0)
    {
        str.remove(str.length()-1, 1);
    }
    ui.edit_exp->setText(str);
}

void MainWindow::exp_clear()
{
    ui.edit_exp->clear();
}

