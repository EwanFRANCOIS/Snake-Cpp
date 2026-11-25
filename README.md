# Snake en C++ (Terminal)

Un jeu **Snake** minimaliste en **C++17** jouable directement dans le terminal (Linux / WSL et Windows), sur une grille **10×10** où le serpent grandit à chaque nourriture mangée.

---

## Prérequis

- Un compilateur C++ compatible **C++17** (`g++` sous Linux / WSL, ou MinGW / MSVC sous Windows).
- `make` (optionnel, si tu utilises le `makefile` fourni).
- Un terminal supportant les séquences d'échappement ANSI (ex. Windows Terminal, GNOME Terminal, iTerm2).

---

## Étapes d'installation et de compilation

### Étape 1 : Se placer dans le dossier du projet

Ouvre ton terminal et place-toi dans le dossier contenant ton fichier source (`main.cpp`) et ton `makefile` :

```bash
cd chemin/vers/le/dossier
```

*(Sous WSL, si ton projet est sur le Bureau Windows : `cd /mnt/c/Users/<ton-utilisateur>/Desktop/...`)*

### Étape 2 : Installer `g++` et `make` (Linux / WSL)

1. Mets à jour la liste des paquets :
   ```bash
   sudo apt update
   ```
2. Installe `g++` et `make` :
   ```bash
   sudo apt install -y g++ make
   ```
3. Vérifie que `g++` est bien installé :
   ```bash
   g++ --version
   ```

### Étape 3 : Compiler le programme

Choisis la méthode qui correspond à ton environnement :

- **Avec `make` (Linux / WSL)** — si ton fichier source s'appelle `main.cpp` :
  ```bash
  make nom=main
  ```
  *(Si tu as un autre fichier `<nom>.cpp`, remplace `main` par `<nom>`.)*

- **Sans `make` (Linux / WSL)** :
  ```bash
  g++ -std=c++17 -Wall -Wextra main.cpp -o main
  ```

- **Sous Windows (MinGW)** :
  ```bash
  g++ -std=c++17 -Wall -Wextra main.cpp -o main.exe
  ```

---

## Étapes pour lancer le jeu

1. Assure-toi que la compilation s'est terminée sans erreur.
2. Lance l'exécutable depuis le dossier du projet :
   - **Linux / WSL :**
     ```bash
     ./main
     ```
   - **Windows :**
     ```powershell
     .\main.exe
     ```

---

## Comment jouer

### Contrôles

| Touche | Action |
| :--- | :--- |
| **Z** | Déplacer le serpent vers le haut |
| **S** | Déplacer le serpent vers le bas |
| **Q** | Déplacer le serpent vers la gauche |
| **D** | Déplacer le serpent vers la droite |
| **X** | Quitter la partie |

### Légende de la grille

- `1` : Le serpent (tête et corps)
- `8` : La nourriture
- `.` : Case vide

### Règles de la partie

- Guide le serpent (`1`) vers la nourriture (`8`) pour marquer des points.
- Chaque nourriture mangée augmente le **score de 1** et **allonge le serpent d'une case**.
- Les demi-tours instantanés dans la direction opposée sont ignorés pour éviter de te retourner sur place.
- La partie se termine (**Perdu !**) si le serpent sort de la grille ou entre en collision avec son propre corps.

---

## Nettoyage

Pour supprimer l'exécutable généré avec `make` :

```bash
make clean
```
