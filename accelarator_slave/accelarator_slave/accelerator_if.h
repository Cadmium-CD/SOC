class accelerator_if
: public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket<accelerator_if> socket;
//port for interfacing with accelerator
	sc_core::sc_port<sc_core::sc_signal_out_if<bool> > trigger_doblock;
	sc_core::sc_port<sc_core::sc_signal_in_if<bool> > end_doblock;
//flag to let cpu know whether the computation is done of not
	int32_t ssflag;
	accelerator_if(sc_core::sc_module_name name);
	virtual void b_transport(tlm::tlm_generic_payload& trans,
					sc_time& delay);
	virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);
	void wait_endof_doblock();
};
