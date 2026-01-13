# Projektpolicy - Feedback Station Pico W

Detta dokument beskriver de regler och processer vi följer i projektet för att säkerställa hög kvalitet, kunskapsspridning och en professionell arbetsmetodik.

## 1. Arbetssätt
Vi använder Trello som vår centrala lla gällande arbetsuppgifter.
* **Backlog:** Alla framtida idéer och krav.
* **Sprint Backlog (Todo):** Uppgifter vi har lovat att genomföra i nuvarande sprint.
* **In Progress:** Här ligger kortet när man börjar ko Fördela kortet i gruppen så att alla får möjligheten att vara delaktiga.
* **Review:** Här hamnar uppgifter när en Pull Request (PR) har skapats.
* **Done:** Uppgiften flyttas hit först när den uppfyller vår "Definition of Done".

## 2. Branching & Git-strategi
Vi arbetar med en strukturerad branching-modell för att hålla `main` stabil.
* **Main:** Innehåller endast kod som är redo för produktion/hårdvara.
* **Develop:** Vår huvudsakliga arbetsbranch där features integreras.
* **Feature branches:** Vi separerar på main med en extra branch som skapas utifrån 'develop'. Varje task/kort på Trello får en egen branch som skapas utifrån `developp
`.
  * Namngivning: `feat/beskrivning-av-task` (t.ex. `feat/voting-logic`).
* **Merge:** Ingen kod mergas direkt till `develop`. Det sker via en Pull Request på GitHub.

## 3. Kodpolicy
Syftet med denna policy är att säkerställa att koden är enhetlig, lättläst och enkel att enhetstesta genom att strikt separera logik från hårdvara.

### 3.1. Namngivning (Naming Conventions)
För att undvika krockar och öka tydligheten använder vi följande prefix och stilar:
* Filer: Använd snake_case. Hårdvarunära filer prefixas med drivrutinsnamn (t.ex. led_driver.c), logikfiler med modulnamn (t.ex. vote_logic.c).
* Funktioner: modulnamn_funktions_namn() (t.ex. vote_register_increment()).
* Variabler: snake_case (t.ex. button_press_count).
* Konstanter/Macros: SCREAMING_SNAKE_CASE (t.ex. MAX_VOTES_PER_SESSION).* Typer (Structs/Enums): Slutar på _t (t.ex. vote_state_t).

### 3.2. Arkitektur: Separation av Logik och I/O
För att uppfylla kravet på modularitet ska koden delas upp i tre lager:
* Hårdvarulager (I/O): Hanterar Pico W-specifik kod (SDK-anrop, GPIO, Wi-Fi). Här tillåts printf och hårdvaruanrop.
* Logiklager: Innehåller ren C-kod för beräkningar och tillstånd. Detta lager får inte inkludera pico/stdlib.h eller använda I/O.
* Applikationslager (Main): Binder samman logik och hårdvara.

### 3.3. Hantering av Data (Inga globala variabler)
* Enligt policyn får globala variabler inte användas. All data ska kapslas in i structs och skickas som pekare.
* State-structs: Varje modul ska definiera en struct som håller dess tillstånd.
* Dependency Injection: Funktioner ska ta emot den data de behöver som argument.

Exempel:
```
// I vote_logic.h
typedef struct {
    int upvotes;
    int downvotes;
} vote_data_t;
// Logik-funktion (testbar utan hårdvara)
void vote_process_input(vote_data_t *state, int input_type) {
    if (input_type == 1) state->upvotes++;
}
```

### 3.4. Felhantering och Typer
* Returkoder: Funktioner som kan misslyckas ska returnera en bool eller en enum för felstatus (t.ex. result_t), inte magiska siffror.
* Standardtyper: Använd <stdint.h> för exakta bitbredder (t.ex. uint8_t, int32_t) istället för int eller char där storleken är kritisk.
* Ingen GPL: Innan ett externt bibliotek inkluderas måste licensen kontrolleras (MIT, Apache eller BSD är godkända).

### 3.5. Formatering (Style Guide)
* Standard: C99 eller C11.
* Indentering: 4 mellanslag (inga tabs).
* Måsvingar: Samma rad som if/while/for.
* Kommentarer: * Doxygen-format (/** ... */) i headerfiler för att beskriva funktioners in- och utdata.
* "Varför" snarare än "Vad" i källkodsfiler.


## 4. Kodgranskning (Code Review)
För att sprida kunskap inom teamet (enligt krav för Sprint 1):
* Varje Pull Request måste granskas och godkännas av minst en annan teammedlem.
* Granskaren ska kontrollera att koden följer policyn och att den är begriplig.

## 5. Tekniska Standarder
* **Språk:** All kod skrivs i ren C (C99/C11 standard)
* **Licens:** Ingen GPL-licensierad kod får användas i projektet.
* **Modularitet:** Logik (beräkningar) ska vara helt separerad från I/O (printf/scanf/hårdvara).
* **Inga globala variabler:** All data skickas via structs för att möjliggöra tester.

## 6. Enhetstester (Unit Testing)
* Eftersom logiken är separerad från hårdvaran och vi skickar in structs, ska varje logikmodul ha en motsvarande testfil.
*Testerna ska kunna köras på en vanlig dator (Host-based testing) utan att en Pico W är inkopplad.
*Mocks: Om logiken behöver interagera med något externt, använd funktionspekare eller mock-objekt i din struct.

## 7. Definition of Done (DoD)
Ett kort anses vara "Done" när:
1. Koden kompilerar utan varningar.
2. Enhetstester har skrivits för ny logik och alla tester passerar.
3. Koden har genomgått en Code Review och blivit godkänd.
4. Dokumentationen (t.ex. README) är uppdaterad vid behov.
