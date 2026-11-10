# X01 — Kernel / Memory Bypass Test

[Français](#français) · [English](#english)

> **⚠️ AVERTISSEMENT — PROJET À RISQUE**
>
> Ce dépôt contient du code bas niveau qui exécute un shell Linux via un appel système direct (`execve`) et, dans un exemple, via du code machine (shellcode) placé dans une zone mémoire exécutable. L'exécution peut donner à ce programme les mêmes possibilités que l'utilisateur qui le lance. **N'exécutez pas ces fichiers sur une machine de production, un poste sensible ou un système dont vous ne maîtrisez pas la configuration.**
>
> Utilisez uniquement une machine virtuelle ou un environnement de laboratoire isolé, sur des systèmes que vous possédez ou pour lesquels vous avez une autorisation explicite. Le terme « bypass » dans le nom du dépôt ne signifie pas qu'une élévation de privilèges ou un contournement des protections du noyau est fourni ou garanti.

---

## Français

### Présentation

**X01 — Kernel / Memory Bypass Test** est un petit projet expérimental écrit en C pour étudier certains mécanismes bas niveau de Linux, notamment les appels système, le passage en mode noyau et l'exécution de code en mémoire.

Le projet est destiné à l'apprentissage et à l'expérimentation en laboratoire. Il ne constitue pas un outil de sécurité prêt pour la production.

### Fonctionnalités et fichiers

| Fichier | Description |
|---|---|
| `X01_exec_bash.c` | Exemple d'appel direct à `execve` avec assembleur intégré : `syscall` sous Linux x86-64 et `int 0x80` sous Linux i386. |
| `X01_shellcode_bash.c` | Exemple x86-64 qui copie un shellcode dans une page mémoire exécutable et tente de lancer `/bin/bash`. |
| `X01_kernel_adresses.c` | Recherche plusieurs symboles dans `/proc/kallsyms` et affiche les lignes correspondantes lorsque le système autorise leur lecture. |
| `X01_build_exe.bat` | Script Windows qui recherche GCC et compile les trois fichiers C en exécutables `.exe`. |

### Environnement

- Linux pour exécuter les démonstrations.
- Architecture x86-64 pour les exemples principaux.
- GCC ou un compilateur C compatible.
- WSL2 ou une machine virtuelle Linux peuvent servir d'environnement de test.

La variante `int 0x80` dans `X01_exec_bash.c` est prévue pour Linux i386. Le shellcode fourni est spécifique à Linux x86-64.

### Compilation et exécution sous Linux

Depuis le dossier du projet :

```bash
gcc -Wall -Wextra -O0 -o X01_exec_bash X01_exec_bash.c
gcc -Wall -Wextra -O0 -o X01_shellcode_bash X01_shellcode_bash.c
gcc -Wall -Wextra -O0 -o X01_kernel_adresses X01_kernel_adresses.c
```

Dans une **machine virtuelle de laboratoire isolée**, les programmes peuvent être lancés individuellement :

```bash
./X01_exec_bash
./X01_shellcode_bash
./X01_kernel_adresses
```

Les deux premiers exemples tentent de remplacer le processus courant par `/bin/bash`. Le programme d'adresses consulte `/proc/kallsyms` ; selon la configuration du noyau, les adresses peuvent être masquées ou la lecture restreinte. N'augmentez pas les privilèges uniquement pour contourner une restriction sur une machine que vous ne contrôlez pas.

### Compilation sous Windows

Lancez :

```bat
X01_build_exe.bat
```

Le script recherche GCC dans le `PATH` et dans quelques emplacements courants, puis compile les trois fichiers.

**Important :** la compilation sous Windows ne rend pas le code compatible avec le noyau Windows. Les appels système, `/bin/bash` et `/proc/kallsyms` sont spécifiques à Linux. Les exécutables Windows produits ne constituent donc pas un moyen de lancer ces démonstrations Linux. Pour les tester, compilez-les et exécutez-les dans Linux ou WSL2.

### Limites et précautions techniques

- Le code dépend de l'architecture et de l'ABI Linux ciblées.
- Les protections mémoire (NX, politiques W^X, SELinux ou autres mécanismes de sécurité) peuvent empêcher l'exécution d'une page mémoire à la fois inscriptible et exécutable.
- L'accès à `/proc/kallsyms` varie selon la configuration du noyau et les privilèges du processus.
- Les symboles et adresses du noyau ne sont pas des valeurs stables : ils peuvent varier d'une version ou d'un démarrage à l'autre.
- Ce dépôt ne fournit pas de méthode garantie de contournement du noyau, d'élévation de privilèges ou de désactivation des protections.

[![Architecture diagram](https://gitdiagram.com/diagram-badge.svg)](https://gitdiagram.com/korneevscp/bypass-kernel-memoire-test?utm_source=readme&utm_medium=badge)

### Utilisation responsable

En utilisant ce dépôt, vous vous engagez à :
- travailler uniquement dans un environnement de test autorisé ;
- ne pas exécuter le code sur des systèmes tiers sans autorisation explicite ;
- ne pas utiliser ces exemples pour accéder à des données, comptes ou ressources sans permission ;
- vérifier le code source avant toute compilation ou exécution.

### Licence

Ce projet est distribué sous licence **MIT**. Consultez le fichier [LICENSE](LICENSE) pour les conditions complètes.

---

## English

### Overview

**X01 — Kernel / Memory Bypass Test** is a small experimental C project for studying selected low-level Linux mechanisms, including system calls, transitions into kernel mode, and in-memory code execution.

It is intended for learning and controlled laboratory experimentation. It is not a production-ready security tool.

### Components

| File | Description |
|---|---|
| `X01_exec_bash.c` | Demonstrates a direct `execve` system call using inline assembly: `syscall` on Linux x86-64 and `int 0x80` on Linux i386. |
| `X01_shellcode_bash.c` | x86-64 example that copies shellcode into an executable memory page and attempts to launch `/bin/bash`. |
| `X01_kernel_adresses.c` | Searches `/proc/kallsyms` for selected kernel symbols and prints matching lines when access is permitted. |
| `X01_build_exe.bat` | Windows batch script that locates GCC and compiles the three C files into `.exe` binaries. |

### Requirements

- Linux to run the demonstrations.
- x86-64 architecture for the primary examples.
- GCC or a compatible C compiler.
- WSL2 or a Linux virtual machine can be used as a test environment.

The `int 0x80` path in `X01_exec_bash.c` is intended for Linux i386. The supplied shellcode targets Linux x86-64 only.

### Build and run on Linux

From the project directory:

```bash
gcc -Wall -Wextra -O0 -o X01_exec_bash X01_exec_bash.c
gcc -Wall -Wextra -O0 -o X01_shellcode_bash X01_shellcode_bash.c
gcc -Wall -Wextra -O0 -o X01_kernel_adresses X01_kernel_adresses.c
```

Inside an **isolated laboratory virtual machine**, run each program separately:

```bash
./X01_exec_bash
./X01_shellcode_bash
./X01_kernel_adresses
```

The first two examples attempt to replace the current process with `/bin/bash`. The symbol utility reads `/proc/kallsyms`; depending on kernel configuration, addresses may be hidden or access may be restricted. Do not increase privileges merely to bypass such restrictions on a system you do not control.

### Build on Windows

Run:

```bat
X01_build_exe.bat
```

The script searches the `PATH` and several common locations for GCC, then compiles the three C files.

**Important:** compiling on Windows does not make this code compatible with the Windows kernel. The system calls, `/bin/bash`, and `/proc/kallsyms` are Linux-specific. The resulting Windows executables are therefore not a way to run the Linux demonstrations. Build and run them in Linux or WSL2 instead.

### Technical limitations and safety notes

- The code is dependent on the target Linux architecture and ABI.
- Memory protections (NX, W^X policies, SELinux, and other security controls) may prevent execution from a memory page that is both writable and executable.
- Access to `/proc/kallsyms` depends on kernel configuration and process privileges.
- Kernel symbols and addresses are not stable values; they may change across kernel versions and system boots.
- This repository does not provide a guaranteed kernel bypass, privilege-escalation method, or security-control disablement.

[![Architecture diagram](https://gitdiagram.com/diagram-badge.svg)](https://gitdiagram.com/korneevscp/bypass-kernel-memoire-test?utm_source=readme&utm_medium=badge)

### Responsible use

By using this repository, you agree to:
- work only in an authorized test environment;
- never run the code on third-party systems without explicit permission;
- never use these examples to access data, accounts, or resources without authorization;
- review the source code before compiling or running it.

### License

This project is released under the **MIT License**. See [LICENSE](LICENSE) for the full terms.
