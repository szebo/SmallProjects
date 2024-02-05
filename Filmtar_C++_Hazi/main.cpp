#include <iostream>
#include <sstream>
#include "Film.h"
#include "FamilyFilm.h"
#include "gtest_lite.h"
#include "FilmStorage.h"
#include "FamilyFilm.h"
#include "DocumentFilm.h"
#include "string5.h"
#include "memtrace.h"


int main()
{
    FilmList storage = FilmList();

    Film f1 = Film(120, String("Karib-tenger kalozai"), 2003);
    DocumentFilm f2 = DocumentFilm(143, 2022, String("Nem valodi film"), String("Sajnos a fejemben vetittek csak."));
    FamilyFilm f3 = FamilyFilm(95, 2010, String("Gru"), 6);
    Film f4 = Film(100, String("Andom film"), 2014);

    TEST(test0, test_add_get){
        std::stringstream ss;
        storage.add(&f1);
        storage.add(&f2);
        storage.add(&f3);
        storage.get(ss, String("title"), String("Karib-tenger kalozai"));
        EXPECT_STREQ("Karib-tenger kalozai, 120, 2003", ss.str().c_str()) << "Rossz filmet adott vissza!\n";
        ss.str("");
        storage.get(ss, String("title"), String("Nem valodi film"));
        EXPECT_STREQ("Nem valodi film, 143, 2022, Sajnos a fejemben vetittek csak.", ss.str().c_str()) << "Rossz filmet adott vissza!\n";
        ss.str("");
        storage.get(ss, String("year"), 2010);
        EXPECT_STREQ("Gru, 95, 2010, 6", ss.str().c_str()) << "Rossz filmet adott vissza!\n";
    } ENDM

    TEST(test1, test_sort){
        std::stringstream ss;

        storage.get(ss, 0);
        EXPECT_STREQ("Gru, 95, 2010, 6", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
        storage.get(ss, 1);
        EXPECT_STREQ("Karib-tenger kalozai, 120, 2003", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
        storage.get(ss, 2);
        EXPECT_STREQ("Nem valodi film, 143, 2022, Sajnos a fejemben vetittek csak.", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");

        storage.sort("year");

        storage.add(&f4);
        storage.get(ss, 0);
        EXPECT_STREQ("Karib-tenger kalozai, 120, 2003", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
        storage.get(ss, 1);
        EXPECT_STREQ("Gru, 95, 2010, 6", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
        storage.get(ss, 2);
        EXPECT_STREQ("Andom film, 100, 2014", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
        storage.get(ss, 3);
        EXPECT_STREQ("Nem valodi film, 143, 2022, Sajnos a fejemben vetittek csak.", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
    } ENDM

    TEST(test2, test_mod){
        std::stringstream ss;

        storage.mod(String("Nem valodi film"), String("title"), String("Vadi uj film"));
        storage.mod(String("Vadi uj film"), String("length"), 130);
        storage.mod(String("Vadi uj film"), String("desc"), String("Meg mindig nem volt semmi."));
        storage.mod(String("Gru"), String("age"), 4);
        storage.mod(String("Gru"), String("year"), 2011);

        storage.sort(String("title"));
        storage.get(ss, 3);
        EXPECT_STREQ("Vadi uj film, 130, 2022, Meg mindig nem volt semmi.", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
    } ENDM

    TEST(test3, test_remove){
        std::stringstream ss;

        storage.sort(String("title"));
        storage.remove(String("Andom film"));
        storage.remove(String("Vadi uj film"));

        EXPECT_ANY_THROW(storage.remove(String("Nem valodi film"))) << "== Kivetelt kellett volni dobnia! ==\n";
        ss.str("");
        storage.get(ss, 0);
        EXPECT_STREQ("Gru, 95, 2011, 4", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
        storage.get(ss, 1);
        EXPECT_STREQ("Karib-tenger kalozai, 120, 2003", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
        ss.str("");
    } ENDM

    TEST(test4, test_getall){
        std::stringstream ss;

        storage.get(ss);
        EXPECT_STREQ("Gru, 95, 2011, 4\nKarib-tenger kalozai, 120, 2003\n", ss.str().c_str()) << "== Rossz filmet adott vissza! ==\n";
    } ENDM

    FamilyFilm f5 = FamilyFilm(f3);
    DocumentFilm f6 = DocumentFilm(f2);
    Film f7 = Film(f1);

    TEST(test5, test_random){
        storage.sort("length");
    } ENDM

    return 0;
}
