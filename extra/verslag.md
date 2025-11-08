## algoritme

### cascadetrap met $C_1, C_2, ..., C_n$

$C_1 \Rightarrow BF_1(C_2, C_3, ..., C_n) \Rightarrow C_1'$

$C_2 \Rightarrow BF_2(C_1', C_3, C_3, ..., C_n) \Rightarrow C_2'$

$C_3 \Rightarrow BF_3(C_1', C_2', C_4, C_5, ..., C_n) \Rightarrow C_3'$

$...$

$C_n \Rightarrow BF_n(C_1', C_2', ..., C_{n-1}') \Rightarrow C_n'$

Bij $BF_i$ als het antwoord "neen" is, dan weten we dat het sowieso in de bijhorende $C_i$ zit.

Zo hebben we al veel elementen kunnen uitsluiten.

Dan kunnen we de volgende cascadetrap analoog doen met $C_1', C_2', ..., C_n'$

Indien de categoriëen die we in een bloomfilter zullen steken allemaal leeg zijn, stopt ons algoritme.

Indien alle categoriëen behalve 1 leeg zijn na het afwerken van de volledige trap, stopt ons algoritme ook.

Na het stoppen zal er 1 niet-lege categorie overblijven, hiermee kunnen wij dus de gevallen classificeren die overal (false-)positives zijn.

### cascade voor $C_1$, cascade voor $C_2$, ..., cascade voor $C_3$

Het vorige algoritme is simpel en snel, maar we kunnen beter doen omtrent opslag.

De titel van de paragraaf doet denken dat het algoritme slecht gaat zijn, een volledig nieuwe cascade voor elke categorie. Maar we kunnen hem zodanig opbouwen dat het veel minder opslag opneemt (het zal wel trager zijn dan het vorig algoritme, maar opslag is hier belangrijker).

#### cascade voor $C_i$

`<placeholder>`

## # bits en hashfuncties

\# hashfuncties optimaal: $k = \frac{n}{m} * \ln{2}$ met $m = \text{\# elementen}$ en $n = \text{\# bits}$

kans op false positives: $(1 - e^{\frac{-km}{n}})^k$

kans bij optimale k: $(1 - e^{-\ln{2}})^{\frac{n}{m} * \ln{2}} = (1 - \frac{1}{2})^{\frac{n}{m} * \ln{2}} = (\frac{1}{2})^{\frac{n}{m} * \ln{2}}$

hoeveel moet n i.f.v. met m zijn om de $\text{kans} \leq x$ te maken?

$$
(\frac{1}{2})^{\frac{n}{m} * \ln{2}} \leq x 
\Leftrightarrow 
2^{\frac{n}{m} * \ln{2}} \geq \frac{1}{x}
\Leftrightarrow
\frac{n}{m} * (\ln{2})^2 \geq \ln{(\frac{1}{x})}
\Leftrightarrow
n \geq -m*\frac{\ln{x}}{(\ln{2})^2}
$$

neem voor x telkens een negatieve macht van 2: $x = 2^{-p}$ met $p = 1, 2, 3, ...$

dan wordt onze formule:

$n \geq \frac{m * p}{\ln 2}$

indien het een gelijkenis is heb je:

$k = \frac{\frac{m * p}{\ln 2}}{m} * \ln 2 = p$

Vanaf nu zal ik k gebruiken om naar p te refereren

Dus als we kans op false positives 1/32 willen en m = 1 MB (1 000 000 B) dan hebben we:

$n \geq \frac{5}{\ln 2} \text{ MB} = 7.213... \text{ MB}$

en $k = 5$

We zullen dus nu een k meegeven aan de bloomfilter om de n mee te berekenen zodanig dat de kans op false positives kleiner is dan $2^{-k}$

Waarom doe ik $2^{-k}$ als kans en geef ik de k mee om daaruit de n te halen i.p.v. de kans op false positives mee te geven en daaruit de k en n te halen?

Omdat de afrondingen dan veel minder doorwegen. Nu heb ik afrondingen op mijn aantal bits dat makkelijk een paar miljoen kan zijn, terwijl ik geen afrondingen heb op mijn k.

Bv. een afronding van 5.4 naar 5 bij de k zal meer verschil geven dan een afronding van 1234567.4 naar 1234567, niet alleen doordat het getal daardoor relatief minder verkleint, maar ook omdat ik die n toch moet afronden naar een getal deelbaar door 8 (aangezien ik een `uint8_t*` gebruik voor de bloomfilter bits). Dus 1234567.4 wordt 1234567, maar dan 1234568 om deelbaar door 8 te worden, dus de afronding maakt geen verschil.

## bestandsformaat cascade

vooraf:
- 1 bit die het soort algoritme aangeeft (0 = snel algoritme, 1 = algoritme met weinig opslag)
- aantal categoriëen (32 bits) = aantal bloomfilters per trap
- namen van categoriëen in juiste volgorde (8 bits lengte + 8 bits * len)

elke cascade trap:
- /

elke bloomfilter binnen de trap:
- aantal hashfuncties in bloomfilter (8 bits)
- seeds voor hashfuncties (8 bits seeds * aantal hashfuncties)
- aantal bits in bloomfilter / 8 (32 bits)
- bloomfilter bits (veelvoud van 8)

indien de categorie horende bij een bloomfilter leeg is, zet je het aantal hashfuncties gewoon 0 en ga je verder, dit is beter dan elke keer alle nieuwe lege categoriëen aan te kondigen of nutteloze bloomfilters aan te maken

- einde van bloomfilters aankondigen met 8 x 1-bits, aangezien 8 x 0-bits een lege categorie betekent

- dan eindigen met de laatste niet lege categorie naam (8 bits lengte + 8 bits * len) (indien in dezelfde trap alle niet-lege categoriëen simultaan leeg geworden zijn, dan is de lengte 0)

## limitaties

Voor de categorie namen is er een limitatie van 256 chars bij zowel `train` als `classify`

Indien een ingegeven element bij `classify` in geen enkele categorie zit, dan zal deze geclassificeerd worden als een random categorie. Meestal de eerste waarvan een cascade wordt gemaakt.

Voor de nummers (aantal elementen, aantal bits ...) is er een limitatie van 32 bits (0 - 4,294,967,295)

Bv. het aantal elementen in `large.txt` is 25,000,000 en pas vanaf we een $k \geq 120$ hebben zal het aantal bits daar groter zijn dan de limitatie, maar dit is een onrealistische situatie, in quasi alle gevallen zal $k \leq 8$ zijn en dan kunnen we 372,130,558 elementen hebben, dus deze limiet is voldoende. 

Ik kan het verhogen naar 64 bit unsigned integers, maar dat zal het aantal geheugen onnodeloos vergroten, aangezien de laatste 32 bits quasi nooit gebruikt zullen worden en we veel zo'n nummers zullen gebruiken. 

Ook zal ik daardoor een andere hash functie moeten vinden, aangezien de implementatie van murmur has die ik op het internet vond 32 bit unsigned integers retourneert.


