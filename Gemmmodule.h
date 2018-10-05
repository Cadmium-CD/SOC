#ifndef Gemmmodule_h
#define Gemmmodule_h

// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#define min(a,b) (((a)<(b))?(a):(b))

struct Gemmmodule: sc_module
{
  int mem[7];
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Gemmmodule> socket;
	//define block
		void do_block (int lda,int ldb, int ldc, int M, int N, int K, int* A, int* B, int* C)
		{
			int i,j,k;

			for (i = 0; i < M; ++i) {
				for (k = 0; k < K; ++k) {
					register int A_PART = A[i*lda + k];
					for (j = 0; j < N; ++j) {
						C[i*ldc + j] += A_PART * B[k*ldb + j];
						}
				}
			}
		}

//define dgemm
void square_dgemm (int row,int lda, int ldb, int ldc,int* A, int* B, int* C)
{
	int i,j,k;
	int BLOCK_1 = 1;
	int BLOCK_2 = 1; 
	int BLOCK_3 = 1; 
  /* For each block-row of A */ 
	for (i = 0; i < row; i += BLOCK_1)
    /* For each block-column of B */
		for ( j = 0; j < ldb; j += BLOCK_3)
      /* Accumulate block dgemms into block of C */
			for ( k = 0; k < lda; k += BLOCK_2)
			{
	/* Correct block dimensions if block "goes off edge of" the matrix */
				int M = min (BLOCK_1, row-i);
				int K = min (BLOCK_2, lda-k);
				int N = min (BLOCK_3, ldb-j);

	/* Perform individual block dgemm */
				do_block(lda,ldb,ldc, M, N, K, A + i*lda + k, B + k*ldb + j, C + i*ldc + j);
			}
}
  SC_CTOR(Gemmmodule)
  : socket("socket")
  {
    // Register callback for incoming b_transport interface method call
    socket.register_b_transport(this, &Gemmmodule::b_transport);
	SC_THREAD(gemm);
  }

 // TLM-2 blocking transport method
  virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay)
  {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    adr = trans.get_address() / 4;
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();
    unsigned char*   byt = trans.get_byte_enable_ptr();
    unsigned int     wid = trans.get_streaming_width();

    // Obliged to check address range and check for unsupported features,
    //   i.e. byte enables, streaming, and bursts
    // Can ignore DMI hint and extensions
    // Using the SystemC report handler is an acceptable way of signalling an error

    if (adr >= sc_dt::uint64(7) || byt != 0 || len > 4 || wid < len)
      SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");

    // Obliged to implement read and write commands
    if ( cmd == tlm::TLM_READ_COMMAND )
     ;
    else if ( cmd == tlm::TLM_WRITE_COMMAND )
      memcpy(&mem[adr], ptr, len);

    // Obliged to set response status to indicate successful completion
    trans.set_response_status( tlm::TLM_OK_RESPONSE );
            cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << adr
           << " } , data = " << hex << *(int*)mem[adr] << endl;
  }
///define thread gemm
	void gemm()
	{
		int row = *(int*) mem[3];
		int lda = *(int*) mem[4];
		int ldb = *(int*) mem[5];
		int ldc = *(int*) mem[6];
		int* A = (int*) mem[0];
		int* B = (int*) mem[1];
		int* C = (int*) mem[2];

		square_dgemm (row,lda, ldb,  ldc, A,  B,  C);
	}
 

};

#endif


//define

//define
