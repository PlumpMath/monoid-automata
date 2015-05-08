
#include "traverser.cpp"

using namespace std;

#include <fcntl.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>

int open_sequential_read(const char* const filename)
{
	int fd = open(filename, O_RDONLY | O_BINARY | O_SEQUENTIAL);
	return fd;
}
#else
#include <unistd.h>

int open_sequential_read(const char* const filename)
{
	int fd = open(filename, O_RDONLY);
	posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_WILLNEED);
	return fd;
}
#endif

char* read_input(const char* const filename, int& textSize)
{
	int input_fd = open_sequential_read(filename);

	struct stat input_file_stats;
	fstat(input_fd, &input_file_stats);
	int size = input_file_stats.st_size;

	char* result = (char*)malloc(size + 1);

	read(input_fd, result, size-1);
	close(input_fd);
	result[size] = '$';
	textSize = size;
	return result;
}


int main(int argc, char *argv[])
{
	const char* input_filename = argv[1];
	int stringSize = 0;
    char* inputString;
	inputString = read_input(input_filename, stringSize);

    /*** DEBUG ***/
//    cout << "Input Regex is:" << endl;
//    for (int i = 0; i < stringSize; ++i) {
//        cout << inputString[i];
//    }
//    cout << endl;
    /*************/

	buildMonoidAutomata(inputString, stringSize);

    cout << "BUILD IS READY" << endl;

//    for (int j = 0; j < Aut[lastIndex].first.size(); ++j) {
//        cout << Aut[lastIndex].first[j] << ";  ";
//        for (int k = 0; k < labels[Aut[lastIndex].first[j]].size(); ++k) {
//            cout << labels[Aut[lastIndex].first[j]][k];
//        }
//        cout << endl;
//    }

	string word;
	while(true) {
        cin >> word;
        vector<char> vword(word.size());

        for (int i = 0; i < word.size(); ++i) {
            vword[i] = word[i];
        }

        /*** DEBUG ***/
//        	cout << "Input vec is:   ";
//        	for (int i = 0; i < vword.size(); ++i) {
//        		cout << vword[i];
//        	}
//        	cout << endl;
        /************/

        vector<int> result = traverseAndSumWord(vword);

        /*** DEBUG ***/
            cout << "RESULT:" << endl;
            for( int i = 0; i < result.size(); ++i) {
                cout << result[i] << endl;
            }
        /************/
	}

	return 0;
}
