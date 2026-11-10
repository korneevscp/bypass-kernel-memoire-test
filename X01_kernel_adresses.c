// affiche les adresses du noyau depuis /proc/kallsyms
// faut etre root sinon ca affiche des 000000000000000000
// (pas de /proc sur windows donc message d'erreur)

#include <stdio.h>
#include <string.h>

int X01_main(void);

// cherche un motif dans kallsyms et affiche les lignes qui matchent
static void chercher(const char* mot)
{
    FILE* f = fopen("/proc/kallsyms", "r");
    if (f == NULL) {
        printf("impossible d'ouvrir /proc/kallsyms (linux + root requis)\n");
        return;
    }

    char ligne[512];
    while (fgets(ligne, sizeof ligne, f) != NULL) {
        if (strstr(ligne, mot) != NULL) {
            printf("%s", ligne);
        }
    }
    fclose(f);
}

int X01_main(void)
{
    // les trucs que je veux voir
    char* trucs[] = {
        "entry_SYSCALL_64",
        "entry_INT80",
        "sys_call_table",
        "__x64_sys_execve",
        NULL
    };

    printf("adresses noyau depuis /proc/kallsyms :\n");

    int i = 0;
    while (trucs[i] != NULL) {
        printf("--- %s ---\n", trucs[i]);
        chercher(trucs[i]);
        i++;
    }

    return 0;
}

int main(void)
{
    return X01_main();
}
