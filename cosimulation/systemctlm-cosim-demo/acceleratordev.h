/*
 * Copyright (c) 2013 Xilinx Inc.
 * Written by Edgar E. Iglesias.
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

class acceleratordev
: public sc_core::sc_module
{
public:
	//sc_out<bool> irq;
	sc_core::sc_port<sc_core::sc_signal_out_if<int> > doblock_done;
	sc_core::sc_port<sc_core::sc_signal_in_if<int> > start_doblock;
	acceleratordev(sc_core::sc_module_name name);
	void do_block(void);
};

/*SC_MODULE(acceleratordev)
{
	sc_core::sc_port<sc_core::sc_signal_out_if<bool> > doblock_done;
	sc_core::sc_port<sc_core::sc_signal_in_if<bool> > start_doblock;

	void do_block(void);
};*/