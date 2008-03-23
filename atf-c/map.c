/*
 * Automated Testing Framework (atf)
 *
 * Copyright (c) 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND
 * CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>

#include "atf-c/map.h"
#include "atf-c/mem.h"
#include "atf-c/sanity.h"
#include "atf-c/text.h"

/* ---------------------------------------------------------------------
 * Auxiliary functions.
 * --------------------------------------------------------------------- */

struct map_entry {
    char *m_key;
    void *m_value;
    bool m_managed;
};

static
atf_error_t
new_entry(struct map_entry **mep, const char *key, void *value, bool managed)
{
    atf_error_t err;
    struct map_entry *me;

    err = atf_mem_alloc((void **)&me, sizeof(*me));
    if (!atf_is_error(err)) {
        err = atf_text_dup(&me->m_key, key);
        if (atf_is_error(err)) {
            atf_mem_free(me);
        } else {
            me->m_value = value;
            me->m_managed = managed;

            *mep = me;
        }
    }

    return err;
}

/* ---------------------------------------------------------------------
 * The "atf_map_citer" type.
 * --------------------------------------------------------------------- */

/*
 * Getters.
 */

const void *
atf_map_citer_data(const atf_map_citer_t citer)
{
    const struct map_entry *me = citer.m_entry;
    PRE(me != NULL);
    return me->m_value;
}

bool
atf_equal_map_citer_map_citer(const atf_map_citer_t i1,
                              const atf_map_citer_t i2)
{
    return i1.m_map == i2.m_map && i1.m_entry == i2.m_entry;
}

/* ---------------------------------------------------------------------
 * The "atf_map_iter" type.
 * --------------------------------------------------------------------- */

/*
 * Getters.
 */

void *
atf_map_iter_data(const atf_map_iter_t iter)
{
    const struct map_entry *me = iter.m_entry;
    PRE(me != NULL);
    return me->m_value;
}

bool
atf_equal_map_iter_map_iter(const atf_map_iter_t i1,
                            const atf_map_iter_t i2)
{
    return i1.m_map == i2.m_map && i1.m_entry == i2.m_entry;
}

/* ---------------------------------------------------------------------
 * The "atf_map" type.
 * --------------------------------------------------------------------- */

/*
 * Constructors and destructors.
 */

atf_error_t
atf_map_init(atf_map_t *m)
{
    atf_error_t err;

    err = atf_list_init(&m->m_list);
    if (atf_is_error(err))
        goto out;

    atf_object_init(&m->m_object);

out:
    return err;
}

void
atf_map_fini(atf_map_t *m)
{
    atf_list_iter_t iter;

    atf_list_for_each(iter, &m->m_list) {
        struct map_entry *me = atf_list_iter_data(iter);

        if (me->m_managed)
            atf_mem_free(me->m_value);
        atf_mem_free(me->m_key);
        atf_mem_free(me);
    }
    atf_list_fini(&m->m_list);

    atf_object_fini(&m->m_object);
}

/*
 * Getters.
 */

atf_map_iter_t
atf_map_end(atf_map_t *m)
{
    atf_map_iter_t iter;
    iter.m_map = m;
    iter.m_entry = NULL;
    return iter;
}

atf_map_citer_t
atf_map_end_c(const atf_map_t *m)
{
    atf_map_citer_t iter;
    iter.m_map = m;
    iter.m_entry = NULL;
    return iter;
}

atf_map_iter_t
atf_map_find(atf_map_t *m, const char *key)
{
    atf_list_iter_t iter;

    atf_list_for_each(iter, &m->m_list) {
        struct map_entry *me = atf_list_iter_data(iter);

        if (strcmp(me->m_key, key) == 0) {
            atf_map_iter_t i;
            i.m_map = m;
            i.m_entry = me;
            return i;
        }
    }

    return atf_map_end(m);
}

atf_map_citer_t
atf_map_find_c(const atf_map_t *m, const char *key)
{
    atf_list_citer_t iter;

    atf_list_for_each_c(iter, &m->m_list) {
        const struct map_entry *me = atf_list_citer_data(iter);

        if (strcmp(me->m_key, key) == 0) {
            atf_map_citer_t i;
            i.m_map = m;
            i.m_entry = me;
            return i;
        }
    }

    return atf_map_end_c(m);
}

size_t
atf_map_size(const atf_map_t *m)
{
    return atf_list_size(&m->m_list);
}

/*
 * Modifiers.
 */

atf_error_t
atf_map_insert(atf_map_t *m, const char *key, void *value, bool managed)
{
    struct map_entry *me;
    atf_error_t err;
    atf_map_iter_t iter;

    iter = atf_map_find(m, key);
    if (atf_equal_map_iter_map_iter(iter, atf_map_end(m))) {
        me = NULL; /* Silence GCC warning. */
        err = new_entry(&me, key, value, managed);
        if (!atf_is_error(err)) {
            err = atf_list_append(&m->m_list, me);
            if (atf_is_error(err)) {
                if (managed)
                    atf_mem_free(value);
                atf_mem_free(me);
            }
        }
    } else {
        me = iter.m_entry;
        if (me->m_managed)
            atf_mem_free(me->m_value);

        INV(strcmp(me->m_key, key) == 0);
        me->m_value = value;
        me->m_managed = managed;

        err = atf_no_error();
    }

    return err;
}
