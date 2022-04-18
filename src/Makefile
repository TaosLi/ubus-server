 
CFLAGS  += -I$(COMMON_USRDIR)/include
LDFLAGS += -L$(COMMON_USRDIR)/lib
LIBS   += -lubus -lubox -lblobmsg_json -ljson_script -ljson-c
 
all:	 ubus_server ubus_client
 
ubus_server:   ubus_server.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ubus_server.c -o ubus_server 
 
ubus_client:   ubus_client.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ubus_client.c -o ubus_client
 
clean:
	rm -rf *.o ubus_server ubus_client
