#pragma once
#include "Types.h"

#ifdef PARSER_EXPORTS
#define PARSER_API  __declspec(dllexport)
#else 
#define PARSER_API  __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

	parser_reader PARSER_API* parser_reader_create(const char* path);

	void PARSER_API parser_reader_destroy(parser_reader** parser);

	psBool PARSER_API parser_reader_open(parser_reader* parser, const char* path);

	void PARSER_API parser_reader_read(parser_reader* parser);

	psBool PARSER_API  parser_reader_still_reading(parser_reader* parser);

	psBool PARSER_API parser_reader_actual_object_is(parser_reader* parser, const char* object);

	psBool PARSER_API parser_reader_is_object_ended(parser_reader* parser);

	psBool PARSER_API parser_reader_get_value_char(parser_reader* parser, const char* key, char* value);
	psBool PARSER_API parser_reader_get_value_short(parser_reader* parser, const char* key, short* value);
	psBool PARSER_API parser_reader_get_value_int(parser_reader* parser, const char* key, int* value);
	psBool PARSER_API parser_reader_get_value_long(parser_reader* parser, const char* key, long* value);
	psBool PARSER_API parser_reader_get_value_float(parser_reader* parser, const char* key, float* value);
	psBool PARSER_API parser_reader_get_value_double(parser_reader* parser, const char* key, double* value);
	psBool PARSER_API parser_reader_get_value_string(parser_reader* parser, const char* key, char** value);



	parser_writer PARSER_API* parser_write_create(const char* path);

	void PARSER_API parser_writer_destroy(parser_writer** parser);

	psBool PARSER_API parser_writer_open(parser_writer* parser, const char* path);

	void PARSER_API parser_writer_start_new_object(parser_writer* parser, const char* object);

	void PARSER_API parser_writer_end_object(parser_writer* parser);

	void PARSER_API parser_writer_end_writing(parser_writer* parser);

	void PARSER_API parser_writer_put_value_char(parser_writer* parser, const char* key, char value);
	void PARSER_API parser_writer_put_value_short(parser_writer* parser, const char* key, short value);
	void PARSER_API parser_writer_put_value_int(parser_writer* parser, const char* key, int value);
	void PARSER_API parser_writer_put_value_long(parser_writer* parser, const char* key, long value);
	void PARSER_API parser_writer_put_value_float(parser_writer* parser, const char* key, float value);
	void PARSER_API parser_writer_put_value_double(parser_writer* parser, const char* key, double value);
	void PARSER_API parser_writer_put_value_string(parser_writer* parser, const char* key, const char* value);

#ifdef __cplusplus
}
#endif