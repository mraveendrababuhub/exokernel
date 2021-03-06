
/*
 * Copyright (C) 1997 Massachusetts Institute of Technology 
 *
 * This software is being provided by the copyright holders under the
 * following license. By obtaining, using and/or copying this software,
 * you agree that you have read, understood, and will comply with the
 * following terms and conditions:
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose and without fee or royalty is
 * hereby granted, provided that the full text of this NOTICE appears on
 * ALL copies of the software and documentation or portions thereof,
 * including modifications, that you make.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS," AND COPYRIGHT HOLDERS MAKE NO
 * REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED. BY WAY OF EXAMPLE,
 * BUT NOT LIMITATION, COPYRIGHT HOLDERS MAKE NO REPRESENTATIONS OR
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR
 * THAT THE USE OF THE SOFTWARE OR DOCUMENTATION WILL NOT INFRINGE ANY
 * THIRD PARTY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS. COPYRIGHT
 * HOLDERS WILL BEAR NO LIABILITY FOR ANY USE OF THIS SOFTWARE OR
 * DOCUMENTATION.
 *
 * The name and trademarks of copyright holders may NOT be used in
 * advertising or publicity pertaining to the software without specific,
 * written prior permission. Title to copyright in this software and any
 * associated documentation will at all times remain with copyright
 * holders. See the file AUTHORS which should have accompanied this software
 * for a list of all copyright holders.
 *
 * This file may be derived from previously copyrighted software. This
 * copyright applies only to those changes made by the copyright
 * holders listed in the AUTHORS file. The rest of this file is covered by
 * the copyright notices, if any, listed below.
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h> 
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <exos/debug.h>

#include "fd/proc.h"

#include "exos/vm-layout.h"

#include "arp_table.h"
 
#include <exos/net/arp.h>
#include <sys/mman.h>


arp_table_p arp_table = (arp_table_p)0;

int
arp_init(void) {
  int status;
  

  status = fd_shm_alloc(ARP_SHM_OFFSET,
			sizeof(arp_table_t),
			(char *)ARP_SHARED_REGION);
  arp_table = (arp_table_p)ARP_SHARED_REGION;

  StaticAssert((sizeof(arp_table_t)) <= ARP_SHARED_REGION_SZ);

  if (status == -1) {
    demand(0, problems attaching shm);
    return -1;
  }

  if (status) {
    arp_init_table();
  } 
  protect_arp_table();
  return 0;
}

void 
protect_arp_table(void) {
  int status;
  status = mprotect((char *)ARP_SHARED_REGION, ARP_SHARED_REGION_SZ,PROT_READ);
  if (status != 0) printf("mprotect %p:%d ro failed\n",
			  (char *)ARP_SHARED_REGION, ARP_SHARED_REGION_SZ);
}

void 
unprotect_arp_table(void) {
  int status;
  status = mprotect((char *)ARP_SHARED_REGION, ARP_SHARED_REGION_SZ,PROT_READ|PROT_WRITE);
  if (status != 0) printf("mprotect %p:%d ro failed\n",
			  (char *)ARP_SHARED_REGION, ARP_SHARED_REGION_SZ);
}
