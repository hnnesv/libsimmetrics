/*
 * euclidean_distance.c
 *
 *  Created on: 10 Dec 2012
 *      Author: johnathan
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "cost.h"
#include "uthash.h"
#include "utarray.h"
#include "tokenizer.h"
#include "euclidean_distance.h"

const float euclidean_distance_custom(const char *str1, const char *str2, std_tokenizer_t *tokenizer) {

	UT_array *t1 = tokenizer->tok_utarr_func(str1, tokenizer->delimiters);
	UT_array *t2 = tokenizer->tok_utarr_func(str2, tokenizer->delimiters);

	hash_token_t *h1 = tokenizer->tok_uq_hash_func(str1, tokenizer->delimiters);
	hash_token_t *h2 = tokenizer->tok_uq_hash_func(str2, tokenizer->delimiters);

	hash_token_t *all = merge_tokens(h1, h2);
	hash_token_t *s;

	int cs1, cs2;
	float td = 0;

	char **tmp;

	for(s = all; s != NULL; s = s->hh.next) {

		cs1 = 0;
		cs2 = 0;

		while((tmp = (char **) utarray_next(t1, tmp))) {

			if(strcmp(*tmp, s->value) == 0)
				cs1++;

		}

		while((tmp = (char **) utarray_next(t2, tmp))) {

			if(strcmp(*tmp, s->value) == 0)
				cs2++;

		}

		td += (float)(((float)cs1 - (float)cs2) * ((float)cs1 - (float)cs2));

	}

	utarray_free(t1);
	utarray_free(t2);

	hash_token_free(h1);
	hash_token_free(h2);
	hash_token_free(all);

	return sqrtf(td);

}

const float euclidean_distance(const char *str1, const char *str2) {

	std_tokenizer_t tokenizer = {
			.delimiters = WHITESPACE_DELIMITERS,
			.tok_utarr_func = &tokenize_to_utarray,
			.tok_uq_hash_func = &uq_tokenize_to_hash
	};

	return euclidean_distance_custom(str1, str2, &tokenizer);

}

const float euclidean_distance_similarity_custom(const char *str1, const char *str2, std_tokenizer_t *tokenizer) {

	UT_array *tokarr1 = tokenizer->tok_utarr_func(str1, tokenizer->delimiters);
	UT_array *tokarr2 = tokenizer->tok_utarr_func(str2, tokenizer->delimiters);

	const float tl1 = utarray_len(tokarr1), tl2 = utarray_len(tokarr2);
	const float tp = sqrtf((tl1 * tl1) + (tl2 * tl2));
	const float td = euclidean_distance_custom(str1, str2, tokenizer);

	utarray_free(tokarr1);
	utarray_free(tokarr2);

	return (tp - td) / tp;

}

const float euclidean_distance_similarity(const char *str1, const char *str2) {

	std_tokenizer_t tokenizer = {
			.delimiters = WHITESPACE_DELIMITERS,
			.tok_utarr_func = &tokenize_to_utarray,
			.tok_uq_hash_func = &uq_tokenize_to_hash
	};

	return euclidean_distance_similarity_custom(str1, str2, &tokenizer);

}
