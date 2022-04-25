/* tb.cc
 * 
 * The cpp file is used as testbench to drive the simulation of `top`.
 * Use the following command to run the simulation.
 *   verilator -cc --exe -O3 top.v tb.cc --trace
 *   make -j -C obj_dir -f Vtop.mk
 *   obj_dir/Vtop
 */
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <iostream>
#include <random>
#include <ctime>
#include "Vtop.h"

using namespace std;

int main(int argc, char *argv[])
{
  VerilatedContext *contextp = new VerilatedContext();
  contextp->debug(0);
  contextp->traceEverOn(true);
  contextp->commandArgs(argc, argv);

  Vtop *top = new Vtop(contextp, "top");
  top->clk_i = 0;

  VerilatedVcdC *tfp = new VerilatedVcdC;
  top->trace(tfp, 5);
  tfp->open("top.vcd");

  int gt;
  unsigned char opd1, opd2, op = 0;
  srand(time(nullptr));

  int posedge_cnt = 0;
  while (!contextp->gotFinish() && posedge_cnt < 30) {
    top->rst_ni = 1;
    if (contextp->time() >= 3 && contextp->time() < 5) {
      top->rst_ni = 0;
      top->addr_i = 0;
      top->data_i = 0;
      top->wr_en_i = 0;
    }

    top->clk_i ^= 1;
    top->eval();  /* generate clock edge */

    /* clk transition from 0 to 1, generating posedge */
    if (top->clk_i == 1) {
      posedge_cnt++;
      switch (posedge_cnt) {
      /* This section tests the design for Problem3-1. Given opd1, opd2, and op,
       * we write the data to addr_i = 0, and retrieve the data back at
       * addr_i = 1.
       */
      case 6:
      case 8:
      case 10:
      case 12:
        /* Ramdomly generate the data to test */
        opd1 = rand() & 0xff;
        opd2 = rand() & 0xff;

        /* Write the input at addr_i = 0 */
        top->wr_en_i = 1;
        top->addr_i = 0x00000000;
        top->data_i = (op << 16) | (opd2 << 8) | (opd1);
        op = op + 1;  /* testing next op */
        break;
      case 7:
      case 9:
      case 11:
      case 13:
        /* Read the result from circuit */
        top->wr_en_i = 0;
        top->addr_i = 0x00000001;
        break;
      /* This section tests the design for Problem3-3. Given a 32-bit number,
       * the circuit should output its parity bit.
       */
      case 14:
      case 16:
      case 18:
        top->wr_en_i = 1;
        top->addr_i = 0x20000000;
        top->data_i = rand();
        break;
      case 15:
      case 17:
      case 19:
        top->wr_en_i = 0;
        top->addr_i = 0x20000001;
        break;
      default:
        break;
      }
    }

    tfp->dump(contextp->time());
    contextp->timeInc(1);
  }

  top->final();
  tfp->flush();
  tfp->close();
  return 0;
}
