CC              = cc
SRC             = main.c
TARGET          = proj2
OBJ             = $(SRC:.c=.o)
DDDEBUG         =
#------------------------------------------------------
$(TARGET): $(OBJ)
	$(CC)  -o $@ $(OBJ) -lm

$(OBJ): $(SRC)
	$(CC) -g  -c $(SRC) $(DDDEBUG)

clean:
	rm -f *.o *~ $(TARGET)
