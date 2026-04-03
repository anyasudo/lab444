#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Book.hpp"
#include <QRegularExpression>
#include <QMessageBox>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

// Кнопка СБРОС (очищает всё, включая колонки) ОБНОВИЛАА
void MainWindow::on_btnReset_clicked() {
    ui->lineAuthor->clear();
    ui->lineTitle->clear();
    ui->lineCode->clear();
    ui->lineRatio->clear();

    ui->radioPaper->setAutoExclusive(false);
    ui->radioElec->setAutoExclusive(false);

    ui->radioPaper->setChecked(false);
    ui->radioElec->setChecked(false);

    ui->radioPaper->setAutoExclusive(true);
    ui->radioElec->setAutoExclusive(true);

    ui->checkDet->setChecked(false);
    ui->checkFan->setChecked(false);
    ui->checkRom->setChecked(false);
}


// Кнопка СОХРАНИТЬ
void MainWindow::on_btnSave_clicked() {
    QString auth = ui->lineAuthor->text();
    QString titl = ui->lineTitle->text();
    QString code = ui->lineCode->text();
    QString rati = ui->lineRatio->text();

    // 1. Валидация текста
    QRegularExpression reAuth("^[А-Я][а-я]+\\s[А-Я]{1,2}$");
    if (!reAuth.match(auth).hasMatch()) {
        QMessageBox::critical(this, "Ошибка", "Автор: Клоц АН (Русское слово + пробел + 1/2 заглавные)");
        return;
    }

    if (!titl.startsWith("\"") || !titl.endsWith("\"") || titl.length() < 3) {
        QMessageBox::critical(this, "Ошибка", "Название должно быть в кавычках \"...\"");
        return;
    }

    QRegularExpression reCode("^\\d{3}-\\d{5}$");
    if (!reCode.match(code).hasMatch()) {
        QMessageBox::critical(this, "Ошибка", "Формат кода: 123-12345");
        return;
    }

    QRegularExpression reRati("^(\\d+)/(\\d+)$");
    auto m = reRati.match(rati);
    if (!m.hasMatch() || m.captured(2).toInt() >= m.captured(1).toInt()) {
        QMessageBox::critical(this, "Ошибка", "Наполнение: Число1/Число2 (Число2 < Число1)");
        return;
    }

    // 2. Сбор данных из колонки "Тип издания"
    QString typeStr = ui->radioPaper->isChecked() ? "Бумажное" : "Электронное";

    // 3. Сбор данных из колонки "Жанры" (может быть выбрано несколько)
    QStringList selectedGenres;
    if (ui->checkDet->isChecked()) selectedGenres << "Детектив";
    if (ui->checkFan->isChecked()) selectedGenres << "Фантастика";
    if (ui->checkRom->isChecked()) selectedGenres << "Роман";

    // Если жанры не выбраны, пишем "Не указано"
    QString genresStr = selectedGenres.isEmpty() ? "Не указано" : selectedGenres.join(", ");

    // 4. Создание объекта и запись в файл
    Book b(auth, titl, code, rati, typeStr, genresStr);
    b.save();

    QMessageBox::information(this, "Успех", "Данные книги сохранены в result.txt");
}
