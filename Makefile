#Serve para para compilar o código do Buddy System. Ele especifica as regras de compilação e limpeza do projeto.
#Esse makefile permite compilar o código do Buddy System e construir o executável "buddy_test" usando o compilador GCC. Ao executar o comando "make", ele se encarregará de compilar os arquivos fonte necessários e gerar os arquivos objeto, e finalmente, linkar esses arquivos objetos para criar o executável "buddy_test".
all: buddy_test

buddy_test: buddy.o buddy_test.o
	gcc -o $@ $^

%.o: %.c
	gcc -c $^

clean:
	rm -rf *.o buddy_test
