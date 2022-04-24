/* tb.cc
 * 
 * The cpp file is used as testbench to drive the simulation of `top`.
 * Use the following command to run the simulation.
 *   verilator -cc --exe -O3 top.v tb.cc --trace
 *   make -j -C obj_dir -f Vtop.mk
 *   obj_dir/Vtop
 */
#include <verilated.h>
#include <iostream>
#include <random>
#include <ctime>
#include "Vtop.h"

#define sim(contextp, top, clk, t)   \
  do {                               \
    for (int i = 0; i < (t); ++i) {  \
      (contextp)->timeInc(1);        \
      (top)->clk = !(top)->clk;      \
      (top)->eval();                 \
    }                                \
  } while (0)

using namespace std;

int main(int argc, char *argv[])
{

  VerilatedContext *contextp = new VerilatedContext();
  contextp->debug(0);
  contextp->traceEverOn(true);
  contextp->commandArgs(argc, argv);

  Vtop *top = new Vtop(contextp, "top");
  top->clk_i = 0;
  top->rst_ni = 0;
  top->wr_en_i = 0;
  top->addr_i = 0;
  top->data_i = 0;
  sim(contextp, top, clk_i, 5);  /* trigger reset for 5 half clock cycles */

  unsigned char opd1, opd2, op;
  srand(time(nullptr));

  /* This section tests the design for Problem3-1. Given opd1, opd2, and op, we
   * write the data to addr_i = 0, and retrieve the data back at addr_i = 1.
   */
  top->rst_ni = 1;
  for (int i = 0; i < 4; ++i) {
    /* Ramdomly generate the data to test */
    opd1 = rand() & 0xff;
    opd2 = rand() & 0xff;
    op = i;  /* testing op = 00, 01, 10, 11 */

    /* Write the input at addr_i = 0 */
    top->wr_en_i = 1;
    top->addr_i = 0x00000000;
    top->data_i = (op << 16) | (opd2 << 8) | (opd1);
    sim(contextp, top, clk_i, 2);

    top->wr_en_i = 0;
    top->addr_i = 0x00000001;
    sim(contextp, top, clk_i, 2);
  }

  /* This section tests the design for Problem3-3. Given a 32-bit number, the 
   * circuit should output its parity bit.
   */
  for (int i = 0; i < 3; ++i) {
    /* Randomly generate and write the data to test */
    top->wr_en_i = 1;
    top->addr_i = 0x20000000;
    top->data_i = rand();
    sim(contextp, top, clk_i, 2);

    top->wr_en_i = 0;
    top->addr_i = 0x20000001;
    sim(contextp, top, clk_i, 2);
  }

  sim(contextp, top, clk_i, 40960);

  top->final();
  return 0;
}
