#include <stdio.h>
#include <string.h>
 
typedef struct huffmanAgac {           //huffmanaðac yapýsý olusturuldu
	struct huffmanAgac *sol, *sag;
	int frekans;
	char c;
} *node;	
 
struct huffmanAgac pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[128] = {0}, buf[1024];
 
node new_node(int frekans, char c, node a, node b) //yeni düðüm noktasý
{
	node n = pool + n_nodes++;
	if (frekans) n->c = c, n->frekans = frekans;
	else {
		n->sol = a, n->sag = b;
		n->frekans = a->frekans + b->frekans;
	}
	return n;
}
 
/* Öncelik sýrasý */
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->frekans <= n->frekans) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 
node qremove()
{
	int i, l;
	node n = q[i = 1];
 
	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->frekans < q[l]->frekans) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}
 
/* 
Aðacda yürü ve 0s ve 1s koy */
void build_code(node n, char *s, int len)
{
	static char *out = buf;
	if (n->c) {
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}
 
	s[len] = '0'; build_code(n->sol,  s, len + 1);
	s[len] = '1'; build_code(n->sag, s, len + 1);
}
 
void init(const char *s)
{
	int i, frekans[128] = {0};
	char c[16];
 
	while (*s) frekans[(int)*s++]++;
 
	for (i = 0; i < 128; i++)
		if (frekans[i]) qinsert(new_node(frekans[i], i, 0, 0));
 
	while (qend > 2) 
		qinsert(new_node(0, 0, qremove(), qremove()));
 
	build_code(q[1], c, 0);
}
 
void encode(const char *s, char *out)
{
	while (*s) {
		strcpy(out, code[*s]);
		out += strlen(code[*s++]);
	}
}
 
void decode(const char *s, node t)
{
	node n = t;
	while (*s) {
		if (*s++ == '0') n = n->sol;
		else n = n->sag;
 
		if (n->c) putchar(n->c), n = t;
	}
 
	putchar('\n');
	if (t != n) printf("garbage input\n");
}
 
int main(void)
{
	int i;
	const char *str = "hello word";
        char buf[1024];
 
	init(str);
	for (i = 0; i < 128; i++)
		if (code[i]) printf("'%c': %s\n", i, code[i]);
 
	encode(str, buf);
	printf("kodlamaYapildi: %s\n", buf);
 
	printf("Cumle: ");
	decode(buf, q[1]);
 
	return 0;
}
