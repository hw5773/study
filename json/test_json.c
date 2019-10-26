#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "json.h"

int main()
{
  struct json_object *jobj, *tmp, *ttmp, *ntmp, *itmp;
  struct json_list *question, *answer;
  int i, len, type, nlen, ilen;
  char *ip, *name;
  char nbuf[1024], ibuf[1024];
  char *str = "{\"Status\": 0,\"TC\": false,\"RD\": true, \"RA\": true, \"AD\": false,\"CD\": false,\"Question\":[{\"name\": \"my.snu.ac.kr.\", \"type\": 1}],\"Answer\":[{\"name\": \"my.snu.ac.kr.\", \"type\": 5, \"TTL\": 3303, \"data\": \"portal.snu.ac.kr.\"},{\"name\": \"portal.snu.ac.kr.\", \"type\": 1, \"TTL\": 3303, \"data\": \"147.46.10.20\"}]}";

  jobj = get_json_object_from_str(str, strlen(str));
  assert(jobj != NULL);
  printf("===== JSON object =====\n");
  print_json_object(jobj);
  printf("=======================\n\n");

  question = get_json_list_by_key(jobj, "Question");
  assert(question != NULL);
  printf("===== JSON list (question) =====\n");
  print_json_list(question);
  printf("================================\n\n");

  ntmp = get_json_object_by_index(question, 0);
  assert(ntmp != NULL);
  printf("===== JSON object =====\n");
  print_json_object(ntmp);
  printf("=======================\n\n");

  name = get_str_value_by_key(ntmp, "name", &nlen);
  assert(ntmp != NULL);
  memset(nbuf, 0x0, 1024);
  memcpy(nbuf, name, nlen);
  printf("name: %s\n", nbuf);

  answer = get_json_list_by_key(jobj, "Answer");
  assert(answer != NULL);
  print_json_list(answer);

  len = get_length_of_json_list(answer);
  assert(len > 0);
  printf("length of the list: %d\n", len);

  for (i=0; i<len; i++)
  {
    tmp = get_json_object_by_index(answer, i);
    assert(tmp != NULL);
    print_json_object(tmp);

    type = get_int_value_by_key(tmp, "type");
    if (type == 1)
    {
      ip = get_str_value_by_key(tmp, "data", &ilen);
      assert(ip != NULL);
      memset(ibuf, 0x0, 1024);
      memcpy(ibuf, ip, ilen);
      printf("name: %s, ip: %s\n", nbuf, ibuf);
    }
  }

  return 0;
}
