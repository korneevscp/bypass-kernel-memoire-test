// petit truc pour lancer /bin/bash sans passer par la libc
// trouve sur un forum, j'ai juste adapte pour que ca compile chez moi
// ca marche que sur linux 64 bits (j'ai testé en wsl)

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef __linux__
   #ifdef __x86_64__
      #include <sys/mman.h>
   #endif
#endif

// le shellcode. en gros c'est des octets qui font un execve.
// je me souviens plus exactement de chaque instruc mais ca marche donc bon
// (30 octets en tout, sans octet 0 dedans c'est important apparement)
static unsigned char shellcode[] = {
  0x31,0xd2,        // xor edx, edx
  0x52,             // push rdx
  0x6a,0x68,        // push 'h'
  0x48,0xb8, 0x2f,0x62,0x69,0x6e, 0x2f,0x62,0x61,0x73,   // /bin/bas en hexa
  0x50,             // push rax
  0x48,0x89,0xe7,   // mov rdi, rsp
  0x52,             // push rdx
  0x57,             // push rdi
  0x48,0x89,0xe6,   // mov rsi, rsp
  0x31,0xc0,        // xor eax
  0xb0,0x3b,        // 59 = execve (a verifier mais ca marche)
  0x0f,0x05         // syscall
};

int X01_main(void); // ma fonction principale

// je crois que c'est 59 le numero de execve sur 64 bits.
// sur 32 bits c'est 11. enfin peu importe.

void lancer(void)
{
    #if defined(__linux__) && defined(__x86_64__)
    // alloue une page qu'on peut executer. RWX c'est pas propre mais bon
    void* m = mmap(0, 4096, PROT_READ|PROT_WRITE|PROT_EXEC,
                 MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (m == (void*)-1) {
        printf("mmap a rate :(\n");
        return;
    }

    memcpy(m, shellcode, sizeof(shellcode));

    // petite transformation pour appeler la page comme une fonction
    void (*f)(void) = (void (*)(void)) m;

    printf("je saute dessus @%p\n", m);
    fflush(stdout);

    f(); // normalement on revient jamais ici

    printf("bon bah execve a echoué...\n");
    munmap(m, 4096);

    #else
    printf("que sur linux 64bits désolé\n");
    #endif
}

int X01_main(void)
{
    printf("shellcode execve /bin/bash (%d octets)\n", (int)sizeof(shellcode));

    lancer();

    // ca devrait pas arriver
    printf("fin (si tu vois ca c'est que ca a foiré)\n");
    return 0;
}

int main()
{
    return X01_main();
}
