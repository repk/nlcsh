#include <malloc.h>
#include <stdlib.h>
#include <print.h>
#include <exit.h>
#include <brk.h>

/**
 * TODO: Thread safe implementation ??
 * TODO: Replace printf
 * TODO: Replace abort
 * TODO: Replace assert
 */

#define FATAL print

#ifndef ABORT
#define ABORT() exit(-1)
#endif

#ifndef ASSERT
#define ASSERT(t) if(!(t)) ABORT()
#endif

#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef ROUNDUP
#define ROUNDUP(v, a) (((((v) - 1) / (a)) + 1) * (a))
#endif

#ifndef offsetof
#define offsetof(type, member) ((size_t) &((type *)0)->member)
#endif

#ifndef containerof
#define containerof(ptr, type, member) ({				\
        const typeof(((type *)0)->member) *__mptr = (ptr);		\
        (type *)((char *)__mptr - offsetof(type,member));})
#endif

struct memchunk {
	struct memchunk *next;
	uintptr_t magic;
	size_t len;
	char memdata[];
};

#define MEMCHUNK_IS_FREE(m) (((uintptr_t)(m)) == ~(m)->magic)
#define MEMCHUNK_FREE(m) ((m)->magic = ~((uintptr_t)(m)))

#define MEMCHUNK_IS_ALLOC(m) (((uintptr_t)(m)) == (m)->magic)
#define MEMCHUNK_ALLOC(m) ((m)->magic = ((uintptr_t)(m)))

#define MEMCHUNK_MERGE(m, n) ((m)->len += (n)->len + sizeof(*(n)))


/**
 * Address increasing ordonned free chunk list
 */
static struct memchunk *freechunk;
/**
 * Keep freechunk's list tail for performance purpose
 * As it will be usual to insert in tail for new free chunk
 */
static struct memchunk **lfreechunk = &freechunk;

#define FREECHUNK_FOREACHP(ptr)						\
	for((ptr) = &freechunk; *(ptr) != NULL; (ptr) = &((*(ptr))->next))

#define FREECHUNK_REPLACE(ptr, m) do {					\
	(m)->next = (*(ptr))->next;					\
	*(ptr) = (m);							\
	if((*(ptr))->next == NULL)					\
		lfreechunk = &(*(ptr))->next;				\
} while(/*CONSTCOND*/0)

#define FREECHUNK_INSERTAT(ptr, m) do {					\
	(m)->next = *(ptr);						\
	*(ptr) = m;							\
	if((m)->next == NULL)						\
		lfreechunk = &(m)->next;				\
} while(/*CONSTCOND*/0)

#define FREECHUNK_REMOVEAT(ptr) do {					\
	*(ptr) = (*(ptr))->next;					\
	if(*(ptr) == NULL)						\
		lfreechunk = ptr;					\
} while(/*CONSTCOND*/0)


#define FREECHUNK_INSERT_LAST(m) do {					\
	FREECHUNK_INSERTAT(lfreechunk, m);				\
} while(/*CONSTCOND*/0)


static void memchunk_free(struct memchunk *m)
{
	struct memchunk **n, **p = &freechunk;
	size_t slen;

	FREECHUNK_FOREACHP(n) {
		if(*n > m)
			break;
		p = n;
	}

	ASSERT((p == &freechunk) || ((uintptr_t)((*p)->memdata + (*p)->len)
				<= (uintptr_t)m));
	ASSERT((*n == NULL) || ((uintptr_t)(m->memdata + m->len)
				<= (uintptr_t)(*n)));

	if((p != &freechunk) && ((uintptr_t)((*p)->memdata + (*p)->len)
				== (uintptr_t)m)) {
		MEMCHUNK_MERGE(*p, m);
		m = *p;
	}

	if((*n != NULL) && ((uintptr_t)(m->memdata + m->len)
				== (uintptr_t)(*n))) {
		MEMCHUNK_MERGE(m, *n);
		FREECHUNK_REMOVEAT(n);
		*n = NULL;
	}

	/**
	 * Revoke memory
	 */
	if((*n == NULL) && (((m->len + sizeof(*m)) & (PAGE_SIZE - 1)) == 0)) {
		/**
		 * The whole memchunk
		 */
		sbrk(-(m->len + sizeof(*m)));
		m = *p;
	} else if((*n == NULL) && (m->len > PAGE_SIZE)) {
		/**
		 * Partial memchunk
		 */
		/* Pagesz align Roundup */
		slen = ((m->len - 1) >> PAGE_SHIFT) << PAGE_SHIFT;
		sbrk(-slen);
		m->len = m->len - slen;
		ASSERT(m->len);
	}

	if(m != *p) {
		FREECHUNK_INSERTAT(p, m);
		MEMCHUNK_FREE(m);
	}
}


