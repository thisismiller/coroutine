#include "coroutine++.h"

using namespace std;

void Coroutine::SetDefaultStackSize(size_t size) {
  Coro_setDefaultStackSize(size);
}

Coroutine& Coroutine::Main() {
  initThread();
  return *main_coro;
}

Coroutine& Coroutine::Running() {
  initThread();
  return *running;
}

void Coroutine::KillMain() {
  delete main_coro;
}

Coroutine::Coroutine(function<void()> to_run)
  : coro(Coro_new(), &Coro_free)
  , code(to_run)
{
  if (code) {
    Coro_setup(coro.get(), &Coroutine::Trampoline, this);
  }
}

Coroutine::~Coroutine() {
}

void Coroutine::run() {
  initThread();

  Coroutine* old = running;
  running = this;
  Coro_switchTo(old->coro.get(), running->coro.get());
}

size_t Coroutine::stackSpaceLeft() {
  return Coro_bytesLeftOnStack(coro.get());
}

void Coroutine::Trampoline(void *arg) {
  static_cast<Coroutine*>(arg)->code();
}

void Coroutine::initThread() {
  if (main_coro == NULL) {
    main_coro = new Coroutine(NULL);
    Coro_initializeMainCoro(main_coro->coro.get());
  }
  if (running == NULL) {
    running = main_coro;
  }
}

__thread Coroutine* Coroutine::running = NULL;
__thread Coroutine* Coroutine::main_coro = NULL;
