#include "btor_tools.h"

BtorBitVector *
bv_GCF(BtorMemMgr *mm, BtorBitVector *bv1, BtorBitVector *bv2) //greatest common factor
{
    int32_t compare = btor_bv_compare(bv1, bv2);
    if (compare == 0)
        return bv1;
    BtorBitVector *div = btor_bv_new(mm, btor_bv_get_width(bv1));
    if (compare == -1)
        div = bv_GCF(mm, bv1, btor_bv_sub(mm, bv2, bv1));
    else
        div = bv_GCF(mm, bv2, btor_bv_sub(mm, bv1, bv2));
    return div;
}

BtorBitVector *
bv_LCM(BtorMemMgr *mm, BtorBitVector *bv1, BtorBitVector *bv2) //least common multiple
{
    BtorBitVector *GCF = bv_GCF(mm, bv1, bv2);
    if (btor_bv_is_umulo(mm, bv1, btor_bv_udiv(mm, bv2, GCF)))
        return btor_bv_ones(mm, btor_bv_get_width(bv1));
    else
        return btor_bv_mul(mm, bv1, btor_bv_udiv(mm, bv2, GCF));
}

BtorBitVector *
uint_LCM(BtorMemMgr *mm, BtorBitVector *bv1, BtorBitVector *bv2) //least common multiple
{
    uint64_t n1 = btor_bv_to_uint64(bv1);
    uint64_t n2 = btor_bv_to_uint64(bv2);
    uint32_t width = btor_bv_get_width(bv1);
    uint64_t max = btor_bv_to_uint64(btor_bv_ones(mm, width));
    uint64_t gcf = GCF(n1, n2);
    uint64_t lcm = n1 * n2 / gcf;
    if (lcm > max)
        return btor_bv_ones(mm, width);
    else
        return btor_bv_uint64_to_bv(mm, lcm, width);
}

bool
only_this_var(BtorNode *exp, BtorNode *parent, BtorNode *var)
{
    if (exp->kind > 3) {
        if (!only_this_var(exp->e[0], exp, var) && !btor_node_is_bv_const(exp->e[0])) {
            return false;
        }
        return (only_this_var(exp->e[1], exp, var) || btor_node_is_bv_const(exp->e[1]));
    } else
        return (exp == var);
}

bool
without_this_var(BtorNode *exp, BtorNode *var)
{
    if (exp->kind > 3) {
        if (!without_this_var(exp->e[0], var))
            return false;
        return without_this_var(exp->e[1], var);
    } else
        return (exp != var);
}

void
printf_exps_info(Btor *btor)
{
    BtorNode *exp;
    for (int i = 1; i < stack_size; i++) {
        exp = btor_node_copy(btor, BTOR_PEEK_STACK(btor->nodes_id_table, i));
        printf("id: %d kind: %d\n", btor_node_get_id(exp), exp->kind);
        if (exp->kind > 3) {
            printf("kinds of children: %d %d\n\n", exp->e[0]->kind, exp->e[1]->kind);
        } else
            printf("\n");
    }
}

BtorNode*
get_exists_var(Btor *btor)
{
    int i = 0;
    bool is_exists_var = 0;
    while (i + 1 < stack_size && !is_exists_var) {
        i++;
        exists_var = btor_node_copy(btor, BTOR_PEEK_STACK(btor->nodes_id_table, i));
        if (btor_node_is_param(exists_var))
            is_exists_var = btor_node_param_is_exists_var(exists_var);
    }
    assert(btor_node_param_is_exists_var(exists_var));
    return exists_var;
}
