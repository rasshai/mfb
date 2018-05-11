#include <ap_int.h>
#include "mfb.hpp"

ap_uint<32> mul_add(ap_uint<1> j, ap_uint<32> c, ap_uint<1> c_vld, ap_uint<COUNT_WIDTH> cnt, ap_uint<32> prev);

void mfb(ap_uint<32> c, ap_uint<1> c_vld, ap_uint<32> *res, ap_uint<1> *res_vld)
{
#pragma HLS PIPELINE
    static ap_uint<COUNT_WIDTH> init_cnt = 0;
    static ap_uint<32>          C[N] = {};
#pragma HLS ARRAY_PARTITION variable=C complete dim=1
    static ap_uint<1>           C_VLD[N] = {};
#pragma HLS ARRAY_PARTITION variable=C_VLD complete dim=1
    static ap_uint<COUNT_WIDTH> CNT[N] = {};
#pragma HLS ARRAY_PARTITION variable=CNT complete dim=1
    static ap_uint<N>           J[N] = {0b1010, 0b0011, 0b1100, 0b0101};
#pragma HLS ARRAY_PARTITION variable=J complete dim=1

    static ap_uint<32>          PROD_SUM[N];
    static ap_uint<1>           PROD_SUM_VLD;
    static ap_uint<COUNT_WIDTH> OUTPUT_COUNTER = 0;

    static ap_uint<1>           init = 0;

    if (!init) {
        CNT[0] = N - 1;
        init = 1;
    } else {
        /* output */
        *res = PROD_SUM[OUTPUT_COUNTER];
        *res_vld = PROD_SUM_VLD;

        /* calculate product sum */
        prod_sum_loop: for (int i = N - 1; i >= 0; --i) {
#pragma HLS LOOP_FLATTEN
            ap_uint<1> j = J[i].bit(N - 1 - CNT[i]);
            PROD_SUM[i] = mul_add(j, C[i], C_VLD[i], CNT[i], PROD_SUM[i]);
            if (i < N - 1) {
                C[i+1] = C[i];
                C_VLD[i+1] = C_VLD[i];
                CNT[i+1] = CNT[i];
            }
        }
        PROD_SUM_VLD = C_VLD[0] & (CNT[0] == N - 1);
        if (PROD_SUM_VLD) {
            OUTPUT_COUNTER = 0;
        } else {
            ++OUTPUT_COUNTER;
        }

        /* update counter */
        if (c_vld) {
            CNT[0] = CNT[0] + 1;
        }

        /* input */
        C[0] = c;
        C_VLD[0] = c_vld;
    }
}

ap_uint<32> mul_add(ap_uint<1> j, ap_uint<32> c, ap_uint<1> c_vld, ap_uint<COUNT_WIDTH> cnt, ap_uint<32> prev)
{
#pragma HLS INLINE
    ap_uint<32> product;
    if (c_vld) {
        product = j * c;
    } else {
        product = 0;
    }
    ap_uint<32> acc;
    if (cnt == 0) {
        acc = 0;
    } else {
        acc = prev;
    }

    return product + acc;
}
