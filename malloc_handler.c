#include <stdio.h>
#include <malloc.h>

static long m_count = 0;

static void swap_hook (void);

static void * new_malloc (size_t size, const void * caller);
static void new_free (void *ptr, const void *caller);

static void * (*old_hook_malloc)(size_t size, const void * caller);
static void (*old_hook_free)(void *ptr, const void *caller);

void (*__malloc_initialize_hook) (void) = swap_hook;

static void swap_hook(void)
{
	old_hook_malloc = __malloc_hook;
	__malloc_hook = new_malloc;
	old_hook_free = __free_hook;
	__free_hook = new_free;
}

static void * new_malloc(size_t size, const void *caller)
{
	void *ptr;
	__malloc_hook = old_hook_malloc;
	ptr = malloc(size);
	m_count++;
	old_hook_malloc = __malloc_hook;
	printf("malloc() called: size = %lu; caller: %p; ptr_res = %p; state: %ld \n", size, caller, ptr, m_count);
	__malloc_hook = new_malloc;

	return ptr;
}

static void new_free(void * ptr, const void *caller)
{
	__free_hook = old_hook_free;
	free(ptr);
	m_count--;
	old_hook_free = __free_hook;
	printf("free() called: caller: %p; ptr = %p; state: %ld \n", caller, ptr, m_count);
	__free_hook = new_free;
}
