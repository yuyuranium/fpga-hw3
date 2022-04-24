#include <verilated.h>
#include <iostream>
#include <random>
#include <ctime>
#include "Vtop.h"

#define sim(contextp, top, clk, t) \
  do {                             \
    for (int i = 0; i < (t); ++i) {  \
      (contextp)->timeInc(1);      \
      (top)->clk = !(top)->clk;    \
      (top)->eval();               \
    }                              \
  } while (0);

using namespace std;

int main(int argc, char *argv[])
{

  VerilatedContext *contextp = new VerilatedContext();
  contextp->debug(0);
  contextp->traceEverOn(true);
  contextp->commandArgs(argc, argv);

  unsigned char opd1, opd2, op;
  srand(time(nullptr));

  Vtop *top = new Vtop(contextp, "top");
  top->clk_i = 0;
  top->rst_ni = 0;
  top->wr_en_i = 0;
  top->addr_i = 0;
  top->data_i = 0;
  sim(contextp, top, clk_i, 5);

  top->rst_ni = 1;
  for (int i = 0; i < 4; ++i) {
    opd1 = rand() & 0xff;
    opd2 = rand() & 0xff;
    op = rand() & 0x3;

    top->wr_en_i = 1;
    top->addr_i = 0;
    top->data_i = (op << 16) | (opd2 << 8) | (opd1);
    sim(contextp, top, clk_i, 2);

    top->wr_en_i = 0;
    top->addr_i = 1;
    sim(contextp, top, clk_i, 6);
  }

  sim(contextp, top, clk_i, 40960);

  top->final();
  return 0;
}
