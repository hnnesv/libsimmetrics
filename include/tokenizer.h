/*
 * tokenizer.h
 *
 *  Created on: 05 Dec 2012
 *      Author: johnathan
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#define WHITESPACE_DELIMITERS "\r\n\t \u00A0"

#define QGRAM_SP "#"
#define QGRAM_EP "#"

typedef struct {

	unsigned int key;
	char *value;
	UT_hash_handle hh;

} hash_token_t;

typedef struct ll_token_t {

	char *token;
	struct ll_token_t *next;

} ll_token_t;

typedef struct dl_token_t {

	char *token;
	struct dl_token_t *next, *prev;

} dl_token_t;

typedef enum {
	QGRAM_1 = 1,
	QGRAM_2 = 2,
	QGRAM_3 = 3
} qgram_e;

typedef struct {

	qgram_e qgram_len;
	bool extended;

} qgram_t;

typedef struct {

	qgram_t *qgram;
	UT_array *(*tok_utarr_func) (const char *, const qgram_t *);
	hash_token_t *(*tok_uq_hash_func) (const char *, const qgram_t *);

} qgram_tokenizer_t;

typedef struct {

	const char *delimiters;
	UT_array *(*tok_utarr_func) (const char *, const char *);
	hash_token_t *(*tok_uq_hash_func) (const char *, const char *);

} std_tokenizer_t;

//Doubly linked list tokenizers
dl_token_t *tokenize_to_dllist(const char *str, const char *delimiters);
dl_token_t *qgram_tokenize_to_dllist(const char *str, const qgram_t *qtype);

//Unique hashtable tokenizers
hash_token_t *merge_tokens(hash_token_t *t1, hash_token_t *t2);
hash_token_t *uq_tokenize_to_hash(const char *str, const char *delimiters);
hash_token_t *qgram_uq_tokenize_to_hash(const char *str, const qgram_t *qtype);

//Dynamic array tokenizers
UT_array *tokenize_to_utarray(const char *str, const char *delimiters);
UT_array *qgram_tokenize_to_utarray(const char *str, const qgram_t *qtype);

//Cleanup functions
void hash_token_free(hash_token_t *table);
void dll_token_free(dl_token_t *head);

#endif /* TOKENIZER_H_ */
