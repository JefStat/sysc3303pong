## Declare a variable of objects to output
objects = GameController

## Define rules should be run
milestone4 : $(objects)

GameController : GameController.c
	gcc -I/usr/xenomai/include -D_GNU_SOURCE -D_REENTRANT -D__XENO__   -L/usr/xenomai/lib -lpthread  -lnative -lrtdk -Xlinker -rpath -Xlinker /usr/xenomai/lib  GameController.c -lm -o GameController

PlayerProxy : PlayerProxy.c
	gcc -I/usr/xenomai/include -D_GNU_SOURCE -D_REENTRANT -D__XENO__   -L/usr/xenomai/lib -lpthread  -lnative -lrtdk -Xlinker -rpath -Xlinker /usr/xenomai/lib  PlayerProxy.c -lm -o PlayerProxy

## Rule to clean all output
clean :
	rm $(objects)
