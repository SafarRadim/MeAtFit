-- -------- ----------------------------------------------------------------- --
-- Nazev:    xkmeto00_xsafa2r7.sql                                             --
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
-- Oracle db verze "DROP smth IF EXISTS"   --
-- ------------------------------------------ --

-- Najito zde: 
--      https://stackoverflow.com/questions/1799128/oracle-if-table-exists

-- View
BEGIN
    EXECUTE IMMEDIATE 'DROP MATERIALIZED VIEW LOG ON kniha';
    EXECUTE IMMEDIATE 'DROP MATERIALIZED VIEW sklad';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE != -12003 AND SQLCODE != -12002 THEN
            RAISE;
        END IF;
END;
/

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

-- ------- --
-- 4. cast --
-- ------- --

-- TRIGGERY --
-- Netrivialni trigger I + predvedeni
-- Kdyz je pridana objednavka, je ji prirazen nejaky zamestnanec.
CREATE OR REPLACE TRIGGER pridat_zamestnance
BEFORE INSERT ON Objednavka
FOR EACH ROW
DECLARE
    var_id INT;
BEGIN
    SELECT id INTO var_id FROM (
        SELECT zamestnanec.id, count(objednavka.id_zamestnanec) AS pocet_objednavek
        FROM zamestnanec
        LEFT JOIN objednavka
        ON zamestnanec.id = objednavka.id_zamestnanec
        GROUP BY zamestnanec.id
        ORDER BY pocet_objednavek ASC
    )
    WHERE ROWNUM = 1;

    :NEW.id_zamestnanec := var_id;
END;
/

-- Netrivialni trigger II + predvedeni
-- jakmile je pocet knih v kosiku 0, vymaze se kosik
CREATE OR REPLACE TRIGGER kosik_check_pocet
FOR UPDATE OR DELETE ON KNIHAKOSIK
COMPOUND TRIGGER

    PROCEDURE delete_empty_kosik(p_id_kosik IN kosik.id%TYPE) IS
        var_pocet KNIHAKOSIK.pocet%TYPE;
    BEGIN
        SELECT SUM(pocet) INTO var_pocet
        FROM KNIHAKOSIK
        WHERE id_kosik = p_id_kosik;

        -- ak je pocet knih v kosiku 0, vymaze sa kosik
        IF var_pocet = 0 THEN
            DELETE FROM Kosik WHERE id = p_id_kosik;
        END IF;
    END;

    AFTER EACH ROW IS
    BEGIN
        -- ak je pocet knih v kosiku 0, vymaze sa kosik (mutating table error)
        IF :NEW.pocet = 0 THEN
            delete_empty_kosik(:NEW.id_kosik);
        END IF;
    END AFTER EACH ROW;

END kosik_check_pocet;
/


-- PROCEDURY --
-- Alespon jednou: Kurzor, exception handle, %TYPE nebo %ROWTYPE
-- Netrivialni procedura I + predvedeni
CREATE OR REPLACE PROCEDURE aktualizuj_sklad(index_objednavky IN INT)
AS
CURSOR objednavka_kurzor IS 
SELECT KnihaKosik.isbn_kniha, KnihaKosik.pocet
FROM objednavka
JOIN KnihaKosik
ON objednavka.id_kosik = KnihaKosik.id_kosik
WHERE objednavka.id = index_objednavky;

objednavka_item objednavka_kurzor%ROWTYPE;
mnozstvi INT;

BEGIN
    OPEN objednavka_kurzor;

    DBMS_OUTPUT.PUT_LINE('Uprava skladovych zasob pres objednavku ' || index_objednavky);
    SAVEPOINT pred_upravou;
    LOOP
        FETCH objednavka_kurzor INTO objednavka_item;
        EXIT WHEN objednavka_kurzor%NOTFOUND;

        DBMS_OUTPUT.PUT_LINE('ISBN: ' || objednavka_item.isbn_kniha);

        SELECT pocet 
        INTO mnozstvi
        FROM kniha 
        WHERE isbn = objednavka_item.isbn_kniha;

        DBMS_OUTPUT.PUT_LINE('Stav pred: ' || mnozstvi);

        mnozstvi := mnozstvi - objednavka_item.pocet;
        IF mnozstvi < 0
        THEN
            DBMS_OUTPUT.PUT_LINE('Novy stav by byl zaporny!');
            ROLLBACK TO pred_upravou;
            DBMS_OUTPUT.PUT_LINE('Pocty byly obnoveny.');
            EXIT;
        END IF;

        UPDATE kniha 
        SET pocet = mnozstvi
        WHERE isbn = objednavka_item.isbn_kniha;

        DBMS_OUTPUT.PUT_LINE('Stav po: ' || mnozstvi);
    END LOOP;

