/*------------------------ ubus_server.c -----------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.
An example for Openwrt UBUST communication.
Midas Zhou
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <libubus.h>

static struct ubus_context *ctx;
static struct ubus_request_data req_data;
static struct blob_buf	bb;

static int ering_handler( struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg );

static int repair_handler( struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg );

enum {
	ERING_ID,
	ERING_DATA,
	ERING_MSG,
	__ERING_MAX,
};

/* Ubus Policy */
static const struct blobmsg_policy ering_policy[] =
{
	[ERING_ID]  = { .name="id", .type=BLOBMSG_TYPE_INT32},
	[ERING_DATA] = { .name="data", .type=BLOBMSG_TYPE_INT32 },
	[ERING_MSG] = { .name="msg", .type=BLOBMSG_TYPE_STRING },
};

static const struct blobmsg_policy repair_policy[] =
{
};

/* Ubus Methods */
static const struct ubus_method ering_methods[] =
{
	UBUS_METHOD("ering_method", ering_handler, ering_policy),
	UBUS_METHOD("repair", repair_handler, repair_policy),
};

/* Ubus object type */
static struct ubus_object_type ering_obj_type =
	UBUS_OBJECT_TYPE("ering_uobj", ering_methods);

/* Ubus object */
static struct ubus_object ering_obj=
{
	.name = "ering.host", 	/* with APP name */
	.type = &ering_obj_type,
	.methods = ering_methods,
	.n_methods = ARRAY_SIZE(ering_methods),
	//.path= /* useless */
};

void main(void)
{
	int ret;
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
		printf("Add '%s' to ubus @%u successfully.\n", ering_obj.name, ering_obj.id);
	}

	/* 5. uloop routine: events monitoring and callback provoking */
	uloop_run();


	uloop_done();
UBUS_FAIL:
	ubus_free(ctx);
}


/*---------------------------------------------
A callback function for ubus methods handling
----------------------------------------------*/
static int ering_handler( struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg )
{
	struct blob_attr *tb[__ERING_MAX]; /* for parsed attr */

	/* Parse blob_msg from the caller to request policy */
	blobmsg_parse(ering_policy, ARRAY_SIZE(ering_policy), tb, blob_data(msg), blob_len(msg));

	/* print request msg */
	printf("Receive msg from caller: ");
	if(tb[ERING_ID])
	    printf("UBUS_ID=%u  ", blobmsg_get_u32(tb[ERING_ID]));
	if(tb[ERING_DATA])
	    printf("DATA=%u  ", blobmsg_get_u32(tb[ERING_DATA]));
	if(tb[ERING_MSG])
	    printf("MSG='%s' \n", blobmsg_data(tb[ERING_MSG]));

	/* Do some job here according to caller's request */

	/* send a reply msg to the caller for information */
	blob_buf_init(&bb, 0);
	blobmsg_add_string(&bb,"Ering reply", "Request is being proceeded!");
	ubus_send_reply(ctx, req, bb.head);

	/* 	-----  reply results to the caller -----
	 * NOTE: we may put proceeding job in a timeout task, just to speed up service response.
	 */
	ubus_defer_request(ctx, req, &req_data);
	ubus_complete_deferred_request(ctx, req, UBUS_STATUS_OK);
}

static int repair_handler( struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg )
{
	struct blob_attr *tb[__ERING_MAX]; /* for parsed attr */

	/* Parse blob_msg from the caller to request policy */
	blobmsg_parse(repair_policy, ARRAY_SIZE(repair_policy), tb, blob_data(msg), blob_len(msg));


	/* Do some job here according to caller's request */

	/* send a reply msg to the caller for information */
	blob_buf_init(&bb, 0);
	blobmsg_add_string(&bb, "Johnny msg", "repair");
	ubus_send_reply(ctx, req, bb.head);

	/* 	-----  reply results to the caller -----
	 * NOTE: we may put proceeding job in a timeout task, just to speed up service response.
	 */
	ubus_defer_request(ctx, req, &req_data);
	ubus_complete_deferred_request(ctx, req, UBUS_STATUS_OK);
}

