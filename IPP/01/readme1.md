## Implementační dokumentace k 1. úloze do IPP 2022/2023
## Jméno a příjmení: Radim Šafář
## Login: xsafar27
---

## Myšlenka implementace
Skript je implementován objektově, i když nejspíš není podle žádného návrhového vzoru. Proto také neuvádím rozšíření. Osobně musím říct, že vidět objekty po třech semestrech  v C byla celkem úleva.  
Žádný UML návrh této části projektu jsem nedělal, což bych řekl, že se mi i celkem vymstilo. Kód zdaleka nedává takový smysl, jako by mohl.

## Implementace
Ve skriptu jsou použity dvě třídy, Parser a Element. Jako knihovnu na XML jsem si vybral simpleXML.

### Parser
Třída parser zajišťuje kontrolu hlavičky, její existenci a případnou duplicitu, a držení počídala instrukcí.
Následně řeší sanitaci vstupu. Odstranění komentářů, přebytečných mezer, přeskakuje volné řádky, upravuje speciálni xml znaky.
Upravený řádek následně pošle nové instanci objektu Element, který ho zpracuje dále.

### Element
Jako vstup jí je objekt XML, pořadí instrukce a řádek na zpracování.  
Zjistí OPCODE instrukce. Pokud její první argument není správného typu ukončí program s kódem 23. Následně generuje argumenty instrukce. Na kontrolu datových typů jsou používané regulární výrazy. 

