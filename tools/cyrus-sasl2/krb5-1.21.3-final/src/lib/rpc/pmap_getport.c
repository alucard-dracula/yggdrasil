/* @(#)pmap_getport.c	2.2 88/08/01 4.0 RPCSRC */
/*
 * Copyright (c) 2010, Oracle America, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the "Oracle America, Inc." nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)pmap_getport.c 1.9 87/08/11 Copyr 1984 Sun Micro";
#endif

/*
 * pmap_getport.c
 * Client interface to pmap rpc service.
 */

#include <unistd.h>
#include <gssrpc/rpc.h>
#include <gssrpc/pmap_prot.h>
#include <gssrpc/pmap_clnt.h>
#include <sys/socket.h>
#ifdef OSF1
#include <net/route.h>
#include <sys/mbuf.h>
#endif
#include <net/if.h>

static struct timeval timeout = { 5, 0 };
static struct timeval tottimeout = { 60, 0 };

/*
 * Find the mapped port for program,version.
 * Calls the pmap service remotely to do the lookup.
 * Returns 0 if no map exists.
 */
u_short
pmap_getport(
	struct sockaddr_in *address,
	rpcprog_t program,
	rpcvers_t version,
	rpcprot_t protocol)
{
	unsigned short port = 0;
	int sock = -1;
	CLIENT *client;
	struct pmap parms;

	address->sin_port = htons(PMAPPORT);
	client = clntudp_bufcreate(address, PMAPPROG,
	    PMAPVERS, timeout, &sock, RPCSMALLMSGSIZE, RPCSMALLMSGSIZE);
	if (client != (CLIENT *)NULL) {
		parms.pm_prog = program;
		parms.pm_vers = version;
		parms.pm_prot = protocol;
		parms.pm_port = 0;  /* not needed or used */
		if (CLNT_CALL(client, PMAPPROC_GETPORT, (xdrproc_t)&xdr_pmap, &parms,
		    (xdrproc_t)&xdr_u_short, &port, tottimeout) != RPC_SUCCESS){
			rpc_createerr.cf_stat = RPC_PMAPFAILURE;
			clnt_geterr(client, &rpc_createerr.cf_error);
		} else if (port == 0) {
			rpc_createerr.cf_stat = RPC_PROGNOTREGISTERED;
		}
		CLNT_DESTROY(client);
	}
	(void)close(sock);
	address->sin_port = 0;
	return (port);
}
