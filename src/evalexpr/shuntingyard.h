#ifndef SHUNTINGYARD_H
# define SHUNTINGYARD_H

void shuntingyard(s_opstack *so, s_numstack *sn, const s_op *o);
void rpn_eval(s_opstack *so, s_numstack *sn);

#endif /* !SHUNTINGYARD_H */
