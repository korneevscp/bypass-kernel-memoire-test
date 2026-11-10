# X01 - lancer /bin/bash en passant par le noyau

un petit projet pour comprendre comment on appelle le noyau depuis un
programme, en C sur linux x86-64. j'ai fait ça pour le fun / apprendre.

## comment ca marche vite fait

- `SYSCALL` (les octets 0F 05) -> le cpu saute dans le noyau (a l'adresse
  du MSR LSTAR 0xC0000082)
- `sys_call_table` contient les numeros des appels systeme. execve = 59 en
  64 bits, 11 en 32 bits
- `int 0x80` c'est la vieille porte 32 bits

pour execve on remplit les registres :
```
RAX = 59         (numéro de l'appel execve)
RDI = "/bin/bash"
RSI = les arguments
RDX = NULL
```

## les fichiers

| fichier | c'est quoi |
|---|---|
| `X01_shellcode_bash.c` | un shellcode de 30 octets + un programme qui le copie dans une page et saute dessus |
| `X01_exec_bash.c` | appelle le noyau en asm inline, avec la variante 32 bits aussi |
| `X01_kernel_adresses.c` | lit /proc/kallsyms et affiche les adresses du noyau |
| `X01_build_exe.bat` | compile le tout sous windows avec gcc (codeblocks / mingw) |

## compiler sous windows

double clic sur `X01_build_exe.bat`. il cherche gcc tout seul et compile les 3.

## tester sur linux (wsl2, vm...)

```
gcc -o X01_shellcode_bash X01_shellcode_bash.c
./X01_shellcode_bash              # hop un bash

echo 'echo PWNED-$((6*7))' | ./X01_shellcode_bash   # doit sortir PWNED-42
```

pour exec_bash pareil. pour les adresses il faut etre root :
```
sudo ./X01_kernel_adresses
```

## note

sous windows les .exe compilent mais font rien (pas de syscall en mode user
windows, et /bin/bash c'est du linux). faut vraiment du linux pour tester.

si le noyau refuse la page RWX (selinux / W^X), compile avec `gcc -z execstack`
et utilise mprotect avant le saut. (pas encore trop testé ça)
