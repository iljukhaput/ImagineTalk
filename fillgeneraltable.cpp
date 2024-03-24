#include <QProgressDialog>
#include <QDebug>
#include <QBuffer>
#include <QSqlError>
#include <QStringList>
#include <QGuiApplication>
#include "mainwindow.h"


int queryInsertRow(QSqlDatabase &db, const QString &question, QPixmap *image, int parent_id)
{
    int row = 0;

    QSqlQuery query(db);
    if(image) {
        QByteArray byte_array;
        QBuffer in_buffer(&byte_array);
        in_buffer.open(QIODevice::WriteOnly);
        image->save(&in_buffer, "PNG");

        query.prepare("INSERT INTO general_0 (question, image, parentId) VALUES (?, ?, ?)");
        query.addBindValue(question);
        query.addBindValue(byte_array);
        query.addBindValue(parent_id);
    } else {
        query.prepare("INSERT INTO general_0 (question, parentId) VALUES (?, ?)");
        query.addBindValue(question);
        query.addBindValue(parent_id);
    }
    if(!query.exec()) {
        qDebug() << "Error in SqlTreeModel::addRow:" << query.lastError().text();
        query.finish();
        return -1;
    }
    query.exec("SELECT * FROM general_0");
    while(query.next()) {
        ++row;
    }
    query.finish();
    return row;
}


