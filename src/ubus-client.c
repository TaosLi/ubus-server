/*-------------------------  ubus_client.c  ------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.
An example for Openwrt UBUS communication.
Midas Zhou
------------------------------------------------------------------*/
#include <stdio.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
 
static struct ubus_context *ctx;
static struct blob_buf	bb;
 
static void result_handler(struct ubus_request *req, int type, struct blob_attr *msg);
 
/* ubus object assignment */
static struct ubus_object ering_obj=
{
	.name = "ering_caller",
	#if 0
	.type = &ering_obj_type,
	.methods = ering_methods,
	.n_methods = ARRAY_SIZE(ering_methods),
	.path= /* useless */
	#endif
};
 
 
void main(void)
{
	int ret;
	uint32_t host_id; /* ering host id */
	const char *ubus_socket=NULL; /* use default UNIX sock path: /var/run/ubus.sock */
 
	/* 1. create an epoll instatnce descriptor poll_fd */
	uloop_init();
 
	/* 2. connect to ubusd and get ctx */
	ctx=ubus_connect(ubus_socket);
	if(ctx==NULL) {
		printf("Fail to connect to ubusd!\n");
		return;
	}
 
	/* 3. registger epoll events to uloop, start sock listening */
	ubus_add_uloop(ctx);
 
	/* 4. register a usb_object to ubusd */
	ret=ubus_add_object(ctx, &ering_obj);
	if(ret!=0) {
		printf("Fail to register an object to ubus.\n");
		goto UBUS_FAIL;
 
	} else {
		printf("Add '%s' to ubus @%u successfully.\n",ering_obj.name, ering_obj.id);
	}
 
	/* 5. search a registered object with a given name */
	if( ubus_lookup_id(ctx, "ering.host", &host_id) ) {
		printf("ering.host is NOT found in ubus!\n");
		goto UBUS_FAIL;
	}
	printf("ering.host is found in ubus @%u.\n",host_id);
 
	/* 6. prepare request method policy and data */
	blob_buf_init(&bb,0);
	blobmsg_add_u32(&bb,"id", ering_obj.id); 	/* ERING_ID */
	blobmsg_add_u32(&bb,"data", 123456); 		/* ERING_DATA */
	blobmsg_add_string(&bb,"msg", "Hello, ERING!"); /* ERING_DATA */
 
	/* 7. call the ubus host object */
	ret=ubus_invoke(ctx, host_id, "ering_method", bb.head, result_handler, 0, 3000);
	printf("Call result: %s\n", ubus_strerror(ret));
 
	/* 8. uloop routine: events monitoring and callback provoking
	      However, we just ignore uloop in this example.
	 */
	 //uloop_run();
 
	uloop_done();
UBUS_FAIL:
	ubus_free(ctx);
}
 
/* callback function for ubus_invoke to process result from the host
 * Here we just print out the message.
 */
static void result_handler(struct ubus_request *req, int type, struct blob_attr *msg)
{
        char *strmsg;
 
        if(!msg)
                return;
 
        strmsg=blobmsg_format_json_indent(msg,true, 0); /* 0 type of format */
        printf("Response from the host: %s\n", strmsg);
        free(strmsg); /* need to free strmsg */
}
