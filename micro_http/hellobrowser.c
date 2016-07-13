#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <microhttpd.h>

#define PORT 8888
#define FILENAME "picture.png"
#define MIMETYPE "image/png"
	
const char *askpage = "<html><body>What's your name, Sir?<br><form action=\"/namepost\" method=\"post\">\
						<input name=\"name\" type=\"text\">\
						<input type=\"submit\" value=\" Send \"></form>\
						</body></html>";
const char *greatingpage = "<html><body><h1><center>Welcome, %s!</center></h1></body></html>";
const char *errorpage = "<html><body>This doesn't seem to be right.</body></html>";

struct connection_info_struct
{
	int connectiontype;
	char *answerstring;
	struct MHD_PostProcessor *postprocessor;
};

static int iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
						const char *filename, const char *content_type,
						const char *transfer_encoding, const char *data,
						uint64_t off, size_t size)
{
	struct connection_info_struct *con_info = coninfo_cls;

	if (strcmp(key, "name") == 0)
	{
		if ((size > 0) && (size <= MAXNAMESIZE))
		{
			char *answerstring;
			answerstring = malloc(MAXANSWERSIZE);
			if (!answerstring) return MHD_NO;

			snprintf(answerstring, MAXANSWERSIZE, greatingpage, data);
			con_info->answerstring = answerstring;
		}
		else con_info->answerstring = NULL;

		return MHD_NO;
	}

	return MHD_YES;
}

int answer_to_connection(void *cls, struct MHD_Connection *connection,
							const char *url,
							const char *method, const char *version,
							const char *upload_data,
							size_t *upload_data_size, void **con_cls)
{
	unsigned char *buffer = NULL;

	struct MHD_Response *response;

	int fd;
	int ret;
	struct stat sbuf;

	if (strcmp(method, "GET") != 0)
		return MHD_NO;

	if (((fd = open(FILENAME, O_RDONLY)) == -1) || (fstat(fd, &sbuf) != 0))
	{
		if (fd != -1) close(fd);
		const char *errorstr = "<html><body>An internal server error has occurred!</body></html>";

		response = MHD_create_response_from_buffer(strlen(errorstr),
													(void *)errorstr,
													MHD_RESPMEM_PERSISTENT);

		if (response)
		{
			ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
			MHD_destroy_response(response);
			return MHD_YES;
		}
		else
			return MHD_NO;
	}

	if (!ret)
	{
		const char *errorstr = "<html><body>An internal server error has occurred!</body></html>";

		if (buffer) free(buffer);

		response = MHD_create_response_from_buffer(strlen(errorstr), (void *)errorstr,
													MHD_RESPMEM_PERSISTENT);

		if (response)
		{
			ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
			MHD_destroy_response(response);
	
			return MHD_YES;
		}
		else return MHD_NO;
	}

	response = MHD_create_response_from_buffer(strlen(page),
												(void *)page, MHD_RESPMEM_PERSISTENT);

//	response = MHD_create_response_from_fd_at_offset(sbuf.st_size, fd, 0);
//	MHD_add_response_header(response, "Content-Type", MIMETYPE);
	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);

	return ret;
}

int main()
{

	struct MHD_Daemon *daemon;

	daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);

	if (daemon == NULL) return 1;

	getchar();

	MHD_stop_daemon(daemon);
	return 0;
}