void fillGeneralTable(QSqlDatabase &db, MainWindow *mw)
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QProgressDialog progress("Формирование списка общих вопросов...", "Отменить", 0, 10, mw);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);

    QStringList name_list;

    int start = queryInsertRow(db, "Начало", nullptr, 0);

    int what_you_want = queryInsertRow(db, "Чего хочешь?", nullptr, start);
    int where_to_go = queryInsertRow(db, "Куда пойти?", nullptr, start);

    QPixmap *image = new QPixmap(":/base_questions/dacha.jpg");
    int dacha = queryInsertRow(db, "Дача", image, start);
    delete image;

    image = new QPixmap(":/base_questions/fishing.jpg");
    int fishing = queryInsertRow(db, "Рыбалка", image, start);
    delete image;

    image = new QPixmap(":/base_questions/transport.jpg");
    int transport = queryInsertRow(db, "Транспорт", image, start);
    delete image;

    progress.setValue(1);
    // ===============================================================
    // dacha 31
    name_list << "Копать" << "Убирать траву" << "Делать грядки" << "Обрезать кусты" << "Чинить забор" << "Ремонтировать дом" << "Помидоры"
              << "Капуста" << "Огурцы" << "Баклажаны" << "Редис" << "Морковь" << "Свекла" << "Лук" << "Картофель" << "Петрушка" << "Укроп"
              << "Перцы" << "Персики" << "Яблони" << "Груша" << "Ранетка" << "Цветы" << "Лежать в кресле" << "Шашлык" << "Гости"
              << "Футбол" << "Волейбол" << "Бадминтон" << "Купаться" << "Костер";
    for (int i = 0; i < name_list.count(); i++) {
        image = new QPixmap(":/base_questions/dacha/" + QString::number(i+1) + ".jpg");
        queryInsertRow(db, name_list[i], image, dacha);
        delete image;
    }
    name_list.clear();


    // ===============================================================
    // fishing 8
    name_list << "На лодке" << "На льду" << "Хариус" << "Щука" << "Окунь" << "Форель" << "Пелядь" << "Сиг";
    for (int i = 0; i < name_list.count(); i++) {
        image = new QPixmap(":/base_questions/fishing/" + QString::number(i+1) + ".jpg");
        queryInsertRow(db, name_list[i], image, fishing);
        delete image;
    }
    name_list.clear();

    // ===============================================================
    // transport 7
    name_list << "Автобус" << "Троллейбус" << "Автомобиль" << "Такси" << "Трамвай" << "Самолет" << "Поезд";
    for (int i = 0; i < name_list.count(); i++) {
        image = new QPixmap(":/base_questions/transport/" + QString::number(i+1) + ".jpg");
        queryInsertRow(db, name_list[i], image, transport);
        delete image;
    }
    name_list.clear();

    progress.setValue(2);
    // ===============================================================
    // what_you_want 19
    image = new QPixmap(":/base_questions/WhatYouWant/eat.jpg");
    int eat = queryInsertRow(db, "Есть", image, what_you_want);
    delete image;
    {
        // ===============================================================
        // eat 6
        image = new QPixmap(":/base_questions/WhatYouWant/eat/breakfast.jpg");
        int breakfast = queryInsertRow(db, "Завтрак", image, eat);
        delete image;
        {
            // ===============================================================
            // breakfast 8
            image = new QPixmap(":/base_questions/WhatYouWant/eat/breakfast/porridge.jpg");
            int porridge = queryInsertRow(db, "Каша", image, breakfast);
            delete image;
            {
                progress.setValue(3);
                // ===============================================================
                // porridge 5
                name_list << "Манная" << "Гречневая" << "Рисовая" << "Перловая" << "Овсяная";
                for (int i = 0; i < name_list.count(); i++) {
                    image = new QPixmap(":/base_questions/WhatYouWant/eat/porridge/" + QString::number(i+1) + ".jpg");
                    queryInsertRow(db, name_list[i], image, porridge);
                    delete image;
                }
                name_list.clear();
            }
            name_list << "Салат" << "Яичница" << "Творог" << "Оладьи" << "Сосиски" << "Жареная колбаса" << "Бутерброды";
            for (int i = 0; i < name_list.count(); i++) {
                image = new QPixmap(":/base_questions/WhatYouWant/eat/breakfast/" + QString::number(i+1) + ".jpg");
                queryInsertRow(db, name_list[i], image, breakfast);
                delete image;
            }
            name_list.clear();
        }
        image = new QPixmap(":/base_questions/WhatYouWant/eat/fish.jpg");
        int fish = queryInsertRow(db, "Блюда из рыбы", image, eat);
        delete image;
        {
            progress.setValue(4);
            // ===============================================================
            // fish 5
            name_list << "Запеченая" << "Жареная" << "Уха" << "Соленая" << "Копченая";
            for (int i = 0; i < name_list.count(); i++) {
                image = new QPixmap(":/base_questions/WhatYouWant/eat/fish/" + QString::number(i+1) + ".jpg");
                queryInsertRow(db, name_list[i], image, fish);
                delete image;
            }
            name_list.clear();
        }
        image = new QPixmap(":/base_questions/WhatYouWant/eat/mushrooms.jpg");
        int mushrooms = queryInsertRow(db, "Блюда с грибами", image, eat);
        delete image;
        {
            // ===============================================================
            // mushrooms 6
            name_list << "Соленые" << "Маринованные" << "Жареные" << "Сушеные" << "Грабной суп" << "Пироги с грибами";
            for (int i = 0; i < name_list.count(); i++) {
                image = new QPixmap(":/base_questions/WhatYouWant/eat/mushrooms/" + QString::number(i+1) + ".jpg");
                queryInsertRow(db, name_list[i], image, mushrooms);
                delete image;
            }
            name_list.clear();
        }
        image = new QPixmap(":/base_questions/WhatYouWant/eat/breakfast/porridge.jpg");
        int porridge = queryInsertRow(db, "Каша", image, eat);
        delete image;
        {
            progress.setValue(5);
            // ===============================================================
            // porridge 5
            name_list << "Манная" << "Гречневая" << "Рисовая" << "Перловая" << "Овсяная";
            for (int i = 0; i < name_list.count(); i++) {
                image = new QPixmap(":/base_questions/WhatYouWant/eat/porridge/" + QString::number(i+1) + ".jpg");
                queryInsertRow(db, name_list[i], image, porridge);
                delete image;
            }
            name_list.clear();
        }
        image = new QPixmap(":/base_questions/WhatYouWant/eat/potato.jpg");
        int potato = queryInsertRow(db, "Блюда из картофеля", image, eat);
        delete image;
        {
            progress.setValue(6);
            // ===============================================================
            // potato 3
            name_list << "Отварной картофель" << "Жареный картофель" << "Пюре";
            for (int i = 0; i < name_list.count(); i++) {
                image = new QPixmap(":/base_questions/WhatYouWant/eat/potato/" + QString::number(i+1) + ".jpg");
                queryInsertRow(db, name_list[i], image, potato);
                delete image;
            }
            name_list.clear();
        }
        image = new QPixmap(":/base_questions/WhatYouWant/eat/soup.jpg");
        int soup = queryInsertRow(db, "Суп", image, eat);
        delete image;
        {
            // ===============================================================
            // soup 6
            name_list << "Куриный" << "Борщ" << "Щи" << "Рассольник" << "Грибной" << "Окрошка";
            for (int i = 0; i < name_list.count(); i++) {
                image = new QPixmap(":/base_questions/WhatYouWant/eat/soup/" + QString::number(i+1) + ".jpg");
                queryInsertRow(db, name_list[i], image, soup);
                delete image;
            }
            name_list.clear();
        }
    }

    image = new QPixmap(":/base_questions/WhatYouWant/drink.jpg");
    int drink = queryInsertRow(db, "Пить", image, what_you_want);
    delete image;
    progress.setValue(7);
        // ===============================================================
        // drink 12
        name_list << "Молоко" << "Чай" << "Кофе" << "Сок" << "Вода" << "Какао" << "Квас" << "Кола" << "Вино" << "Пиво" << "Коньяк" << "Водка";
        for (int i = 0; i < name_list.count(); i++) {
            image = new QPixmap(":/base_questions/WhatYouWant/drink/" + QString::number(i+1) + ".jpg");
            queryInsertRow(db, name_list[i], image, drink);
            delete image;
        }
        name_list.clear();

    name_list << "Чистить зубы" << "Расчесаться" << "Подстричься" << "Побриться" << "Принять ванну" << "В душ" << "Умыться" << "Встать" << "Сесть"
        << "Лечь" << "Одеться" << "Раздеться" << "Спать" << "В туалет" << "Измерить давление" << "Таблетки" << "Телевизор";
    for (int i = 0; i < name_list.count(); i++) {
        image = new QPixmap(":/base_questions/WhatYouWant/" + QString::number(i+1) + ".jpg");
        queryInsertRow(db, name_list[i], image, what_you_want);
        delete image;
    }
    name_list.clear();

    progress.setValue(8);
    // ===============================================================
    // where_to_go 15
    name_list << "В магазин" << "На прогулку" << "На работу" << "К врачу" << "В гости" << "Спать" << "Есть" << "Театр" << "Кинотеатр" << "Цирк"
         << "Пить" << "Смотреть телевизор" << "Читать" << "За грибами";
    for (int i = 0; i < name_list.count(); i++) {
        image = new QPixmap(":/base_questions/WhereToGo/" + QString::number(i+1) + ".jpg");
        queryInsertRow(db, name_list[i], image, where_to_go);
        delete image;
    }
    name_list.clear();

    image = new QPixmap(":/base_questions/WhereToGo/fishing.jpg");
    fishing = queryInsertRow(db, "На рыбалку", image, where_to_go);
    delete image;
    {
        progress.setValue(9);
        // ===============================================================
        // fishing 8
        name_list << "На лодке" << "На льду" << "Хариус" << "Щука" << "Окунь" << "Форель" << "Пелядь" << "Сиг";
        for (int i = 0; i < name_list.count(); i++) {
            image = new QPixmap(":/base_questions/fishing/" + QString::number(i+1) + ".jpg");
            queryInsertRow(db, name_list[i], image, fishing);
            delete image;
        }
        name_list.clear();
    }
    progress.setValue(10);
    QGuiApplication::restoreOverrideCursor();
}