END;
/


-- Netrivialni procedura II + predvedeni
-- pocitadlo dopravy
CREATE OR REPLACE PROCEDURE doprava_pocitadlo (
    order_id IN INT
)
AS
    celkova_cena NUMBER;
    doprava_zdarma BOOLEAN := FALSE;
    objednavka_id objednavka.id%TYPE;
BEGIN
    SELECT objednavka.id, SUM(Kniha.cena * KnihaKosik.pocet) 
    INTO objednavka_id, celkova_cena
    FROM objednavka
    JOIN KnihaKosik ON objednavka.id_kosik = KnihaKosik.id_kosik
    JOIN Kniha ON Kniha.isbn = KnihaKosik.isbn_kniha
    WHERE objednavka.id = order_id
    GROUP BY objednavka.id;

    IF celkova_cena > 2500 THEN
        doprava_zdarma := TRUE;
    END IF;

    IF doprava_zdarma THEN
        DBMS_OUTPUT.PUT_LINE('Celkova cena objednavky ' || objednavka_id ||': ' || celkova_cena || ', doprava zdarma.');
    ELSE
        DBMS_OUTPUT.PUT_LINE('Celkova cena objednavky ' || objednavka_id ||': ' || celkova_cena || ', doprava: 100-,');
    END IF;

EXCEPTION 
    WHEN NO_DATA_FOUND THEN 
        DBMS_OUTPUT.PUT_LINE('Objednavka neexistuje!'); 
    WHEN OTHERS THEN 
        RAISE;
END;
/


-- OPTIMALIZACE --
EXPLAIN PLAN FOR
SELECT objednavka.doprava, count(kniha.isbn) as pocet, sum(kniha.cena * KNIHAKOSIK.pocet) as hodnota 
FROM objednavka
JOIN KnihaKosik
ON KnihaKosik.Id_Kosik = Objednavka.Id_Kosik
JOIN Kniha
ON KnihaKosik.ISBN_Kniha = Kniha.ISBN
GROUP BY objednavka.doprava
ORDER BY hodnota DESC;

SELECT * FROM TABLE(DBMS_XPLAN.display);

-- Indexujeme FK u 1:n
CREATE INDEX kosik_index ON knihakosik(id_kosik);
CREATE INDEX kniha_index ON knihakosik(isbn_kniha);

DROP INDEX kosik_index;
DROP INDEX kniha_index;

-- DRUHY CLEN --

-- Materializovany pohled z mych tabulek pro druheho
CREATE MATERIALIZED VIEW LOG
ON Kniha 
WITH PRIMARY KEY, ROWID INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW sklad
CACHE
BUILD IMMEDIATE
REFRESH FAST ON COMMIT
ENABLE QUERY REWRITE
AS SELECT k.isbn, k.nazev, k.pocet FROM Kniha k;

INSERT INTO xkmeto00.kniha (isbn, nazev, autor, pocet) values ('020420011-0', 'dobiden', 'whoislisalisa', '24');
COMMIT;

SELECT * FROM sklad;

-- Pridani prav
DECLARE
    xlogin varchar(8);
BEGIN
    --xlogin := 'xkmeto00';
    xlogin := 'xsafar27';

    FOR record IN (
        SELECT table_name
        FROM USER_TABLES
    )
    LOOP
        EXECUTE IMMEDIATE 'GRANT ALL ON ' || record.table_name || ' TO ' || xlogin;
    END LOOP;


    EXECUTE IMMEDIATE 'GRANT ALL ON sklad TO ' || xlogin;

END;
/

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

insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('Zasilkovna', '1', '1');
insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('Zasilkovna', '2', '2');
insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('PPL', '2', '2');
insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('DHL', '3', '3');
insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('Balikovna', '4', '4');
insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('PPL', '4', '4');
insert into Objednavka (doprava, id_zakaznik, id_kosik) values ('Ceska posta', '5', '4');

insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807546131-2', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('800005417-5', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('178435228-4', '2', '3');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('800000498-4',' 3', '1');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807244313-x', '5', '4');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802572825-0', '7', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802710370-3', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807229178-5', '4', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '5', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802641906-5', '6', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '2', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807229178-5', '8', '5');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('178435228-4', '4', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802641906-5', '4', '4');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807546131-2', '2', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('800000498-4', '5', '3');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '4', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('178435228-4', '1', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802641906-5', '7', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807244313-x', '2', '3');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('807546131-2', '4', '2');
insert into KnihaKosik (isbn_kniha, id_kosik, pocet) values ('802493450-7', '4', '2');

-- SELECT s WITH a CASE
-- suhrn informacii o zakaznicich a objednavkach a seradi je podle velkosti objednavky
WITH velkosti_objednavek AS (
  SELECT
    id_zakaznik,
    COUNT(*) AS pocet_polozek,
    SUM(KnihaKosik.pocet * Kniha.cena) AS celkova_cena
  FROM
    Objednavka
    JOIN KnihaKosik ON Objednavka.id_kosik = KnihaKosik.id_kosik
    JOIN Kniha ON KnihaKosik.isbn_kniha = Kniha.isbn
  GROUP BY
    id_zakaznik
)

SELECT
  Uzivatel.id,
  Uzivatel.email,
  Uzivatel.telefon,
  Uzivatel.mesto,
  Uzivatel.ulice,
  Uzivatel.cislo,
  Uzivatel.PSC,
  velkosti_objednavek.pocet_polozek,
  velkosti_objednavek.celkova_cena,
  CASE
    WHEN velkosti_objednavek.celkova_cena > 4500 THEN 'Velká objednávka'
    WHEN velkosti_objednavek.celkova_cena > 2500 THEN 'Střední objednávka'
    ELSE 'Malá objednávka'
  END AS typ_objednavky
FROM
  Uzivatel
  JOIN Objednavka ON Uzivatel.id = Objednavka.id_zakaznik
  JOIN velkosti_objednavek ON Uzivatel.id = velkosti_objednavek.id_zakaznik
ORDER BY
  velkosti_objednavek.celkova_cena DESC;

-- ------ --
-- UKAZKY --
-- ------ --

-- TRIGGERY --
-- Trigger I
-- Ukazka ze to je rovnomerne rozhazene
SELECT id_zamestnanec, count(id) 
    FROM objednavka 
    GROUP BY id_zamestnanec;

-- Trigger II
SELECT id_kosik, SUM(pocet)
FROM knihakosik
GROUP BY id_kosik;

DELETE FROM knihakosik WHERE pocet = 1;

SELECT id_kosik, SUM(pocet)
FROM knihakosik
GROUP BY id_kosik;
-- PROCEDURY --

-- Procedura I
-- Kontrola stavu knih
SELECT kniha.isbn, kniha.pocet
FROM objednavka
JOIN KnihaKosik
ON objednavka.id_kosik = KnihaKosik.id_kosik
JOIN Kniha
ON KnihaKosik.isbn_kniha = kniha.isbn
WHERE objednavka.id = 230000;

-- Zobrazeni poctu v objednavce
SELECT isbn_kniha, pocet
FROM objednavka
JOIN KnihaKosik
ON objednavka.id_kosik = KnihaKosik.id_kosik
WHERE objednavka.id = 230000;

-- Volani procedury
CALL aktualizuj_sklad(230000);

-- Procedura II
-- doprava 100-,
SELECT kniha.isbn, kniha.cena, knihaKosik.pocet
FROM objednavka
JOIN knihaKosik 
ON knihaKosik.id_kosik = objednavka.id_kosik
JOIN kniha
ON knihaKosik.isbn_kniha = kniha.isbn
WHERE objednavka.id = 230000;

-- Volani procedury
CALL doprava_pocitadlo(230000);

-- doprava zdarma
SELECT kniha.isbn, kniha.cena, knihaKosik.pocet
FROM objednavka
JOIN knihaKosik 
ON knihaKosik.id_kosik = objednavka.id_kosik
JOIN kniha
ON knihaKosik.isbn_kniha = kniha.isbn
WHERE objednavka.id = 230006;

-- Volani procedury
CALL doprava_pocitadlo(230006);

CALL doprava_pocitadlo(240000);