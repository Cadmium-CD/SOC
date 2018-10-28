/*
 * A demo/debug device.
 *
 * Copyright (c) 2013 Xilinx Inc.
 * Written by Edgar E. Iglesias
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define SC_INCLUDE_DYNAMIC_PROCESSES
#define min(a,b) (((a)<(b))?(a):(b))

#include <inttypes.h>

#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace std;

#include "accelerator_if.h"
#include <sys/types.h>
#include <time.h>



accelerator_if::accelerator_if(sc_module_name name)
	: sc_module(name), socket("socket")
{
	socket.register_b_transport(this, &accelerator_if::b_transport);
	socket.register_transport_dbg(this, &accelerator_if::transport_dbg);
	//SC_HAS_PROCESS(accelerator_if);
	//SC_THREAD(wait_endof_doblock);
	//dont_initialize();
	//cout<<"ini if"<<endl;

}

void accelerator_if::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay)
{
	tlm::tlm_command cmd = trans.get_command();
	sc_dt::uint64 addr = trans.get_address();
	unsigned char *data = trans.get_data_ptr();
	unsigned int len = trans.get_data_length();
	unsigned char *byt = trans.get_byte_enable_ptr();
	unsigned int wid = trans.get_streaming_width();

	if (byt != 0) {
		trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
		return;
	}

	if (len > 4 || wid < len) {
		trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
		return;
	}

	// Pretend this is slow!
	delay += sc_time(1, SC_US);

	if (trans.get_command() == tlm::TLM_READ_COMMAND) {
		//sc_time now = sc_time_stamp() + delay;

		switch (addr) {
			case 0x0:
				cout << "ssflag = " << ssflag<< endl;
				memcpy(data, &ssflag, len);
				break;
			default:
				//cout << addr << endl;
				if (ssflag == 6){				
					transformc(addr,data,len);
				}					
				break;
		}
	} else if (cmd == tlm::TLM_WRITE_COMMAND) {
		switch (addr) {
			case 0:
				memcpy(&ssflag,data,len);
        			cout << "ssflag = " << ssflag<< endl;
				break;
			case 0x4:
				memcpy(&lda,data,len);
				cout<<"lda = "<< lda<<endl;
				break;
			case 0x8:
				memcpy(&ldb,data,len);
				break;
			case 0xc:
				memcpy(&ldc,data,len);
				break;
			case 0x10:
				memcpy(&M,data,len);
				break;
			case 0x14:
				memcpy(&N,data,len);
				break;
			case 0x18:
				memcpy(&K,data,len);
				A = (int*) malloc(M*K*sizeof(int));
				B = (int*) malloc(K*N*sizeof(int));
				C = (int*) malloc(M*N*sizeof(int));
				count = 0;
				maflag = 1;
				break;
			default:
				//cout << addr << endl;
				if (maflag){				
					transformab(addr,data,len);
				}					
				break;
		}
		
		if(ssflag == 0x0000000c)
		{
		cout<< "trigger"<<endl;
		do_block(lda,ldb, ldc, M,  N,  K, A, B, C);
		ssflag = 6;
		/*do{ }while(ssflag==6);		
		free(A);
		free(B);
		free(C);
		count = 0;
		maflag = 0;*/
		//ssflag = 0;
		}
		if(ssflag == 0x00000003)
		{
		free(A);
		free(B);
		free(C);
		count = 0;
		maflag = 0;
		ssflag = 0;
		}
	}

	trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

unsigned int accelerator_if::transport_dbg(tlm::tlm_generic_payload& trans)
{
	unsigned int len = trans.get_data_length();
	return len;
}
void accelerator_if::do_block (int lda,int ldb, int ldc, int M, int N, int K, int* A, int* B, int* C)
{
		cout<<"blocking"<<endl;
		register int i,j,k;
		for (i = 0; i < M; ++i) {
        for (k = 0; k < K; ++k) {
    		  register int A_PART = A[i*K + k];
            for (j = 0; j < N; ++j) {
			C[i*N + j] += A_PART * B[k*N + j];
						}
				}
		}
	cout << "C[0]=" << C[0] << endl;
	}
void accelerator_if::transformab(sc_dt::uint64 addr,unsigned char* data,unsigned int len)
{	
	if(count<(M*K)){
		memcpy(&A[count],data,len);
  } else if (count<(M*K+K*N)){
			memcpy(&B[count-M*K],data,len);
		} else{
				memcpy(&C[count-M*K-K*N],data,len);			
			}
	count++;
}

void accelerator_if::transformc(sc_dt::uint64 addr,unsigned char* data,unsigned int len)
{	
	int index = (addr-4*(M*K+K*N))/4-7;
	memcpy(data,&C[index],len);		
}
/*void accelerator_if::wait_endof_doblock()
{
	for(;;)
	{
		wait(end_doblock->value_changed_event());
	  ssflag = 0x1;
	}	
}*/

