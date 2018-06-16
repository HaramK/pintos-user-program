#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <devices/shutdown.h>
#include <threads/vaddr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "process.h"
#include "pagedir.h"

static void syscall_handler (struct intr_frame *);
static void unexpected_exit(void);
static void verity_address_multiple(int * p, int num);
static void verity_address(void * p);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void unexpected_exit(){
  printf("%s: exit(-1)\n",thread_name());
  thread_exit ();
}


void verity_address_multiple(int *p, int num){
  int *p2=p;
  for(int i=0; i<num; i++,p2++) verity_address((void *)p2);
}

void verity_address(void *p){
  if(!is_user_vaddr(p)) unexpected_exit();
  if(!pagedir_get_page(thread_current()->pagedir,p)) unexpected_exit();

}


static void
syscall_handler (struct intr_frame *f UNUSED)
{
//  printf ("system call!\n");
  int * p =f->esp;
  verity_address((void *)p);
  int type=*p++;

  switch (type){
    case SYS_HALT:
      shutdown_power_off();

    case SYS_EXIT:
      verity_address((void *)p);
      printf("%s: exit(%d)\n",thread_name(),*(p));
      thread_exit ();

    case SYS_EXEC:
      verity_address((void *)p);
      verity_address((void*)*p);
      f->eax = process_execute((char*)*p);
      break;

    case SYS_WAIT:
      verity_address((void *)p);
      break;

    case SYS_CREATE:
      verity_address_multiple(p, 2);
      verity_address((void*)*p);
      break;

    case SYS_REMOVE:
      verity_address((void *)p);
      verity_address((void*)*p);
      break;

    case SYS_OPEN:
      verity_address((void *)p);
      verity_address((void*)*p);
      break;

    case SYS_FILESIZE:
      verity_address((void *)p);
      break;

    case SYS_READ:
      verity_address_multiple(p, 3);
      verity_address((void*)*(p+1));
      break;

    case SYS_WRITE:
      verity_address_multiple(p, 3);
      verity_address((void*)*(p+1));
      int fd = *p;
      if (fd<=0) unexpected_exit();
      if (fd==1) putbuf((char*)*(p+1),*(p+2));
      else{
          // Todo: write file
      }
      break;

    case SYS_SEEK:
      verity_address_multiple(p, 2);
      break;

    case SYS_TELL:
      verity_address((void *)p);
      break;

    case SYS_CLOSE:
      verity_address((void *)p);
      break;

    default:
      printf("System call not supported!");
      // TODO: terminal process, release resources
      break;
  }
//  thread_exit ();
}
