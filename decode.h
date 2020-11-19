#ifndef __DECODE
#define __DECODE

#include "multihart_ip.h"

void init_decode_status(
  decode_status_t *decode_status,
  bit_t           *decode_status_is_full);
void decode(
  decode_in_from_fetch_t decode_in_from_fetch,
  bit_t                 *issue_status_is_full,
  decode_status_t       *decode_status,
  decode_out_to_fetch_t *decode_out_to_fetch,
  decode_out_to_issue_t *decode_out_to_issue,
  bit_t                 *decode_status_is_full);

#endif
