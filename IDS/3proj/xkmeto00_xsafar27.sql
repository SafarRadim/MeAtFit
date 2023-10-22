-- -------- ----------------------------------------------------------------- --
-- Nazev:    xkmeto00_xsafar7.sql                                             --
-- Autori:   xkmeto00, xsafar27                                               --
-- Popis:    Creates and fills tables in oracle db.                           --
-- -------- ----------------------------------------------------------------- --

-- -------------------------------------------------------------------------- --
-- Pozn.:   Generalizace/specializace byla prevedena na dve samostatne        --
--          tabulky.                                                          --
--          Sortiment pozmenen specificky na Knihu.                           --
--          CHECK: ISBN; pocet knih v kosiku > 0                              --
--          ISBN je ve formatu 0-123-45678-9 kde '-' jsou nepovinne.          --
--          Registrovany uzivatel prejmenovan na Uzivatel (v db jsou jen      --
--          zaregistrovani).                                                  --
--          Transakce u objednavky zmenena na cislo objednavky a je PK.       --
-- -------------------------------------------------------------------------- --

-- ------------------------------------------ --
-- Oracle db verze "DROP tabulka IF EXISTS"   --
-- ------------------------------------------ --

-- Najito zde: 
--      https://stackoverflow.com/questions/1799128/oracle-if-table-exists

-- Tabulky
BEGIN
    FOR record IN (
        SELECT table_name
        FROM USER_TABLES
    )
    LOOP
        EXECUTE IMMEDIATE 'DROP TABLE ' || record.table_name || ' CASCADE CONSTRAINTS';
    END LOOP;
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE != -942 THEN
            RAISE;
        END IF;
END;
/

-- --------------- --
-- Tvoreni tabulek --
-- --------------- --
CREATE TABLE Kniha (
    isbn VARCHAR(13) CHECK (REGEXP_LIKE(isbn, 
    '^[0-9]-?([0-9]){3}-?([0-9]){5}-?([0-9]|x)$'
    )) PRIMARY KEY, -- isbn vo formate x-xxx-xxxxx-x
    nazev VARCHAR(50) NOT NULL,
    autor VARCHAR(50) NOT NULL,
    nakladatel VARCHAR(50),
    rok_vydania INTEGER,
    jazyk VARCHAR(50),
    cena NUMBER(5),
    pocet INTEGER
);

CREATE TABLE Uzivatel (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    prihlasovaci_jmeno VARCHAR(50) NOT NULL,
    heslo VARCHAR(50) NOT NULL,
    email VARCHAR(50) NOT NULL,
    telefon VARCHAR(15),
    mesto VARCHAR(50),
    ulice VARCHAR(50),
    cislo INT,
    PSC INT
);

CREATE TABLE Kosik (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    jmeno VARCHAR(50),
    id_uzivatel INT NOT NULL,
    FOREIGN KEY (id_uzivatel) REFERENCES Uzivatel(id)
);

CREATE TABLE KnihaKosik (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    isbn_kniha VARCHAR(13) NOT NULL,
    id_kosik INT NOT NULL,
    pocet INTEGER NOT NULL CHECK (pocet > 0),
    FOREIGN KEY (isbn_kniha) REFERENCES Kniha(isbn),
    FOREIGN KEY (id_kosik) REFERENCES Kosik(id)
);

CREATE TABLE Zamestnanec (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    prihlasovaci_jmeno VARCHAR(50) NOT NULL,
    heslo VARCHAR(50) NOT NULL
);

CREATE TABLE Objednavka (
    id INT GENERATED ALWAYS AS IDENTITY START WITH 230000 PRIMARY KEY,
    doprava VARCHAR(50) NOT NULL,
    id_zakaznik INT NOT NULL,
    id_kosik INT NOT NULL,
    id_zamestnanec INT NOT NULL,
    FOREIGN KEY (id_zakaznik) REFERENCES Uzivatel(id),
    FOREIGN KEY (id_kosik) REFERENCES Kosik(id),
    FOREIGN KEY (id_zamestnanec) REFERENCES Zamestnanec(id)
);

-- ------------- --
-- Vlozeni radku --
-- ------------- --
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('SvobMa', '8dQDeYO');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('CernJa', 'j2FyM5ISXA9y');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('VargJa', 'Ui3HIc59garX');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('SzabMi', 'Fx0bANIDBp');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('GajdOl', '6dauc3yD');

insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, jazyk)          values ('807546131-2', 'Valka s mloky', '161', '45', 'Karel Capek', 'Millennium Publishing','Czech');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania)    values ('800005417-5', 'Zahada hlavolamu', '317', '79', 'Jaroslav Foglar', 'Olympia', '2003');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania)    values ('178435228-4', 'Rozmarne leto', '66', '18', 'Vladislav Vancura', 'Odeon', '1926');
insert into Kniha (isbn, nazev, cena, pocet, autor, rok_vydania, jazyk)         values ('800000498-4', 'Lovci mamutu', '167', '77', 'Eduard Storch', '1980', 'Czech');
insert into Kniha (isbn, nazev, cena, pocet, autor, rok_vydania, jazyk)         values ('807244313-x', 'Saturnin', '221', '24', 'Zdenek Jirotka', '2008', 'Czech');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, jazyk)          values ('802572825-0', 'Trhlina', '208', '23', 'Jozef Karika', 'Ikar', 'Slovak');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania)    values ('802710370-3', 'Divka v ledu', '184', '65', 'Robert Bryndza', 'Cosmopolis', '2016');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania)    values ('807229178-5', 'Cachticka pani', '199', '25', 'Jozef Niznansky', 'Petrklic', '2007');
insert into Kniha (isbn, nazev, cena, pocet, autor, rok_vydania, jazyk)         values ('802493450-7', 'Mengeleho devce', '181', '68', 'Viola Stern Fischerova', '2017', 'Slovak');
insert into Kniha (isbn, nazev, cena, pocet, autor, rok_vydania, jazyk)         values ('802641906-5', 'Gerda: Pribeh velryby', '244', '73', 'Adrian Macho', '2018', 'Slovak');

insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('egadsden0', 'JRSLiGU', 'dmedhurst0@sun.com', '387-240-7235', 'Šenkovec', 'West', 45, 61245);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('rchidlow1', '27aTKjCM', 'abrowell1@hhs.gov', '775-480-6450', 'Mekarsari', 'Sommers', 745, 34567);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('adener2', '87UXmiZwNNy', 'tbazek2@arizona.edu', '150-599-6358', 'La Sarrosa', 'Schmedeman', 9020, 12345);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('cberrington3', 'TXYAaVSLp0Qf', 'ssykes3@independent.co.uk', '328-373-6658', 'Bayshint', 'Gerald', 058, 99803);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('gklimkov4', 'LyldZC', 'yandren4@opera.com', '414-257-2378', 'Milwaukee', 'Oak Valley', 9, 20384);

insert into Kosik (jmeno, id_uzivatel) values ('moje oblubene', '1');
insert into Kosik (jmeno, id_uzivatel) values ('pohadky', '1');
insert into Kosik (jmeno, id_uzivatel) values ('Muj Kosik', '2');
insert into Kosik (jmeno, id_uzivatel) values ('Muj Kosik(1)', '2');
insert into Kosik (jmeno, id_uzivatel) values ('na vanoce', '3');
insert into Kosik (jmeno, id_uzivatel) values ('pro deti', '3');
insert into Kosik (jmeno, id_uzivatel) values ('Pro Arnolda', '4');
insert into Kosik (jmeno, id_uzivatel) values ('odmeny na tabor', '5');

insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Zasilkovna', '1', '1', '1');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Zasilkovna', '2', '2', '2');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('PPL', '2', '2', '2');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('DHL', '3', '3', '3');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Balikovna', '4', '4', '4');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('PPL', '4', '4', '4');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Ceska posta', '5', '4', '5');

insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807546131-2', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('800005417-5', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('178435228-4', '2', '3');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('800000498-4',' 3', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807244313-x', '5', '4');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802572825-0', '7', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802710370-3', '8', '6');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807229178-5', '4', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '5', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802641906-5', '6', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '2', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807229178-5', '8', '5');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('178435228-4', '4', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802641906-5', '3', '4');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807546131-2', '2', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('800000498-4', '5', '3');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '3', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('178435228-4', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802641906-5', '7', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807244313-x', '2', '3');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807546131-2', '3', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '4', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807229178-5', '7', '3');

-- ------- --
-- SELECTY --
-- ------- --

-- 2 tabulky I
-- Zjisteni jaky zamestnanec vyrizuje kterou objednavku
SELECT prihlasovaci_jmeno, objednavka.id FROM Zamestnanec
    INNER JOIN Objednavka
    ON id_zamestnanec = Zamestnanec.id
    ORDER BY zamestnanec.id
    ;

-- 2 tabulky II
-- Zjisteni kdo vlastni ktery kosik
SELECT prihlasovaci_jmeno, jmeno FROM Uzivatel
    INNER JOIN Kosik
    ON id_uzivatel = uzivatel.id
    ORDER BY uzivatel.id
    ;

-- 3 tabulky
-- Zjisteni v jakem kosiku je kniha saturnin
SELECT jmeno FROM Kosik
    INNER JOIN KnihaKosik
    ON id_kosik = kosik.id
    INNER JOIN Kniha
    ON isbn_kniha = isbn
    WHERE nazev LIKE 'Saturnin'
    ;

-- GROUP BY + agregacni I (+3 tabulky)
-- Zjisteni v kolika kosicich je kniha
SELECT nazev, count(jmeno) as pocet
    FROM Kniha
    INNER JOIN KnihaKosik
    ON isbn_kniha = isbn
    INNER JOIN Kosik
    ON id_kosik = kosik.id
    GROUP BY nazev
    ORDER BY pocet DESC
    ;

-- GROUP BY + agregacni II
-- Zjisteni ktery uzivatel udelal kolik objednavek
SELECT prihlasovaci_jmeno, COUNT(*) as pocet
    FROM Objednavka
    INNER JOIN uzivatel
    ON id_zakaznik = uzivatel.id
    GROUP BY prihlasovaci_jmeno
    ORDER BY pocet DESC
    ;


-- EXISTS
-- Zjisteni kterym uzivatelum zacina jmeno alespon jednoho kosiku na "Muj"
SELECT prihlasovaci_jmeno
    FROM Uzivatel
    WHERE EXISTS (
        SELECT 1
            FROM kosik
            WHERE jmeno LIKE 'Muj%'
            AND uzivatel.id = ID_UZIVATEL
    )
    ;

-- predikat IN
-- Zjisteni kteri zamestnance expedovali zbozi pomoci zasilkovny
SELECT prihlasovaci_jmeno
    FROM zamestnanec
    WHERE zamestnanec.id IN (
        SELECT DISTINCT id_zamestnanec
            FROM objednavka
            WHERE doprava LIKE 'Zasilkovna'
    )
    ;
