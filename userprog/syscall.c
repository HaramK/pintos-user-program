#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <devices/shutdown.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "process.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void verify_address(int * p){
  
  printf("%s: exit(%d)\n",thread_name(),*(p+1));
  thread_exit ();
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
//  printf ("system call!\n");
  int * p =f->esp;
  verify_address(p);

  int type=*p;

  switch (type){
    case SYS_HALT:
      shutdown_power_off();
    case SYS_EXIT:
      printf("%s: exit(%d)\n",thread_name(),*(p+1));
      thread_exit ();

    case SYS_EXEC:
      process_execute(*(p+1));
      break;
    case SYS_WAIT:
      break;
    case SYS_CREATE:
      break;
    case SYS_REMOVE:
      break;
    case SYS_OPEN:
      break;
    case SYS_FILESIZE:
      break;
    case SYS_READ:
      break;

    case SYS_WRITE:
//      printf("sysnum:%d\n",*p);
//      printf("fd:%d\n"s,*(p+1));
      printf("%s",(char*)*(p+2));
//      printf("size:%d\n",*(p+3));
      break;
    case SYS_SEEK:
      break;
    case SYS_TELL:
      break;
    case SYS_CLOSE:
      break;
    default:
      printf("System call not supported!");
      // TODO: terminal process, release resources
      break;
  }
//  thread_exit ();
}
