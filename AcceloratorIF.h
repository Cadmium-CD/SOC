#ifndef AcceloratorIF_h
#define AcceloratorIF_h
// -*- C++ -*- vim600:syntax=cpp:sw=2:tw=78:fmr=<<<,>>>
#include <systemc>
/*
int sc_main(int argc, char* argv[])
{
  Gemmmodule gemm("gemm");
  sc_start();
  return 0;
}*/

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"


// Initiator module generating generic payload transactions

struct AcceloratorIF: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<AcceloratorIF> socket;

  SC_CTOR(AcceloratorIF)
  : socket("socket")  // Construct and name socket
  {
    SC_THREAD(thread_process);
  }

  void thread_process()
  {
    // TLM-2 generic payload transaction, reused across calls to b_transport
    tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
    sc_time delay = sc_time(10, SC_NS);

    // Generate a random sequence of reads and writes
    for (int i = 0; i < 7; i++)
    {

      tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND;
      //if (cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000 | i;
      // Initialize 8 out of the 10 attributes, byte_enable_length and extensions being unused
      trans->set_command( cmd );
      trans->set_address( i );
      trans->set_data_ptr( reinterpret_cast<unsigned char*>(&memcell[i]) );
      trans->set_data_length( 4 );
      trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
      trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
      trans->set_dmi_allowed( false ); // Mandatory initial value
      trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

      socket->b_transport( *trans, delay );  // Blocking transport call

      // Initiator obliged to check response status and delay
      if ( trans->is_response_error() )
        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

      cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << i
           << " } , data = " << hex << memcell[i] << " at time " << sc_time_stamp()
           << " delay = " << delay << endl;

      // Realize the delay annotated onto the transport call
      wait(delay);
    }
  }

  // Internal data buffer used by initiator with generic payload

  int A[8] = {1,4,6,3,6,3,7,8};
  int B[12] = {3,5,78,34,3,6,89,34,6,2,4,2};
  int C[6];
  int M = 2;
  int lda = 4;
  int ldb = 3;
  int ldc = 3;
  int* memcell[7] = {&A[0],&B[0],&C[0],&M,&lda,&ldb,&ldc};
};

#endif
