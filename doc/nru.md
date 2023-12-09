# Algoritmo NRU (Not Recently Used)
### TP 4 de Sistemas Operacionais cujo objetivo foi implementar o algoritmo de substituição de páginas NRU no Nanvix

### Limpeza do bit de Referência (Accessed)
Para a limpeza do bit de referência, foi definido o período de 8 acessos e controlado pela variável `time`. Nós usamos de 2 estruturas de repetição, onde uma percorre pelos processos e a outra pelos frames associados a um processo. Uma vez encontrado um frame que é associado à um processo, a tabela é buscada e o bit `accessed` recebe 0.

```c
for (proc = FIRST_PROC; proc <= LAST_PROC; proc++){
	for (j = 0; j < NR_FRAMES; j++){

		if (proc->pid == frames[j].owner){
			pg = getpte(proc, frames[j].addr);
			pg->accessed = 0;
		}

	}
}
````

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

A cada 8 chamadas da função `allocf` (controladas pela variável `time`), o código percorre todas as páginas presentes na memória física associadas aos processos, reseta o bit de acesso (`accessed`) de cada página e incrementa a "idade" (`age`) de todas as páginas. Essa estratégia é utilizada para simular uma política de envelhecimento, onde as páginas que não são acessadas frequentemente acabam sendo mais propensas a serem substituídas em comparação com aquelas que são acessadas frequentemente.
