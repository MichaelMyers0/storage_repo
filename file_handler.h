#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <ctime>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
using std::cerr;
using std::endl;
using std::cout;
using std::cin;
#define TXT_FILE "txt"
class file_handler
{
	std::string mod_file;
	std::string res_path;
	std::string file_suffix;
	char action;
	time_t program_timer = 0;
	time_t poll_timer;
	long eight_byte_variable_to_mod_files;
	bool delete_input_files_flag = false;
	int fd;
	int file_size;
	char my_buf[BUFSIZ];
	void open_file(int& fd, const char* fname, int flags, int mode=0);
	void read_file();
	void close_file(int ffd);
	void set_position(int offset, int whence);
	void delete_input_file(const char* fname);
	void xor_modify();
	void xor_modify_timer();
	void ask_for_an_input_file();
	void set_options();
public:
	file_handler() = default;
	void modify_file();
	~file_handler() = default;
};
#endif