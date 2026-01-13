# Projektpolicy - Feedback Station Pico W

Detta dokument beskriver de regler och processer vi följer i projektet för att säkerställa hög kvalitet, kunskapsspridning och en professionell arbetsmetodik.

## 1. Arbetssätt
Vi använder Trello som vår centrala lla gällande arbetsuppgifter.
* **Backlog:** Alla framtida idéer och krav.
* **Sprint Backlog (Todo):** Uppgifter vi har lovat att genomföra i nuvarande sprint.
* **In Progress:** Här ligger kortet när man börjar ko F?rdela kortet i gruppen så att alla f?r m?jligheten att vara delaktiga.
* **Review:** Här hamnar uppgifter när en Pull Request (PR) har skapats.
* **Done:** Uppgiften flyttas hit först när den uppfyller vår "Definition of Done".

## 2. Branching & Git-strategi
Vi arbetar med en strukturerad branching-modell för att hålla `main` stabil.
* **Main:** Innehåller endast kod som är redo för produktion/hårdvara.
* **Develop:** Vår huvudsakliga arbetsbranch där features integreras.
* **Feature branches:** Vi separerar p? main med en extra branch som skapas utifr?n 'develop'. Varje task/kort på Trello får en egen branch som skapas utifrån `developp
`.
  * Namngivning: `feat/beskrivning-av-task` (t.ex. `feat/voting-logic`).
* **Merge:** Ingen kod mergas direkt till `develop`. Det sker via en Pull Request på GitHub.

## 3. Kodgranskning (Code Review)
För att sprida kunskap inom teamet (enligt krav för Sprint 1):
* Varje Pull Request måste granskas och godkännas av minst en annan teammedlem.
* Granskaren ska kontrollera att koden följer policyn och att den är begriplig.

## 4. Tekniska Standarder
* **Språk:** All kod skrivs i ren C (C99/C11 standard)
* **Licens:** Ingen GPL-licensierad kod får användas i projektet.
* **Modularitet:** Logik (beräkningar) ska vara helt separerad från I/O (printf/scanf/hårdvara).
* **Inga globala variabler:** All data skickas via structs för att möjliggöra tester.

## 5. Definition of Done (DoD)
Ett kort anses vara "Done" när:
1. Koden kompilerar utan varningar.
2. Enhetstester har skrivits för ny logik och alla tester passerar.
3. Koden har genomgått en Code Review och blivit godkänd.
4. Dokumentationen (t.ex. README) är uppdaterad vid behov.
