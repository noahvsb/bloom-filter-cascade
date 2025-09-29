
---
title: "Bloom filter cascades"
course: AD3
year: 2025-2026
---

# Opgave project AD3: Bloom filter cascades met meerdere verzamelingen

In de theorielessen zagen jullie Bloom filters en Bloom filter cascades met twee verzamelingen. In dit project breid je dat concept uit naar meerdere verzamelingen. Je schrijft een CLI-tool die de nodige Bloom filters voor meerdere categorieën aanmaakt, opslaat en kan gebruiken om items te classificeren.

## Voorbeeld
Je maakt bijvoorbeeld een Bloom filter cascade voor onderstaande categorieën:

| **Amfibieën** | **Reptielen** | **Vissen** | **Vogels**  | **Zoogdieren** |
|---------------|---------------|------------|-------------|----------------|
| Kikker        | Alligator     | Goudvis    | Arend       | Chimpansee     |
| Salamander    | Gekko         | Karper     | Havik       | Giraffe        |
|               | Hagedis       | Snoek      | Roodborstje | Leeuw          |
|               | Krokodil      |            | Valk        | Muis           |
|               |               |            |             | Wombat         |

Je programma zou dan aan de hand van jouw Bloom filters met 100% zekerheid moeten kunnen zeggen dat een Kikker behoort tot de amfibieën categorie. Voor een dier dat niet in de oorspronkelijke input zit, mag elke categorie teruggegeven worden en mag je programma niet crashen.

## Functionele vereisten programma

Schrijf een command-line tool die:

