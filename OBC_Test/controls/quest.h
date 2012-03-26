#ifndef QUEST_H
#define QUEST_H

#define SS_HALF 2000
#define SS_MAX_ANGLE 85
#define MAG_WEIGHT 0.9
#define N_SS 6

#define TAU_W 60
#define A_F 0.032258064516129031

uint8_t quest(vector v_B_c, vector v_sun_c, quaternion q_triad, uint8_t * p_w_ctrl);

void omega_estimation(quaternion q_B, vector v_w);

#endif
