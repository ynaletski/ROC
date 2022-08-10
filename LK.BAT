cc +L +1 -Z6000 -DFLOBOSS -ansi ng_an_mn.c -o gost_fb.o
as -DMODEL=3 start.asm
as -DGOST407B -DMODEL=3 na_ints.asm -o gost407b.o

ln -C B000 -D 6400 -t -o vnic407b.exe gost_fb gost407b start -lfblib -lml -lcl
hex86 -x -s64 -p64 vnic407b.exe
del vnic407b.exe
