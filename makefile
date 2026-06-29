EXECS=server client
SRC_DIR=src
INCLUDE_DIR=include

all: $(EXECS)

server: $(SRC_DIR)/server.c $(SRC_DIR)/utils.c $(SRC_DIR)/handler.c $(INCLUDE_DIR)/utils.h $(INCLUDE_DIR)/server.h
	gcc -o server $(SRC_DIR)/server.c $(SRC_DIR)/utils.c $(SRC_DIR)/handler.c

client: $(SRC_DIR)/client.c $(SRC_DIR)/admin.c $(SRC_DIR)/faculty.c $(SRC_DIR)/student.c $(SRC_DIR)/utils.c $(INCLUDE_DIR)/utils.h $(INCLUDE_DIR)/client.h
	gcc -o client $(SRC_DIR)/client.c $(SRC_DIR)/admin.c $(SRC_DIR)/faculty.c $(SRC_DIR)/student.c $(SRC_DIR)/utils.c 

clean:
	rm -f $(EXECS)

reset:
	rm -f $(EXECS)
