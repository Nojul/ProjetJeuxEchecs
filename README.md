# ProjetJeuxEchecs

Jeu d'échecs interactif avec interface graphique Qt, développé en C++ dans le cadre d'un cours de programmation orientée objet. L'application permet de rejouer des parties historiques célèbres et de les explorer de manière interactive.

*Latendresse & Julien — avril / mai 2025*

---

## Aperçu
<p align="center">
  <img width="550" alt="Interface principale" src="https://github.com/user-attachments/assets/04bf2494-da00-47ec-875c-3fa3ca8e60b5" />
</p>

---

## Fonctionnalités

### Positions historiques célèbres

Quatre parties emblématiques de l'histoire des échecs sont disponibles, reconstituées fidèlement :

| Partie | Année |
|---|---|
| La Bourdonnais vs. McDonnell | 1834 |
| Karpov vs. Ftacnik | 1988 |
| J. Polgar vs. Kasparov | 1996 |
| Alekhine vs. Capablanca | 1927 |

### Interface graphique échiquier

- Échiquier interactif : sélection et déplacement des pièces au clic
- Mise en évidence de la pièce sélectionnée (bordure verte)
- Indicateur de tour (Blanc / Noir) mis à jour en temps réel
- Réinitialisation de la partie à tout moment

### Détection de fin de partie

L'application détecte automatiquement la fin de partie et affiche un message de résultat. Les cases sont désactivées dès que la partie est terminée.

---

## Technologies

- **C++17**
- **Qt 6** — Core + Widgets (AUTOUIC / AUTORCC / AUTOMOC)
- **CMake** ≥ 3.10
- **GoogleTest** — inclus dans le dépôt

---

## Installation et compilation

### Prérequis (macOS)

```bash
brew install qt
```

### Compiler et lancer

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/ProjetJeuxEchecs
```

> Si Qt est installé via l'installeur officiel :
> `cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/<version>/lib/cmake`

---

## Structure du projet

```
ProjetJeuxEchecs/
├── src/                  # Fichiers source (.cpp)
├── include/              # En-têtes (.h)
├── resources/            # Fichiers .ui et .qrc
├── bibliotheque_cours/   # Utilitaires du cours
├── googletest/           # Framework de tests
└── visual_studio/        # Fichiers VS originaux (référence)
```

---

## Auteurs

- **Marina Latendresse**
- **Noé Julien**
