class accelerator_if
: public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket<accelerator_if> socket;
//port for interfacing with accelerator
	//sc_core::sc_port<sc_core::sc_signal_out_if<int> > trigger_doblock;
	//sc_core::sc_port<sc_core::sc_signal_in_if<int> > end_doblock;
//flag to let cpu know whether the computation is done of not
	int ssflag,lda,ldb,ldc,M,N,K;
	int* A;
	int* B;
	int* C;
	int count;
	int maflag;
	accelerator_if(sc_core::sc_module_name name);
	virtual void b_transport(tlm::tlm_generic_payload& trans,
					sc_time& delay);
	virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);
	void do_block(int lda,int ldb, int ldc, int M, int N, int K, int* A, int* B, int* C);
	void transformab(sc_dt::uint64 addr,unsigned char* data,unsigned int len);
	void transformc(sc_dt::uint64 addr,unsigned char* data,unsigned int len);
	//void wait_endof_doblock();
};
