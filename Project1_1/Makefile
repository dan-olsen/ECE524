CC              = cc
SRC             = main.c given.c user.c
TARGET          = proj1
INCLUDE       = /opt/net/apps/cudd/include 
WHERE	 = /opt/net/apps/cudd
LIB           = $(WHERE)/dddmp/libdddmp.a $(WHERE)/cudd/libcudd.a \
	$(WHERE)/mtr/libmtr.a $(WHERE)/st/libst.a $(WHERE)/util/libutil.a \
	$(WHERE)/epd/libepd.a
MYLIB		= bdd.a
OBJ             = $(SRC:.c=.o)
DDDEBUG         =
#------------------------------------------------------
$(TARGET): $(OBJ)
	$(CC)  -o $@ $(OBJ)  -L$(LIB) -lm

$(OBJ): $(SRC)
	$(CC) -g  -I$(INCLUDE) -c $(SRC) $(DDDEBUG)

clean:
	rm -f *.o *~ $(TARGET)
