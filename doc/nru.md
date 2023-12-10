# Algoritmo NRU (Not Recently Used)
### TP 4 de Sistemas Operacionais cujo objetivo foi implementar o algoritmo de substituição de páginas NRU no Nanvix

### Limpeza do bit de Referência (Accessed)
Para a limpeza do bit de referência, foi definido o período de 40 acessos que é controlado pela variável `time`. Nós usamos de 2 estruturas de repetição, onde uma percorre pelos processos e a outra pelos frames associados a um processo. Uma vez encontrado um frame que é associado à um processo, a tabela é buscada e o bit `accessed` recebe 0.

```c
if (time == 40)
    {
        for (j = 0; j < NR_FRAMES; j++)
        {
            for (proc = FIRST_PROC; proc <= LAST_PROC; proc++)
            {
		//encontrando processo dono daquele frame
                if (proc->pid == frames[j].owner)
                {
		    //lendo sua tabela de páginas
                    pg = getpte(proc, frames[j].addr);
			
		    //limpando seu bit de referenciado
                    pg->accessed = 0;

                    // Incrementa a idade de todas as páginas
                    frames[j].age++;
                }
            }
        }
        time = 0;
    }
    else
        time++;
}
````

### NRU
Prioridades de alocação:
1 - páginas vazias
2 - páginas do processo que quer alocar uma página
2.1 - páginas que não estão compartilhadas
2.1.1 - não modificada e não acessada nos últimos 40 acessos
2.1.2 - não modificada e acessada nos últimos 40 acessos
2.1.3 - modificada e não acessada nos últimos 40 acessos
2.1.4 - modificada e acessada nos últimos 40 acessos

Variável `replaced` salva o índice do frame que contém a página que será substituída.

```c
 for (j = 0; j < NR_FRAMES; j++)
    {
        /* Found it. Frame empty.*/
        if (frames[j].count == 0)
            goto found;

        /* Local page replacement policy. */
        if (frames[j].owner == curr_proc->pid)
        {
            /* Skip shared pages. */
            if (frames[j].count > 1)
                continue;

            // Finding page table
            pg = getpte(curr_proc, frames[j].addr);

            // Priority to be replaced.
            if (pg->dirty == 0 && pg->accessed == 0)
            {
                replaced = i;
            }
            else if (pg->dirty == 0 && pg->accessed == 1)
            {
                replaced = i;
            }
            else if (pg->dirty == 1 && pg->accessed == 0)
            {
                replaced = i;
            }
            else if (pg->dirty == 1 && pg->accessed == 1)
            {
                replaced = i;
            }
        }
    }
```

### Aging

O conceito de *aging* (envelhecimento) neste código realiza estratégia de atualização da "idade" de cada página presente na memória física. O código faz isso através do seguinte trecho:

```c
if (time == 8)
{
    for (proc = FIRST_PROC; proc <= LAST_PROC; proc++)
    {
        for (j = 0; j < NR_FRAMES; j++)
        {
            if (proc->pid == frames[j].owner)
            {
                pg = getpte(proc, frames[j].addr);
                pg->accessed = 0;

                // Incrementa a idade de todas as páginas
                frames[j].age++;
            }
        }
    }
    time = 0;
}
else
    time++;
```

A cada 40 chamadas da função `allocf` (controladas pela variável `time`), o código percorre todas as páginas presentes na memória física associadas aos processos, reseta o bit de acesso (`accessed`) de cada página e incrementa a "idade" (`age`) de todas as páginas. Essa estratégia é utilizada para simular uma política de envelhecimento, onde as páginas que não são acessadas frequentemente acabam sendo mais propensas a serem substituídas em comparação com aquelas que são acessadas frequentemente.
