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
    '^[0-9]-?([0-9]){3}-?([0-9]){5}-?[0-9]$'
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
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('giiannoni0', 'Ui3HIc59garX');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('mpalethorpe1', '8dQDeYO');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('dsparshatt2', 'Fx0bANIDBp');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('ahaycock3', 'j2FyM5ISXA9y');
insert into Zamestnanec (prihlasovaci_jmeno, heslo) values ('bdarrach4', '6dauc3yD');

insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania, jazyk) values ('261953593-5', 'in', '3', '45.96', 'Hashim McTerry', 'Mayert, Block and Padberg', '1993', 'Tajik');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania, jazyk) values ('817285659-8', 'pellentesque at nulla suspendisse', '47', '79.86', 'Lorinda Entwisle', 'Weissnat, Baumbach and Witting', 2011, 'Northern Sotho');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania, jazyk) values ('125361508-5', 'duis aliquam convallis', '36', '18.23', 'Allyn Garratt', 'Smith LLC', '2004', 'Nepali');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania, jazyk) values ('066914900-4', 'massa id lobortis', '59', '77.15', 'Web Pieter', 'Hegmann-Rippin', '1998', 'Lao');
insert into Kniha (isbn, nazev, cena, pocet, autor, nakladatel, rok_vydania, jazyk) values ('065831377-0', 'integer ac leo pellentesque', '79', '24.63', 'Rosemary Cannell', 'Casper-Crona', '2007', 'Filipino');

insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('egadsden0', 'JRSLiGU', 'dmedhurst0@sun.com', '387-240-7235', 'Šenkovec', 'West', 45, 61245);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('rchidlow1', '27aTKjCM', 'abrowell1@hhs.gov', '775-480-6450', 'Mekarsari', 'Sommers', 745, 34567);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('adener2', '87UXmiZwNNy', 'tbazek2@arizona.edu', '150-599-6358', 'La Sarrosa', 'Schmedeman', 9020, 12345);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('cberrington3', 'TXYAaVSLp0Qf', 'ssykes3@independent.co.uk', '328-373-6658', 'Bayshint', 'Gerald', 058, 99803);
insert into Uzivatel (prihlasovaci_jmeno, heslo, email, telefon, mesto, ulice, cislo, PSC) values ('gklimkov4', 'LyldZC', 'yandren4@opera.com', '414-257-2378', 'Milwaukee', 'Oak Valley', 9, 20384);

insert into Kosik (jmeno, id_uzivatel) values ('moje oblubene', '1');
insert into Kosik (jmeno, id_uzivatel) values ('pohadky', '1');
insert into Kosik (jmeno, id_uzivatel) values ('na vanoce', '3');
insert into Kosik (jmeno, id_uzivatel) values ('pro deti', '3');
insert into Kosik (jmeno, id_uzivatel) values ('odmeny na tabor', '5');

insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Bayer, Brakus and Kertzmann', '1', '1', '1');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Monahan, McGlynn and Denesik', '2', '2', '2');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Kreiger-Skiles', '3', '3', '3');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Konopelski Group', '4', '4', '4');
insert into Objednavka (doprava, id_zakaznik, id_kosik, id_zamestnanec) values ('Romaguera Inc', '5', '4', '5');

insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('261953593-5', '1', '11');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('817285659-8', '2', '16');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('125361508-5', '3', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('066914900-4',' 4', '15');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('065831377-0', '5', '18');