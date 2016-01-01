#include <openssl/sha.h>

#include "qetag.h"

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

#define BLOCKS_PER_ARRAY 16
#define BLOCK_SIZE (1 << 22)

typedef struct _Qiniu_Qetag_Block
{
    unsigned char digest[SHA_DIGEST_LENGTH];
} Qiniu_Qetag_Block;

typedef Qiniu_Qetag_Block Qiniu_Qetag_Block_Array[BLOCKS_PER_ARRAY];

typedef struct _Qiniu_Qetag_Context
{
    int blkCount;
    int arrCount;
    Qiniu_Qetag_Block_Array * blkArray;

    int arrId;
    int blkId;
} Qiniu_Qetag_Context;

Qiniu_Error Qiniu_Qetag_New(struct _Qiniu_Qetag_Context ** ctx, size_t fsize)
{
    Qiniu_Error err;
    int i = 0;
    int blkMaxCount = 0;
    Qiniu_Qetag_Context * newCtx = NULL;

    // 分配主结构体
    newCtx = calloc(1, sizeof(*newCtx));
    if (newCtx == NULL) {
        goto NEW_NO_ENOUGH_MEMORY_ERROR;
    }

    if (fsize <= 0) {
        // 计算合适的块数和数组数
        blkMaxCount = fsize / BLOCK_SIZE;
        if (fsize % BLOCK_SIZE > 0) {
            blkMaxCount += 1;
        }

        newCtx->arrCount = blkMaxCount / BLOCKS_PER_ARRAY;
        if (blkMaxCount % BLOCKS_PER_ARRAY > 0) {
            newCtx->arrCount += 1;
        }
    } else {
        newCtx->arrCount = 1;
    } // if

    // 分配块数组
    newCtx->blkArray = calloc(newCtx->arrCount, sizeof(Qiniu_Qetag_Block_Array));
    if (newCtx->blkArray == NULL) {
        goto NEW_NO_ENOUGH_MEMORY_ERROR;
    }

    *ctx = newCtx;
    err.code = 0;
    err.message = "ok";
    return err;

NEW_NO_ENOUGH_MEMORY_ERROR:
    Qiniu_Qetag_Destroy(newCtx);
    err.code = 9999;
    err.message = "not enough memory";
    return err;
} // Qiniu_Qetag_New

void Qiniu_Qetag_Destroy(struct _Qiniu_Qetag_Context * ctx)
{
    if (ctx) {
        if (ctx->blkArray) {
            free(ctx->blkArray);
        }
        free(ctx);
    } // if
} // Qiniu_Qetag_Destroy

Qiniu_Error Qiniu_Qetag_Next(struct _Qiniu_Qetag_Context * ctx, struct _Qiniu_Qetag_Block ** blk, size_t * blkMaxSize)
{
    Qiniu_Error err;
    Qiniu_Qetag_Block_Array * newArr = NULL;

    if (ctx->blkId == BLOCKS_PER_ARRAY) {
        if (ctx->arrId + 1 == ctx->arrCount) {
            newArr = calloc(ctx->arrCount + 1, sizeof(Qiniu_Qetag_Block_Array));
            if (newArr == NULL) {
                err.code = 9999;
                err.message = "not enough memory";
                return err;
            }

            memcpy(newArr, ctx->blkArray, ctx->arrCount * sizeof(Qiniu_Qetag_Block_Array));
            free(ctx->blkArray);
            ctx->blkArray = newArr;

            ctx->arrCount += 1;
        } // if

        ctx->arrId += 1;
        ctx->blkId = 0;
    } // if

    *blk = &ctx->blkArray[ctx->arrId][ctx->blkId];
    *blkMaxSize = BLOCK_SIZE;

    ctx->blkId += 1;
    ctx->blkCount += 1;
    err.code = 0;
    err.message = "ok";
    return err;
} // Qiniu_Qetag_Next

