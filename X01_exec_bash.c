// appelle execve directement avec syscall, sans utiliser la fonction
// execve de la libc. c'etait pour comprendre comment ca marche sous le capot
// (linux uniquement, sur windows ca fait rien)

#include <stdio.h>

int X01_main(void);

// la fonction qui fait le vrai boulot
static long lancer_bash(void)
{
#if defined(__linux__) && defined(__x86_64__)
    // 64 bits
    char* chemin = "/bin/bash";
    char* args[] = { chemin, 0 };

    long r = 0;

    // 59 = execve. je met les registres a la main et je fais syscall
    asm volatile(
        "syscall"
        : "=a"(r)
        : "a"(59), "D"(chemin), "S"(args), "d"(0)
        : "rcx", "r11", "memory");

    // normalement on passe pas ici vu que execve remplace le process
    return r;

#elif defined(__linux__) && defined(__i386__)
    // vieux 32 bits, au cas ou
    char* chemin = "/bin/bash";
    char* args[] = { chemin, 0 };

    long r = 0;

    asm volatile(
        "int $0x80"
        : "=a"(r)
        : "a"(11), "b"(chemin), "c"(args), "d"(0)
        : "memory");

    return r;

#else
    // pas linux => on peut rien faire
    return -1;
#endif
}

int X01_main(void)
{
    long r = lancer_bash();

#if defined(__linux__)
    if (r != 0) {
        printf("execve a echoue, retour = %ld\n", r);
        return 1;
    }
#else
    (void)r;
    printf("pas de syscall sur windows, teste dans wsl ou une vm\n");
#endif
    return 0;
}

int main(void)
{
    return X01_main();
}
