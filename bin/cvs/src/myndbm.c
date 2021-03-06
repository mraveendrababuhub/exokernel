/*
 * Copyright (c) 1992, Brian Berliner
 * 
 * You may distribute under the terms of the GNU General Public License as
 * specified in the README file that comes with the CVS 1.4 kit.
 * 
 * A simple ndbm-emulator for CVS.  It parses a text file of the format:
 * 
 * key	value
 * 
 * at dbm_open time, and loads the entire file into memory.  As such, it is
 * probably only good for fairly small modules files.  Ours is about 30K in
 * size, and this code works fine.
 */

#include <assert.h>
#include "cvs.h"
#include "getline.h"

#ifdef MY_NDBM

static void mydbm_load_file ();

/* ARGSUSED */
DBM *
mydbm_open (file, flags, mode)
    char *file;
    int flags;
    int mode;
{
    FILE *fp;
    DBM *db;

    fp = CVS_FOPEN (file, FOPEN_BINARY_READ);
    if (fp == NULL && !(existence_error (errno) && (flags & O_CREAT)))
	return ((DBM *) 0);

    db = (DBM *) xmalloc (sizeof (*db));
    db->dbm_list = getlist ();
    db->modified = 0;
    db->name = xstrdup (file);

    if (fp != NULL)
    {
	mydbm_load_file (fp, db->dbm_list);
	if (fclose (fp) < 0)
	    error (0, errno, "cannot close %s", file);
    }
    return (db);
}

static int write_item PROTO ((Node *, void *));

static int
write_item (node, data)
    Node *node;
    void *data;
{
    FILE *fp = (FILE *)data;
    fputs (node->key, fp);
    fputs (" ", fp);
    fputs (node->data, fp);
    fputs ("\012", fp);
    return 0;
}

void
mydbm_close (db)
    DBM *db;
{
    if (db->modified)
    {
	FILE *fp;
	fp = CVS_FOPEN (db->name, FOPEN_BINARY_WRITE);
	if (fp == NULL)
	    error (1, errno, "cannot write %s", db->name);
	walklist (db->dbm_list, write_item, (void *)fp);
	if (fclose (fp) < 0)
	    error (0, errno, "cannot close %s", db->name);
    }
    free (db->name);
    dellist (&db->dbm_list);
    free ((char *) db);
}

datum
mydbm_fetch (db, key)
    DBM *db;
    datum key;
{
    Node *p;
    char *s;
    datum val;

    /* make sure it's null-terminated */
    s = xmalloc (key.dsize + 1);
    (void) strncpy (s, key.dptr, key.dsize);
    s[key.dsize] = '\0';

    p = findnode (db->dbm_list, s);
    if (p)
    {
	val.dptr = p->data;
	val.dsize = strlen (p->data);
    }
    else
    {
	val.dptr = (char *) NULL;
	val.dsize = 0;
    }
    free (s);
    return (val);
}

datum
mydbm_firstkey (db)
    DBM *db;
{
    Node *head, *p;
    datum key;

    head = db->dbm_list->list;
    p = head->next;
    if (p != head)
    {
	key.dptr = p->key;
	key.dsize = strlen (p->key);
    }
    else
    {
	key.dptr = (char *) NULL;
	key.dsize = 0;
    }
    db->dbm_next = p->next;
    return (key);
}

datum
mydbm_nextkey (db)
    DBM *db;
{
    Node *head, *p;
    datum key;

    head = db->dbm_list->list;
    p = db->dbm_next;
    if (p != head)
    {
	key.dptr = p->key;
	key.dsize = strlen (p->key);
    }
    else
    {
	key.dptr = (char *) NULL;
	key.dsize = 0;
    }
    db->dbm_next = p->next;
    return (key);
}

/* Note: only updates the in-memory copy, which is written out at
   mydbm_close time.  Note: Also differs from DBM in that on duplication,
   it gives a warning, rather than either DBM_INSERT or DBM_REPLACE
   behavior.  */
int
mydbm_store (db, key, value, flags)
    DBM *db;
    datum key;
    datum value;
    int flags;
{
    Node *node;

    node = getnode ();
    node->type = NDBMNODE;

    node->key = xmalloc (key.dsize + 1);
    strncpy (node->key, key.dptr, key.dsize);
    node->key[key.dsize] = '\0';

    node->data = xmalloc (value.dsize + 1);
    strncpy (node->data, value.dptr, value.dsize);
    node->data[value.dsize] = '\0';

    db->modified = 1;
    if (addnode (db->dbm_list, node) == -1)
    {
	error (0, 0, "attempt to insert duplicate key `%s'", node->key);
	freenode (node);
	return 0;
    }
    return 0;
}

static void
mydbm_load_file (fp, list)
    FILE *fp;
    List *list;
{
    char *line = NULL;
    size_t line_len;
    /* FIXME: arbitrary limit.  */
    char value[MAXLINELEN];
    char *cp, *vp;
    int len, cont;

    for (cont = 0; getline (&line, &line_len, fp) >= 0;)
    {
	if ((cp = strrchr (line, '\012')) != NULL)
	    *cp = '\0';			/* strip the newline */
	cp = line + strlen (line);
	if (cp > line && cp[-1] == '\015')
	    /* If the file (e.g. modules) was written on an NT box, it will
	       contain CRLF at the ends of lines.  Strip them (we can't do
	       this by opening the file in text mode because we might be
	       running on unix).  */
	    cp[-1] = '\0';	    

	/*
	 * Add the line to the value, at the end if this is a continuation
	 * line; otherwise at the beginning, but only after any trailing
	 * backslash is removed.
	 */
	vp = value;
	if (cont)
	    vp += strlen (value);

	/*
	 * See if the line we read is a continuation line, and strip the
	 * backslash if so.
	 */
	len = strlen (line);
	if (len > 0)
	    cp = &line[len - 1];
	else
	    cp = line;
	if (*cp == '\\')
	{
	    cont = 1;
	    *cp = '\0';
	}
	else
	{
	    cont = 0;
	}
	(void) strcpy (vp, line);
	if (value[0] == '#')
	    continue;			/* comment line */
	vp = value;
	while (*vp && isspace (*vp))
	    vp++;
	if (*vp == '\0')
	    continue;			/* empty line */

	/*
	 * If this was not a continuation line, add the entry to the database
	 */
	if (!cont)
	{
	    Node *p = getnode ();
	    char *kp;

	    kp = vp;
	    while (*vp && !isspace (*vp))
		vp++;
	    *vp++ = '\0';		/* NULL terminate the key */
	    p->type = NDBMNODE;
	    p->key = xstrdup (kp);
	    while (*vp && isspace (*vp))
		vp++;			/* skip whitespace to value */
	    if (*vp == '\0')
	    {
		error (0, 0, "warning: NULL value for key `%s'", p->key);
		freenode (p);
		continue;
	    }
	    p->data = xstrdup (vp);
	    if (addnode (list, p) == -1)
	    {
		error (0, 0, "duplicate key found for `%s'", p->key);
		freenode (p);
	    }
	}
    }
    free (line);
}

#endif				/* MY_NDBM */
