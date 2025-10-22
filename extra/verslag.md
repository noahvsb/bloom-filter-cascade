## algoritme

### cascadetrap met $C_1, C_2, ..., C_n$

$C_1 \Rightarrow BF_1(C_2, C_3, ..., C_n) \Rightarrow C_1'$

$C_2 \Rightarrow BF_2(C_1', C_3, C_3, ..., C_n) \Rightarrow C_2'$

$C_3 \Rightarrow BF_3(C_1', C_2', C_4, C_5, ..., C_n) \Rightarrow C_3'$

$...$

$C_n \Rightarrow BF_n(C_1', C_2', ..., C_{n-1}') \Rightarrow C_n'$

Bij $BF_i$ als het antwoord "neen" is, dan weten we dat het sowieso niet in de bijhorende $C_i$ zit.

Zo hebben we al veel elementen kunnen uitsluiten.

Dan de volgende cascade doen we met $C_1', C_2', ..., C_n'$

<!--
Weggecomment omdat na efkes nadenken dit niet zoveel zal uitmaken, het is vermoedelijk zelfs beter om het niet om te draaien

Het is wel duidelijk dat $C_1'$ gemiddeld groter zal zijn dan $C_n'$ en dezelfde volgorde opnieuw aanhouden zal deze kloof steeds meer en meer vergroten. De beste manier om dit tegen te gaan en daardoor minder cascadetrappen te hebben is om alles te sorteren op grootte en de grootste laatst te doen. Maar dit zal traag zijn, dus een mogelijkheid is om met een heuristiek te werken, bv. telkens omdraaien, eerst $C_1, ..., C_n,$ dan $C_n', ..., C_1',$ dan $C_1'', ..., C_n''$ enz.
-->


## # bits en hashfuncties

\# hashfuncties optimaal: $k = \frac{n}{m} * \ln{2}$

kans op false positives: $(1 - e^{\frac{-km}{n}})^k$

kans bij optimale k: $(1 - e^{-\ln{2}})^{\frac{n}{m} * \ln{2}} = (1 - \frac{1}{2})^{\frac{n}{m} * \ln{2}} = (\frac{1}{2})^{\frac{n}{m} * \ln{2}}$

hoeveel moet n i.f.v. met m zijn om de $\text{kans} \leq x$ te maken?

$
(\frac{1}{2})^{\frac{n}{m} * \ln{2}} \leq x 
\Leftrightarrow 
2^{\frac{n}{m} * \ln{2}} \geq \frac{1}{x}
\Leftrightarrow
\frac{n}{m} * \ln{2} \geq \ln{(\frac{1}{x})}
\Leftrightarrow
n \geq -m*\frac{\ln{x}}{\ln{2}}
$

neem voor x telkens een negatieve macht van 2: $x = 2^{-p}$ met $p = 1, 2, 3, ...$

dan wordt onze formule:

$n \geq m * p$

Dus als we kans op false positives 1/32 willen en m = 1 MB (1 000 000 B) dan hebben we:

$n \geq 5 \text{ MB}$

en bij $n = 5 \text{ MB}$ hebben we:

$k = \lfloor 5 * \ln{2} \rfloor = \lfloor 3.465... \rfloor = 3$

deze p zullen we meegeven aan de bloomfilter om de n mee te berekenen

met benchmarks kunnen we dan achteraf een goede heuristiek voor p vinden voor mijn algoritme

## bestandsformaat cascade

vooraf:
- aantal categoriëen (32 bits) = aantal bloomfilters per trap
- namen van categoriëen in juiste volgorde (8 * (lengte + 1) bits), +1 door het eindigen met \0 na elke naam

elke cascade trap:
- /

elke bloomfilter binnen de trap:
- aantal hashfuncties in bloomfilter (8 bits)
- seeds voor hashfuncties (8 bits elk)
- aantal bits in bloomfilter / 8 (32 bits)
- bloomfilter bits

indien de categorie horende bij een bloomfilter leeg is, is het aantal bits gewoon 0 en ga je verder, dit is beter dan elke keer alle nieuwe lege categoriëen aan te kondigen of nutteloze bloomfilters aan te maken

eindigen met 32 1 bits, aangezien 32 0 bits een lege bloomfilter betekent
en dan de laatste niet lege categorie naam in ascii (8 * lengte bits) en een EOF, geen nut om een nieuwe trap te maken indien nog maar 1 categorie overschiet die niet leeg is

