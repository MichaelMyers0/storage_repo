#include "file_handler.h"

void file_handler::open_file(int& fd, const char* fname, int flags, int mode)
{
	errno = 0;
	fd = open(fname, flags, mode);
	if (fd == -1)
	{
		cerr << "ERROR: Failed to open a file:open_file(int& fd, const char* fname, int flags, int mode)\n";
		exit(2);
	}
}

void file_handler::read_file()
{
	errno = 0;
	file_size = read(fd, my_buf, BUFSIZ);
	if (file_size == -1)
	{
		cerr << "ERROR: Failed to read from a file\n";
		exit(3);
	}
	*(my_buf + file_size) = 0;
}

void file_handler::close_file(int ffd)
{
	int res;
	res = close(ffd);
	if (res == -1)
	{
		cerr << "ERROR: Failed to close a file\n";
		exit(4);
	}
}

void file_handler::set_options()
{
#if 1
	char answer;
	printf("a) Provide a file sufffix\n");
	cin >> file_suffix;
	printf("b)  Do you want us to delete input files? Enter y or Y if you want, and enter n or N if you don`t want us to delete files\n");
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		delete_input_files_flag = true;
	printf("c) Provide a path to store a resulting files\n");
	cin >> res_path;
	printf("d) What to do if an output file does exsist? If you want to rewrite a file enter r or R. Or enter m or M to modify a file\n");
	cin >> action;
	printf("e) Do you want to set a timer? If so enter y or Y\n");
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		printf("Okey, provide us with timer value\n");
		cin >> program_timer;
	}
	printf("f) Set a timer to ask for an input file availability\n");
	cin >> poll_timer;
	printf("g) Enter an eight byte variable value with which you will modify files\n");
	cin >> eight_byte_variable_to_mod_files;
#endif
	printf("Enter a filename\n");
	cin >> mod_file;
}

void file_handler::set_position(int offset, int whence)
{
	off_t offst;
	offst = lseek(fd, offset, whence);
	if (offst == -1)
	{
		cerr << "ERROR: Failed to set a new offset";
		exit(5);
	}
}

void file_handler::delete_input_file(const char* fname)
{
	int res;
	errno = 0;
	res = unlink(fname);
	if (res == -1)
	{
		cerr << "ERROR: Failed to delete a file. delete_input_file()";
		exit(6);
	}
}

void file_handler::xor_modify()
{
	int i = 0, ffd;
	if (file_suffix == TXT_FILE)
	{
		for (i = 0; i < file_size; i++)
		{
			if (*(my_buf + i) == '\n')
				continue;
			*(my_buf + i) = *(my_buf + i) ^ eight_byte_variable_to_mod_files;	
		}
		open_file(ffd, res_path.data(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
		write(ffd, my_buf, file_size);
		close_file(ffd);
		*my_buf = 0;
	}
	if (delete_input_files_flag)
	{
		close_file(fd);
		delete_input_file(mod_file.data());
	}
}

void file_handler::xor_modify_timer()
{
	int i = 0, ffd;
	time_t start_time;
	bool flag = false;
	if (file_suffix == TXT_FILE)
	{
		printf("I`m here\n");
		start_time = time(NULL);
		while ((difftime(time(NULL), start_time) < program_timer))
		{
			if (difftime(time(NULL), start_time) == program_timer)
			{
				printf("Warning: sorry time has expired\n");
				close(fd);
				break;
			}
			if (*(my_buf + i) == '\n')
			{
				i++;
				continue;
			}
			if (i < file_size)
			{
				*(my_buf + i) = *(my_buf + i) ^ eight_byte_variable_to_mod_files;
				i++;
				continue;
			}
			if ((i == file_size) && (difftime(time(NULL), start_time) < program_timer))
			{	
				printf("xor_modify_write()\n");
				open_file(ffd, res_path.data(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
				write(ffd, my_buf, file_size);
				close_file(ffd);
				*my_buf = 0;
				flag = true;
			}
			if (flag)
				break;
		}
	}
	if (delete_input_files_flag)
	{
		close_file(fd);
		delete_input_file(mod_file.data());
	}

}

void file_handler::ask_for_an_input_file()
{
	time_t start_time = time(NULL);
	while ((difftime(time(NULL), start_time) < poll_timer))
	{

	}
	printf("Provide a file to modify\n");
}

void file_handler::modify_file()
{
	int c;
	for (;;)
	{
		set_options();
		open_file(fd, mod_file.data(), O_RDONLY);
		read_file();
		if (program_timer)
			xor_modify_timer();
		else
			xor_modify();
		if (!delete_input_files_flag)
			close_file(fd);
		printf("Do you want to continue? Enter y or Y to continue, or n or N to quit");
		c = getchar();
		if (c == 'y' || c == 'Y')
			break;
		ask_for_an_input_file();
	}
}
