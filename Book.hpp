#ifndef BOOK_HPP
#define BOOK_HPP

#include <QString>
#include <QFile>
#include <QTextStream>

struct Book {
    QString author, title, code, ratio, type, genres;

    Book(QString a, QString t, QString c, QString r, QString ty, QString g)
        : author(a), title(t), code(c), ratio(r), type(ty), genres(g) {}

    void save() {
        QFile file("result.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << author << " | " << title << " | " << code << " | " << ratio
                << " | " << type << " | " << genres << "\n";
            file.close();
        }
    }
};

#endif
