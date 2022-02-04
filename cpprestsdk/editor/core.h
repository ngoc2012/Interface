// Function write, sleep
#include <unistd.h>

char *int_char(int i){
	switch (i) {
		case 0:
		return "0";
		case 1:
		return "1";
		case 2:
		return "2";
		case 3:
		return "3";
		case 4:
		return "4";
		case 5:
		return "5";
		case 6:
		return "6";
		case 7:
		return "7";
		case 8:
		return "8";
		case 9:
		return "9";
	}

}

int str_len(char *str){
	int i=0;
	while (str[i]!='\0'){
		i++;
	}
	return i;
}

void write_str(char *str){
	write(1, str, str_len(str));
	write(1, "\n", 1);
}

void print_int(int i){
	// 45678
	// Number of digital
	
	while (i != 0) {

		write(1,int_char(i-((int) i/10)*10),1);

		i/=10;
	}
	
	/*
	int n=1;
	while (i != 0) {
		n*=10;
		i/=10;
	}
	*/

}