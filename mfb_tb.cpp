#include <ap_int.h>
#include <stdio.h>
#include "mfb.hpp"

using namespace std;

int main(void)
{
    ap_uint<N>  J[N] = {0b1010, 0b0011, 0b1100, 0b0101};
    ap_uint<32> C[N] = {1, 2, 3, 4};
    ap_uint<1>  C_VLD[N] = {1, 1, 1, 1};
    ap_uint<32> A[N] = {4, 7, 3, 6};

    ap_uint<32> res;
    ap_uint<1>  res_vld = 0;
    ap_uint<32> res_cnt = 0;

    int error_num = 0;

    mfb(0, 0, &res, &res_vld);

    for (int i = 0; i < N; ++i) {
        mfb(C[i], C_VLD[i], &res, &res_vld);
        cout << res_vld << '\t' << res << endl;
    }

    for (int i = 0; i < N * 2; ++i) {
        mfb(0, 0, &res, &res_vld);
        cout << res_vld << '\t' << res << endl;
    }

    return 0;
}
