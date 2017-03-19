/**************************************************************************************************
*                                                                                                 *
* This file is part of BLASFEO.                                                                   *
*                                                                                                 *
* BLASFEO -- BLAS For Embedded Optimization.                                                      *
* Copyright (C) 2016-2017 by Gianluca Frison.                                                     *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* HPMPC is free software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPMPC is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPMPC; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, giaf (at) dtu.dk                                                       *
*                          gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "../include/blasfeo_common.h"
#include "../include/blasfeo_i_aux_ext_dep.h"
#include "../include/blasfeo_d_aux_ext_dep.h"
#include "../include/blasfeo_v_aux_ext_dep.h"
#include "../include/blasfeo_d_aux.h"
#include "../include/blasfeo_d_kernel.h"
#include "../include/blasfeo_d_blas.h"


int main()
	{

#if defined(LA_HIGH_PERFORMANCE)

	printf("\nLA provided by HIGH_PERFORMANCE\n\n");

#elif defined(LA_REFERENCE)

	printf("\nLA provided by REFERENCE\n\n");

#elif defined(LA_BLAS)

	printf("\nLA provided by BLAS\n\n");

#else

	printf("\nLA provided by ???\n\n");
	exit(2);

#endif

	int ii;

	int n = 8;

	//
	// matrices in column-major format
	//
	double *A; d_zeros(&A, n, n);
	for(ii=0; ii<n*n; ii++) A[ii] = ii;
//	d_print_mat(n, n, A, n);

	double *B; d_zeros(&B, n, n);
	for(ii=0; ii<n; ii++) B[ii*(n+1)] = 1.0;
//	d_print_mat(n, n, B, n);

	double *C; d_zeros(&C, n, n);

	double *x_n; d_zeros(&x_n, n, 1);
//	for(ii=0; ii<n; ii++) x_n[ii] = 1.0;
	x_n[1] = 1.0;
//	x_n[1] = 1.0;
//	x_n[2] = 2.0;
//	x_n[3] = 3.0;
	double *x_t; d_zeros(&x_t, n, 1);
//	for(ii=0; ii<n; ii++) x_n[ii] = 1.0;
	x_t[0] = 1.0;
	double *y_n; d_zeros(&y_n, n, 1);
	double *y_t; d_zeros(&y_t, n, 1);
	double *z_n; d_zeros(&z_n, n, 1);
	double *z_t; d_zeros(&z_t, n, 1);

	double *x0; d_zeros(&x0, n, 1); x0[0] = 1.0;
	double *x1; d_zeros(&x1, n, 1); x1[1] = 1.0;
	double *x2; d_zeros(&x2, n, 1); x2[2] = 1.0;
	double *x3; d_zeros(&x3, n, 1); x3[3] = 1.0;
	double *x4; d_zeros(&x4, n, 1); x4[4] = 1.0;
	double *x5; d_zeros(&x5, n, 1); x5[5] = 1.0;
	double *x6; d_zeros(&x6, n, 1); x6[6] = 1.0;
	double *x7; d_zeros(&x7, n, 1); x7[7] = 1.0;
	double *x8; d_zeros(&x8, n, 1); x8[8] = 1.0;
	double *x9; d_zeros(&x9, n, 1); x9[9] = 1.0;

	int *ipiv; int_zeros(&ipiv, n, 1);

	//
	// matrices in matrix struct format
	//
	int size_strmat = 4*d_size_strmat(n, n);
	void *memory_strmat; v_zeros_align(&memory_strmat, size_strmat);
	char *ptr_memory_strmat = (char *) memory_strmat;

	struct d_strmat sA;
//	d_allocate_strmat(n, n, &sA);
	d_create_strmat(n, n, &sA, ptr_memory_strmat);
	ptr_memory_strmat += sA.memory_size;
	d_cvt_mat2strmat(n, n, A, n, &sA, 0, 0);
//	d_cast_mat2strmat(A, &sA);
	d_print_strmat(n, n, &sA, 0, 0);

	struct d_strmat sB;
//	d_allocate_strmat(n, n, &sB);
	d_create_strmat(n, n, &sB, ptr_memory_strmat);
	ptr_memory_strmat += sB.memory_size;
	d_cvt_mat2strmat(n, n, B, n, &sB, 0, 0);
	d_print_strmat(n, n, &sB, 0, 0);

	struct d_strmat sD;
//	d_allocate_strmat(n, n, &sD);
	d_create_strmat(n, n, &sD, ptr_memory_strmat);
	ptr_memory_strmat += sD.memory_size;

	struct d_strmat sE;
//	d_allocate_strmat(n, n, &sE);
	d_create_strmat(n, n, &sE, ptr_memory_strmat);
	ptr_memory_strmat += sE.memory_size;

	struct d_strvec sx_n;
	d_allocate_strvec(n, &sx_n);
	d_cvt_vec2strvec(n, x_n, &sx_n, 0);

	struct d_strvec sx_t;
	d_allocate_strvec(n, &sx_t);
	d_cvt_vec2strvec(n, x_t, &sx_t, 0);

	struct d_strvec sy_n;
	d_allocate_strvec(n, &sy_n);
	d_cvt_vec2strvec(n, y_n, &sy_n, 0);

	struct d_strvec sy_t;
	d_allocate_strvec(n, &sy_t);
	d_cvt_vec2strvec(n, y_t, &sy_t, 0);

	struct d_strvec sz_n;
	d_allocate_strvec(n, &sz_n);
	d_cvt_vec2strvec(n, z_n, &sz_n, 0);

	struct d_strvec sz_t;
	d_allocate_strvec(n, &sz_t);
	d_cvt_vec2strvec(n, z_t, &sz_t, 0);

	struct d_strvec sx0; d_create_strvec(n, &sx0, x0);
	struct d_strvec sx1; d_create_strvec(n, &sx1, x1);
	struct d_strvec sx2; d_create_strvec(n, &sx2, x2);
	struct d_strvec sx3; d_create_strvec(n, &sx3, x3);
	struct d_strvec sx4; d_create_strvec(n, &sx4, x4);
	struct d_strvec sx5; d_create_strvec(n, &sx5, x5);
	struct d_strvec sx6; d_create_strvec(n, &sx6, x6);
	struct d_strvec sx7; d_create_strvec(n, &sx7, x7);
	struct d_strvec sx8; d_create_strvec(n, &sx8, x8);
	struct d_strvec sx9; d_create_strvec(n, &sx9, x9);

	struct d_strvec sz0; d_allocate_strvec(n, &sz0);
	struct d_strvec sz1; d_allocate_strvec(n, &sz1);
	struct d_strvec sz2; d_allocate_strvec(n, &sz2);
	struct d_strvec sz3; d_allocate_strvec(n, &sz3);
	struct d_strvec sz4; d_allocate_strvec(n, &sz4);
	struct d_strvec sz5; d_allocate_strvec(n, &sz5);
	struct d_strvec sz6; d_allocate_strvec(n, &sz6);
	struct d_strvec sz7; d_allocate_strvec(n, &sz7);
	struct d_strvec sz8; d_allocate_strvec(n, &sz8);
	struct d_strvec sz9; d_allocate_strvec(n, &sz9);

	// tests
	dmatse_libstr(n, n, 100.0, &sD, 0, 0);

	for(ii=0; ii<n; ii++)
		dvecin1_libstr(ii+1, &sx_n, ii);
	d_print_tran_strvec(n, &sx_n, 0);
	d_print_strmat(n, n, &sD, 0, 0);
	// ddiain_libstr(4, -1.0, &sx_n, 1, &sD, 3, 2);
	ddiaad_libstr(4, -1.0, &sx_n, 1, &sD, 3, 2);
	d_print_strmat(n, n, &sD, 0, 0);
	return 0;

//	d_print_tran_strvec(n, &sx_n, 0);
//	dgemm_l_diag_libstr(n, n, 1.0, &sx_n, 0, &sA, 0, 0, 0.0, &sD, 0, 0, &sD, 0, 0);
//	dgemm_r_diag_libstr(n, n, 1.0, &sA, 0, 0, &sx_n, 0, 0.0, &sD, 0, 0, &sD, 0, 0);
//	d_print_strmat(n, n, &sD, 0, 0);
//	exit(1);

//	dsetmat_libstr(n, n, 0.0, &sD, 0, 0);
//	dmatin1_libstr(2.0, &sD, 0, 0);
//	dmatin1_libstr(2.0, &sD, 1, 1);
//	dmatin1_libstr(2.0, &sD, 2, 2);
//	dmatin1_libstr(1.0, &sD, 1, 0);
//	dmatin1_libstr(1.0, &sD, 2, 1);
//	dmatin1_libstr(0.5, &sD, 2, 0);
//	d_print_strmat(n, n, &sD, 0, 0);
//	d_print_tran_strvec(n, &sx_n, 0);
//	dtrsv_lnn_libstr(n, n, &sD, 0, 0, &sx_n, 0, &sz_n, 0);
//	d_print_tran_strvec(n, &sz_n, 0);
//	exit(1);

//	dgemm_nt_libstr(8, 8, 8, 1.0, &sB, 0, 0, &sA, 1, 0, 0.0, &sD, 0, 0, &sD, 0, 0);
//	d_print_strmat(n, n, &sD, 0, 0);
//	return 0;

//	double alpha = 1.0;
//	kernel_dtrmm_nn_rl_4x4_gen_lib4(7, &alpha, sB.pA, 2, sA.pA, sA.cn, 1, sD.pA, sD.cn, 0, 4, 1, 4);
//	kernel_dtrmm_nn_rl_4x4_gen_lib4(7, &alpha, sB.pA+sB.cn*4, 2, sA.pA, sA.cn, 1, sD.pA+sD.cn*4, sD.cn, 0, 4, 1, 4);
//	kernel_dtrmm_nn_rl_4x4_lib4(4, &alpha, sB.pA, sA.pA, sA.cn+4*4, sD.pA+4*4);
//	kernel_dtrmm_nn_rl_4x4_gen_lib4(3, &alpha, sB.pA+sB.cn*4+4*4, 2, sA.pA+sB.cn*4+4*4, sA.cn, 1, sD.pA+sD.cn*4+4*4, sD.cn, 0, 4, 0, 4);
	dtrmm_rlnn_libstr(8, 8, 1.0, &sB, 0, 0, &sA, 3, 0, &sD, 2, 1);
	d_print_strmat(n, n, &sD, 0, 0);
	return 0;

	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx0, 0, &sx0, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx1, 0, &sx1, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx2, 0, &sx2, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx3, 0, &sx3, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx4, 0, &sx4, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx5, 0, &sx5, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx6, 0, &sx6, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx7, 0, &sx7, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx8, 0, &sx8, 0);
	dtrmv_lnn_libstr(8, 8, &sA, 0, 0, &sx9, 0, &sx9, 0);
	d_print_tran_strvec(n, &sx0, 0);
	d_print_tran_strvec(n, &sx1, 0);
	d_print_tran_strvec(n, &sx2, 0);
	d_print_tran_strvec(n, &sx3, 0);
	d_print_tran_strvec(n, &sx4, 0);
	d_print_tran_strvec(n, &sx5, 0);
	d_print_tran_strvec(n, &sx6, 0);
	d_print_tran_strvec(n, &sx7, 0);
	d_print_tran_strvec(n, &sx8, 0);
	d_print_tran_strvec(n, &sx9, 0);
	return 0;

	dgemv_t_libstr(2, 8, 1.0, &sA, 2, 0, &sx_n, 0, 0.0, &sy_n, 0, &sz_n, 0);
	d_print_tran_strvec(n, &sz_n, 0);
	return 0;

	dgemm_nt_libstr(4, 8, 8, 1.0, &sB, 0, 0, &sA, 0, 0, 0.0, &sB, 0, 0, &sD, 3, 0);
//	d_print_strmat(n, n, &sB, 0, 0);
	d_print_strmat(n, n, &sD, 0, 0);
	exit(1);

	dpotrf_l_libstr(n, n, &sD, 0, 0, &sD, 0, 0);
//	dgetrf_nopivot_libstr(n, n, &sD, 0, 0, &sD, 0, 0);
//	dgetrf_libstr(n, n, &sD, 0, 0, &sD, 0, 0, ipiv);
	d_print_strmat(n, n, &sD, 0, 0);
#if defined(LA_HIGH_PERFORMANCE) | defined(LA_REFERENCE)
	d_print_mat(1, n, sD.dA, 1);
#endif
	int_print_mat(1, n, ipiv, 1);
	dtrsm_rltn_libstr(n, n, 1.0, &sD, 0, 0, &sB, 0, 0, &sE, 0, 0);
	d_print_strmat(n, n, &sE, 0, 0);
	exit(1);

#if 1 // solve P L U X = P B
	d_print_strmat(n, n, &sB, 0, 0);
	drowpe_libstr(n, ipiv, &sB);
	d_print_strmat(n, n, &sB, 0, 0);

	dtrsm_llnu_libstr(n, n, 1.0, &sD, 0, 0, &sB, 0, 0, &sE, 0, 0);
	d_print_strmat(n, n, &sE, 0, 0);
	dtrsm_lunn_libstr(n, n, 1.0, &sD, 0, 0, &sE, 0, 0, &sE, 0, 0);
	d_print_strmat(n, n, &sE, 0, 0);
#else // solve X^T (P L U)^T = B^T P^T
	d_print_strmat(n, n, &sB, 0, 0);
	dcolpe_libstr(n, ipiv, &sB);
	d_print_strmat(n, n, &sB, 0, 0);

	dtrsm_rltu_libstr(n, n, 1.0, &sD, 0, 0, &sB, 0, 0, &sE, 0, 0);
	d_print_strmat(n, n, &sE, 0, 0);
	dtrsm_rutn_libstr(n, n, 1.0, &sD, 0, 0, &sE, 0, 0, &sE, 0, 0);
	d_print_strmat(n, n, &sE, 0, 0);
#endif

//	d_print_strmat(n, n, &sA, 0, 0);
//	d_print_strmat(n, n, &sB, 0, 0);
//	d_print_strmat(n, n, &sD, 0, 0);
//	d_print_strmat(n, n, &sE, 0, 0);

//	d_cvt_strmat2mat(n, n, &sE, 0, 0, C, n);
//	d_print_mat(n, n, C, n);

	dtrtr_u_libstr(6, 1.0, &sE, 2, 0, &sB, 1, 0);
	d_print_strmat(n, n, &sB, 0, 0);

	d_print_strmat(n, n, &sA, 0, 0);
	dgemv_nt_libstr(6, n, 1.0, 1.0, &sA, 0, 0, &sx_n, 0, &sx_t, 0, 0.0, 0.0, &sy_n, 0, &sy_t, 0, &sz_n, 0, &sz_t, 0);
//	dsymv_l_libstr(5, 5, 1.0, &sA, 0, 0, x_n, 0.0, y_n, z_n);
	d_print_mat(1, n, z_n, 1);
	d_print_mat(1, n, z_t, 1);




//	for(ii=0; ii<sE.pm*sE.cn; ii++) sE.pA[ii] = 0.0;
//	double alpha = 0.0;
//	double beta = 1.0;
//	kernel_dgemm_nt_4x4_gen_lib4(4, &alpha, sA.pA, sB.pA, &beta, 3, sA.pA, sA.cn, 0, sE.pA, sE.cn, 0, 4, 2, 2);
//	d_print_strmat(n, n, &sE, 0, 0);

	// free memory
	free(A);
	free(B);
	free(C);
	free(ipiv);
//	d_free_strmat(&sA);
//	d_free_strmat(&sB);
//	d_free_strmat(&sD);
	v_free_align(memory_strmat);

	return 0;

	}