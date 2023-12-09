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
