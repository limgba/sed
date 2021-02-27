#include "lmb_sed.h"

int main(int argc, char* argv[])
{
	lmb::sed("file", 'd', "123", "123");
	return 0;
}