- Een bestand met categorieën inleest (zie de sectie [Bestandsformaat](#bestandsformaat) voor het bestandsformaat van deze bestanden);
- Op basis van deze categorieën een Bloom filter cascade genereert en deze opslaat in een bestand;
- Een eerder opgeslagen Bloom filter cascade kan inladen om ingevoerde items te classificeren tot de juiste categorie. Als het item dat je probeert te classificeren niet in de oorspronkelijke input zit, mag je programma niet crashen, maar schrijft het eender welke categorie uit.

## Specificaties

### Programmeertaal

In de opleidingscommissie informatica (OCI) werd beslist dat, om meer ervaring met het programmeren in C te verwerven, het project van Algoritmen en Datastructuren 3 in C geïmplementeerd moet worden.
Het is met andere woorden de bedoeling je implementatie in C uit te voeren. Je implementatie moet voldoen aan de ANSI-standaard.
Je mag hiervoor dus gebruikmaken van de laatste features in C17, voor zover die ondersteund worden door de `C` compiler die gebruikt wordt voor dit project op SubGIT (GCC 14.2).

Voor het project kun je de standaard libraries gebruiken, externe libraries
zijn echter niet toegelaten. Het spreekt voor zich dat je normale, procedurele
C-code schrijft en geen platformspecifieke API's (zoals bv. de Win32 API) of
features uit C++ gebruikt. Op Windows bestaat van enkele functies zoals `qsort`
een "safe" versie (in dit geval `qsort_s`), maar om je programma te kunnen
compileren op een Unix-systeem kun je die versie dus niet gebruiken. Let er ook op dat je programma geen geheugenlekken bevat.

**Wat je ontwikkelingsplatform ook mag zijn, controleer geregeld de output van
SubGIT om te verifiëren dat je programma ook bij ons compileert en slaagt voor de minimale testen!**

### Commando's

Je programma moet de volgende commando's ondersteunen. Dit commando zal als tweede argument meegegeven worden aan jouw programma.

- `train`: Berekent de Bloom filter cascade voor de meegegeven categorieën. Dit commando verwacht één argument: het bestand met de categorieën. Daarnaast heeft het programma ook één verplichte optie: -o, waarmee je het pad meegeeft waar de gegenereerde Bloom filter cascade wordt opgeslagen.

  ```shell
  $ ./project train -o Bloom_filters.file categories.file
  ```

- `classify`: Bepaal voor gegeven items tot welke categorie deze behoren. Dit commando vereist één argument: het bestand met de opgeslagen Bloom filters. De te classificeren items geef je mee via stdin, één item per regel. De overeenkomstige categorieën worden vervolgens naar stdout geschreven, één categorie per regel.
  ```shell
  $ ./project classify Bloom_filters.file
  Kikker                    # stdin
  Amfibieën                 # stdout
  Goudvis                   # stdin
  Vissen                    # stdout
  ```

### Datasets

In het mapje `data/` vind je scriptjes om datasets te downloaden of te genereren.

Tijdens het verbeteren zullen we gelijkaardige bestanden gebruiken om jullie tijd- en geheugengebruik mee te meten. Het loont dus zeker de moeite om voor deze bestanden te optimaliseren.

### Bestandsformaat

De bestanden met de categorieën moeten de volgende structuur hebben:

```text
[Amfibieën]
Kikker
Salamander

[Reptielen]
Alligator
Gekko
Hagedis
Krokodil

[Vissen]
Goudvis
Karper
Snoek

[Vogels]
Arend
Havik
Roodborstje
Valk

[Zoogdieren]
Chimpansee
Giraffe
Leeuw
Muis
Wombat
```

Hier wordt elke categorie voorafgegaan door de naam van de categorie. De naam van de categorie staat altijd tussen vierkante haken (`[]`). Onder de naam van de categorie volgen de items. Elk item staat op een afzonderlijke regel. Tussen de categorieën staan één of meer witregels.

Je moet zelf het bestandsformaat ontwerpen waarmee de Bloom filters worden opgeslagen.

### Limieten

We voorzien enkele testen die de basisfunctionaliteit van je programma testen.
Je moet ten minste voor al deze testen slagen om punten te krijgen voor het overeenkomstige onderdeel.
Natuurlijk is het ook de bedoeling om zelf ook testen te schrijven om te garanderen dat jouw implementatie volledig correct is.
Je mag hiervoor gebruikmaken van de meegeleverde `test.h`-header van het framework [acutest](https://github.com/mity/acutest), maar dat is niet verplicht.

Het is niet toegestaan om sockets te openen vanuit je bibliotheek. Je programma moet single threaded zijn en mag dus niet forken.

Als je je het geheugengebruik van je programma in detail wil opvolgen, kun je ook gebruikmaken van het `massif` programma van Valgrind:

```sh
valgrind --tool=massif --stacks=yes --massif-out-file=memory.ms -- ./project_bloom_filters
```

De bovenstaande lijn zal het testprogramma (`./project_bloom_filters`) uitvoeren met de [Valgrind Massif](https://www.valgrind.org/docs/manual/ms-manual.html) tool, die zal een `memory.ms` bestand genereren dat het geheugengebruik beschrijft.
Je kunt dit bestand gewoon openen met een teksteditor of je kan `ms_print` gebruiken om een ASCII art grafiekje te krijgen dat het geheugengebruik doorheen de tijd toont.
Met de [massif-visualizer](https://github.com/KDE/massif-visualizer) tool kun je een mooiere weergave krijgen (met kleuren en grafieken waar je op kunt klikken).

*Belangrijk bij het meten van geheugen*:

- Compileer je programma met `-g` om debuginformatie aan te zetten zodat deze tools mooi kunnen tonen waar er geheugen gealloceerd is.
- De dumpbestanden van Valgrind Massif gebruiken de notatie `kB` voor 1024 byte, een correctere notatie zou KiB zijn.
- Valgrind heeft ook een [memcheck](https://www.valgrind.org/docs/manual/mc-manual.html) tool waarmee je memory leaks kan opsporen.
- Je programma kan tot 100 keer trager worden tijdens het uitvoeren in `valgrind`.

## Tips

We geven je alvast enkele tips om je op weg te helpen:

- Voor je Bloom filters moet je een aantal bits en het aantal hashfuncties kiezen. Je kunt hier gebruik maken van een optimaal aantal bits en hashfuncties afhankelijk van een false positive rate zoals in **Oefening 9**. Maar aangezien je exact weet welke items je allemaal zal testen met de Bloom filter, kan je ook alternatieven overwegen.

- De Bloom filter cascade moet slechts één keer gegenereerd worden en wordt vervolgens veel verspreid en gebruikt. De grootte van het outputbestand is bijgevolg veel belangrijker dan de tijd die het duurt om de Bloom filter cascade te genereren.

- Je programma moet kunnen werken met eender welk aantal categorieën, maar houd bij het optimaliseren vooral rekening met een relatief klein aantal (bijvoorbeeld hoogstens 10 categorieën). Bij een grote hoeveelheid categorieën zou je waarschijnlijk toch eerder aan andere datastructuren denken.

- Als hashfunctie kies je best een goede, geparametriseerde hashfunctie met een seed die je kan meegeven. Een voorbeeld van een mogelijke goede hashfunctie is MurmurHash3. Je hoeft deze hashfunctie niet zelf te implementeren. Je mag open-source implementaties van bestaande hashfuncties overnemen in je project op voorwaarde dat je in het broncodebestand correct verwijst naar de originele implementatie. Verdedig in je verslag de keuze van deze hashfunctie.

- Als je metingen doet, compileer je natuurlijk met zo veel mogelijk optimalisaties aan (compileer met `-O3` en `-march=native`). Met de vlag -march=native optimaliseert de compiler je code voor de instructieset van je eigen CPU. Zo kan die gebruikmaken van processor-specifieke instructies (zoals SSE, AVX of NEON) die niet op oudere hardware beschikbaar zijn. Dit kan een merkbare snelheidswinst opleveren. Het nadeel is dat de gecompileerde binary hierdoor minder compatibel wordt: op andere machines met een verschillende instructieset kan het programma foutieve resultaten geven of zelfs crashen. Deze vlag is dus nuttig voor benchmarking of programma’s die je enkel op je eigen systeem draait, maar minder geschikt als je de binary wilt verspreiden.

## Verslag

Schrijf een bondig verslag over je werk met volgende componenten:

- Het algoritme dat je gebruikt hebt om jouw Bloom filter cascade te genereren.
- De ontwerpbeslissingen die je nam voor de implementatie, hier moet het volgende zeker vermeld worden:
  - Hoe je jouw aantal bits en hashfuncties bepaald hebt per Bloom filter.
  - Het formaat waarin je jouw Bloom filter cascade opslaat.
- Een korte analyse van het geheugengebruik en uitvoeringstijd van jouw programma voor grotere datasets. Vermeld ook hoe groot jouw output bestanden zijn. Je kan deze met elkaar vergelijken door te berekenen hoeveel bits er gebruikt worden per item.

## Indienen

### Directorystructuur

Je indiening moet de volgende structuur hebben:

- `src/` bevat alle broncode

- `tests/` alle testcode (je moet testcode hebben, push geen grote testbestanden
  naar SubGIT, plaats een scriptje dat die bestanden genereert).

- `extra/verslag.pdf` bevat de elektronische versie van je verslag. In deze map
  kun je ook eventueel extra bijlagen plaatsen zoals de broncode van je verslag.

- `sources`: een tekstbestand met de`.h` en `.c` bronbestanden om je programma te compileren, 1 per lijn relatief t.o.v. de `src` directory. Bijvoorbeeld:

      main.c
      bloom_filter/bloom_filter_cascade.h
      bloom_filter/bloom_filter_cascade.c
      bloom_filter/bloom_filter.h
      bloom_filter/bloom_filter.c
      hash.c

Je mappenstructuur ziet er dus ongeveer zo uit:

    |
    |-- extra/
    |   `-- verslag.pdf
    |-- src/
    |   `-- je broncode
    |-- test/
    |   `-- je testcode
    |-- data/
    |   `-- onze datasets
    |-- sources

Al je bestanden moeten als UTF-8 opgeslagen zijn.

### Compileren

De code zal bij het indienen gecompileerd worden met (ruwweg) onderstaande
opdracht door SubGIT met **GCC 14**.

```sh
gcc -std=c17 -O3 -Wall -Werror -lm $(cat ./sources) -o Bloom_filter_cascade
```

De `Dockerfile` en bijhorende bronbestanden die SubGIT gebruikt, kun je vinden in de Git-repository `git@SubGIT.UGent.be:2025-2026/AD3/project-dockerfile`. Je kan de testen van SubGIT lokaal uitvoeren met de volgende commando's in deze repository:

```sh
docker build . -t ad3-project-2025 # hoef je maar één keer te doen
docker run -it --rm --mount type=bind,source={PAD},destination=/submission,readonly ad3-project-2025
```

Waarbij `{PAD}` vervangen dient te worden door het absolute pad naar de hoofdmap
(niet `src`) van je code.

### SubGIT

Het indienen gebeurt via het [SubGIT](https://SubGIT.ugent.be/) platform. Indien je hier nog geen account op hebt, dien je die aan te maken.

#### Repository afhalen

```bash
git clone git@SubGIT.ugent.be:2025-2026/AD3/project/{studentnr} projectAD3
```

Deze repository zal leeg zijn.

#### Opgave als `upstream` instellen

Je kunt de opgave en data voor het project afhalen door de opgave repository als upstream in te stellen met volgende commando's in de `projectAD3` map:

```bash
git remote add upstream git@SubGIT.UGent.be:2025-2026/AD3/project-assignment
git pull upstream main
```

(Je kunt niet pushen naar de upstream, alleen de lesgevers kunnen dat.)

#### Feedback

Als je pusht naar SubGIT, zul je in je terminal te zien krijgen of je code
voldoet aan de minimumvereisten. In dat geval krijg je bij het pushen de melding
dat het pushen geslaagd is:

```
remote: Acceptable submission
```

Je kunt geen code pushen naar de `main` branch als die niet compileert of niet
aan de minimale vereisten voldoet. Je kunt echter wel pushen naar andere
branches en daar zal je push wel aanvaard worden.

#### De `main` branch

De `main` branch op SubGIT stelt jouw indiening voor. Je kunt voor de deadline
zoveel pushen als je wilt. Zorg ervoor dat je voor de deadline zeker je finale
versie naar de **`main`** branch hebt gepusht, want enkel die zal verbeterd worden.

#### Controleren of je zeker goed hebt ingediend

Je kunt jouw indiening bekijken door jouw repository nog eens te clonen in een andere map

```bash
cd eenAndereMap
git clone git@SubGIT.ugent.be:2025-2026/AD3/project/{studentnr} projectAD3Controle
```

### Deadlines en belangrijke data

Zorg ervoor dat je een tussentijdse versie hebt gepusht naar een branch (niet noodzakelijk `main`, moet zelfs niet compileren) op SubGIT voor **woensdag 2025-11-05 om 17:00:00**.
De versie die je hier indient, zal niet beoordeeld worden, we vragen je dit te doen om er zeker van te zijn dat iedereen kan indienen.
Als je niets pusht voor deze deadline verlies je al je punten voor het project.

De code die op **woensdag 2025-12-03 om 17:00:00** op de `main` branch staat, is je finale code-indiening.
Enkel code die op dat moment op de `main` branch staat, wordt verbeterd.
**Als er geen code op je `main` branch staat, krijg je nul op vier voor het project.**
Je kunt na deze deadline nog steeds pushen om wijzigingen aan te brengen aan het verslag en je benchmarks.

Na **woensdag 2025-12-10 om 17:00:00** kun je ook helemaal geen wijzigingen meer aanbrengen aan je repo.
Het verslag dat aanwezig is als pdf op de `main` branch in `extra/verslag.pdf` is je finaal verslag.

## Algemene richtlijnen

- Schrijf efficiënte code, maar ga niet over-optimaliseren: **geef de voorkeur
  aan elegante, goed leesbare code**. Kies zinvolle namen voor methoden en
  variabelen en voorzie voldoende commentaar. Let op geheugenhygiëne: ga actief
  op zoek naar geheugenlekken en alloceer niet onnodig veel geheugen.
- Op SubGIT staat een Dockerfile waarmee je een Linux container kan bouwen die
  jouw code compileert en minimale testen uitvoert. Als alle testen slagen zal
  dit programma `Acceptable submission` uitschrijven als laatste regel en
  stoppen met exit code 0. Code die hier niet aan voldoet, zal geweigerd worden
  door SubGIT en levert geen punten op (0/4 voor het project).
- Het project wordt gequoteerd op **4** van de 20 te behalen punten voor dit
  vak, en deze punten worden ongewijzigd overgenomen naar de tweede
  examenperiode.
- Projecten die ons niet via de `main`-branch op SubGIT bereiken voor de
  deadline worden niet meer verbeterd: dit betekent het verlies van alle te
  behalen punten voor het project.
- Dit is een individueel project en dient dus door jou persoonlijk gemaakt te
  worden. Het is uiteraard toegestaan om ideeën
  uit te wisselen, maar **het is verboden code uit te wisselen**,
  op welke manier dan ook. Het overnemen van code beschouwen we als fraude (van
  **beide** betrokken partijen) en zal in overeenstemming met het
  examenreglement behandeld worden. Op het internet zullen ongetwijfeld ook
  (delen van) implementaties te vinden zijn. Het overnemen of aanpassen van
  dergelijke code is echter **niet toegelaten** en wordt gezien als fraude (behalve voor de hashfunctie). (Als
  je je code op GitHub/Bitbucket/sourcehut/Gitlab/… plaatst moet je die privaat
  houden).
- Het gebruik van AI-assistenten zoals ChatGPT, GitHub CoPilot... is afgeraden, maar niet verboden.
- Alle code die je indient moet je zelf volledig begrijpen en kunnen uitleggen. Bij twijfel over fraude moet je ook kunnen aantonen hoe de code eventueel werd gegenereerd door een AI-systeem. Anders krijg je 0 punten voor het project.
- Er worden punten afgetrokken voor tekst die uitzonderlijk rond de pot draait, of foute beweringen maakt, zoals vaak het geval is bij teksten gegenereerd door AI.
- Essentiële vragen worden **niet** meer beantwoord tijdens de week voor de laatste deadline.

## Vragen

Als je vragen hebt over de opgave of problemen ondervindt, dan kun je je vraag
stellen tijdens het practicum op dinsdagvoormiddag. Contacteer je ons per mail,
stuur dan ook je studentennummer en de branch waarop de code met het probleem
zit mee. Zo kunnen we onmiddellijk je code pullen. Stuur geen screenshots van
code of foutboodschappen.