Qiniu_Error Qiniu_Qetag_Update(struct _Qiniu_Qetag_Block * blk, const char * buf, size_t size)
{
    Qiniu_Error err;
    SHA_CTX sha1Ctx;
    int ret = 0;

    ret = SHA1_Init(&sha1Ctx);
    if (ret == 0) {
        goto UPDATE_OPENSSL_INTERNAL_ERROR;
    }

    ret = SHA1_Update(&sha1Ctx, buf, size);
    if (ret == 0) {
        goto UPDATE_OPENSSL_INTERNAL_ERROR;
    }

    ret = SHA1_Final(blk->digest, &sha1Ctx);
    if (ret == 0) {
        goto UPDATE_OPENSSL_INTERNAL_ERROR;
    }

    err.code = 0;
    err.message = "ok";
    return err;

UPDATE_OPENSSL_INTERNAL_ERROR:
    err.code = 9999;
    err.message = "openssl internal error";
    return err;
} // Qiniu_Qetag_Update

Qiniu_Error Qiniu_Qetag_Final(struct _Qiniu_Qetag_Context * ctx, char ** digest)
{
    Qiniu_Error err;
    SHA_CTX sha1Ctx;
    unsigned char digestSummary[1 + SHA_DIGEST_LENGTH];
    char * newDigest = NULL;
    int i = 0;
    int arrId = 0;
    int blkId = 0;
    int ret = 0;

    if (ctx->blkCount > 0) {
        memset(digestSummary, 0, sizeof(digestSummary));

        if (ctx->blkCount == 1) {
            digestSummary[0] = 0x16;
            memcpy(&digestSummary[1], ctx->blkArray[0][0].digest, SHA_DIGEST_LENGTH);
        } else {
            digestSummary[0] = 0x96;

            ret = SHA1_Init(&sha1Ctx);
            if (ret == 0) {
                goto FINAL_OPENSSL_INTERNAL_ERROR;
            }

            for (i = 0; i < ctx->blkCount; i += 1) {
                ret = SHA1_Update(&sha1Ctx, ctx->blkArray[arrId][blkId].digest, SHA_DIGEST_LENGTH);
                if (ret == 0) {
                    goto FINAL_OPENSSL_INTERNAL_ERROR;
                }

                blkId += 1;
                if (blkId > BLOCKS_PER_ARRAY) {
                    arrId += 1;
                    blkId = 0;
                }
            } // for

            ret = SHA1_Final(&digestSummary[1], &sha1Ctx);
            if (ret == 0) {
                goto FINAL_OPENSSL_INTERNAL_ERROR;
            }
        } // if

        newDigest = Qiniu_String_Encode((const char *)digestSummary);
        if (newDigest == NULL) {
            err.code = 9999;
            err.message = "no enough memory";
            return err;
        }
    } // if

    *digest = newDigest;
    err.code = 0;
    err.message = "ok";
    return err;

FINAL_OPENSSL_INTERNAL_ERROR:
    err.code = 9999;
    err.message = "openssl internal error";
    return err;
} // Qiniu_Qetag_Final

Qiniu_Error Qiniu_Qetag_DigestFile(const char * localFile, size_t fsize, char ** digest)
{
    Qiniu_Error err;

    err.code = 0;
    err.message = "ok";
    return err;
} // Qiniu_Qetag_DigestFile

Qiniu_Error Qiniu_Qetag_DigestBuffer(const char * buf, size_t fsize, char ** digest)
{
    Qiniu_Error err;
    struct _Qiniu_Qetag_Context * ctx = NULL;
    struct _Qiniu_Qetag_Block * blk = NULL;
    size_t blkMaxSize = 0;
    size_t doneSize = 0;
    size_t size = 0;
    const char * pos = buf;

    err = Qiniu_Qetag_New(&ctx, fsize);
    if (err.code != 0) {
        return err;
    }

    pos = buf;
    while (doneSize < fsize) {
        err = Qiniu_Qetag_Next(ctx, &blk, &blkMaxSize);
        if (err.code != 0) {
            Qiniu_Qetag_Destroy(ctx);
            return err;
        }

        size = fsize - doneSize;
        if (size > blkMaxSize) {
            size = blkMaxSize;
        }

        err = Qiniu_Qetag_Update(blk, pos, size);
        if (err.code != 0) {
            Qiniu_Qetag_Destroy(ctx);
            return err;
        }

        doneSize += size;
        pos += size;
    } // while

    err = Qiniu_Qetag_Final(ctx, digest);
    Qiniu_Qetag_Destroy(ctx);
    return err;
} // Qiniu_Qetag_DigestBuffer

#pragma pack()

#ifdef __cplusplus
}
#endif