static void *memchunk_alloc(size_t size)
{
	struct memchunk **m, *next, *new = NULL;
	size_t len = ROUNDUP(size, sizeof(void *));
	size_t mlen = len + sizeof(*new);
	size_t slen, padlen;

	FREECHUNK_FOREACHP(m) {
		ASSERT(MEMCHUNK_IS_FREE(*m));
		new = *m;
		/**
		 * Check if free memory chunk is big enough to hold the new
		 * memchunk:
		 *
		 * 1) The memory chunk is big enough to be splitted in two
		 * parts one containing enough bytes for the requesting size.
		 * We will be split it up in two, one chunk for the new
		 * allocated memory, and the other one keeping remaining free
		 * bytes.
		 *
		 * 2) The free memory chunk is big enough to get the requesting
		 * memory size but too small to be splitted in two memory chunk,
		 * the whole chunk will be used.
		 */
		if(new->len > mlen) {
			/**
			 * Create a memory chunk of the required size and
			 * replace it directly in the freelist by the new
			 * splitted free space
			 */
			next = (struct memchunk *)(new->memdata + len);
			next->len = new->len - mlen;
			MEMCHUNK_FREE(next);
			FREECHUNK_REPLACE(m, next);
			new->len = len;
			break;
		} else if((new->len >= len) || (new->next == NULL)) {
			/**
			 * Just remove the memory chunk from the freelist
			 */
			FREECHUNK_REMOVEAT(m);
			MEMCHUNK_ALLOC(new);
			break;
		}
	}

	if((new != NULL) && (new->len >= len))
		return new;

	/**
	 * Change program break to get enough space to store our memory
	 */
	if(new == NULL) {
		/**
		 * Always use PAGE_SIZE multiples
		 */
		/* Pagesz align Roundup */
		slen = (((mlen - 1) >> PAGE_SHIFT) + 1) << PAGE_SHIFT;
		new = sbrk(slen);
		if(new == (void *)-1)
			return NULL;
		padlen = slen - mlen;
	} else {
		/**
		 * XXX use the last chunk for new data
		 */
		slen = (((len - new->len - 1) >> PAGE_SHIFT) + 1) << PAGE_SHIFT;
		if(sbrk(slen) == (void *)-1)
			return NULL;
		padlen = new->len + slen - len;
	}


	/**
	 * Check if memory is big enough to hold the request memory as well a
	 * free chunk
	 */
	if(padlen > sizeof(*next)) {
		next = (struct memchunk *)(new->memdata + len);
		next->len = padlen - sizeof(*next);
		MEMCHUNK_FREE(next);
		FREECHUNK_INSERT_LAST(next);
		new->len = len;
	} else {
		new->len = len + padlen;
	}

	MEMCHUNK_ALLOC(new);
	return new;
}


void free(void *p)
{
	struct memchunk *m = containerof(p, struct memchunk, memdata);

	if(!MEMCHUNK_IS_ALLOC(m)) {
		if(MEMCHUNK_IS_FREE(m))
			FATAL("Double Free\n");
		else
			FATAL("Invalid Free\n");
		ABORT();
	}

	memchunk_free(m);
}

void *malloc(size_t size)
{
	struct memchunk *m = NULL;

	if(size != 0)
		m = memchunk_alloc(size);

	if(m == NULL)
		return NULL;

	return m->memdata;
}


void *calloc(size_t nmemb, size_t size)
{
	struct memchunk *m = NULL;
	size_t i;

	if(nmemb != 0 && size != 0)
		m = memchunk_alloc(nmemb * size);

	if(m == NULL)
		return NULL;

	for(i = 0; i < nmemb * size; ++i)
		m->memdata[i] = 0;

	return m->memdata;
}

void *realloc(void *ptr, size_t size)
{
	struct memchunk *m;
	char *new;
	size_t i;

	if(ptr == NULL)
		return malloc(size);

	m = containerof(ptr, struct memchunk, memdata);
	if(!MEMCHUNK_IS_ALLOC(m)) {
		if(MEMCHUNK_IS_FREE(m))
			FATAL("Realloc on Free\n");
		else
			FATAL("Invalid realloc\n");
		ABORT();
	}

	new = malloc(size);
	if(new == NULL)
		return NULL;

	for(i = 0; i < MIN(size, m->len); ++i)
		new[i] = m->memdata[i];

	return new;
}
