#ifndef COURSE_BTOR_TOOLS_H
#define COURSE_BTOR_TOOLS_H

#include <stdbool.h>
#include "simp_tools.h"

extern "C" {
#include "btorcore.h"
#include "btorexp.h"
};

BtorBitVector *bv_GCF (BtorMemMgr *mm, BtorBitVector *bv1, BtorBitVector *bv2); //greatest common factor
BtorBitVector *bv_LCM (BtorMemMgr *mm, BtorBitVector *bv1, BtorBitVector *bv2); //least common multiple
BtorBitVector *uint_LCM (BtorMemMgr *mm, BtorBitVector *bv1, BtorBitVector *bv2);
bool only_this_var (BtorNode *exp, BtorNode *parent, BtorNode *var);
bool without_this_var (BtorNode *exp, BtorNode *var);

#endif //COURSE_BTOR_TOOLS_H
