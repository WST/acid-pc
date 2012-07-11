#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent): QDialog(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    resize(width(), minimumSizeHint().height());
    ui->tabWidget->setCurrentIndex(0);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_buttonBox_accepted() {
    hide();
}
