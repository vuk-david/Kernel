#include "thread.h"
#include "queue.h"

#include "pcb.h"
#include "SCHEDULE.h"

#include <stdio.h>

void inic();
void restore();

extern int userMain(int argc, char * argv[]);

int main(int argc, char* argv[])
{

  inic();

  int back = userMain(argc, argv);

  restore();

  return back;
}
