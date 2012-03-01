#ifndef _COROUTINEPP_H_
#define _COROUTINEPP_H_

#include <functional>
#include <memory>

#include "coroutine.h"

class Coroutine : public std::enable_shared_from_this<Coroutine> { 
public:
  static void SetDefaultStackSize(size_t size);
  static Coroutine& Main();
  static Coroutine& Running();
  static void KillMain();
  static size_t CurrentStackSpaceLeft();

  static void SetScheduler(const Coroutine &scheduler);
  static void Yield();

  Coroutine(std::function<void()> to_run);
  ~Coroutine();

  void run();

private:
  static void Trampoline(void *arg);
  static void initThread();

  std::shared_ptr<Coro> coro;
  std::function<void()> code;

  static __thread Coroutine* running;
  static __thread Coroutine* main_coro;
};

#endif
