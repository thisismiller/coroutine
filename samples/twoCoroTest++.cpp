#include "coroutine++.h"

#include <iostream>

using namespace std;

struct WorkData {
  explicit WorkData(int taskNumber) : taskNumber(taskNumber), value(0), other(NULL) {}

  int taskNumber;
  int value;
  Coroutine *other;
};

void WorkWork(void *data) {
  {
    WorkData *wd = static_cast<WorkData*>(data);

    while(wd->value < 10) {
      cout << "Task #" << wd->taskNumber << ": " << ++wd->value << endl;
      wd->other->run();
    }

    cout << "Space left: " << Coroutine::CurrentStackSpaceLeft() << endl;
  }
  // Make sure that all destructors are run before resuming the main thread.
  Coroutine::Yield();
}

int main() {
  // This program can get by with a tiny stack.
  Coroutine::SetDefaultStackSize(2*1024);
  Coroutine::SetScheduler(Coroutine::Main());

  {
    unique_ptr<WorkData> data1(new WorkData(1));
    unique_ptr<WorkData> data2(new WorkData(2));

    Coroutine first(bind(&WorkWork, data1.get()));
    Coroutine second(bind(&WorkWork, data2.get()));

    data1->other = &second;
    data2->other = &first;

    first.run();
  }

  // To fully free everything, this needs to be called at the end of the program.
  // However, since the program is about to end, it is completely optional to call.
  Coroutine::KillMain();

  return 0;
}
