/*
 * Note: After formatting JSON string will be in json_buffer_g[]
 */

#ifndef PCS_JSON_H
#define PCS_JSON_H

#define JSON_BUFFER_SIZE 2048
#define JSON_MAX_OBJECT_SIZE 512
#define JSON_MAX_STRING_SIZE 128

void JSON_Start();
void JSON_Delimit();
void JSON_Append(char *str);
void JSON_Start_Object(char* object_name);
void JSON_Start_Array(char* object_name);
void JSON_Start_ArrayObject();
void JSON_Add_Field_Integer(char* object_name, int object_value);
void JSON_Add_Field_Double(char* object_name, double object_value);
void JSON_Add_Field_String(char* object_name, char* object_value);
void JSON_End_Object();
void JSON_End_Array();
char *JSON_End();

/* Functions for extracting data from JSON string */

int JSON_Extract_String(char* json_string, char* target_object, char* return_buffer, size_t return_buffer_size);
int JSON_Extract_Integer(char* json_string, char* target_object, int* return_int);
int JSON_Extract_Object(char *json_string, char *target_object, char *return_object);
int JSON_Extract_Object_From_Arr(char *json_string, char *target_arr, char **return_object, unsigned char obj_num);
int JSON_Extract_Double(char *json_string, char *target_object, double *return_double);

#endif /* PCS_JSON_H */
