#include <stdlib.h>
#include <time.h>

#include "stdio.h"
#include "timeutil.h"

// todo
// this might be inefficient
GameTime get_current_time()
{
	GameTime curr_time;
	time_t t;
	struct tm *tmp;

	time(&t);

	tmp = localtime(&t);

	strftime(curr_time.hour, sizeof(char) * 3, "%I", tmp);
	strftime(curr_time.minute, sizeof(char) * 3, "%M", tmp);
	strftime(curr_time.ampm, sizeof(char) * 3, "%p", tmp);

	return curr_time;
}

void update_time(GameTime *current_time) { *current_time = get_current_time(); }
