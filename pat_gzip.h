/*
 * pat_gzip.h
 *
 *  Created on: 2017年6月12日
 *      Author: root
 */

#ifndef COMMON_PAT_GZIP_H_
#define COMMON_PAT_GZIP_H_

#include <stdio.h>
#include <stdint.h>
#include <zlib.h>

struct pat_gzip_ctx
{
    int32_t        is_init ;
    z_stream    zlib_ctx ;
};

/*
* @brief  基于gzip 的解压缩函数
*             该函数自动完成解压缩会话上下文的初始化,当进行分段解压缩时，需要使用同一个 上下文数据
*
*   使用示例：
*          struct pat_gzip_ctx   zctx ;
*          int32_t  len = 0 ;
*          zctx.int32_t = 0 ;
*          len += pat_gzip_uncomperss(zctx,......) ;
*          len += pat_gzip_uncomperss(zctx,......) ;
*          len += pat_gzip_uncomperss(zctx,......) ;
*
* @param
*     zctx:    解压缩任务上下文
*     compress:    压缩的数据缓冲区指针
*     len:   压缩内容长度
*     out:    解压缩输出的目标缓冲区
*     out_len:   目标缓冲区长度
* @return
*     成功返回解压缩后的实际长度，失败返回 -1
*
*/
static inline int32_t  __attribute__((always_inline)) pat_gzip_uncompress(struct pat_gzip_ctx  *zctx,uint8_t* compress,int32_t len,uint8_t* out,int32_t out_len)
{
    int32_t   o_len =  0 ;
    int32_t   ret = 0 ;

    if(1 != zctx->is_init)
    {
        //printf("inflateInit2.\n") ;
        zctx->zlib_ctx.zalloc = Z_NULL ;
        zctx->zlib_ctx.zfree = Z_NULL ;
        zctx->zlib_ctx.opaque = Z_NULL ;
        zctx->zlib_ctx.next_in = Z_NULL ;
        zctx->zlib_ctx.avail_in = 0 ;
        ret = inflateInit2(&zctx->zlib_ctx,47);
        if(ret!=Z_OK)
        {
             printf("inflateInit2 error:%d",ret);
            return -1;
        }
        zctx->is_init = 1 ;
    }

    zctx->zlib_ctx.next_in=compress;
    zctx->zlib_ctx.avail_in=len;
    zctx->zlib_ctx.next_out=out;
    zctx->zlib_ctx.avail_out=out_len;

    ret = inflate(&zctx->zlib_ctx,Z_NO_FLUSH);
    switch (ret)
    {
    case Z_NEED_DICT:
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
        inflateEnd(&zctx->zlib_ctx);
        zctx->is_init = 0 ;
        return -1;
    }
    o_len = out_len - zctx->zlib_ctx.avail_out ;

    return o_len ;
}

//解压缩完成后释放函数
//如果是压缩内容全部解压缩，不需要调用本函数
//因为完全解压缩后解压缩函数会自动释放
//只有在部分内容解压缩的情况下，需要调用本函数进行手动释放
static inline int32_t  __attribute__((always_inline)) pat_gzip_free(struct pat_gzip_ctx  *zctx)
{
    inflateEnd(&zctx->zlib_ctx) ;
    return 0 ;
}

#endif /* COMMON_PAT_GZIP_H_ */
